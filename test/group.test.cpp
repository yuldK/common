#include "gtest/gtest.h"
#include "code/group.h"

using namespace ycl;
TEST(group, all)
{
    constexpr const bool a = false;
    constexpr const bool b = false;

    ASSERT_TRUE(group::all(a, b) == false);
}

TEST(group, any)
{
    constexpr const bool a = false;
    constexpr const bool b = true;

    ASSERT_TRUE(group::any(a, b) == true);
}

TEST(group, any_num)
{
    constexpr const int a = 1;
    constexpr const int b = 2;

    ASSERT_TRUE(1 == group::any(a, b));
}

#include <string_view>
#include <vector>

TEST(group, all_num)
{
    const size_t a{ 1 };
    const std::string_view b{ "a" };
    const std::vector<int> c{ 1 };

    ASSERT_TRUE(1ULL == group::all(a, b.length(), c.size()));
}
