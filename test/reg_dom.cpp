#include <gtest/gtest.h>

#include <boost/algorithm/string.hpp>

extern "C" {
#include <reg_dom/regdom.h>
};

TEST(LibRegDom, LoadAndFreeTree)
{
    auto *tree = loadTldTree();
    ASSERT_TRUE(!!tree);

    freeTldTree(tree);
}

TEST(LibRegDom, OriginalAPI)
{
    auto *tree = loadTldTree();
    ASSERT_TRUE(!!tree);

    namespace ba = boost::algorithm;
    {
        const auto *host = "www.adblock.org";
        const auto *domain = getRegisteredDomain(host, tree);
        EXPECT_TRUE(ba::equals("adblock.org", domain)) << domain;
    }
    {
        const auto *host = "some.random.deep.domain.com";
        const auto *domain = getRegisteredDomain(host, tree);
        EXPECT_TRUE(ba::equals("domain.com", domain)) << domain;
    }
    {
        const auto *host = "non.existent.brah";
        const auto *domain = getRegisteredDomain(host, tree);
        EXPECT_TRUE(ba::equals("existent.brah", domain)) << domain;
    }
    {
        const auto *host = "non.existent.brah";
        const auto *domain = getRegisteredDomainDrop(host, tree, true);
        EXPECT_EQ(nullptr, domain) << domain;
    }

    freeTldTree(tree);
}

TEST(LibRegDom, CustomAPI)
{
    auto *tree = loadTldTree();
    ASSERT_TRUE(!!tree);

    {
        const auto host = boost::as_literal("www.adblock.org");
        const auto *domain = getRegisteredDomain_(
                                  host.begin(), host.size(), tree);
        namespace ba = boost::algorithm;
        EXPECT_TRUE(ba::equals("adblock.org", domain)) << domain;
    }

    freeTldTree(tree);
}
