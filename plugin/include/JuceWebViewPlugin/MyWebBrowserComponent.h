#pragma once

#include "juce_gui_extra/juce_gui_extra.h"

class MyWebBrowserComponent : public juce::WebBrowserComponent
{
public:
    MyWebBrowserComponent(juce::WebBrowserComponent::Options options = {})
      : juce::WebBrowserComponent(options) {}

    ~MyWebBrowserComponent() override = default;

    // Override pageAboutToLoad to intercept JS calls
    bool pageAboutToLoad(const juce::String& newURL) override {
      // Check if the new URL starts with "external://log", which will be invoked via JS
      if (newURL.startsWith("external://log")) {
        juce::String logMessage = newURL.fromFirstOccurrenceOf("external://log/", false, true);
        logMessage = juce::URL::removeEscapeChars(logMessage);
        std::cout << "\t(JS): " << logMessage << std::endl;

        // Return false to prevent the WebView from trying to load this URL
        return false;
      }

      // Allow other URLs to load normally
      return true;
    }
};
