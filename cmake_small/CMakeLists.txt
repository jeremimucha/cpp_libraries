cmake_minimum_required( VERSION 3.0.0 )

set( PROJECT_NAME
     main
   )

# Dependencies / headers - 
set( PROJECT_HEADERS_DIR
     include
   )
# glob all headers in the include dir
file( GLOB PROJECT_HEADERS
     "${PROJECT_HEADERS_DIR}/*.hpp"
    )
# or declare them explicitly
# set( PROJECT_HEADERS ${PROJECT_HEADERS_DIR}/List.hpp )

# Source files
set( PROJECT_SOURCE_DIR
     src
   )
# glob all source files in the src dir
file( GLOB PROJECT_SOURCE
     "${PROJECT_SOURCE_DIR}/*.cpp"
    )
# or declare them explicitly
# set( SOURCE ${PROJECT_SOURCE_DIR}/List.cpp ${PROJECT_SOURCE_DIR}/main.cpp )

# set main executable file
set( PROJECT_EXECUTABLE 
     ${PROJECT_SOURCE_DIR}/main.cpp
   )

# include external libraries as needed
# find_package( )
# Link external libraries
set( PROJECT_EXT_LIBS )
set( CMAKE_CXX_STANDARD 14 )
set( CXX_FLAGS "-O2 -Wall -Wextra -pedantic -std=c++14" )
project( ${PROJECT_NAME} )


enable_testing( )

include_directories( ${PROJECT_HEADERS_DIR} )
add_executable( ${PROJECT_NAME} ${PROJECT_EXECUTABLE} )
set_target_properties( ${PROJECT_NAME} 
                       PROPERTIES
                       COMPILE_FLAGS
                       ${CXX_FLAGS} )
        
target_link_libraries( ${PROJECT_NAME}
                       ${PROJECT_EXT_LIBRARIES}
                     )

add_subdirectory( test )
