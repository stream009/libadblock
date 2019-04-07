#include "api.h"
#include "type.hpp"

#include <cassert>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <gtest/gtest.h>

namespace bfs = boost::filesystem;
static const bfs::path projectRoot { PROJECT_ROOT };

class FilterFile : public std::ofstream
{
public:
    FilterFile()
        : m_path { std::tmpnam(nullptr) }
    {
        this->open(m_path);
        if (this->fail()) {
            std::cout << m_path << "\n";
            throw std::runtime_error { "file open error" };
        }

        *this << "[Adblock Plus 2.0]\n";
    }

    ~FilterFile()
    {
        bfs::remove(m_path);
    }

    auto const& path() const { return m_path; }

private:
    bfs::path m_path;
};

class API_F : public ::testing::Test
{
public:
    static void SetUpTestCase()
    {
        m_adblock = ::adblock_create();
        ASSERT_TRUE(m_adblock);

        const auto &path1 = projectRoot / "test/data/easylist.txt";
        ASSERT_TRUE(bfs::exists(path1)) << path1;

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

    const std::string uri1 { "http://a.kickass.to/sc-b98b537.js" };

    const char *filterSet = nullptr;
    size_t filterSetLen = 0;
    const char *reason = nullptr;
    size_t reasonLen = 0;
    bool rv = ::adblock_should_block(this->adblock(),
                uri1.c_str(), uri1.size(), &context,
                &filterSet, &filterSetLen, &reason, &reasonLen);
    ASSERT_TRUE(rv);
    EXPECT_NE(nullptr, filterSet);
    EXPECT_NE(0, filterSetLen);
    EXPECT_NE(nullptr, reason);
    EXPECT_NE(0, reasonLen);

    context.content_type = TYPE_DOCUMENT;

    const std::string uri2 { "http://barhbarhbarh.com" };
    rv = ::adblock_should_block(this->adblock(),
                uri2.c_str(), uri2.size(), &context,
                &filterSet, &filterSetLen, &reason, &reasonLen);
    ASSERT_FALSE(rv);
    EXPECT_EQ(nullptr, filterSet);
    EXPECT_EQ(0, filterSetLen);
    EXPECT_EQ(nullptr, reason);
    EXPECT_EQ(0, reasonLen);
}

TEST_F(API_F, adblock_element_hide_css)
{
    const std::string uri { "http://www.adblockplus.org/" };

    const char *css = nullptr;
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
    const std::string uri1 { "http://www.adblockplus.org/" };
    const char *css1 = nullptr;
    size_t css1Len = 0;

    adblock_element_hide_css(
            this->adblock(),
            uri1.data(), uri1.size(),
            &css1, &css1Len
    );
    ASSERT_NE(0, css1Len);

    const std::string uri2 { "http://www.google.org/" };
    const char *css2 = nullptr;
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
    const char *json = nullptr;
    size_t json_len = 0;

    ::adblock_statistics(this->adblock(), &json, &json_len);

    std::istringstream iss { std::string { json, json_len } };

    namespace bpt = boost::property_tree;

    bpt::ptree stats;
    boost::property_tree::read_json(iss, stats);

    EXPECT_EQ(20757, stats.get<size_t>("Filter rule"));
    EXPECT_EQ(33565, stats.get<size_t>("Element hide rule"));
    EXPECT_EQ(54322, stats.get<size_t>("Total"));

    adblock_free(json);
}

static boost::property_tree::ptree
parseJsonStats(const char* const json, const size_t json_len)
{
    namespace bpt = boost::property_tree;

    std::istringstream iss { std::string { json, json_len } };
    bpt::ptree result;
    bpt::read_json(iss, result);

    return result;
}

TEST_F(API_F, reload) //TODO more reliable test
{
    namespace bpt = boost::property_tree;

    const char *json = nullptr;
    size_t json_len = 0;

    ::adblock_statistics(this->adblock(), &json, &json_len);
    const auto &before = parseJsonStats(json, json_len);
    ::adblock_free(json);

    ::adblock_reload(this->adblock());

    ::adblock_statistics(this->adblock(), &json, &json_len);
    const auto &after = parseJsonStats(json, json_len);
    ::adblock_free(json);

    EXPECT_EQ(after.get<size_t>("Filter rule"),
              before.get<size_t>("Filter rule"));
    EXPECT_EQ(after.get<size_t>("Element hide rule"),
              before.get<size_t>("Element hide rule"));
    EXPECT_EQ(after.get<size_t>("Total"),
              before.get<size_t>("Total"));
}

TEST_F(API_F, remove_filter_set)
{
    const auto &path = projectRoot / "test/data/fanboy.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;

    ::adblock_add_filter_set(
                this->adblock(), path.c_str(), strlen(path.c_str()));

    const char *json = nullptr;
    size_t json_len = 0u;

    ::adblock_statistics(this->adblock(), &json, &json_len);

    const auto &before = parseJsonStats(json, json_len);
    ::adblock_free(json);
    EXPECT_EQ(66244, before.get<size_t>("Total"));

    const auto &rc = ::adblock_remove_filter_set(
                        this->adblock(), path.c_str(), strlen(path.c_str()));
    EXPECT_TRUE(rc);

    ::adblock_statistics(this->adblock(), &json, &json_len);

    const auto &after = parseJsonStats(json, json_len);
    ::adblock_free(json);
    EXPECT_EQ(54322, after.get<size_t>("Total"));
}

TEST(Main_API, adblock_create)
{
    const auto adblock = ::adblock_create();
    EXPECT_NE(nullptr, adblock);
}

TEST(Main_API, adblock_destroy)
{
    const auto adblock = ::adblock_create();
    EXPECT_NE(nullptr, adblock);

    const auto rc = ::adblock_destroy(adblock);
    EXPECT_TRUE(rc);
}

TEST_F(API_F, adblock_filter_set_parameters)
{
    const auto &path = projectRoot / "test/data/fanboy.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;

    ::adblock_add_filter_set(
                this->adblock(), path.c_str(), strlen(path.c_str()));


    ::adblock_string_t fsPath;
    fsPath.ptr = path.c_str();
    fsPath.length = path.size();

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
    using namespace adblock;
    FilterFile fs;

    fs << "||adblock.com$csp=script-src 'self' * 'unsafe-inline' 'unsafe-eval'\n";
    fs << "@@||adblock.com/search=$csp=script-src 'self' * 'unsafe-inline' 'unsafe-eval'\n";
    fs << std::flush;

    auto const& path = fs.path().string();
    auto const uri = "http://www.adblock.com"_r;

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

