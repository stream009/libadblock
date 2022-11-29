#include "basic_match_pattern.hpp"

#include "namespace.hpp"

#include "core/string_range.hpp"
#include "core/uri.hpp"

#include <stream9/strings/starts_with.hpp>
#include <stream9/strings/ends_with.hpp>
#include <stream9/strings/trim.hpp>
#include <stream9/strings/view/split.hpp>

namespace adblock {

BasicMatchPattern::
BasicMatchPattern(StringRange const pattern,
                  bool const beginMatch/*= false*/,
                  bool const endMatch/*= false*/)
    : Base { pattern }
{
    if (beginMatch && !str::starts_with(pattern, "*")) {
        m_anchor = static_cast<Anchor>(m_anchor | Begin);
    }
    if (endMatch && !str::ends_with(pattern, "*")) {
        m_anchor = static_cast<Anchor>(m_anchor | End);
    }
}

bool BasicMatchPattern::
doMatchUrl(Uri const& uri, bool caseSensitive) const
{
    StringRange target { &*uri.begin(), &*uri.end() };

    auto pattern = this->pattern();
    auto pred = [](char c) { return c == '*'; };

    str::trim(pattern, pred);
    Base::Tokens tokens = str::views::split(
        pattern,
        pred,
        str::views::split_option::skip_empty_item
    );

    return this->doMatch(
        target, tokens,
        m_anchor & Begin, m_anchor & End,
        caseSensitive
    );
}

} // namespace adblock
