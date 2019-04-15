#include "basic_match_pattern.hpp"

#include <boost/algorithm/string.hpp>

namespace adblock {

BasicMatchPattern::
BasicMatchPattern(StringRange const& pattern,
                  bool const beginMatch/*= false*/,
                  bool const endMatch/*= false*/)
    : Base { pattern, beginMatch, endMatch }
{
    namespace ba = boost::algorithm;

    // trim leading and trailing "*"
    auto range = pattern;
    range = ba::trim_copy_if(range, ba::is_any_of("*"));

    ba::split(m_tokens, range, ba::is_any_of("*"), ba::token_compress_on);
}

bool BasicMatchPattern::
doMatchUrl(Uri const& uri) const
{
    StringRange const range { &*uri.begin(), &*uri.end() };
    return this->doMatch(range, m_tokens);
}

} // namespace adblock
