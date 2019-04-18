#include "basic_match_pattern.hpp"

#include <optional>
#include <ostream>

#include <boost/algorithm/string.hpp>
#include <boost/range/iterator_range.hpp>

namespace adblock {

class Equals
{
public:
    Equals(bool const caseSensitive)
        : m_caseSensitive { caseSensitive }
    {}

    bool operator()(char const lhs, char const rhs) const
    {
        namespace ba = boost::algorithm;

        static auto const& is_equal = ba::is_equal();
        static auto const& is_iequal = ba::is_iequal();
        static auto const& is_separator =
                           !(ba::is_alnum() || ba::is_any_of("_~.%"));
        if (rhs != '^') {
            return m_caseSensitive ? is_equal(lhs, rhs)
                                   : is_iequal(lhs, rhs);
        }
        else {
            return is_separator(lhs);
        }
    }

private:
    bool m_caseSensitive;
};

//
// BaseMatchPattern
//
BaseMatchPattern::
BaseMatchPattern(StringRange const& range)
    : m_str { range }
{}

bool BaseMatchPattern::
doMatch(StringRange const& target, Tokens const& tokens,
        bool const beginAnchor, bool const endAnchor, bool const caseSensitive) const
{
    namespace ba = boost::algorithm;

    if (tokens.empty()) return true;

    auto tokensCopy = tokens; //TODO no need to copy
    auto range = target;
    std::optional<std::string> uriCopy;
    Equals const compare { caseSensitive };

    // A separator has to match with end of the input.
    // So, if a pattern ends with '^' and a URI doesn't have
    // a separator on its tail, we copy URI string and append
    // a dummy separator to it.
    auto const& lastToken = tokens.back();
    if ((!lastToken.empty() && lastToken.back() == '^') &&
        (!range.empty() && !compare(range.back(), '^')))
    {
        uriCopy.emplace(range.begin(), range.end());
        uriCopy->push_back('/');
        range = StringRange { &*uriCopy->begin(), &*uriCopy->end() };
    }

    if (beginAnchor) {
        auto const& firstToken = tokensCopy.front();
        if (!ba::starts_with(range, firstToken, compare)) {
            return false;
        }

        if (!endAnchor || tokensCopy.size() > 1) {
            tokensCopy.erase(tokensCopy.begin());
            range.drop_front(static_cast<ptrdiff_t>(firstToken.size()));
        }
    }

    if (endAnchor) {
        auto const& lastToken = tokensCopy.back();
        if (!ba::ends_with(range, lastToken, compare)) {
            return false;
        }

        tokensCopy.pop_back();
        range.drop_back(static_cast<ptrdiff_t>(lastToken.size()));
    }

    for (auto const& token: tokensCopy) {
        // empty token matches to everything
        if (token.empty()) return true;

        auto const& rv = ba::find(range, ba::first_finder(token, compare));
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
