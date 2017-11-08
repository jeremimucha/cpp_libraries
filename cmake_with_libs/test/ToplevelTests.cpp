#include "gtest/gtest.h"


namespace
{

TEST( TopLevelTests, Dummy ){
    ASSERT_EQ( 1, 2 ) << "1 != 2";
}

} // namespace
