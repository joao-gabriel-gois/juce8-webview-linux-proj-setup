#include "JuceWebViewPlugin/PluginProcessor.h"
#include "JuceWebViewPlugin/PluginEditor.h"

namespace webview_plugin {

namespace {
  auto streamToVector (juce::InputStream& stream) {
    using namespace juce;
    std::vector<std::byte> result ((size_t) stream.getTotalLength());
    stream.setPosition (0);
    [[maybe_unused]] const auto bytesRead = stream.read (result.data(), result.size());
    jassert (bytesRead == (ssize_t) result.size());
    return result;
  }

  const char* getMimeForExtension (const juce::String& extension) {
    using namespace juce;
    static const std::unordered_map<String, const char*> mimeMap =
    {
      { { "htm"   },  "text/html"                },
      { { "html"  },  "text/html"                },
      { { "txt"   },  "text/plain"               },
      { { "jpg"   },  "image/jpeg"               },
      { { "jpeg"  },  "image/jpeg"               },
      { { "svg"   },  "image/svg+xml"            },
      { { "ico"   },  "image/vnd.microsoft.icon" },
      { { "json"  },  "application/json"         },
      { { "png"   },  "image/png"                },
      { { "css"   },  "text/css"                 },
      { { "map"   },  "application/json"         },
      { { "js"    },  "text/javascript"          },
      { { "woff2" },  "font/woff2"               }
    };

    if (const auto it = mimeMap.find (extension.toLowerCase()); it != mimeMap.end())
      return it->second;

    jassertfalse;
    return "";
  }

  constexpr auto LOCAL_DEV_SERVER_ADDRESS = "http://127.0.0.1:8080";
}
// namespace

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(
  AudioPluginAudioProcessor& p)
  : AudioProcessorEditor(&p),
    processorRef(p),
    webView{juce::WebBrowserComponent::Options{}
            .withResourceProvider(
                [this](const auto& url) { return getResource(url); },
                juce::URL{LOCAL_DEV_SERVER_ADDRESS}.getOrigin())
            .withNativeIntegrationEnabled()
            .withUserScript(R"(console.log("C++ Backend Here: Running before any loading.");)")
            .withInitialisationData("vendor", JUCE_COMPANY_NAME)
            .withInitialisationData("pluginName", JUCE_PRODUCT_NAME)
            .withInitialisationData("pluginVersion", JUCE_PROJECT_VERSION)
            .withNativeFunction(
              juce::Identifier{"nativeFunction"},
              [this] (const juce::Array<juce::var>& args,
                  juce::WebBrowserComponent::NativeFunctionCompletion completion) {
                nativeFunction(args, std::move(completion));
              }
            )
            .withNativeFunction(
              juce::Identifier{"log"},
              [this] (const juce::Array<juce::var>& args,
                  juce::WebBrowserComponent::NativeFunctionCompletion completion) {
                log(args, std::move(completion));
              }
            )
            .withEventListener(
              "exampleInnerEvent2", 
              [this] (juce::var objFromJS) {
                labelUpdatedFromJS.setText("exampleInnerEvent2: frontend event happened with value = "
                  + objFromJS.getProperty("emitCounter", 0).toString(),
                  juce::dontSendNotification);
              }
            ) } {
  juce::ignoreUnused (processorRef);

  addAndMakeVisible(webView);
  
  // Staticly serving the UI
  // webView.goToURL(webView.getResourceProviderRoot());

  // Serving UI through a server (npx http-server)
  webView.goToURL(LOCAL_DEV_SERVER_ADDRESS);

  addAndMakeVisible(runJSButton);
  runJSButton.onClick = [this] {
    const juce::String JAVASCRIPT_TO_RUN =
      "log(\"\\n\\t[PluginEditor] evaluateJS:\", \"Hello World! Passing Math.sqrt(4) as result;\"); Math.sqrt(4);";

    webView.evaluateJavascript(
      JAVASCRIPT_TO_RUN,
      [](juce::WebBrowserComponent::EvaluationResult result) {
        if (const auto* resultPtr = result.getResult()) {
          std::cout << "\n\t[PluginEditor] evaluateJS.getResult() | Result: " << resultPtr->toString() << std::endl;
        }
        else if (const auto* errorPtr = result.getError()) {
          std::cout << "\n\t[PluginEditor] evaluateJS.getError() | Error: " << errorPtr->message << std::endl;
        }
      }
    );
  };

  addAndMakeVisible(emitJSEventButton);
  emitJSEventButton.onClick = [this] {
    static const juce::Identifier EVENT_ID("exampleEvent");
    webView.emitEventIfBrowserIsVisible(EVENT_ID, 42.0);
  };

  addAndMakeVisible(emitJSEventWEvalButton);
  emitJSEventWEvalButton.onClick = [this] {
    webView.evaluateJavascript(
      "window.dispatchEvent(new Event('exampleInnerEvent'));"
    );
  };

  addAndMakeVisible(labelUpdatedFromJS);

  setResizable(true, true);
  setSize (800, 600);

  startTimer(60);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}



void AudioPluginAudioProcessorEditor::resized()
{
  auto bounds = getLocalBounds();
  
  webView.setBounds(bounds.removeFromRight(getWidth() / 2));
  runJSButton.setBounds(bounds.removeFromTop(50).reduced(5));
  emitJSEventButton.setBounds(bounds.removeFromTop(50).reduced(5));
  emitJSEventWEvalButton.setBounds(bounds.removeFromTop(50).reduced(5));
  labelUpdatedFromJS.setBounds(bounds.removeFromTop(50).reduced(5));
}

void AudioPluginAudioProcessorEditor::timerCallback() {
  webView.emitEventIfBrowserIsVisible("outputLevel", juce::var{});
}

auto AudioPluginAudioProcessorEditor::getResource(const juce::String& url) -> std::optional<Resource> {
  static const auto resourceFileRoot = juce::File{R"(/home/pudou/Projects/wize-mice/juce8-webview-tutorial/plugin/ui/public)"};

  const auto resourceToRetrieve = url == "/" ? "index.html" : url.fromFirstOccurrenceOf("/", false, false);

  if (resourceToRetrieve == "outputLevel.json") {
    juce::DynamicObject::Ptr data{new juce::DynamicObject{}};
    data->setProperty("left", processorRef.outputLevelLeft.load());
    const auto string = juce::JSON::toString(data.get());
    juce::MemoryInputStream stream{string.getCharPointer(),
                                  string.getNumBytesAsUTF8(), false};
    return Resource{streamToVector(stream), juce::String{"application/json"}};
  }

  if (resourceToRetrieve == "data.json") {
    juce::DynamicObject::Ptr data{new juce::DynamicObject{}};
    data->setProperty("sampleProperty", 30.0);
    const auto string = juce::JSON::toString(data.get());
    juce::MemoryInputStream stream{string.getCharPointer(),
                                  string.getNumBytesAsUTF8(), false};
    return Resource{streamToVector(stream), juce::String{"application/json"}};
  }

  const auto resource = resourceFileRoot.getChildFile(resourceToRetrieve).createInputStream();

  if (resource) {
    const auto extension = resourceToRetrieve.fromLastOccurrenceOf(".", false, false);
    return Resource{streamToVector(*resource), getMimeForExtension(extension)};
  }

  return std::nullopt;
}

void AudioPluginAudioProcessorEditor::nativeFunction(const juce::Array<juce::var>& args,
                      juce::WebBrowserComponent::NativeFunctionCompletion completion) {

  juce::String concatenatedArgs;

  for (const auto& arg : args) {
    concatenatedArgs += arg.toString();
  }

  labelUpdatedFromJS.setText("Native Function called with args: " + concatenatedArgs,
    juce::dontSendNotification);

  completion("nativeFunction Callback: All Ok!");
}

void AudioPluginAudioProcessorEditor::log(const juce::Array<juce::var>& args,
                      juce::WebBrowserComponent::NativeFunctionCompletion completion) {
  
  juce::String concatenatedArgs;
  for (int i = 0; i < args.size(); ++i) {
    concatenatedArgs += args[i].toString();
    if (i < args.size() - 1) {
      concatenatedArgs += " ";
    }
  }
  
  std::cout << concatenatedArgs << std::endl;
  completion(true);
}


}
