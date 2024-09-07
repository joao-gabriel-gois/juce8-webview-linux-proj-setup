#pragma once

/*
  Not in use anymore. We are assigning a native logging function instead
  of this approach.
  In order to use it, check comments bellow:
*/
// You need to change JUCE's WebBrowserComponent to this class
// in case of using the other log fn approach in plugin/ui/public/index.js
// toggle comment/uncommend example lines bellow in index.js:
/*
  const nativeFunction = getNativeFunction('nativeFunction');
  const log = getNativeFunction('log');
  // const log = log2;

  // // (...)

  // function log2(...args) {
  //   const message = args.join(' ');
  //   const encodedMessage = encodeURIComponent(message);
  //   window.location.href = `external://log/${encodedMessage}`;
  // }
*/
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
