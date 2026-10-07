add_library(regex_engine_headers INTERFACE)

target_include_directories(regex_engine_headers INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/elements
)

target_sources(regex_engine_headers INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/enums.h
    ${CMAKE_CURRENT_SOURCE_DIR}/regex_element.h
    ${CMAKE_CURRENT_SOURCE_DIR}/regex_engine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/token.h
    ${CMAKE_CURRENT_SOURCE_DIR}/elements/any_char_element.h
    ${CMAKE_CURRENT_SOURCE_DIR}/elements/char_class_element.h
    ${CMAKE_CURRENT_SOURCE_DIR}/elements/literal_element.h
)

add_library(regex_headers ALIAS regex_engine_headers)
