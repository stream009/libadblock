#include "domain_match_pattern.hpp"

#include "namespace.hpp"

#include "core/string_range.hpp"
#include "core/uri.hpp"

#include <algorithm>
#include <utility>

#include <stream9/array.hpp>
#include <stream9/strings/ends_with.hpp>
#include <stream9/strings/trim.hpp>
#include <stream9/strings/view/split.hpp>

namespace adblock {

static auto
findSeparator(StringRange const pattern)
{
    return std::find_if(
        pattern.begin(), pattern.end(),
        [](auto c) {
            return c == '/'
                || c == '^';
        });
}

static std::pair<StringRange, StringRange>
splitPattern(StringRange const pattern)
{
    auto const p_host_end = findSeparator(pattern);
    StringRange const p_host { pattern.begin(), p_host_end };
    StringRange const p_path { p_host_end, pattern.end() };

    return { p_host, p_path };
}

static auto
splitUri(Uri const& uri)
{
    auto const& host_ = uri.host();

    StringRange const host  { host_.begin(), host_.end() };
    StringRange const path { host.end(), &*uri.end() };

    return std::make_pair(host, path);
}

static bool
isWildcard(char const c)
{
    return c == '*';
}

static StringRange
subDomain(StringRange const host)
{
    auto const end = host.end();
    auto it = std::find(host.begin(), end, '.');

    if (it == end) {
        return { end, end };
    }
    else {
        return { it + 1, end };
    }
}

//
// DomainMatchPattern
//
DomainMatchPattern::
DomainMatchPattern(StringRange const pattern,
                   bool const endMatch/*= false*/)
    : Base { pattern }
    , m_endAnchor { endMatch }
{
    if (str::ends_with(pattern, "*")) {
        m_endAnchor = false;
    }
}

StringRange DomainMatchPattern::
domainPattern() const
{
    auto [result, _] = splitPattern(this->pattern());

    return result;
}

bool DomainMatchPattern::
doMatchUrl(Uri const& url, bool const caseSensitive) const
{
    auto [t_host, t_path] = splitUri(url);
    if (t_host.empty()) return false;

    auto [p_host, p_path] = splitPattern(this->pattern());

    Base::Tokens tokens;

    bool const t_beginAnchor = p_host.front() != '*';
    bool const t_endAnchor = !p_path.empty() && p_host.back() != '*';
    bool const p_beginAnchor = !p_path.empty() && p_path.front() != '*';

    str::trim(p_host, isWildcard);
    str::trim(p_path, isWildcard);

    tokens = str::views::split(
        p_host,
        isWildcard,
        str::views::split_option::skip_empty_item
    );

    while (true) {
        auto const match = this->doMatch(
                t_host, tokens, t_beginAnchor, t_endAnchor, caseSensitive);
        if (match) break;

        t_host = subDomain(t_host);
        if (t_host.empty()) {
            return false;
        }
    }

    tokens = str::views::split(
        p_path,
        isWildcard,
        str::views::split_option::skip_empty_item
    );

    return this->doMatch(t_path, tokens, p_beginAnchor, m_endAnchor, caseSensitive);
}

} // namespace adblock
