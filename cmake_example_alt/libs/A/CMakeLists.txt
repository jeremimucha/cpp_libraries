
cmake_minimum_required(VERSION 3.0.0)

set( PROJECT_NAME
     ALibrary
)

set( LIBRARY_NAME
     A
)

# Library headers
set( LIBRARY_HEADERS_DIR
     include/${LIBRARY_NAME}
)
set( LIBRARY_HEADERS
     ${LIBRARY_HEADERS_DIR}/A.h
)

# Library source files
set( LIBRARY_SOURCE_DIR
     src
)
set( LIBRARY_SOURCE
     ${LIBRARY_SOURCE_DIR}/A.cpp
)

# External libraries
set( LIBRARY_EXT_LIBS )


project( ${PROJECT_NAME} )


add_library( ${LIBRARY_NAME} STATIC
             ${LIBRARY_HEADERS}
             ${LIBRARY_SOURCE}
)

target_include_directories( ${LIBRARY_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${LIBRARY_NAME}>
    $<INSTALL_INTERFACE:include/${LIBRARY_NAME}>
)

target_include_directories( ${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries( ${LIBRARY_NAME}
                       ${LIBRARY_EXT_LIBS}
                     )

add_subdirectory(test)
