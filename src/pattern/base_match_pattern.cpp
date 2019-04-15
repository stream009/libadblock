#include "basic_match_pattern.hpp"

#include <optional>
#include <ostream>

#include <boost/algorithm/string.hpp>
#include <boost/range/iterator_range.hpp>

namespace adblock {

bool BaseMatchPattern::Compare::
operator()(char const left, char const right) const
{
    namespace ba = boost::algorithm;

    static auto const& is_equal = ba::is_equal();
    static auto const& is_iequal = ba::is_iequal();
    static auto const& is_separator =
                       !(ba::is_alnum() || ba::is_any_of("_~.%"));
    if (right != '^') {
        return m_case ? is_equal(left, right)
                      : is_iequal(left, right);
    }
    else {
        return is_separator(left);
    }
}

void  BaseMatchPattern::Compare::
setCaseSensitive(bool const caseSensitive)
{
    m_case = caseSensitive;
}

BaseMatchPattern::
BaseMatchPattern(StringRange const& range,
                 bool const beginMatch,
                 bool const endMatch)
    : m_str { range }
    , m_beginMatch { beginMatch }
    , m_endMatch { endMatch }
{}

bool BaseMatchPattern::
doMatch(StringRange const& target, TokensRange const& tokens) const
{
    namespace ba = boost::algorithm;

    if (tokens.empty()) return true;

    auto tokensCopy = tokens;
    auto range = target;
    std::optional<std::string> uriCopy;

    // A separator has to match with end of the input.
    // So, if a pattern ends with '^' and a URI doesn't have
    // a separator on its tail, we copy URI string and append
    // a dummy separator to it.
    auto const& lastToken = tokens.back();
    if ((!lastToken.empty() && lastToken.back() == '^') &&
        (!range.empty() && !m_compare(range.back(), '^')))
    {
        uriCopy.emplace(range.begin(), range.end());
        uriCopy->push_back('/');
        range = StringRange { &*uriCopy->begin(), &*uriCopy->end() };
    }

    if (m_beginMatch) {
        auto const& firstToken = tokensCopy.front();
        if (!ba::starts_with(range, firstToken, m_compare)) {
            return false;
        }

        if (!m_endMatch || tokensCopy.size() > 1) {
            tokensCopy.drop_front();
            range.drop_front(static_cast<ptrdiff_t>(firstToken.size()));
        }
    }

    if (m_endMatch) {
        auto const& lastToken = tokensCopy.back();
        if (!ba::ends_with(range, lastToken, m_compare)) {
            return false;
        }

        tokensCopy.drop_back();
        range.drop_back(static_cast<ptrdiff_t>(lastToken.size()));
    }

    for (auto const& token: tokensCopy) {
        // empty token matches to everything
        if (token.empty()) return true;

        auto const& rv = ba::find(range, ba::first_finder(token, m_compare));
        if (rv.empty()) return false;

        range = boost::make_iterator_range(rv.end(), range.end());
    }

    return true;
}

void BaseMatchPattern::
print(std::ostream& os) const
{
    os << m_str;
}

} // namespace adblock
