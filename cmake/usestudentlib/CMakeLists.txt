cmake_minimum_required(VERSION 2.8.9)
project (TestLibrary)

#For the shared library:
set ( PROJECT_LINK_LIBS libtestStudent.dll )
link_directories( C:/Users/muchajer/Documents/Cpp/cpp_libraries/cmake/studentlib_shared/build )

#For the static library:
#set ( PROJECT_LINK_LIBS libtestStudent.a )
#link_directories( C:/Users/muchajer/Documents/Cpp/cpp_libraries/cmake/studentlib_static/build )

include_directories( C:/Users/muchajer/Documents/Cpp/cpp_libraries/cmake/student/include )

add_executable(libtest libtest.cpp)
target_link_libraries(libtest ${PROJECT_LINK_LIBS} )
