//  filesystem tut2.cpp  ---------------------------------------------------------------//

//  Copyright Beman Dawes 2009

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  Library home page: http://www.boost.org/libs/filesystem
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <iostream>
#include <boost/filesystem.hpp>
using namespace std;
namespace fs = boost::filesystem;

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    cout << "Usage: tut2 path\n";
    return 1;
  }

  fs::path p(argv[1]);  // avoid repeated path construction below

  if (fs::exists(p))    // does path p actually exist?
  {
    if (fs::is_regular_file(p))        // is path p a regular file?
      cout << p << " size is " << file_size(p) << '\n';

    else if (fs::is_directory(p))      // is path p a directory?
      cout << p << " is a directory\n";

    else
      cout << p << " exists, but is not a regular file or directory\n";
  }
  else
    cout << p << " does not exist\n";

  return 0;
}
