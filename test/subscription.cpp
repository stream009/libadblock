#include "type.hpp"
#include "subscription.hpp"

#include <exception>

#include <boost/range/algorithm.hpp>

#include <gtest/gtest.h>

using namespace adblock;

namespace bfs = boost::filesystem;
static const bfs::path projectRoot { PROJECT_ROOT };

TEST(Subscription, Elementary)
{
    const auto &path = projectRoot / "test/data/elementary.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;
    Subscription subscription { path };

    const auto lines = boost::count(subscription, '\n');

    EXPECT_EQ(32, lines);
    EXPECT_EQ(31, subscription.rules().size());
}

TEST(Subscription, FileDoesNotExist)
{
    const auto &path = projectRoot / "test/data/xxxxx.txt";
    ASSERT_FALSE(bfs::exists(path)) << path;
    EXPECT_THROW(Subscription subscription { path }, std::ios_base::failure);
}

TEST(Subscription, WrongHeader)
{
    const auto &path = projectRoot / "test/data/wrong_header.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;
    EXPECT_THROW(Subscription subscription { path },
                 Subscription::ParseError);
}

TEST(Subscription, WrongVersion)
{
    const auto &path = projectRoot / "test/data/wrong_version.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;
    EXPECT_NO_THROW(Subscription subscription { path });
}

TEST(Subscription, Statistics)
{
    const auto &path = projectRoot / "test/data/elementary.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;
    Subscription subscription { path };

    const auto &stats = subscription.statistics();
    EXPECT_EQ(3, stats.basicFilterRule.basicMatchPattern);
    EXPECT_EQ(3, stats.basicFilterRule.domainMatchPattern);
    EXPECT_EQ(3, stats.basicFilterRule.regexPattern);
    EXPECT_EQ(9, stats.basicFilterRule.total);
    EXPECT_EQ(3, stats.exceptionFilterRule.basicMatchPattern);
    EXPECT_EQ(3, stats.exceptionFilterRule.domainMatchPattern);
    EXPECT_EQ(3, stats.exceptionFilterRule.regexPattern);
    EXPECT_EQ(9, stats.exceptionFilterRule.total);
    EXPECT_EQ(3, stats.basicElementHideRule.basic);
    EXPECT_EQ(3, stats.basicElementHideRule.domainRestricted);
    EXPECT_EQ(6, stats.basicElementHideRule.total);
    EXPECT_EQ(3, stats.exceptionElementHideRule.basic);
    EXPECT_EQ(3, stats.exceptionElementHideRule.domainRestricted);
    EXPECT_EQ(6, stats.exceptionElementHideRule.total);
    EXPECT_EQ(1, stats.commentRule);
}

TEST(Subscription, DISABLED_EasyList)
{
    const auto &path = projectRoot / "test/data/easylist.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;
    Subscription subscription { path };

    //const auto lines = boost::count(subscription, '\n');

    //EXPECT_EQ(46671, lines); // last line doesn't have '\n'
    //EXPECT_EQ(46671, subscription.rules().size());

    //std::cout << subscription.statistics();
}
