cmake_minimum_required (VERSION 3.2)
set(PROJECT_NAME_STR producer)

project(${PROJECT_NAME_STR} CXX C)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4714 /W4 /WX")
endif()

include_directories(${PROJECT_NAME_STR}  src)
include_directories(${PROJECT_NAME_STR}  include)


file(GLOB_RECURSE PRODUCER_SRCS
         RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
         ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
         ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
         ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

add_executable(${PROJECT_NAME_STR} ${PRODUCER_SRCS})

target_sources(${PROJECT_NAME_STR}
    PRIVATE
        ${PRODUCER_SRCS}
    PUBLIC
)


set_target_properties(${PROJECT_NAME_STR} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set_target_properties(${PROJECT_NAME_STR} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set_target_properties(${PROJECT_NAME_STR} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})


target_link_libraries(${PROJECT_NAME_STR} ${Boost_LIBRARIES})

