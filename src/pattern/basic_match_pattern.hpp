#ifndef BASIC_MATCH_PATTERN
#define BASIC_MATCH_PATTERN

#include "base_match_pattern.hpp"

namespace adblock {

class StringRange;
class Uri;

class BasicMatchPattern : public BaseMatchPattern
{
    using Base = BaseMatchPattern;
public:
    enum Anchor { None = 0, Begin = 1, End = 2 };

public:
    BasicMatchPattern(StringRange const& pattern,
                      bool beginMatch = false,
                      bool endMatch = false);

    bool isBeginMatch() const { return m_anchor & Begin; }
    bool isEndMatch() const { return m_anchor & End; }

private:
    // @override BaseMatchPattern
    bool doMatchUrl(Uri const&, bool caseSensitive) const override;

private:
    Anchor m_anchor = None;
};

} // namespace adblock

#endif // BASIC_MATCH_PATTERN
