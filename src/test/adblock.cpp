#include "mock_context.hpp"

#include "adblock.hpp"
#include "context.hpp"
#include "type.hpp"

#include <iostream>

#include <boost/range/algorithm.hpp>
#include <boost/timer/timer.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <gtest/gtest.h>

using namespace adblock;

namespace bfs = boost::filesystem;
static const bfs::path projectRoot { PROJECT_ROOT };

TEST(AdBlock, Statistics)
{
    std::locale locale { "" };
    std::locale::global(locale);
    std::cout.imbue(locale);

    const auto &path = projectRoot / "test/data/easylist.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;

    AdBlock adBlock;
    adBlock.addFilterSet(path);

    const auto &stats = adBlock.statistics();
    //boost::property_tree::write_json(std::cout, stats);

    EXPECT_EQ(20757, stats.get<size_t>("Filter rule"));
    EXPECT_EQ(33565, stats.get<size_t>("Element hide rule"));
    EXPECT_EQ(54322, stats.get<size_t>("Total"));
}

TEST(AdBlock, DISABLED_EasyList)
{
    const auto &path = projectRoot / "test/data/easylist.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;

    boost::timer::cpu_timer t;
    AdBlock adBlock;
    adBlock.addFilterSet(path);
    t.stop();
    std::cout << t.format();
}

TEST(AdBlock, DISABLED_Fanboy)
{
    const auto &path = projectRoot / "test/data/fanboy.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;

    AdBlock adBlock;
    adBlock.addFilterSet(path);
}

TEST(AdBlock, DISABLED_elementHideRule)
{
    const auto &path = projectRoot / "test/data/easylist.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;

    AdBlock adBlock;
    adBlock.addFilterSet(path);
    boost::timer::cpu_timer t;
    const auto &css = adBlock.elementHideCss("http://www.adblockplus.org/"_u);
    t.stop();
    std::cout << css.size() << "\n" << t.format();
}

TEST(AdBlock, Reload)
{
    const auto &path = projectRoot / "test/data/easylist.txt";
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

TEST(AdBlock, FilterSets)
{
    AdBlock adBlock;

    const auto &path1 = projectRoot / "test/data/easylist.txt";
    ASSERT_TRUE(bfs::exists(path1)) << path1;

    adBlock.addFilterSet(path1);

    const auto &path2 = projectRoot / "test/data/fanboy.txt";
    ASSERT_TRUE(bfs::exists(path2)) << path2;

    adBlock.addFilterSet(path2);

    const auto &path3 = projectRoot / "test/data/customlist.txt";
    ASSERT_TRUE(bfs::exists(path3)) << path3;

    adBlock.addFilterSet(path3);

    const auto &filterSets = adBlock.filterSets();
    EXPECT_EQ(3, filterSets.size());

    auto it = boost::find_if(filterSets,
        [&](const FilterSet &filterSet) {
            return path1 == filterSet.path();
        });
    EXPECT_NE(it, filterSets.end());

    it = boost::find_if(filterSets,
        [&](const FilterSet &filterSet) {
            return path2 == filterSet.path();
        });
    EXPECT_NE(it, filterSets.end());

    it = boost::find_if(filterSets,
        [&](const FilterSet &filterSet) {
            return path3 == filterSet.path();
        });
    EXPECT_NE(it, filterSets.end());
}

TEST(AdBlock, RemoveFilterSet)
{
    AdBlock adBlock;

    const auto &path1 = projectRoot / "test/data/easylist.txt";
    ASSERT_TRUE(bfs::exists(path1)) << path1;

    adBlock.addFilterSet(path1);

    const auto &path2 = projectRoot / "test/data/fanboy.txt";
    ASSERT_TRUE(bfs::exists(path2)) << path2;

    adBlock.addFilterSet(path2);

    EXPECT_EQ(2, adBlock.filterSets().size());

    const auto &beforeStats = adBlock.statistics();
    //std::cout << beforeStats.get<size_t>("Total") << "\n";
    ASSERT_EQ(66244, beforeStats.get<size_t>("Total"));

    auto it = boost::find_if(adBlock.filterSets(),
        [&](const FilterSet &filterSet) {
            return path1 == filterSet.path();
        });
    EXPECT_NE(it, adBlock.filterSets().end());

    adBlock.removeFilterSet(*it);

    EXPECT_EQ(1, adBlock.filterSets().size());

    const auto &afterStats = adBlock.statistics();
    //std::cout << afterStats.get<size_t>("Total") << "\n";
    ASSERT_EQ(11922, afterStats.get<size_t>("Total"));
}
