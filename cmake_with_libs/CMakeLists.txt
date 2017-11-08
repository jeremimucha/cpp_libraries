cmake_minimum_required( VERSION 3.1.0 )

set( PROJECT_NAME
     SomeProject
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


# --- Executables
set( SOURCE_DIR
     src
   )

# Build first executable
set( MAIN_EXECUTABLE_NAME
     main
   )
set( MAIN_SOURCE
     ${SOURCE_DIR}/main_1.cpp
   )

# Build other executables analogously (OPTIONAL)
set( ANOTHER_EXECUTABLE_NAME
     main2
   )
set( ANOTHER_SOURCE
     ${SOURCE_DIR}/main_2.cpp
   )


# Build and link libraries for the project
set( LIBRARIES_DIR
     libs
   )
set( LIBRARIES
     LibraryName
   )

# External libraries
# find_package(  )
set( PROJECT_EXT_LIBS )
# set compiler flags
set( CMAKE_CXX_STANDARD 14 )
set( CXX_FLAGS "-O2 -Wall -Wextra -pedantic -std=c++14" )


project( ${PROJECT_NAME} LANGUAGES CXX )

enable_testing()


#--- Project --- #
include_directories( ${PROJECT_HEADERS_DIR} )
add_executable( ${MAIN_EXECUTABLE_NAME} ${MAIN_SOURCE} )
set_target_properties( ${MAIN_EXECUTABLE_NAME}
                       PROPERTIES 
                       COMPILE_FLAGS
                       ${CXX_FLAGS}
                     )
target_link_libraries( ${MAIN_EXECUTABLE_NAME} 
                       ${LIBRARIES}
                       ${PROJECT_EXT_LIBS}
                     )

add_executable( ${ANOTHER_EXECUTABLE_NAME} ${ANOTHER_SOURCE} )
set_target_properties( ${ANOTHER_EXECUTABLE_NAME}
                       PROPERTIES 
                       COMPILE_FLAGS
                       ${CXX_FLAGS}
                     )
target_link_libraries( ${ANOTHER_EXECUTABLE_NAME} 
                       ${LIBRARIES}
                       ${PROJECT_EXT_LIBS}
                     )

foreach( LIBRARY ${LIBRARIES} )
    add_subdirectory( "${LIBRARIES_DIR}/${LIBRARY}" )
endforeach( LIBRARY )


# OPTIONALLY - add top level tests
add_subdirectory( test )
