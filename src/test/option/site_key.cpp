#include "option/site_key_option.hpp"
#include "type.hpp"
#include "../mock_context.hpp"

#include <boost/range/algorithm.hpp>
#include <boost/range/iterator_range.hpp>

#include <gtest/gtest.h>

namespace adblock {

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
        { "abcdsitekeydcba"_r, "bcdesitekeyedcb"_r, }
    };

    const auto &siteKeys = option.siteKeys();
    ASSERT_EQ(2, siteKeys.size());
    EXPECT_EQ(1, boost::count(siteKeys, "abcdsitekeydcba"_r));
    EXPECT_EQ(1, boost::count(siteKeys, "bcdesitekeyedcb"_r));
}

TEST(SiteKeyOption, Match)
{
    const SiteKeyOption option {
        { "abcdsitekeydcba"_r, "bcdesitekeyedcb"_r, }
    };

    const auto &uri = "http://whatever.idontcare.com"_u;
    const SiteKeyContext context { "bcdesitekeyedcb"_r };

    EXPECT_TRUE(option.match(uri, context));
}

TEST(SiteKeyOption, NoMatch)
{
    const SiteKeyOption option {
        { "abcdsitekeydcba"_r, "bcdesitekeyedcb"_r, }
    };

    const auto &uri = "http://whatever.idontcare.com"_u;
    const SiteKeyContext context { "foobarxyzzy"_r };

    EXPECT_FALSE(option.match(uri, context));
}

} // namespace adblock
