#include "core/filter_list.hpp"
#include "core/string_range.hpp"
#include "core/uri.hpp"

#include <exception>
#include <filesystem>

#include <boost/property_tree/json_parser.hpp>
#include <boost/range/algorithm.hpp>

#include <gtest/gtest.h>

using namespace adblock;

namespace fs = std::filesystem;
static const fs::path projectRoot { PROJECT_ROOT };

TEST(Core_FilterList, Elementary)
{
    auto const& path = projectRoot / "test/data/elementary.txt";
    ASSERT_TRUE(fs::exists(path)) << path;
    FilterList filterList { path };

    auto const lines = boost::count(filterList, '\n');

    EXPECT_EQ(39, lines);
    EXPECT_EQ(37, filterList.rules().size());
}

TEST(Core_FilterList, FileDoesNotExist)
{
    auto const& path = projectRoot / "test/data/xxxxx.txt";
    ASSERT_FALSE(fs::exists(path)) << path;
    EXPECT_THROW(FilterList filterList { path }, std::ios_base::failure);
}

TEST(Core_FilterList, WrongVersion)
{
    auto const& path = projectRoot / "test/data/wrong_version.txt";
    ASSERT_TRUE(fs::exists(path)) << path;
    EXPECT_NO_THROW(FilterList filterList { path });
}

TEST(Core_FilterList, Statistics)
{
    auto const& path = projectRoot / "test/data/elementary.txt";
    ASSERT_TRUE(fs::exists(path)) << path;
    FilterList filterList { path };

    auto const& stats = filterList.statistics();

    EXPECT_EQ(10, stats.get<size_t>("Basic filter rule"));
    EXPECT_EQ(9, stats.get<size_t>("Exception filter rule"));
    EXPECT_EQ(6, stats.get<size_t>("Basic element hide rule"));
    EXPECT_EQ(6, stats.get<size_t>("Exception element hide rule"));
    EXPECT_EQ(4, stats.get<size_t>("Comment rule"));
}

TEST(Core_FilterList, DISABLED_EasyList)
{
    auto const& path = projectRoot / "test/data/easylist.txt";
    ASSERT_TRUE(fs::exists(path)) << path;
    FilterList filterList { path };

    //const auto lines = boost::count(filterList, '\n');

    //EXPECT_EQ(46671, lines); // last line doesn't have '\n'
    //EXPECT_EQ(46671, filterList.rules().size());

    //boost::property_tree::write_json(std::cout, filterList.statistics());
}

TEST(Core_FilterList, Parameter)
{
    auto const& path = projectRoot / "test/data/easylist.txt";
    ASSERT_TRUE(fs::exists(path)) << path;
    FilterList filterList { path };

    auto&& params = filterList.parameters();

    ASSERT_EQ(5, params.size());
    EXPECT_EQ("bj7+igbVYKyRz2SssbmDOw"_r, params["Checksum"_r]);
    EXPECT_EQ("4 days (update frequency)"_r, params["Expires"_r]);
    EXPECT_EQ("https://easylist.adblockplus.org/"_r, params["Homepage"_r]);
    EXPECT_EQ("EasyList"_r, params["Title"_r]);
    EXPECT_EQ("201505100550"_r, params["Version"_r]);
}
