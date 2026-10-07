add_library(regex_engine STATIC
    any_char_element.cpp
    char_class_element.cpp
    literal_element.cpp
    regex_engine.cpp
)

target_link_libraries(regex_engine PUBLIC regex_engine_headers)

add_library(lab3::regex_engine ALIAS regex_engine)

if(MSVC)
    target_compile_options(regex_engine PRIVATE /W4)
else()
    target_compile_options(regex_engine PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(lab3
    main.cpp
)

target_link_libraries(lab3 PRIVATE regex_engine)

if(MSVC)
    target_compile_options(lab3 PRIVATE /W4)
else()
    target_compile_options(lab3 PRIVATE -Wall -Wextra -Wpedantic)
endif()
