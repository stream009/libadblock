#include "../mock_context.hpp"

#include "core/adblock.hpp"
#include "core/context.hpp"
#include "core/filter_set.hpp"
#include "core/type.hpp"

#include <iostream>

#include <boost/range/algorithm.hpp>
#include <boost/timer/timer.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <gtest/gtest.h>

using namespace adblock;

namespace bfs = boost::filesystem;
static const bfs::path projectRoot { PROJECT_ROOT };

TEST(Core_AdBlock, Statistics)
{
    std::locale locale { "" };
    std::locale::global(locale);
    std::cout.imbue(locale);

    auto const& path = projectRoot / "test/data/easylist.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;

    AdBlock adBlock;
    adBlock.addFilterSet(path);

    auto const& stats = adBlock.statistics();
    //boost::property_tree::write_json(std::cout, stats);

    EXPECT_EQ(20757, stats.get<size_t>("Filter rule"));
    EXPECT_EQ(33565, stats.get<size_t>("Element hide rule"));
    EXPECT_EQ(54322, stats.get<size_t>("Total"));
}

TEST(Core_AdBlock, DISABLED_EasyList)
{
    auto const& path = projectRoot / "test/data/easylist.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;

    boost::timer::cpu_timer t;
    AdBlock adBlock;
    adBlock.addFilterSet(path);
    t.stop();
    std::cout << t.format();
}

TEST(Core_AdBlock, DISABLED_Fanboy)
{
    auto const& path = projectRoot / "test/data/fanboy.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;

    AdBlock adBlock;
    adBlock.addFilterSet(path);
}

TEST(Core_AdBlock, DISABLED_elementHideRule)
{
    auto const& path = projectRoot / "test/data/easylist.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;

    AdBlock adBlock;
    adBlock.addFilterSet(path);
    boost::timer::cpu_timer t;
    auto const& css = adBlock.elementHideCss("http://www.adblockplus.org/"_u);
    t.stop();
    std::cout << css.size() << "\n" << t.format();
}

TEST(Core_AdBlock, Reload)
{
    auto const& path = projectRoot / "test/data/easylist.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;

    boost::timer::cpu_timer t;

    AdBlock adBlock;
    adBlock.addFilterSet(path);

    t.stop();
    std::cout << t.format();

    auto stats = adBlock.statistics();
    EXPECT_EQ(20757, stats.get<size_t>("Filter rule"));
    EXPECT_EQ(33565, stats.get<size_t>("Element hide rule"));
    EXPECT_EQ(54322, stats.get<size_t>("Total"));

    t.start();

    adBlock.reload();

    t.stop();
    std::cout << t.format();

    stats = adBlock.statistics();
    EXPECT_EQ(20757, stats.get<size_t>("Filter rule"));
    EXPECT_EQ(33565, stats.get<size_t>("Element hide rule"));
    EXPECT_EQ(54322, stats.get<size_t>("Total"));
}

TEST(Core_AdBlock, FilterSets)
{
    AdBlock adBlock;

    auto const& path1 = projectRoot / "test/data/easylist.txt";
    ASSERT_TRUE(bfs::exists(path1)) << path1;

    adBlock.addFilterSet(path1);

    auto const& path2 = projectRoot / "test/data/fanboy.txt";
    ASSERT_TRUE(bfs::exists(path2)) << path2;

    adBlock.addFilterSet(path2);

    auto const& path3 = projectRoot / "test/data/customlist.txt";
    ASSERT_TRUE(bfs::exists(path3)) << path3;

    adBlock.addFilterSet(path3);

    auto const& filterSets = adBlock.filterSets();
    EXPECT_EQ(3, filterSets.size());

    auto it = boost::find_if(filterSets,
        [&](auto& filterSet) {
            return path1 == filterSet.path();
        });
    EXPECT_NE(it, filterSets.end());

    it = boost::find_if(filterSets,
        [&](auto& filterSet) {
            return path2 == filterSet.path();
        });
    EXPECT_NE(it, filterSets.end());

    it = boost::find_if(filterSets,
        [&](auto& filterSet) {
            return path3 == filterSet.path();
        });
    EXPECT_NE(it, filterSets.end());
}

TEST(Core_AdBlock, RemoveFilterSet)
{
    AdBlock adBlock;

    auto const& path1 = projectRoot / "test/data/easylist.txt";
    ASSERT_TRUE(bfs::exists(path1)) << path1;

    adBlock.addFilterSet(path1);

    auto const& path2 = projectRoot / "test/data/fanboy.txt";
    ASSERT_TRUE(bfs::exists(path2)) << path2;

    adBlock.addFilterSet(path2);

    EXPECT_EQ(2, adBlock.filterSets().size());

    auto const& beforeStats = adBlock.statistics();
    //std::cout << beforeStats.get<size_t>("Total") << "\n";
    ASSERT_EQ(66244, beforeStats.get<size_t>("Total"));

    auto it = boost::find_if(adBlock.filterSets(),
        [&](auto& filterSet) {
            return path1 == filterSet.path();
        });
    EXPECT_NE(it, adBlock.filterSets().end());

    adBlock.removeFilterSet(*it);

    EXPECT_EQ(1, adBlock.filterSets().size());

    auto const& afterStats = adBlock.statistics();
    //std::cout << afterStats.get<size_t>("Total") << "\n";
    ASSERT_EQ(11922, afterStats.get<size_t>("Total"));

    adBlock.removeFilterSet(adBlock.filterSets().front());
    EXPECT_EQ(0, adBlock.filterSets().size());
    ASSERT_EQ(0, adBlock.statistics().get<size_t>("Total"));
}

TEST(Core_AdBlock, FilterSet)
{
    AdBlock adBlock;

    auto const& path = projectRoot / "test/data/easylist.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;

    adBlock.addFilterSet(path);

    auto* const fs = adBlock.filterSet(path);
    ASSERT_TRUE(fs);

    EXPECT_EQ(path, fs->path());
}
