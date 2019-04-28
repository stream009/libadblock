#include <adblock/database.hpp>

#include "core/json.hpp"

#include <iostream>

#include <gtest/gtest.h>

namespace fs = std::filesystem;

using namespace adblock;

static fs::path
dataPath(std::string_view const filePath)
{
    static fs::path const root { PROJECT_ROOT };

    return root / filePath;
}

TEST(Api_Database, AddFilterList)
{
    adblock::database db;

    auto const path1 = dataPath("test/data/elementary.txt");
    ASSERT_TRUE(fs::exists(path1)) << path1;

    auto const before = json::parse(db.statistics()).get_object();
    EXPECT_EQ(0, to_number(before["Total"]));

    db.add_filter_list(path1);

    auto const after = json::parse(db.statistics()).get_object();
    EXPECT_EQ(34, to_number(after["Total"]));
}

TEST(Api_Database, RemoveFilterList)
{
    adblock::database db;

    auto const path1 = dataPath("test/data/elementary.txt");
    ASSERT_TRUE(fs::exists(path1));

    db.add_filter_list(path1);

    auto const before = json::parse(db.statistics()).get_object();
    EXPECT_EQ(34, to_number(before["Total"]));

    db.remove_filter_list(path1);

    auto const after = json::parse(db.statistics()).get_object();
    EXPECT_EQ(0, to_number(after["Total"]));
}

TEST(Api_Database, ShouldBlockRequest)
{
    adblock::database db;

    auto const path1 = dataPath("test/data/elementary.txt");
    ASSERT_TRUE(fs::exists(path1));

    db.add_filter_list(path1);

    auto const result = db.should_block_request(
        "http://example.com/?&ad_box_",
        adblock::request_type::other,
        "http://example.com"
    );

    EXPECT_TRUE(result.block);
    EXPECT_EQ("&ad_box_", result.filter);

    ASSERT_TRUE(result.filter_list);
    EXPECT_EQ(path1, *result.filter_list);
}

TEST(Api_Database, ElementHidingSelectors)
{
    adblock::database db;

    auto const path1 = dataPath("test/data/elementary.txt");
    ASSERT_TRUE(fs::exists(path1));

    db.add_filter_list(path1);

    auto const selectors1 = db.element_hiding_selectors(
        "http://example.com"
    );
    EXPECT_EQ(3, selectors1.size());

    auto const selectors2 = db.element_hiding_selectors(
        "http://aol.com"
    );
    EXPECT_EQ(4, selectors2.size());
}

TEST(Api_Database, ExtendedElementHidingSelectors)
{
    adblock::database db;

    auto const path1 = dataPath("test/data/elementary.txt");
    ASSERT_TRUE(fs::exists(path1));

    db.add_filter_list(path1);

    auto const selectors1 = db.extended_element_hiding_selectors(
        "http://aliexpress.com"
    );
    EXPECT_EQ(1, selectors1.size());
}

TEST(Api_Database, ContentSecurityPolicy)
{
    adblock::database db;

    auto const path1 = dataPath("test/data/elementary.txt");
    ASSERT_TRUE(fs::exists(path1));

    db.add_filter_list(path1);

    auto const policy = db.content_security_policy(
        "http://example.com"
    );
    EXPECT_EQ("worker-src 'none'", policy);
}

TEST(Api_Database, Snippets)
{
    adblock::database db;

    auto const path1 = dataPath("test/data/elementary.txt");
    ASSERT_TRUE(fs::exists(path1));

    db.add_filter_list(path1);

    auto const rules = db.snippets(
        "http://example.com"
    );
    EXPECT_EQ(1, rules.size());
}

TEST(Api_Database, FilterListParameters)
{
    adblock::database db;

    auto const path1 = dataPath("test/data/elementary.txt");
    ASSERT_TRUE(fs::exists(path1));

    db.add_filter_list(path1);

    auto const params = db.filter_list_parameters(path1);
    ASSERT_EQ(1, params.size());
    EXPECT_EQ("Title", params[0].key);
    EXPECT_EQ("Elementary", params[0].value);
}

TEST(Api_Database, FilterListErrors)
{
    adblock::database db;

    auto const path1 = dataPath("test/data/error.txt");
    ASSERT_TRUE(fs::exists(path1));

    db.add_filter_list(path1);

    auto const errors = db.filter_list_errors(path1);
    ASSERT_EQ(2, errors.size());
}
