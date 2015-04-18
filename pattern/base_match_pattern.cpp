#include "basic_match_pattern.hpp"

#include <ostream>

#include <boost/algorithm/string.hpp>
#include <boost/range/iterator_range.hpp>

namespace adblock {

bool BaseMatchPattern::Compare::
operator()(const char left, const char right) const
{
    namespace ba = boost::algorithm;

    static const auto &is_iequal = ba::is_iequal();
    static const auto &is_separator =
                       !(ba::is_alnum() || ba::is_any_of("_~.%"));
    if (right != '^') {
        return is_iequal(left, right);
    }
    else {
        return is_separator(left);
    }
}

BaseMatchPattern::
BaseMatchPattern(const StringRange &range,
                 const bool beginMatch,
                 const bool endMatch)
    : m_str { range },
      m_beginMatch { beginMatch },
      m_endMatch { endMatch }
{}

bool BaseMatchPattern::
doMatch(const UriRange &target, const TokenRange &tokens) const
{
    namespace ba = boost::algorithm;
    static const auto &compare = Compare {};

    if (tokens.empty()) return true;

    auto tokensCopy = tokens;
    auto range = target;
    boost::optional<std::string> uriCopy;

    // A separator has to match with end of the input.
    // So, if a pattern ends with '^' and a URI doesn't have
    // a separator on its tail, we copy URI string and append
    // a dummy separator to it.
    if (tokens.back().back() == '^' && !compare(range.back(), '^')) {
        uriCopy.emplace(range.begin(), range.end());
        uriCopy->push_back('/');
        range = UriRange { uriCopy->begin(), uriCopy->end() };
    }

    if (m_beginMatch) {
        const auto &firstToken = tokensCopy.front();
        if (!ba::starts_with(range, firstToken, compare)) {
            return false;
        }

        if (!m_endMatch || tokensCopy.size() > 1) {
            tokensCopy.drop_front();
            range.drop_front(firstToken.size());
        }
    }

    if (m_endMatch) {
        const auto &lastToken = tokensCopy.back();
        if (!ba::ends_with(range, lastToken, compare)) {
            return false;
        }

        tokensCopy.drop_back();
        range.drop_back(lastToken.size());
    }

    for (const auto &token: tokensCopy) {
        const auto &rv = ba::find(range, ba::first_finder(token, compare));
        if (rv.empty()) return false;

        range = boost::make_iterator_range(rv.end(), target.end());
    }

    return true;
}

void BaseMatchPattern::
print(std::ostream &os) const
{
    os << m_str;
}

} // namespace adblock
