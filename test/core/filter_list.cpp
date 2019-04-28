#include "core/filter_list.hpp"
#include "core/json.hpp"
#include "core/string_range.hpp"
#include "core/uri.hpp"

#include <filesystem>

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

    EXPECT_EQ(10, to_number(stats["Basic filter rule"]));
    EXPECT_EQ(9, to_number(stats["Exception filter rule"]));
    EXPECT_EQ(6, to_number(stats["Basic element hide rule"]));
    EXPECT_EQ(6, to_number(stats["Exception element hide rule"]));
    EXPECT_EQ(4, to_number(stats["Comment rule"]));
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

TEST(Core_FilterList, Errors)
{
    auto const& path = projectRoot / "test/data/error.txt";
    ASSERT_TRUE(fs::exists(path)) << path;
    FilterList filterList { path };

    auto const& errors = filterList.errors();

    ASSERT_EQ(2, errors.size());
    EXPECT_EQ(3, errors[0].line_no);
    EXPECT_EQ(3, errors[0].from);
    EXPECT_EQ(8, errors[0].to);
    EXPECT_EQ("ad$foobar", errors[0].line);
    EXPECT_EQ("invalid option", errors[0].message);

    EXPECT_EQ(4, errors[1].line_no);
    EXPECT_EQ(3, errors[1].from);
    EXPECT_EQ(8, errors[1].to);
    EXPECT_EQ("ad$domain", errors[1].line);
    EXPECT_EQ("option must have value", errors[1].message);
}
