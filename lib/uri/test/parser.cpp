#include <boost/test/unit_test.hpp>

#include "../parser.hpp"

#include <iostream>

namespace uri {

using namespace std::literals;

BOOST_AUTO_TEST_SUITE(parser)

void test(std::string_view const input, uri_components const& expected)
{
    auto result = uri::parse(input);

    BOOST_CHECK_EQUAL(result.scheme, expected.scheme);
    BOOST_CHECK_EQUAL(result.user_info, expected.user_info);
    BOOST_CHECK_EQUAL(result.host, expected.host);
    BOOST_CHECK_EQUAL(result.port, expected.port);
    BOOST_CHECK_EQUAL(result.path, expected.path);
    BOOST_CHECK_EQUAL(result.query, expected.query);
    BOOST_CHECK_EQUAL(result.fragment, expected.fragment);
}

void test_error(std::string_view const input)
{
    BOOST_CHECK_THROW(uri::parse(input), uri::parse_error);
}

BOOST_AUTO_TEST_CASE(complete)
{
    auto const input =
        "http://foo@www.google.com:8080/path?foo=bar#segment"sv;
    uri_components const expected {
        "http",
        "foo",
        "www.google.com",
        "8080",
        "/path",
        "foo=bar",
        "segment"
    };

    test(input, expected);
}

BOOST_AUTO_TEST_CASE(no_authority)
{
    auto const input = "about:blank"sv;
    uri_components const expected {
        "about",
        "",
        "",
        "",
        "blank",
        "",
        ""
    };

    test(input, expected);
}

BOOST_AUTO_TEST_CASE(invalid_but_still_parsable_1)
{
    auto const input = "scheme:path:@path#/bar"sv;
    uri_components const expected {
        "scheme",
        "",
        "",
        "",
        "path:@path",
        "",
        "/bar"
    };

    test(input, expected);
}

BOOST_AUTO_TEST_CASE(invalid_but_still_parsable_2)
{
    auto const input = "http:///www.google.com/index.html"sv;
    uri_components const expected {
        "http",
        "",
        "",
        "",
        "/www.google.com/index.html",
        "",
        ""
    };

    test(input, expected);
}

BOOST_AUTO_TEST_CASE(invalid_but_still_parsable_3)
{
    auto const input = ":"sv;
    uri_components const expected {
        "",
        "",
        "",
        "",
        "",
        "",
        ""
    };

    test(input, expected);
}

BOOST_AUTO_TEST_CASE(parse_error_)
{
    test_error("aaa/b");
    test_error("");
}

BOOST_AUTO_TEST_SUITE_END() // parser

} // namespace uri
