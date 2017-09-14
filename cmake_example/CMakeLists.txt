cmake_minimum_required( VERSION 3.0.0 )

set( PROJECT_NAME
     ListMain
)

# Project source files
set( PROJECT_SOURCE_DIR src )
set( PROJECT_SOURCE
     ${PROJECT_SOURCE_DIR}/main.cpp
)
# or glob all files
#file( GLOB PROJECT_SOURCE
#      "${PROJECT_SOURCE_DIR}/*.cpp"
#)

# Build libraries
set( LIBRARIES_DIR
     libs
)
set( LIBRARIES
     List
)

# Project external libraries
set( PROJECT_EXT_LIBS 
)

project( ${PROJECT_NAME} )


enable_testing()


# --- Project --- #
add_executable( ${PROJECT_NAME} ${SOURCE} )

foreach( LIBRARY ${LIBRARIES} )
    add_subdirectory( "${LIBRARIES_DIR}/${LIBRARY}" )
endforeach( LIBRARY )

target_link_libraries( ${PROJECT_NAME} ${LIBRARIES} ${PROJECT_EXT_LIBS} )
