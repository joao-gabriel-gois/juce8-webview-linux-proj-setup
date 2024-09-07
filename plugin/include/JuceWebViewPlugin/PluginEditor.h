#pragma once

#include "JuceWebViewPlugin/PluginProcessor.h"
// using  <lib/lib.h> notation in linux is not working
#include "juce_gui_extra/juce_gui_extra.h"
// Switch between above one and bellow one in case of need
// #include "MyWebBrowserComponent.h"

#include <optional>


namespace webview_plugin {
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
  explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
  ~AudioPluginAudioProcessorEditor() override;

  // No need for paint method in webviews
  void resized() override;

private:
  using Resource = juce::WebBrowserComponent::Resource;
  std::optional<Resource> getResource(const juce::String& url);

  void nativeFunction(const juce::Array<juce::var>& args,
                      juce::WebBrowserComponent::NativeFunctionCompletion completion);

  void log(const juce::Array<juce::var>& args,
                        juce::WebBrowserComponent::NativeFunctionCompletion completion);

  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  AudioPluginAudioProcessor& processorRef;

  juce::TextButton runJSButton{"Run some JavaScript"};
  juce::TextButton emitJSEventButton{"Emit some JavaScript Event from Cpp"};
  juce::TextButton emitJSEventWEvalButton{"Emit some JavaScript Event from JS Eval"};
  
  
  juce::Label labelUpdatedFromJS{"label", "To be updated by calling a native c++ fn from js side!"};

  juce::WebBrowserComponent webView;
  // MyWebBrowserComponent webView;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};

}