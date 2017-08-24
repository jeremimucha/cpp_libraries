/* Any archives included prior to 'myclasses.hpp
 * would also apply to the registration
 */
#include "myclasses.hpp"
#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>

#include <iostream>
#include <fstream>


int main()
{
    {
        std::ofstream ofs("polymorphism_test.xml");
        cereal::XMLOutputArchive oarchive( ofs );

        // Create instances of the derived classes, but only keep base class pointers
        std::shared_ptr<BaseClass> ptr1 = std::make_shared<DerivedClassOne>();
        std::shared_ptr<BaseClass> ptr2 = std::make_shared<EmbarrassingDerivedClass>();
        oarchive(ptr1, ptr2);
    }

    {
        std::ifstream ifs("polymorphism_test.xml");
        cereal::XMLInputArchive iarchive( ifs );

        // De-serialize the data as base class pointers, and watch as they are
        // re-instantiated as derived classes
        std::shared_ptr<BaseClass> ptr1;
        std::shared_ptr<BaseClass> ptr2;
        iarchive( ptr1, ptr2 );

        // Ta-da! this should output:
        ptr1->say_type();   // "DerivedClassOne"
        ptr2->say_type();   // "EmbarrassingDerivedClass. Wait... I mean DerivedClassTwo!"
    }

    return 0;
}
