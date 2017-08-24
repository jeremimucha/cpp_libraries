#include "example.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <memory>
#include <random>
#include <ctime>
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
std::uniform_int_distribution<unsigned> uniform_dist(0,2);

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
    typedef vector<shared_ptr<AbstractBase>> Vector;
    Vector vec;
    vec.reserve(10);

    for(size_t i=0; i<10; ++i){
        switch(randint())
        {
            case 0: vec.emplace_back(make_shared<DerivedOne>(names[i], i)); 
                    break;
            case 1: vec.emplace_back(make_shared<DerivedTwo>(names[i], i));
                    break;
            case 2: vec.emplace_back(make_shared<DerivedMultiple>(names[i], i));
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
    // for_each(vec.cbegin(), vec.cend(), std::bind(&AbstractBase::info, _1));
// !!!
}
