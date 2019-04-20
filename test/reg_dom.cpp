#include <string_view>

#include <boost/algorithm/string.hpp>

#include <gtest/gtest.h>

extern "C" {
#include "reg_dom/regdom.h"
};

TEST(Main_LibRegDom, LoadAndFreeTree)
{
    auto *tree = loadTldTree();
    ASSERT_TRUE(!!tree);

    freeTldTree(tree);
}

TEST(Main_LibRegDom, OriginalAPI)
{
    auto *tree = loadTldTree();
    ASSERT_TRUE(!!tree);

    namespace ba = boost::algorithm;
    {
        auto const host = "www.adblock.org";
        auto const domain = getRegisteredDomain(host, tree);
        EXPECT_TRUE(ba::equals("adblock.org", domain)) << domain;
    }
    {
        auto const host = "some.random.deep.domain.com";
        auto const domain = getRegisteredDomain(host, tree);
        EXPECT_TRUE(ba::equals("domain.com", domain)) << domain;
    }
    {
        auto const host = "non.existent.brah";
        auto const domain = getRegisteredDomain(host, tree);
        EXPECT_TRUE(ba::equals("existent.brah", domain)) << domain;
    }
    {
        auto const host = "non.existent.brah";
        auto const domain = getRegisteredDomainDrop(host, tree, true);
        EXPECT_EQ(nullptr, domain) << domain;
    }

    freeTldTree(tree);
}

TEST(Main_LibRegDom, CustomAPI)
{
    namespace ba = boost::algorithm;

    auto *tree = loadTldTree();
    ASSERT_TRUE(!!tree);

    {
        std::string_view const host = "www.adblock.org";
        auto const domain = getRegisteredDomain_(
                                  host.begin(), host.size(), tree);
        EXPECT_TRUE(ba::equals("adblock.org", domain)) << domain;
    }

    freeTldTree(tree);
}
