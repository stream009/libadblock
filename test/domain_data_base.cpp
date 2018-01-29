#include "option/domain_data_base.hpp"

#include <boost/range/algorithm.hpp>

#include <gtest/gtest.h>

using namespace adblock;

TEST(DomainDataBase, Query)
{
    DomainDataBase db;
    const Uri uri { "http://www.adblock.org" };

    const auto &range = db.query(uri);

    EXPECT_TRUE(boost::equals("adblock.org", range));
}

TEST(DomainDataBase, QueryDeepDomain)
{
    DomainDataBase db;
    const Uri uri { "http://www.adblock.org.uk" };

    const auto &range = db.query(uri);

    EXPECT_TRUE(boost::equals("adblock.org.uk", range));
}

TEST(DomainDataBase, WrongQuery)
{
    DomainDataBase db;
    const Uri uri { "http://com/com/com" };

    const auto &range = db.query(uri);

    EXPECT_TRUE(range.empty());
}

TEST(DomainDataBase, DISABLED_ValidUriButNoHost)
{
    DomainDataBase db;
    const Uri uri { "about:blank" }; // http_parser cant support this type of URI

    EXPECT_TRUE(uri.is_valid());
    const auto &range = db.query(uri);

    EXPECT_TRUE(range.empty());
}
