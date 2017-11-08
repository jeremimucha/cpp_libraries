#define CATCH_CONFIG_MAIN
// #include <gtest/gtest.h>


// TEST( MainTests, Dummy )
// {
//     EXPECT_EQ( 2, 2 );
//     // EXPECT_EQ( 2, 1 ) << "2 != 1";
// }

#include <catch.hpp>
#include <List.hpp>


TEST_CASE( "MainTests", "[dummy]" )
{
    // set up state for each SECTION
    List<int> li{ 1, 2, 3, 4, 5 };

    REQUIRE( li.size() == 5 );
}
