cmake_minimum_required(VERSION 3.1.0)

set( PROJECT_NAME
     LibraryNameLibrary
)

set( LIBRARY_NAME
     LibraryName
)

# Library headers
set( LIBRARY_HEADERS_DIR
     include/${LIBRARY_NAME}
)
file( GLOB LIBRARY_HEADERS
      "${LIBRARY_HEADERS_DIR}/*.hpp"
)

# Library source files
set( LIBRARY_SOURCE_DIR
     src
)
file( GLOB LIBRARY_SOURCE
      "${LIBRARY_SOURCE_DIR}/*.cpp"
)


# External libraries
# find_package( )
set( LIBRARY_EXT_LIBS )
# Compiler flags
set( CMAKE_CXX_STANDARD 14 )


project( ${PROJECT_NAME} )


add_library( ${LIBRARY_NAME} STATIC
             ${LIBRARY_HEADERS}
             ${LIBRARY_SOURCE}
)

# Private interface - so that library source files can use headers directly
# i.e. #include "LibraryHeader.hpp"
target_include_directories( ${LIBRARY_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${LIBRARY_NAME}>
    $<INSTALL_INTERFACE:include/${LIBRARY_NAME}>
)

# Public interface - top level source files need to include the headers
# via. #include "LibraryName/LibraryHeader.hpp"
target_include_directories( ${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries( ${LIBRARY_NAME}
                       ${LIBRARY_EXT_LIBS}
                     )

# Build tests
add_subdirectory(test)
