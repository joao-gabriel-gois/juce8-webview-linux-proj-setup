# Juce8 WebView Linux Setup

This project aims to reproduce the Juce 8 webview tutorial and end up with a minimum setup and notes to produce a robust environment for creating JS based for JUCE Plugins.

## Setting Up the Project
All the setup is done in order to compile in Debian based Linux distros. So, first of all, is required to have all JUCE dependencis for linux installed. Also, this is the current versions of the building tools for building this project:
* **gcc**: `11.4.0`
* **g++**: `11.4.0`
* **cmake**: `3.30.2`

It's also important to make sure you have these lines as options of your `juce_add_plugin` call, indside your `plugin/CMakeLists.txt`:

<pre>
juce_add_plugin(JuceWebViewPlugin
  ...
  
  <i style="font-size:7.5pt;opacity:0.65;"># In order to avoid GTK errors in Linux once running JUCE_WEB_BROWSER=1 in target_compile_definitions</i>
  <b>NEEDS_WEB_BROWSER TRUE</b>
  
  ...
)
</pre>

### Build Script
Update JUCE Lib dependency: `git submodule update --init --recursive`. After that, you can build the project by running `./build_it`. To build the debbuging version you can add `--debug` or `-D` flags.

### Running the Plugin
To easily open the plugin, run `./run`. To run the debbuging version you can add `--debug` or `-D` flags.

___

#### About the Source Code
The actual plugin code is meant to be replaceable, but they're showing basic integration cases between JS and JUCE. It was wrote by following the [**JUCE's YouTube Tutorial**](https://www.youtube.com/watch?v=0ALLRitFE34&list=PLrJPU5Myec8Z-8gEj3kJdMfuuuWFbpy7D&pp=iAQB), but with all adaptations, debuggin, setup and already written build scripts for linux environment. From this setup, any JUCE Plugin  Project with JS UI can be started.

[**License details**](./LICENSE)

___

#### For reference:
  - [JUCE 8 - **What's new?**](https://juce.com/releases/whats-new/)
  - [JUCE 8 Feature Overview: **WebView UIs**](https://juce.com/blog/juce-8-feature-overview-webview-uis/)
  - [JUCE 8 Class Docs: **WebBrowserComponent**](https://docs.juce.com/master/classWebBrowserComponent.html)
