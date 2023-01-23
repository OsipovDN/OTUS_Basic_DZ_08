cmake_minimum_required (VERSION 3.8)

project (countWords)

add_executable (countWords 
    FindWords.cpp
    )

set_target_properties(countWords PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(countWords PRIVATE 
        -g
        -Wall
        -Wextra
        -Werror
        -pedantic
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(countWords PRIVATE
        /EHsc
        /W4
        /WX
    )
endif()