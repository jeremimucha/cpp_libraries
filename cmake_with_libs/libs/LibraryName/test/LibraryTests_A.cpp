#include "LibraryName/LibraryHeader_A.hpp"
#include "gtest/gtest.h"

namespace
{
    
TEST( LibraryTests_A, Dummy ){
    ASSERT_EQ( 1, 2 ) << "1 != 2";
}

}   // namespace
