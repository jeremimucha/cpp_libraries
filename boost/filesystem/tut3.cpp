//  filesystem tut3.cpp  ---------------------------------------------------------------//

//  Copyright Beman Dawes 2009

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  Library home page: http://www.boost.org/libs/filesystem

#include <iostream>
#include <boost/filesystem.hpp>
using std::cout;
namespace fs = boost::filesystem;

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    cout << "Usage: tut3 path\n";
    return 1;
  }

  fs::path p (argv[1]);

  try
  {
    if (fs::exists(p))
    {
      if (fs::is_regular_file(p))
        cout << p << " size is " << fs::file_size(p) << '\n';

      else if (fs::is_directory(p))
      {
        cout << p << " is a directory containing:\n";

        for (fs::directory_entry& x : fs::directory_iterator(p))
          cout << "    " << x.path() << '\n'; 
      }
      else
        cout << p << " exists, but is not a regular file or directory\n";
    }
    else
      cout << p << " does not exist\n";
  }

  catch (const fs::filesystem_error& ex)
  {
    cout << ex.what() << '\n';
  }

  return 0;
}
