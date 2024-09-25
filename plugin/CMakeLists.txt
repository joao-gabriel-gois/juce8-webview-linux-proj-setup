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

get_target_property(COMPANY_NAME JuceWebViewPlugin JUCE_COMPANY_NAME)
get_target_property(PRODUCT_NAME JuceWebViewPlugin JUCE_PRODUCT_NAME)
# target_compile_definitions(JuceWebViewPlugin PRIVATE
#   JUCE_COMPANY_NAME="${COMPANY_NAME}"
#   JUCE_PRODUCT_NAME="${PRODUCT_NAME}"
#   JUCE_PROJECT_VERSION="${PROJECT_VERSION}")
target_compile_definitions(JuceWebViewPlugin   PRIVATE
  JUCE_COMPANY_NAME="${COMPANY_NAME}"
  JUCE_PRODUCT_NAME="${PRODUCT_NAME}"
  JUCE_PROJECT_VERSION="${PROJECT_VERSION}"
)

target_sources(JuceWebViewPlugin
  PRIVATE
    source/PluginEditor.cpp
    source/PluginProcessor.cpp
)

target_include_directories(JuceWebViewPlugin PRIVATE include)

target_link_libraries(JuceWebViewPlugin
  PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
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

file(
  COPY
    "${CMAKE_CURRENT_SOURCE_DIR}/../lib/juce/modules/juce_gui_extra/native/javascript"
  DESTINATION
    "${CMAKE_CURRENT_SOURCE_DIR}/ui/public/js/juce/")
