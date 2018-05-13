#ifndef BASE_MATCH_PATTERN_HPP
#define BASE_MATCH_PATTERN_HPP

#include "pattern.hpp"

#include <algorithm>
#include <cassert>
#include <iosfwd>
#include <vector>

#include <boost/range/iterator_range.hpp>

namespace adblock {

class BaseMatchPattern : public Pattern
{
protected:
    using Token = StringRange;
    using Tokens = std::vector<Token>;
    using TokensRange = boost::iterator_range<Tokens::const_iterator>;

private:
    class Compare
    {
    public:
        bool operator()(const char left, const char right) const;
        void setCaseSensitive(const bool caseSensitive);
    private:
        bool m_case = false;
    };

public:
    bool match(const Uri &url, const bool caseSensitive = false) const override
    {
        m_compare.setCaseSensitive(caseSensitive);
        return doMatchUrl(url);
    }

    const StringRange &pattern() const { return m_str; }
    bool isBeginMatch() const { return m_beginMatch; }
    bool isEndMatch() const { return m_endMatch; }

    TokensRange tokens() const { return doTokens(); }

protected:
    virtual bool doMatchUrl(const Uri&) const = 0;
    virtual TokensRange doTokens() const = 0;

    BaseMatchPattern(const StringRange &range,
                     const bool beginMatch,
                     const bool endMatch);

    bool doMatch(const StringRange&, const TokensRange&) const;

private:
    // @override Pattern
    void print(std::ostream&) const override;

private:
    StringRange m_str;
    bool m_beginMatch = false;
    bool m_endMatch = false;
    mutable Compare m_compare;
};

} // namespace adblock

#endif // BASE_MATCH_PATTERN_HPP
