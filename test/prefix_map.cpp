#include "type.hpp"
#include "prefix_map.hpp"

#include <boost/locale.hpp>

#include <gtest/gtest.h>

using namespace adblock;

TEST(PrefixMap, Basic)
{
    PrefixMap<StringRange, size_t> map;

    map["foo"_r] = 0u;
    map["xyzzy"_r] = 0u;

    const auto &range = map.match("foobar"_r);
    ASSERT_TRUE(!!range);
    EXPECT_EQ(1, std::distance(range->begin(), range->end()));
}

TEST(PrefixMap, CaseInsensitive)
{
    PrefixMap<StringRange, size_t> map;

    map["foo"_r] = 0u;
    map["xyzzy"_r] = 0u;

    const auto &range = map.match("Foobar"_r);
    ASSERT_TRUE(!!range);
    EXPECT_EQ(1, std::distance(range->begin(), range->end()));
}

TEST(PrefixMap, BoundaryIssue)
{
    PrefixMap<StringRange, size_t> map;

    map["/"_r] = 0u;
    map["/!"_r] = 0u;
    map["/0"_r] = 0u;
    map["/00"_r] = 0u;

    const auto &range = map.match("//abcd"_r);
    //const auto &range = map.match("/0"_r);
    ASSERT_TRUE(!!range);
    const auto size = std::distance(range->begin(), range->end());
    EXPECT_EQ(2, size);

    for (const auto &item: *range) {
        std::cout << item.first << "\n";
    }
}
