cmake_minimum_required(VERSION 3.0.0)

set( PROJECT_NAME ListLib )
set( LIBRARY_NAME List )
set( LIBRARY_HEADERS_DIR include/${LIBRARY_NAME} )
set( LIBRARY_HEADERS ${LIBRARY_HEADERS_DIR}/List.hpp )
set( LIBRARY_SOURCE_DIR src )
set( LIBRARY_SOURCE ${LIBRARY_SOURCE_DIR}/List.cpp )

set( CXX_FLAGS "-O2 -Wall -Wextra -pedantic -std=c++14" )

project( ${PROJECT_NAME} )

add_library( ${LIBRARY_NAME} STATIC
             ${LIBRARY_HEADERS}
             ${LIBRARY_SOURCE}
           )
set_target_properties( ${LIBRARY_NAME}
                       PROPERTIES 
                       COMPILE_FLAGS
                       ${CXX_FLAGS}
                     )
target_include_directories( ${LIBRARY_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${LIBRARY_NAME}>
    $<INSTALL_INTERFACE:include/${LIBRARY_NAME}>
)

target_include_directories( ${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# include_directories( ${LIBRARY_NAME} ${LIBRARY_HEADERS_DIR} )

add_subdirectory( test )
