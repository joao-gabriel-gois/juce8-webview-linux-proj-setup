juce_add_plugin(JuceWebViewPlugin
  COMPANY_NAME BuzzyByte
  # In order to avoid GTK errors in Linux once running JUCE_WEB_BROWSER=1 in target_compile_definitions
  NEEDS_WEB_BROWSER TRUE
  IS_SYTN FALSE
  NEEDS_MIDI_INPUT FALSE
  NEEDS_MIDI_OUTPUT FALSE
  PLUGIN_MANUFACTURER_CODE BZZB
  PLUGIN_CODE JWVP
  FORMATS AU VST3 Standalone
)

target_sources(JuceWebViewPlugin
  PRIVATE src/PluginEditor.cpp src/PluginProcessor.cpp
)

target_include_directories(JuceWebViewPlugin PRIVATE include)

target_link_libraries(JuceWebViewPlugin
  PRIVATE
    juce::juce_audio_utils
    # juce::juce_gui_extra
  PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)

target_compile_definitions(JuceWebViewPlugin
  PUBLIC
    # Webview activation bellow
    JUCE_WEB_BROWSER=1
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0)