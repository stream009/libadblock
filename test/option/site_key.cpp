#include "option.hpp"
#include "type.hpp"
#include "../mock_context.hpp"

#include <boost/range/algorithm.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/spirit/include/qi.hpp>

#include <gtest/gtest.h>

namespace adblock {

namespace qi = boost::spirit::qi;

struct SiteKeyContext : MockContext
{
    SiteKeyContext(const StringRange &siteKey)
        : m_siteKey { siteKey.begin(), siteKey.end() }
    {}

    StringRange siteKey() const override
    {
        return { m_siteKey.data(), m_siteKey.data() + m_siteKey.size() };
    }

    std::string m_siteKey;
};

TEST(SiteKeyOption, Init)
{
    const SiteKeyOption option {
        { "abcdsitekeydcba", "bcdesitekeyedcb", }
    };

    const auto &siteKeys = option.siteKeys();
    ASSERT_EQ(2, siteKeys.size());
    EXPECT_EQ(1, boost::count(siteKeys, "abcdsitekeydcba"));
    EXPECT_EQ(1, boost::count(siteKeys, "bcdesitekeyedcb"));
}

TEST(SiteKeyOption, Match)
{
    const SiteKeyOption option {
        { "abcdsitekeydcba", "bcdesitekeyedcb", }
    };

    const auto &uri = "http://whatever.idontcare.com"_u;
    const SiteKeyContext context { "bcdesitekeyedcb" };

    EXPECT_TRUE(option.match(uri, context));
}

TEST(SiteKeyOption, NoMatch)
{
    const SiteKeyOption option {
        { "abcdsitekeydcba", "bcdesitekeyedcb", }
    };

    const auto &uri = "http://whatever.idontcare.com"_u;
    const SiteKeyContext context { "foobarxyzzy" };

    EXPECT_FALSE(option.match(uri, context));
}

} // namespace adblock
