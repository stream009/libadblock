#include "adblock.hpp"
#include "api.h"

#include <gtest/gtest.h>

#include <cstdio>
#include <iostream>

#include <boost/filesystem.hpp>

namespace adblock {

namespace fs = boost::filesystem;

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
        fs::remove(m_path);
    }

    auto const& path() const { return m_path; }

private:
    fs::path m_path;
};

bool operator==(char const lhs[], StringRange const& rhs)
{
    return std::string_view(rhs.begin(), rhs.size()) == lhs;
}

TEST(Csp, Elementary)
{
    FilterFile fs;

    fs << "||adblock.com$csp=script-src 'self' * 'unsafe-inline' 'unsafe-eval'\n";
    fs << "@@||adblock.com/search=$csp=script-src 'self' * 'unsafe-inline' 'unsafe-eval'\n";
    fs << std::flush;

    AdBlock adblock;
    adblock.addFilterSet(fs.path());

    auto policy = adblock.contentSecurityPolicy("http://www.adblock.com"_u);

    EXPECT_TRUE("script-src 'self' * 'unsafe-inline' 'unsafe-eval'" == policy) << policy;

    policy = adblock.contentSecurityPolicy("http://www.google.com"_u);

    EXPECT_TRUE(policy.empty());
}

TEST(Csp, ExceptionRule)
{
    FilterFile fs;

    fs << "||adblock.com$csp=script-src 'self' * 'unsafe-inline' 'unsafe-eval'\n";
    fs << "@@||adblock.com/search=$csp=script-src 'self' * 'unsafe-inline' 'unsafe-eval'\n";
    fs << std::flush;

    AdBlock adblock;
    adblock.addFilterSet(fs.path());

    auto policy = adblock.contentSecurityPolicy("http://www.adblock.com"_u);

    EXPECT_TRUE("script-src 'self' * 'unsafe-inline' 'unsafe-eval'" == policy) << policy;

    policy = adblock.contentSecurityPolicy("http://www.adblock.com/search=foo"_u);

    EXPECT_TRUE(policy.empty());

    policy = adblock.contentSecurityPolicy("http://www.google.com"_u);

    EXPECT_TRUE(policy.empty());
}

TEST(Csp, DISABLED_CantInverseOption)
{
    FilterFile fs;

    fs << "||adblock.com$~csp=script-src 'self' * 'unsafe-inline' 'unsafe-eval'\n";
    fs << std::flush;

    AdBlock adblock;
    adblock.addFilterSet(fs.path());
}

} // namespace adblock
