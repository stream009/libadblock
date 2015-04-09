#include "basic_match_pattern.hpp"

#include <ostream>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/compare.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/finder.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

namespace adblock {

bool BaseMatchPattern::Compare::
operator()(const char left, const char right) const
{
    namespace ba = boost::algorithm;

    static const auto &is_equal = ba::is_equal();
    static const auto &is_separator =
                       !(ba::is_alnum() || ba::is_any_of("_-.%"));
    if (right != '^') {
        return is_equal(left, right);
    }
    else {
        return is_separator(left);
    }
}

BaseMatchPattern::
BaseMatchPattern(const StringRange &range)
    : m_str { range }
{
    init();
#ifndef NDEBUG
    validate();
#endif
}

bool BaseMatchPattern::
doMatch(const UriRange &target, const TokenRange &tokens) const
{
    namespace ba = boost::algorithm;
    static const auto &compare = Compare {};

    auto range = target;
    for (const auto &token: tokens) {
        const auto &rv = ba::find(range, ba::first_finder(token, compare));
        if (rv.empty()) return false;

        range = boost::make_iterator_range(rv.end(), target.end());
    }

    return true;
}

bool BaseMatchPattern::
doMatchUrl(const Uri &uri) const
{
    const UriRange range { uri.begin(), uri.end() };
    return doMatch(range, m_tokens);
}

void BaseMatchPattern::
init()
{
    namespace ba = boost::algorithm;

    // trim leading and trailing "*"
    //auto &&range = boost::make_iterator_range(m_str);
    auto range = m_str;
    range = ba::trim_copy_if(range, ba::is_any_of("*"));

    ba::split(m_tokens, range, ba::is_any_of("*"), ba::token_compress_on);
}

void BaseMatchPattern::
print(std::ostream &os) const
{
    os << m_str;
}

} // namespace adblock
