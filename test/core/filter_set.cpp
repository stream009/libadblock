#include "core/type.hpp"
#include "core/filter_set.hpp"

#include <exception>

#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/range/algorithm.hpp>

#include <gtest/gtest.h>

using namespace adblock;

namespace bfs = boost::filesystem;
static const bfs::path projectRoot { PROJECT_ROOT };

TEST(Core_FilterSet, Elementary)
{
    const auto &path = projectRoot / "test/data/elementary.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;
    FilterSet filterSet { path };

    const auto lines = boost::count(filterSet, '\n');

    EXPECT_EQ(32, lines);
    EXPECT_EQ(31, filterSet.rules().size());
}

TEST(Core_FilterSet, FileDoesNotExist)
{
    const auto &path = projectRoot / "test/data/xxxxx.txt";
    ASSERT_FALSE(bfs::exists(path)) << path;
    EXPECT_THROW(FilterSet filterSet { path }, std::ios_base::failure);
}

TEST(Core_FilterSet, WrongVersion)
{
    const auto &path = projectRoot / "test/data/wrong_version.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;
    EXPECT_NO_THROW(FilterSet filterSet { path });
}

TEST(Core_FilterSet, Statistics)
{
    const auto &path = projectRoot / "test/data/elementary.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;
    FilterSet filterSet { path };

    const auto &stats = filterSet.statistics();

    EXPECT_EQ(9, stats.get<size_t>("Basic filter rule"));
    EXPECT_EQ(9, stats.get<size_t>("Exception filter rule"));
    EXPECT_EQ(6, stats.get<size_t>("Basic element hide rule"));
    EXPECT_EQ(6, stats.get<size_t>("Exception element hide rule"));
    EXPECT_EQ(1, stats.get<size_t>("Comment rule"));

    //boost::property_tree::write_json(std::cout, stats);
}

TEST(Core_FilterSet, DISABLED_EasyList)
{
    const auto &path = projectRoot / "test/data/easylist.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;
    FilterSet filterSet { path };

    //const auto lines = boost::count(filterSet, '\n');

    //EXPECT_EQ(46671, lines); // last line doesn't have '\n'
    //EXPECT_EQ(46671, filterSet.rules().size());

    //boost::property_tree::write_json(std::cout, filterSet.statistics());
}

TEST(Core_FilterSet, Parameter)
{
    const auto &path = projectRoot / "test/data/easylist.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;
    FilterSet filterSet { path };

    auto&& params = filterSet.parameters();

    ASSERT_EQ(5, params.size());
    EXPECT_EQ("bj7+igbVYKyRz2SssbmDOw"_r, params["Checksum"_r]);
    EXPECT_EQ("4 days (update frequency)"_r, params["Expires"_r]);
    EXPECT_EQ("https://easylist.adblockplus.org/"_r, params["Homepage"_r]);
    EXPECT_EQ("EasyList"_r, params["Title"_r]);
    EXPECT_EQ("201505100550"_r, params["Version"_r]);
}
