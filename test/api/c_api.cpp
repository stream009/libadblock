#include "../filter_file.hpp"

#include <adblock/api.h>

#include "core/json.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string_view>

#include <gtest/gtest.h>

namespace fs = std::filesystem;
static const fs::path projectRoot { PROJECT_ROOT };

using adblock::to_number;

class API_F : public ::testing::Test
{
public:
    static void SetUpTestCase()
    {
        m_adblock = ::adblock_create();
        ASSERT_TRUE(m_adblock);

        auto const& path1 = projectRoot / "test/data/easylist.txt";
        ASSERT_TRUE(fs::exists(path1)) << path1;

        ::adblock_add_filter_set(m_adblock, path1.c_str(), strlen(path1.c_str()));
    }

    static adblock_t adblock() { return m_adblock; }

    static void TearDownTestCase()
    {
        ::adblock_destroy(m_adblock);
    }

    static adblock_t m_adblock;
};

adblock_t API_F::m_adblock = nullptr;

TEST_F(API_F, adblock_should_block)
{
    adblock_context_t context;
    memset(&context, 0, sizeof(context));

    context.origin = "http://www.adblock.org";
    context.origin_len = strlen(context.origin);

    context.content_type = TYPE_SCRIPT;

    std::string const uri1 { "http://a.kickass.to/sc-b98b537.js" };

    char const* filterList = nullptr;
    size_t filterListLen = 0;
    char const* reason = nullptr;
    size_t reasonLen = 0;
    bool rv = ::adblock_should_block(this->adblock(),
                uri1.c_str(), uri1.size(), &context,
                &filterList, &filterListLen, &reason, &reasonLen);
    ASSERT_TRUE(rv);
    EXPECT_NE(nullptr, filterList);
    EXPECT_NE(0, filterListLen);
    EXPECT_NE(nullptr, reason);
    EXPECT_NE(0, reasonLen);

    context.content_type = TYPE_DOCUMENT;

    std::string const uri2 { "http://barhbarhbarh.com" };
    rv = ::adblock_should_block(this->adblock(),
                uri2.c_str(), uri2.size(), &context,
                &filterList, &filterListLen, &reason, &reasonLen);
    ASSERT_FALSE(rv);
    EXPECT_EQ(nullptr, filterList);
    EXPECT_EQ(0, filterListLen);
    EXPECT_EQ(nullptr, reason);
    EXPECT_EQ(0, reasonLen);
}

TEST_F(API_F, adblock_element_hide_css)
{
    std::string const uri { "http://www.adblockplus.org/" };

    char const* css = nullptr;
    size_t cssLen = 0;

    ::adblock_element_hide_css(
            this->adblock(),
            uri.data(), uri.size(),
            &css, &cssLen
    );

    ASSERT_NE(0, cssLen);
    adblock_free(css);
}

TEST_F(API_F, adblock_free)
{
    std::string const uri1 { "http://www.adblockplus.org/" };
    char const* css1 = nullptr;
    size_t css1Len = 0;

    adblock_element_hide_css(
            this->adblock(),
            uri1.data(), uri1.size(),
            &css1, &css1Len
    );
    ASSERT_NE(0, css1Len);

    std::string const uri2 { "http://www.google.org/" };
    char const* css2 = nullptr;
    size_t css2Len = 0;

    adblock_element_hide_css(
            this->adblock(),
            uri2.data(), uri2.size(),
            &css2, &css2Len
    );
    ASSERT_NE(0, css2Len);

    EXPECT_TRUE(adblock_free(css1));
    EXPECT_FALSE(adblock_free("some random pointer"));
    EXPECT_FALSE(adblock_free(nullptr));
    EXPECT_TRUE(adblock_free(css2));
}

TEST_F(API_F, statistics)
{
    char const* json = nullptr;
    size_t json_len = 0;

    ::adblock_statistics(this->adblock(), &json, &json_len);

    auto const stats = json::parse(std::string_view(json, json_len)).get_object();

    EXPECT_EQ(20757, to_number(stats["Filter rule"]));
    EXPECT_EQ(33565, to_number(stats["Element hide rule"]));
    EXPECT_EQ(54322, to_number(stats["Total"]));

    adblock_free(json);
}

TEST(Api_C, reload)
{
    auto const handle = ::adblock_create();
    ASSERT_TRUE(handle);

    FilterFile list;
    list << "ad\n";
    list << "##ad\n";
    list << std::flush;

    auto const path = list.path();

    auto const c_path = path.c_str();
    auto const path_len = strlen(c_path);
    ::adblock_add_filter_set(handle, c_path, path_len);

    char const* json = nullptr;
    size_t json_len = 0;

    ::adblock_statistics(handle, &json, &json_len);
    auto const before = json::parse(std::string_view(json, json_len)).get_object();
    ::adblock_free(json);

    ASSERT_EQ(2, to_number(before["Total"]));

    list << "foobar\n";
    list << "#@#ad\n";
    list << "##ac\n";
    list << std::flush;
    list.close();

    ::adblock_reload(handle);

    ::adblock_statistics(handle, &json, &json_len);
    auto const after = json::parse(std::string_view(json, json_len)).get_object();
    ::adblock_free(json);

    ASSERT_EQ(5, to_number(after["Total"]));
}

TEST_F(API_F, remove_filter_set)
{
    auto const& path = projectRoot / "test/data/fanboy.txt";
    ASSERT_TRUE(fs::exists(path)) << path;

    ::adblock_add_filter_set(
                this->adblock(), path.c_str(), strlen(path.c_str()));

    char const* json = nullptr;
    size_t json_len = 0u;

    ::adblock_statistics(this->adblock(), &json, &json_len);
    auto const before = json::parse(std::string_view(json, json_len)).get_object();
    ::adblock_free(json);

    EXPECT_EQ(66244, to_number(before["Total"]));

    auto const& rc = ::adblock_remove_filter_set(
                        this->adblock(), path.c_str(), strlen(path.c_str()));
    EXPECT_TRUE(rc);

    ::adblock_statistics(this->adblock(), &json, &json_len);
    auto const after = json::parse(std::string_view(json, json_len)).get_object();
    ::adblock_free(json);

    EXPECT_EQ(54322, to_number(after["Total"]));
}

TEST(Main_API, adblock_create)
{
    auto const adblock = ::adblock_create();
    EXPECT_NE(nullptr, adblock);
}

TEST(Main_API, adblock_destroy)
{
    auto const adblock = ::adblock_create();
    EXPECT_NE(nullptr, adblock);

    auto const rc = ::adblock_destroy(adblock);
    EXPECT_TRUE(rc);
}

TEST_F(API_F, adblock_filter_set_parameters)
{
    auto const& path = projectRoot / "test/data/fanboy.txt";
    ASSERT_TRUE(fs::exists(path)) << path;

    ::adblock_add_filter_set(
                this->adblock(), path.c_str(), strlen(path.c_str()));


    ::adblock_string_t fsPath;
    fsPath.ptr = path.c_str();
    fsPath.length = path.native().size();

    ::adblock_string_t *keys = nullptr, *values = nullptr;
    size_t keys_len = 0, values_len = 0;

    auto ok = ::adblock_filter_set_parameters(
        this->adblock(),
        &fsPath,
        &keys, &keys_len,
        &values, &values_len
    );

    ASSERT_TRUE(ok);

    EXPECT_EQ(5, keys_len);
    EXPECT_EQ(5, values_len);

    auto to_sv = [](auto const& str) {
        return std::string_view(str.ptr, str.length);
    };

    EXPECT_EQ("Checksum", to_sv(keys[0]));
    EXPECT_EQ("PHWcd7bbJ8yORZTp1Xg3pQ", to_sv(values[0]));

    EXPECT_EQ("Expires", to_sv(keys[1]));
    EXPECT_EQ("4 days (update frequency)", to_sv(values[1]));

    EXPECT_EQ("Homepage", to_sv(keys[2]));
    EXPECT_EQ("https://easylist.adblockplus.org/", to_sv(values[2]));

    EXPECT_EQ("Title", to_sv(keys[3]));
    EXPECT_EQ("Fanboy's Social Blocking List", to_sv(values[3]));

    EXPECT_EQ("Version", to_sv(keys[4]));
    EXPECT_EQ("201505071651", to_sv(values[4]));

    ok = ::adblock_free_keys(keys);
    EXPECT_TRUE(ok);

    ok = ::adblock_free_values(values);
    EXPECT_TRUE(ok);
}

TEST(Main_API, ContentSecurityPolicy)
{
    FilterFile fs;

    fs << "||adblock.com$csp=script-src 'self' * 'unsafe-inline' 'unsafe-eval'\n";
    fs << "@@||adblock.com/search=$csp=script-src 'self' * 'unsafe-inline' 'unsafe-eval'\n";
    fs << std::flush;

    auto const& path = fs.path().string();
    std::string_view const uri { "http://www.adblock.com" };

    auto to_string_view = [](auto const& str) {
        return std::string_view(str.ptr, str.length);
    };

    auto const handle = adblock_create();

    adblock_add_filter_set(handle, path.data(), path.size());

    adblock_string_t u;
    u.ptr = uri.begin(); u.length = uri.size();

    adblock_string_t p;
    p.ptr = nullptr; p.length = 0;

    adblock_content_security_policy(handle, &u, &p);

    ASSERT_TRUE(p.ptr != nullptr);
    ASSERT_TRUE(p.length != 0);
    EXPECT_TRUE(to_string_view(p) == "script-src 'self' * 'unsafe-inline' 'unsafe-eval'");
}
