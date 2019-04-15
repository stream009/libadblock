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
        bool operator()(char left, char right) const;
        void setCaseSensitive(bool caseSensitive);
    private:
        bool m_case = false;
    };

public:
    bool match(Uri const& url, bool const caseSensitive = false) const override
    {
        m_compare.setCaseSensitive(caseSensitive);
        return doMatchUrl(url);
    }

    StringRange const& pattern() const { return m_str; }
    bool isBeginMatch() const { return m_beginMatch; }
    bool isEndMatch() const { return m_endMatch; }

    TokensRange tokens() const { return doTokens(); }

protected:
    virtual bool doMatchUrl(Uri const&) const = 0;
    virtual TokensRange doTokens() const = 0;

    BaseMatchPattern(StringRange const& range,
                     bool beginMatch,
                     bool endMatch);

    bool doMatch(StringRange const&, TokensRange const&) const;

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
