#include "type.hpp"
#include "subscription.hpp"

#include <boost/range/algorithm.hpp>

#include <gtest/gtest.h>

using namespace adblock;

namespace bfs = boost::filesystem;
static const bfs::path projectRoot { PROJECT_ROOT };

TEST(Subscription, EasyList)
{
    const auto &path = projectRoot / "test/data/easylist.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;
    Subscription subscription { path };

    const auto lines = boost::count(subscription, '\n');

    EXPECT_EQ(46671, lines); // last line doesn't have '\n'
    EXPECT_EQ(46671, subscription.rules().size());
}
