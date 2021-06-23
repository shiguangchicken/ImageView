cmake_minimum_required(VERSION 3.5)
project(imCli)

set( BOOST_ROOT "/home/fang/boost/prefix/" CACHE PATH "Boost library path" )
set( Boost_NO_SYSTEM_PATHS on CACHE BOOL "Do not search system for Boost" )
#set( BOOST_LIBRARYDIR "/home/fang/boost/prefix/lib")

FIND_PACKAGE(Boost 1.75.0 REQUIRED COMPONENTS system thread regex)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

    message("boost lib: ${Boost_LIBRARIES}, inc:${Boost_INCLUDE_DIR}")

    file(GLOB SOURCES 
        "src/main.cpp")

    add_executable(${PROJECT_NAME} ${SOURCES})

    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

ENDIF()

