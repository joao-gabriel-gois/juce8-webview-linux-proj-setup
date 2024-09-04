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
  // auto streamToVector(juce::InputStream& stream) -> std::vector<std::byte> {
  //   std::vector<std::byte> result(static_cast<size_t>(stream.getTotalLength()));
  //   stream.setPosition(0);
  //   const auto bytesRead = stream.read(result.data(), result.size());
  //   if (bytesRead != static_cast<ssize_t>(result.size())) {
  //       jassertfalse;  // Ensure that the stream reads the expected amount of data
  //       DBG("Failed to read the expected amount of data from stream.");
  //       std::cout << "Failed to read the expected amount of data from stream." << std::endl;
  //   }
  //   return result;
}
// namespace

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(
    AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p),
      processorRef(p),
      webView{juce::WebBrowserComponent::Options{}
            .withBackend(
              juce::WebBrowserComponent::Options::Backend::webview2)
            .withResourceProvider(
              // [](const auto& url) {
              //   juce::ignoreUnused(url);
              //   DBG("ResourceProvider called with " + url);
              //   return std::nullopt;
              // }
              [this](const auto& url) { return getResource(url); } )
            .withNativeIntegrationEnabled()} {
    juce::ignoreUnused (processorRef);

    addAndMakeVisible(webView);

    // webView.goToURL("/");
    // webView.goToURL("juce://juce.backend/");
    webView.goToURL(webView.getResourceProviderRoot());
    // webView.goToURL("file:///home/pudou/Projects/wize-mice/juce8-webview-tutorial/plugin/ui/public/index.html");
    std::cout << webView.getResourceProviderRoot() << std::endl;

    setResizable(true, true);
    setSize (800, 600);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    webView.setBounds(getLocalBounds());
}

auto AudioPluginAudioProcessorEditor::getResource(const juce::String& url) -> std::optional<Resource> {
  std::cout << url << std::endl;

  static const auto resourceFileRoot = juce::File{R"(/home/pudou/Projects/wize-mice/juce8-webview-tutorial/plugin/ui/public)"};

  const auto resourceToRetrieve = url == "/" ? "index.html" : url.fromFirstOccurrenceOf("/", false, false);
  
  const auto resource = resourceFileRoot.getChildFile(resourceToRetrieve).createInputStream();

  if (resource) {
  const auto extension = resourceToRetrieve.fromLastOccurrenceOf(".", false, false);
    return Resource{streamToVector(*resource), getMimeForExtension(extension)};
  }

  return std::nullopt;
}

}
