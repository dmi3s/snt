cmake_minimum_required (VERSION 3.8)

project(segnetics-test)

if( NOT DEFINED CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Debug CACHE STRING
       "Choose the type of build, options are: Debug Release MinSizeRel RelWithDebInfo."
       FORCE )
endif()

### main output dir 
set(OUTPUT_DIR ${CMAKE_BINARY_DIR})

find_package(Threads REQUIRED)
find_package(Boost 1.64 REQUIRED filesystem)

### Boost ###
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREAD ON)  # enable multithreading

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

link_directories(
    ${Boost_LIBRARY_DIRS}
    ${OUTPUT_DIR}
)

# compiler-specific options


if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    add_definitions(-D_WIN32_WINNT=0x0A00) # Target is windows 10
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4503 /MP")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

add_subdirectory(producer)
add_subdirectory(consumer)

# ENABLE WITH:  -DBUILD_TESTS=ON
option (BUILD_TESTS "Build tests" OFF)
