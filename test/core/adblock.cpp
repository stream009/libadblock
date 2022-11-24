#include "../filter_file.hpp"
#include "../mock_context.hpp"

#include "core/adblock.hpp"
#include "core/context.hpp"
#include "core/filter_list.hpp"
#include "core/json.hpp"
#include "core/uri.hpp"

#include <filesystem>

#include <gtest/gtest.h>

using namespace adblock;

namespace fs = std::filesystem;
static const fs::path projectRoot { PROJECT_ROOT };

TEST(Core_AdBlock, AddFilterList)
{
    AdBlock adblock;

    auto const path1 = projectRoot / "test/data/elementary.txt";
    ASSERT_TRUE(fs::exists(path1)) << path1;

    EXPECT_FALSE(adblock.filterList(path1));
    adblock.addFilterList(path1);
    EXPECT_TRUE(adblock.filterList(path1));
}


TEST(Core_AdBlock, RemoveFilterList)
{
    AdBlock adblock;

    auto const path = projectRoot / "test/data/elementary.txt";
    ASSERT_TRUE(fs::exists(path));

    EXPECT_FALSE(adblock.filterList(path));
    adblock.addFilterList(path);
    EXPECT_TRUE(adblock.filterList(path));

    adblock.removeFilterList(path);
    EXPECT_FALSE(adblock.filterList(path));
}

TEST(Core_AdBlock, ShouldBlock)
{
    AdBlock adblock;

    auto const path = projectRoot / "test/data/elementary.txt";
    ASSERT_TRUE(fs::exists(path));

    adblock.addFilterList(path);

    struct Cxt : Context {
        bool fromScriptTag() const override { return true; }
        Uri const& origin() const override { return m_origin; }
        Uri m_origin { "http://example.com" };
    } cxt;

    auto const [block, rule] =
        adblock.shouldBlock("http://example.com/?&ad_box_"_u, cxt);
    EXPECT_TRUE(block);
    ASSERT_TRUE(rule);
    ASSERT_TRUE(rule->filterList());
    EXPECT_EQ(path, rule->filterList()->path());
    EXPECT_EQ("&ad_box_", rule->line());
}

TEST(Core_AdBlock, ElementHideSelectors)
{
    AdBlock adblock;

    auto const path = projectRoot / "test/data/elementary.txt";
    ASSERT_TRUE(fs::exists(path));

    adblock.addFilterList(path);

    auto rules = adblock.elementHideSelectors("http://example.com"_u);
    ASSERT_EQ(3, rules.size());

    rules = adblock.elementHideSelectors("http://aol.com"_u);
    ASSERT_EQ(4, rules.size());
}

TEST(Core_AdBlock, ExtendedElementHideSelectors)
{
    AdBlock adblock;

    auto const path = projectRoot / "test/data/elementary.txt";
    ASSERT_TRUE(fs::exists(path));

    adblock.addFilterList(path);

    auto const rules =
        adblock.extendedElementHideSelectors("http://aliexpress.com"_u);
    ASSERT_EQ(1, rules.size());
}

TEST(Core_AdBlock, ContentSecurityPolicy)
{
    AdBlock adblock;

    auto const path = projectRoot / "test/data/elementary.txt";
    ASSERT_TRUE(fs::exists(path));

    adblock.addFilterList(path);

    auto const policy = adblock.contentSecurityPolicy("http://example.com");

    EXPECT_EQ("worker-src 'none'", policy);
}

TEST(Core_AdBlock, Snippets)
{
    AdBlock adblock;

    auto const path = projectRoot / "test/data/elementary.txt";
    ASSERT_TRUE(fs::exists(path));

    adblock.addFilterList(path);

    auto const rules = adblock.snippets("http://example.com");
    ASSERT_EQ(1, rules.size());
}

TEST(Core_AdBlock, Statistics)
{
    auto const& path = projectRoot / "test/data/easylist.txt";
    ASSERT_TRUE(fs::exists(path));

    AdBlock adBlock;
    adBlock.addFilterList(path);

    auto const& stats = adBlock.statistics();

    EXPECT_EQ(20757, to_number(stats.at("Filter rule")));
    EXPECT_EQ(33565, to_number(stats.at("Element hide rule")));
    EXPECT_EQ(0,     to_number(stats.at("Snippet rule")));
    EXPECT_EQ(54322, to_number(stats.at("Total")));
}

TEST(Core_AdBlock, Reload)
{
    FilterFile list;
    list << "ad\n";
    list << "##ad\n";
    list << std::flush;

    auto const path = list.path();

    AdBlock adblock;
    adblock.addFilterList(path);

    auto stats = adblock.statistics();
    ASSERT_EQ(2, to_number(stats.at("Total")));

    list << "foobar\n";
    list << "#@#ad\n";
    list << "##ac\n";
    list << std::flush;
    list.close();

    adblock.reload();

    stats = adblock.statistics();

    ASSERT_EQ(5, to_number(stats.at("Total")));
}

TEST(Core_AdBlock, FilterList)
{
    AdBlock adBlock;

    auto const& path = projectRoot / "test/data/elementary.txt";
    ASSERT_TRUE(fs::exists(path)) << path;

    adBlock.addFilterList(path);

    auto* const fs = adBlock.filterList(path);
    ASSERT_TRUE(fs);

    EXPECT_EQ(path, fs->path());
}
