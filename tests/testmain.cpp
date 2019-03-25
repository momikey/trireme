#define BOOST_TEST_MODULE main test
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(dummy)
{
    BOOST_TEST(1 + 1 == 2);
}