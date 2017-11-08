#include "LibraryName/LibraryHeader_B.hpp"
#include "gtest/gtest.h"

namespace
{
    
TEST( LibraryTests_B, Dummy ){
    ASSERT_EQ( 1, 2 ) << "1 != 2";
}

}   // namespace
