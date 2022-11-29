#include "basic_match_pattern.hpp"

#include "namespace.hpp"

#include "core/uri.hpp"

#include <optional>
#include <ostream>

#include <stream9/container/pop_back.hpp>
#include <stream9/strings/core/char/classifier.hpp>
#include <stream9/strings/core/char/comparator.hpp>
#include <stream9/strings/ends_with.hpp>
#include <stream9/strings/first_finder.hpp>
#include <stream9/strings/starts_with.hpp>

namespace adblock {

class Equals
{
public:
    Equals(bool const caseSensitive) noexcept
        : m_caseSensitive { caseSensitive }
    {}

    bool operator()(char const lhs, char const rhs) const noexcept
    {
        if (rhs != '^') {
            return m_caseSensitive ? is_equal(lhs, rhs)
                                   : is_iequal(lhs, rhs);
        }
        else {
            return is_separator(lhs);
        }
    }

    bool is_equal(char lhs, char rhs) const noexcept
    {
        static auto pred = str::equal_to<char>();

        return pred(lhs, rhs);
    }

    bool is_iequal(char lhs, char rhs) const noexcept
    {
        static auto pred = str::iequal_to<char>();

        return pred(lhs, rhs);
    }

    bool is_separator(char c) const noexcept
    {
        static auto is_alnum = str::is_alnum<char>();
        static auto is_special = str::is_any_of("_~.%");

        return !(is_alnum(c) || is_special(c));
    }

private:
    bool m_caseSensitive;
};

//
// BaseMatchPattern
//
BaseMatchPattern::
BaseMatchPattern(StringRange const range)
    : m_str { range }
{}

bool BaseMatchPattern::
doMatch(StringRange const target, Tokens& tokens,
        bool const beginAnchor, bool const endAnchor, bool const caseSensitive) const
{
    if (tokens.empty()) return true;

    auto range = target;
    std::optional<std::string> uriCopy;
    Equals const compare { caseSensitive };

    // A separator has to match with end of the input.
    // So, if a pattern ends with '^' and a URI doesn't have
    // a separator on its tail, we copy URI string and append
    // a dummy separator to it.
    auto const lastToken = tokens.back();
    if ((!lastToken.empty() && lastToken.back() == '^') &&
        (!range.empty() && !compare(range.back(), '^')))
    {
        uriCopy.emplace(range.begin(), range.end());
        uriCopy->push_back('/');
        range = StringRange { &*uriCopy->begin(), &*uriCopy->end() };
    }

    if (beginAnchor) {
        auto const firstToken = tokens.front();
        if (!str::starts_with(range, firstToken, compare)) {
            return false;
        }

        if (!endAnchor || tokens.size() > 1) {
            tokens.erase(tokens.begin());
            range.remove_prefix(firstToken.size());
        }
    }

    if (endAnchor) {
        auto const lastToken = tokens.back();
        if (!str::ends_with(range, lastToken, compare)) {
            return false;
        }

        st9::container::pop_back(tokens);
        range.remove_suffix(lastToken.size());
    }

    for (auto const token: tokens) {
        // empty token matches to everything
        if (token.empty()) return true;

        str::first_finder fn { token, compare };
        auto rv = fn(range);

        if (rv.empty()) return false;

        range = StringRange { rv.end(), range.end() };
    }

    return true;
}

void BaseMatchPattern::
print(std::ostream& os) const
{
    os << m_str;
}

} // namespace adblock
