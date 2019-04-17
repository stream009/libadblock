#include "basic_match_pattern.hpp"

#include <boost/algorithm/string.hpp>

namespace adblock {

BasicMatchPattern::
BasicMatchPattern(StringRange const& pattern,
                  bool const beginMatch/*= false*/,
                  bool const endMatch/*= false*/)
    : Base { pattern }
{
    namespace ba = boost::algorithm;

    if (beginMatch && !ba::starts_with(pattern, "*")) { //TODO test
        m_anchor = static_cast<Anchor>(m_anchor | Begin);
    }
    if (endMatch && !ba::ends_with(pattern, "*")) { //TODO test
        m_anchor = static_cast<Anchor>(m_anchor | End);
    }
}

bool BasicMatchPattern::
doMatchUrl(Uri const& uri) const
{
    StringRange const target { &*uri.begin(), &*uri.end() };

    return this->doMatch(target, doTokens(), m_anchor & Begin, m_anchor & End);
}

BaseMatchPattern::Tokens BasicMatchPattern::
doTokens() const
{
    namespace ba = boost::algorithm;

    auto pattern = this->pattern();
    auto pred = [](auto c) { return c == '*'; };
    Base::Tokens tokens;

    pattern = ba::trim_copy_if(pattern, pred);
    ba::split(tokens, pattern, pred, ba::token_compress_on);

    return tokens;
}

} // namespace adblock
