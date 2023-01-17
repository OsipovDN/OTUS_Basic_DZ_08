cmake_minimum_required (VERSION 3.8)

project (countWords)

file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable (countWords 
    FindWords.cpp
    #src/ThrObj.cpp
)

target_include_directories(
    countWords
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src
    )
target_include_directories(
    countWords PRIVATE 
   ${CMAKE_CURRENT_SOURCE_DIR}/header
    )

set_target_properties(${TARGETS} PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)

#if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
#    CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
#    target_compile_options(countWords PRIVATE 
#        -g
#        -Wall
#        -Wextra
#        -Werror
#        -pedantic
#    )
#elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
#    target_compile_options(countWords PRIVATE
#        /EHsc
#        /W4
#        /WX
#    )
#endif()