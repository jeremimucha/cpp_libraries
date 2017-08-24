#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <iostream>
#include <boost/filesystem.hpp>
#include <algorithm>
#include <iterator>
#include <cstdlib>
using std::cout;
namespace fs = boost::filesystem;


int main(int argc, char* argv[])
{
    if( argc < 2 ){
        cout << "Usage: tut3 path\n";
        exit(EXIT_FAILURE);
    }

    fs::path p(argv[1]);

    try{
        if(fs::exists(p)){  // does p actually exist?
            if(fs::is_regular_file(p))
                cout << p << " size is " << fs::file_size(p) << '\n';
            else if(fs::is_directory(p)){
                cout << p << " is a directory containing:\n";
                std::copy(fs::directory_iterator(p), fs::directory_iterator(),
                          std::ostream_iterator<fs::directory_entry>(cout, "\n"));
            }
            else
                cout << p << " exists, but is neither a regular file nor a directory\n";
        }
        else
            cout << p << " does not exist\n";
    }
    catch(const fs::filesystem_error& ex){
        cout << ex.what() << '\n';
    }

    return 0;
}
