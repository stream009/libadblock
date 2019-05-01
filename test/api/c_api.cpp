#include "../filter_file.hpp"

#include <adblock/api.h>

#include "core/json.hpp"

#include <cassert>
#include <filesystem>
#include <string_view>

#include <gtest/gtest.h>

namespace fs = std::filesystem;
static const fs::path projectRoot { PROJECT_ROOT };

using adblock::to_number;

static adblock_string_t
to_string_t(std::string_view const s)
{
    return { s.data(), s.size() };
}

static std::string_view
to_string_view(adblock_string_t const s)
{
    return { s.ptr, s.length };
}

class API_F : public ::testing::Test
{
public:
    static void SetUpTestCase()
    {
        m_db = ::adblock_db_new();
        ASSERT_TRUE(m_db);

        auto const& path1 = projectRoot / "test/data/easylist.txt";
        ASSERT_TRUE(fs::exists(path1)) << path1;

        adblock_error_t* err {};
        auto const p = to_string_t(path1.native());

        ::adblock_add_filter_list(m_db, &p, &err);

        ASSERT_FALSE(err);
    }

    static adblock_db_t* db() { return m_db; }

    static void TearDownTestCase()
    {
        ::adblock_db_free(m_db);
    }

    static adblock_db_t* m_db;
};

adblock_db_t* API_F::m_db = nullptr;


TEST_F(API_F, adblock_should_block)
{
    auto const origin = to_string_t("http://www.adblock.org");
    auto const url1 = to_string_t("http://a.kickass.to/sc-b98b537.js");
    adblock_string_t filter_list {}, reason {};
    adblock_error_t* err = nullptr;

    auto block = adblock_should_block(this->db(),
        &url1,
        ADBLOCK_REQUEST_SCRIPT,
        &origin,
        nullptr,
        &filter_list,
        &reason,
        &err
    );
    ASSERT_EQ(nullptr, err);

    ASSERT_TRUE(block);
    EXPECT_NE(nullptr, filter_list.ptr);
    EXPECT_NE(0, filter_list.length);
    EXPECT_NE(nullptr, reason.ptr);
    EXPECT_NE(0, reason.length);

    adblock_string_free(&filter_list);
    adblock_string_free(&reason);

    auto const url2 = to_string_t("http://barhbarhbarh.com");

    block = adblock_should_block(this->db(),
        &url2,
        ADBLOCK_REQUEST_SCRIPT,
        &origin,
        nullptr,
        &filter_list,
        &reason,
        &err
    );
    ASSERT_EQ(nullptr, err);

    ASSERT_FALSE(block);
    EXPECT_EQ(nullptr, filter_list.ptr);
    EXPECT_EQ(0, filter_list.length);
    EXPECT_EQ(nullptr, reason.ptr);
    EXPECT_EQ(0, reason.length);
}

TEST_F(API_F, adblock_element_hiding_selectors)
{
    auto const url = to_string_t("http://www.adblockplus.org/");
    adblock_str_array_t selectors;
    adblock_error_t *error = nullptr;

    adblock_element_hiding_selectors(this->db(),
        &url, nullptr, &selectors, &error
    );

    ASSERT_EQ(nullptr, error);
    ASSERT_NE(0, selectors.length);

    adblock_str_array_free(&selectors);
}

TEST_F(API_F, adblock_extended_element_hiding_selectors)
{
    auto const url = to_string_t("http://www.adblockplus.org/");
    adblock_str_array_t selectors {};
    adblock_error_t *error = nullptr;

    adblock_extended_element_hiding_selectors(this->db(),
        &url, nullptr, &selectors, &error
    );

    ASSERT_EQ(nullptr, error);
    ASSERT_EQ(0, selectors.length);
    ASSERT_EQ(nullptr, selectors.ptr);
}

TEST_F(API_F, adblock_content_security_policy)
{
    FilterFile fs;

    fs << "||adblock.com$csp=script-src 'self' * 'unsafe-inline' 'unsafe-eval'\n";
    fs << "@@||adblock.com/search=$csp=script-src 'self' * 'unsafe-inline' 'unsafe-eval'\n";
    fs << std::flush;

    auto const path = to_string_t(fs.path().native());
    auto const url = to_string_t("http://www.adblock.com");
    adblock_error_t* err = nullptr;

    auto const db = adblock_db_new();

    adblock_add_filter_list(db, &path, &err);
    ASSERT_EQ(nullptr, err);

    adblock_string_t policy {};

    adblock_content_security_policy(db, &url, nullptr, &policy, &err);
    ASSERT_EQ(nullptr, err);

    EXPECT_TRUE(to_string_view(policy) == "script-src 'self' * 'unsafe-inline' 'unsafe-eval'");

    adblock_remove_filter_list(db, &path, &err);
    ASSERT_EQ(nullptr, err);
}

TEST_F(API_F, adblock_snippets)
{
    auto const url = to_string_t("http://www.adblockplus.org/");
    adblock_str_array_t snippets {};
    adblock_error_t *error = nullptr;

    adblock_snippets(this->db(),
        &url, nullptr, &snippets, &error
    );

    ASSERT_EQ(nullptr, error);
    ASSERT_EQ(0, snippets.length);
    ASSERT_EQ(nullptr, snippets.ptr);
}

static bool
operator==(std::string_view const s1, adblock_string_t const s2)
{
    return s1 == std::string_view(s2.ptr, s2.length);
}

TEST_F(API_F, adblock_filter_list_parameters)
{
    auto const& p = projectRoot / "test/data/fanboy.txt";
    ASSERT_TRUE(fs::exists(p));

    auto const path = to_string_t(p.native());
    adblock_error_t* err = nullptr;

    adblock_add_filter_list(this->db(), &path, &err);
    ASSERT_EQ(nullptr, err);

    adblock_str_array_t keys {}, values {};

    adblock_filter_list_parameters(this->db(), &path, &keys, &values, &err);
    ASSERT_EQ(nullptr, err);

    EXPECT_EQ(5, keys.length);
    EXPECT_EQ(5, values.length);

    EXPECT_EQ("Checksum", keys.ptr[0]);
    EXPECT_EQ("PHWcd7bbJ8yORZTp1Xg3pQ", values.ptr[0]);

    EXPECT_EQ("Expires", keys.ptr[1]);
    EXPECT_EQ("4 days (update frequency)", values.ptr[1]);

    EXPECT_EQ("Homepage", keys.ptr[2]);
    EXPECT_EQ("https://easylist.adblockplus.org/", values.ptr[2]);

    EXPECT_EQ("Title", keys.ptr[3]);
    EXPECT_EQ("Fanboy's Social Blocking List", values.ptr[3]);

    EXPECT_EQ("Version", keys.ptr[4]);
    EXPECT_EQ("201505071651", values.ptr[4]);

    adblock_str_array_free(&keys);
    adblock_str_array_free(&values);

    adblock_remove_filter_list(this->db(), &path, &err);
    ASSERT_EQ(nullptr, err);
}

TEST(API_C, reload)
{
    auto const db = adblock_db_new();
    ASSERT_TRUE(db);

    FilterFile list;
    list << "ad\n";
    list << "##ad\n";
    list << std::flush;

    auto const path = to_string_t(list.path().native());
    adblock_error_t* err = nullptr;

    adblock_add_filter_list(db, &path, &err);
    ASSERT_EQ(nullptr, err);

    adblock_string_t json {};

    adblock_statistics(db, &json, &err);
    ASSERT_EQ(nullptr, err);

    auto const before = json::parse(to_string_view(json)).get_object();

    adblock_string_free(&json);

    ASSERT_EQ(2, to_number(before["Total"]));

    list << "foobar\n";
    list << "#@#ad\n";
    list << "##ac\n";
    list << std::flush;
    list.close();

    adblock_reload(db, &err);
    ASSERT_EQ(nullptr, err);

    adblock_statistics(db, &json, &err);
    ASSERT_EQ(nullptr, err);

    auto const after = json::parse(to_string_view(json)).get_object();

    adblock_string_free(&json);

    ASSERT_EQ(5, to_number(after["Total"]));
}

TEST_F(API_F, statistics)
{
    adblock_string_t json {};
    adblock_error_t* err = nullptr;

    adblock_statistics(this->db(), &json, &err);

    auto const stats = json::parse(to_string_view(json)).get_object();

    EXPECT_EQ(20757, to_number(stats["Filter rule"]));
    EXPECT_EQ(33565, to_number(stats["Element hide rule"]));
    EXPECT_EQ(54322, to_number(stats["Total"]));

    adblock_string_free(&json);
}

TEST_F(API_F, remove_filter_set)
{
    auto const& p = projectRoot / "test/data/fanboy.txt";
    ASSERT_TRUE(fs::exists(p));

    auto const path = to_string_t(p.native());
    adblock_error_t* err = nullptr;

    adblock_add_filter_list(this->db(), &path, &err);
    ASSERT_EQ(nullptr, err);

    adblock_string_t json {};

    adblock_statistics(this->db(), &json, &err);
    ASSERT_EQ(nullptr, err);

    auto const before = json::parse(to_string_view(json)).get_object();
    adblock_string_free(&json);

    EXPECT_EQ(66244, to_number(before["Total"]));

    adblock_remove_filter_list(this->db(), &path, &err);
    ASSERT_EQ(nullptr, err);

    adblock_statistics(this->db(), &json, &err);
    ASSERT_EQ(nullptr, err);

    auto const after = json::parse(to_string_view(json)).get_object();
    adblock_string_free(&json);

    EXPECT_EQ(54322, to_number(after["Total"]));
}

TEST_F(API_F, adblock_db_free)
{
    auto const db = adblock_db_new();
    EXPECT_NE(nullptr, db);

    adblock_db_free(db);
}
