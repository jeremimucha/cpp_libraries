#include "example.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <memory>
#include <random>
#include <ctime>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>  // for vector serialization
#include <cereal/types/memory.hpp>  // for smart pointer serialization

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::ostream_iterator;
using std::shared_ptr;
using std::make_shared;
using std::placeholders::_1;

const char* const names[] = { "One", "Two", "Three", "Four", "Five",
                              "Six", "Seven", "Eight", "Nine", "Ten" };


namespace
{

std::default_random_engine rand_engine(std::time(nullptr));
std::uniform_int_distribution<unsigned> uniform_dist(0,3);

unsigned int randint() { return uniform_dist(rand_engine); }

template<typename Cont>
void print_container(const Cont& c, const char* msg="", bool newline=false)
{
    std::cout << msg;
    for(typename Cont::const_iterator it(c.cbegin()); it!=c.cend();/*  */){
        std::cout << *it;
        if( ++it != c.cend() )
            std::cout << ", ";
    }
    if(newline)
        std::cout << std::endl;
}

}


int main()
{
    typedef vector<shared_ptr<Base>> Vector;
    Vector vec;
    vec.reserve(10);

    for(size_t i=0; i<10; ++i){
        switch(randint())
        {
            case 0: vec.emplace_back(make_shared<Base>(names[i], i));
                    break;
            case 1: vec.emplace_back(make_shared<DerivedOne>(names[i], i)); 
                    break;
            case 2: vec.emplace_back(make_shared<DerivedTwo>(names[i], i));
                    break;
            case 3: vec.emplace_back(make_shared<DerivedMultiple>(names[i], i));
                    break;
            default: std::cerr << "something broke"; break;
        }
    }

    std::cout << "Before serialization: " << std::endl;
    for(Vector::const_iterator it(vec.cbegin()); it!=vec.cend(); ++it){
        (*it)->info();
        std::cout << std::endl;
    }

// !!!
    // Or just for fun use std::bind and a pointer to member function:
    // for_each(vec.cbegin(), vec.cend(), std::bind(&Base::info, _1));
// !!!

// --- Serialization ---
    // cereal uses RAII - archives are only safely flushed on their destruction
    // we can use a block for this purpose.
    {
        // serialize to xml
        std::ofstream ofs_xml("xml_archive.xml");
        cereal::XMLOutputArchive oarchive_xml(ofs_xml);
        oarchive_xml(vec);
    }

    // read serialized data
    std::cout << "\nAfter restoring the serialized data:\n\n";
    {
        std::ifstream ifs_xml("xml_archive.xml");
        cereal::XMLInputArchive iarchive_xml(ifs_xml);
        Vector vec2;
        iarchive_xml(vec2);
        // for(Vector::const_iterator it(vec2.cbegin()); it!=vec2.cend(); ++it){
        //     (*it)->info();
        //     std::cout << std::endl;
        // }
        std::for_each(vec2.cbegin(), vec2.cend(), std::bind(&Base::info, _1));
    }

    Blob blob;
    for( Vector::const_iterator it(vec.cbegin()); it!=vec.cend(); /*  */ ){
        blob.push_back(**it++);
    }
    // serialize blob
    {
        std::ofstream ofs_xml("archive_blob.xml");
        cereal::XMLOutputArchive oarchive(ofs_xml);
        oarchive( CEREAL_NVP(blob) );
        std::ofstream ofs_bin("archive_blob.bin");
        cereal::BinaryOutputArchive obinarchive(ofs_bin);
        obinarchive(blob);
    }

    return 0;
}
