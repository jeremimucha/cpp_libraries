#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include <algorithm>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>
#include <functional>
#include <cstdlib>
using std::cout;
using std::endl;
using std::placeholders::_1;
using std::placeholders::_2;
using std::shared_ptr;
namespace fs = boost::filesystem;


struct dax
{
    static const fs::path ext_dax;
    static const fs::path ext_mcf;
    static const fs::path ext_s19;
    enum SORT { by_name, by_folder };
    using SortAction = bool(*)(const dax& lhs, const dax& rhs);

    fs::path p_;

    dax(fs::path p)
        : p_(p) { }
    
    bool operator< (const dax& other)
        { return p_.compare(other.p_) == 0; }


    static bool do_sortby_name(const dax& lhs, const dax& rhs)
        { return lhs.p_.filename() < rhs.p_.filename(); }
    static bool do_sortby_folder(const dax& lhs, const dax& rhs)
        { return lhs.p_.filename() < rhs.p_.filename(); }
    static bool sortby(const dax& lhs, const dax& rhs, dax::SORT criterion)
        { return sort_menu[criterion](lhs, rhs); }
    template<typename... Args>
    static bool sortby(SORT criterion=by_name, Args&&... args)
        { return sort_menu[criterion]( std::forward<Args>(args)... ); }
    static SortAction sortby(SORT s)
        { return sort_menu[s]; }

    static SortAction sort_menu[];
};
const fs::path dax::ext_dax{".dax"};
const fs::path dax::ext_mcf{".mcf"};
const fs::path dax::ext_s19{".s19"};
dax::SortAction dax::sort_menu[]{dax::do_sortby_name, dax::do_sortby_folder};

static bool is_dax(fs::path p)
{ return fs::is_regular_file(p) && p.extension() == dax::ext_dax; }

int main(int argc, char* argv[])
{
    if(argc < 2){
        std::cerr << argv[0] << " input_dir\n";
        exit(EXIT_FAILURE);
    }

    fs::path p(argv[1]);

    try{
        if(fs::exists(p)){
            if(!fs::is_directory(p)){
                std::cerr << p << " is not a directory.\n";
                exit(EXIT_FAILURE);
            }

            typedef std::vector<std::shared_ptr<dax>> vec;
            vec vec_dax;
            std::for_each(fs::recursive_directory_iterator(p),
                     fs::recursive_directory_iterator(),
                     [&vec_dax](const fs::path& d)
                        {if(is_dax(d))
                            {vec_dax.emplace_back(std::make_shared<dax>(d));}
                        });

            std::sort(vec_dax.begin(), vec_dax.end(),
                      [](const shared_ptr<dax>& lhs, const shared_ptr<dax>& rhs)
                      {return dax::sortby(*lhs, *rhs, dax::by_name);});

            cout << "Total number of dax files: " << vec_dax.size() << endl;
            for(int i=0; i<11; ++i){
                cout << (*vec_dax[i]).p_ << endl;
            }
        }
    }
    catch(fs::filesystem_error& e){
        std::cerr << e.what() << endl;
    }
}
