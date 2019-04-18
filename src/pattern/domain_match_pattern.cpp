#include "domain_match_pattern.hpp"

#include <algorithm>
#include <iterator>

#include <boost/algorithm/string.hpp>

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

//
// DomainMatchPattern
//
DomainMatchPattern::
DomainMatchPattern(StringRange const& pattern,
                   bool const endMatch/*= false*/)
    : Base { pattern }
    , m_endAnchor { endMatch }
{
    //TODO ||*.com
    namespace ba = boost::algorithm;

    if (ba::ends_with(pattern, "*")) {
        m_endAnchor = false;
    }
}

bool DomainMatchPattern::
doMatchUrl(Uri const& url, bool const caseSensitive) const
{
    namespace ba = boost::algorithm;

    auto [t_host, t_path] = splitUri(url);
    if (t_host.empty()) return false;

    auto [p_host, p_path] = splitPattern(this->pattern());

    Base::Tokens tokens;
    auto is_wildcard = [](auto c) { return c == '*'; };

    p_host = ba::trim_copy_if(p_host, is_wildcard);
    ba::split(tokens, p_host, is_wildcard, ba::token_compress_on);

    if (!this->doMatch(t_host, tokens, false, false, caseSensitive)) {
        return false;
    }

    tokens.clear();
    p_path = ba::trim_copy_if(p_path, is_wildcard);
    ba::split(tokens, p_path, is_wildcard, ba::token_compress_on);

    return this->doMatch(t_path, tokens, true, m_endAnchor, caseSensitive);
}

BaseMatchPattern::Tokens DomainMatchPattern::
doTokens() const
{
    namespace ba = boost::algorithm;

    auto [pattern, _] = splitPattern(this->pattern()); //TODO why?
    auto pred = [](auto c) { return c == '*'; };
    Base::Tokens tokens;

    pattern = ba::trim_copy_if(pattern, pred);
    ba::split(tokens, pattern, pred, ba::token_compress_on);

    return tokens;
}

} // namespace adblock
