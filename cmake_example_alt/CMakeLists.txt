cmake_minimum_required( VERSION 3.0.0 )

set( PROJECT_NAME
     main
)

# Dependencies / headers - glob all headers in the include dir
set( PROJECT_HEADERS_DIR
     include
)
file( GLOB PROJECT_HEADERS
      "${PROJECT_HEADERS_DIR}/*.hpp"
)
# or declare them explicitly
# set( PROJECT_HEADERS ${PROJECT_HEADERS_DIR}/List.hpp )

# Source files - glob all source files in the src dir
set( PROJECT_SOURCE_DIR
     src
)
file( GLOB PROJECT_SOURCE
      "${PROJECT_SOURCE_DIR}/*.cpp"
)
# or declare them explicitly
# set( SOURCE ${PROJECT_SOURCE_DIR}/List.cpp ${PROJECT_SOURCE_DIR}/main.cpp )

# Build and link libraries
set( LIBRARIES_DIR
     libs
)
set( LIBRARIES
     A
)

# Link external libraries
set( PROJECT_EXT_LIBS )

# set compiler flags
set( CXX_FLAGS "-O2 -Wall -Wextra -pedantic -std=c++14" )


project( ${PROJECT_NAME} )


enable_testing()


# --- Project --- #
include_directories( ${PROJECT_HEADERS_DIR} )
add_executable( ${PROJECT_NAME} ${PROJECT_SOURCE} )
set_target_properties( ${PROJECT_NAME}
                       PROPERTIES 
                       COMPILE_FLAGS
                       ${CXX_FLAGS}
                     )

foreach( LIBRARY ${LIBRARIES} )
    add_subdirectory( "${LIBRARIES_DIR}/${LIBRARY}" )
endforeach( LIBRARY )

target_link_libraries( ${PROJECT_NAME} ${LIBRARIES} ${PROJECT_EXT_LIBS} )

add_subdirectory( test )
