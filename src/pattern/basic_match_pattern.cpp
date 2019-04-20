#include "basic_match_pattern.hpp"

#include "core/string_range.hpp"
#include "core/uri.hpp"

#include <boost/algorithm/string.hpp>

namespace adblock {

BasicMatchPattern::
BasicMatchPattern(StringRange const pattern,
                  bool const beginMatch/*= false*/,
                  bool const endMatch/*= false*/)
    : Base { pattern }
{
    namespace ba = boost::algorithm;

    if (beginMatch && !ba::starts_with(pattern, "*")) {
        m_anchor = static_cast<Anchor>(m_anchor | Begin);
    }
    if (endMatch && !ba::ends_with(pattern, "*")) {
        m_anchor = static_cast<Anchor>(m_anchor | End);
    }
}

bool BasicMatchPattern::
doMatchUrl(Uri const& uri, bool const caseSensitive) const
{
    namespace ba = boost::algorithm;

    StringRange const target { &*uri.begin(), &*uri.end() };

    auto pattern = this->pattern();
    auto pred = [](auto c) { return c == '*'; };
    Base::Tokens tokens;

    pattern = ba::trim_copy_if(pattern, pred);
    ba::split(tokens, pattern, pred, ba::token_compress_on);

    return this->doMatch(
        target, tokens,
        m_anchor & Begin, m_anchor & End,
        caseSensitive
    );
}

} // namespace adblock
