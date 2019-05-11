#include "rule/domain_data_base.hpp"

#include "core/string_range.hpp"
#include "core/uri.hpp"

#include <gtest/gtest.h>

using namespace adblock;

TEST(Main_DomainDataBase, Query)
{
    Uri const uri { "http://www.adblock.org" };

    auto const& range = domain_db::getRegisteredDomain(uri);

    EXPECT_EQ("adblock.org", range);
}

TEST(Main_DomainDataBase, QueryDeepDomain)
{
    Uri const uri { "http://www.adblock.org.uk" };

    auto const& range = domain_db::getRegisteredDomain(uri);

    EXPECT_EQ("adblock.org.uk", range);
}

TEST(Main_DomainDataBase, WrongQuery)
{
    Uri const uri { "http://com/com/com" };

    auto const& range = domain_db::getRegisteredDomain(uri);

    EXPECT_TRUE(range.empty());
}

TEST(Main_DomainDataBase, DISABLED_ValidUriButNoHost)
{
    Uri const uri { "about:blank" }; // http_parser cant support this type of URI

    auto const& range = domain_db::getRegisteredDomain(uri);

    EXPECT_TRUE(range.empty());
}
