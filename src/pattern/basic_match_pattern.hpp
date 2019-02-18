#ifndef BASIC_MATCH_PATTERN
#define BASIC_MATCH_PATTERN

#include "base_match_pattern.hpp"
#include "type.hpp"

namespace adblock {

class BasicMatchPattern : public BaseMatchPattern
{
    using Base = BaseMatchPattern;
    using Base::Tokens;
public:
    BasicMatchPattern(const StringRange &pattern,
                      const bool beginMatch = false,
                      const bool endMatch = false);

    using BaseMatchPattern::match;

    bool match(StringRange const& str) const
    {
        return this->doMatch(str, m_tokens);
    }

private:
    // @override BaseMatchPattern
    bool doMatchUrl(const Uri&) const override;
    TokensRange doTokens() const override { return m_tokens; }

private:
    Tokens m_tokens;
};

} // namespace adblock

#endif // BASIC_MATCH_PATTERN
