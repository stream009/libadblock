#include "rule/domain_data_base.hpp"

#include "core/string_range.hpp"
#include "core/uri.hpp"

#include <gtest/gtest.h>

using namespace adblock;

TEST(Main_DomainDataBase, Query)
{
    DomainDataBase db;
    Uri const uri { "http://www.adblock.org" };

    auto const& range = db.query(uri);

    EXPECT_EQ("adblock.org", range);
}

TEST(Main_DomainDataBase, QueryDeepDomain)
{
    DomainDataBase db;
    Uri const uri { "http://www.adblock.org.uk" };

    auto const& range = db.query(uri);

    EXPECT_EQ("adblock.org.uk", range);
}

TEST(Main_DomainDataBase, WrongQuery)
{
    DomainDataBase db;
    Uri const uri { "http://com/com/com" };

    auto const& range = db.query(uri);

    EXPECT_TRUE(range.empty());
}

TEST(Main_DomainDataBase, DISABLED_ValidUriButNoHost)
{
    DomainDataBase db;
    Uri const uri { "about:blank" }; // http_parser cant support this type of URI

    auto const& range = db.query(uri);

    EXPECT_TRUE(range.empty());
}
