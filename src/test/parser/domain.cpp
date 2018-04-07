#include "character_class.hpp"
#include "parser/domain.hpp"

#include <boost/range/iterator_range.hpp>
#include <boost/spirit/include/qi.hpp>

#include <gtest/gtest.h>

using adblock::StringRange;

static std::string
charToStr(const char c)
{
    return str(boost::format { "%c (%d)" } % c % static_cast<int>(c));
}

enum ParseResult { FullMatch, PartialMatch, NoMatch };

static ParseResult
tryParse(const StringRange &range)
{
    namespace qi = boost::spirit::qi;
    static const adblock::parser::Domain grammar;

    auto it = range.begin();
    const auto rv = qi::parse(it, range.end(), grammar);
    if (!rv) return ParseResult::NoMatch;
    if (it != range.end()) {
        //std::cout << "Partial match: " << range << " [" << charToStr(*it) << "]\n";
        return ParseResult::PartialMatch;
    }
    return ParseResult::FullMatch;
}

TEST(DomainParser, standard)
{
    using boost::as_literal;

    EXPECT_EQ(FullMatch, tryParse(as_literal("com")));
    EXPECT_EQ(FullMatch, tryParse(as_literal("google.com")));
    EXPECT_EQ(FullMatch, tryParse(as_literal("www.google.com")));
}

TEST(DomainParser, labelFirstCharacter)
{
    const auto &check = [] (const std::string &chars,
                            const ParseResult expected)
    {
        for (const auto c: chars) {
            const std::string &line = str(boost::format { "%cfoo" } % c);
            const StringRange range { line.data(), line.data() + line.size() };
            EXPECT_EQ(expected, tryParse(range)) << charToStr(c);
        }
    };

    namespace cc = character_class;

    std::string alnum_utf8 = cc::alnum() + cc::utf8();
    std::string all = cc::all();
    boost::sort(alnum_utf8);
    boost::sort(all);
    std::string others;
    boost::set_difference(all, alnum_utf8, std::back_inserter(others));
    boost::remove(others, '.');

    check(alnum_utf8, ParseResult::FullMatch);
    check(others, ParseResult::NoMatch);
}

TEST(DomainParser, labelMiddleCharacter)
{
    const auto &check =
        [] (const std::string &chars, const ParseResult expected)
        {
            for (const auto c: chars) {
                const std::string &line =
                    str(boost::format { "f%co%co%cf%c%co" } % c % c %c %c %c);
                const StringRange range {
                               line.data(), line.data() + line.size() };
                EXPECT_EQ(expected, tryParse(range)) << charToStr(c);
            }
        };

    namespace cc = character_class;

    std::string alnum_hyphen_utf8 = cc::alnum() + '-' + cc::utf8();
    std::string all = cc::all();

    std::string others;
    boost::set_difference(
            boost::sort(all),
            boost::sort(alnum_hyphen_utf8),
            std::back_inserter(others));
    boost::remove(others, '.');

    check(alnum_hyphen_utf8, FullMatch);
    check(others,            PartialMatch);
}

TEST(DomainParser, labelLastCharacter)
{
    const auto &check =
        [] (const std::string &chars, const ParseResult expected)
        {
            for (const auto c: chars) {
                const std::string &line =
                    str(boost::format { "foo%c" } % c);
                const StringRange range {
                               line.data(), line.data() + line.size() };
                EXPECT_EQ(expected, tryParse(range)) << charToStr(c);
            }
        };

    namespace cc = character_class;

    std::string alnum_utf8 = cc::alnum() + cc::utf8();
    std::string all = cc::all();

    std::string others;
    boost::set_difference(
            boost::sort(all),
            boost::sort(alnum_utf8),
            std::back_inserter(others));
    boost::remove(others, '.');

    check(alnum_utf8, FullMatch);
    check(others,     PartialMatch);
}

TEST(DomainParser, startsWithDot)
{
    EXPECT_EQ(NoMatch, tryParse(boost::as_literal(".foo")));
}

TEST(DomainParser, endsWithDot)
{
    EXPECT_EQ(PartialMatch, tryParse(boost::as_literal("foo.")));
}

TEST(DomainParser, consecutiveDots)
{
    EXPECT_EQ(PartialMatch, tryParse(boost::as_literal("foo..bar")));
    EXPECT_EQ(PartialMatch, tryParse(boost::as_literal("foo...bar")));
}
