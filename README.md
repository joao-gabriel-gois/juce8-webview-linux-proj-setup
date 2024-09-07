# Juce8 WebView Tutorial

This project aims to reproduce the Juce 8 webview tutorial and end up with a minimum setup and notes to produce a robust environment for creating JS based for JUCE Plugins.

### Setting Up the Project
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

#### Build Script
Update JUCE Lib dependency: `git submodule update --init --recursive`. After that, you can build the project by running `./build_it`. To build the debbuging version you can add `--debug` or `-D` flags.

#### Runnign the Plugin
To easily open the plugin, run `./run`. To run the debbuging version you can add `--debug` or `-D` flags.


### Notes
After finishing the 5 videos, try a project with React.
