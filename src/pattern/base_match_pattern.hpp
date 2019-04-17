#ifndef BASE_MATCH_PATTERN_HPP
#define BASE_MATCH_PATTERN_HPP

#include "pattern.hpp"

#include <algorithm>
#include <cassert>
#include <iosfwd>
#include <vector>

#include <boost/container/small_vector.hpp>

namespace adblock {

class BaseMatchPattern : public Pattern
{
protected:
    using Token = StringRange;
    using Tokens = boost::container::small_vector<Token, 3>;

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

    Tokens tokens() const { return doTokens(); }

protected:
    virtual bool doMatchUrl(Uri const&) const = 0;
    virtual Tokens doTokens() const = 0;

    BaseMatchPattern(StringRange const& range);

    bool doMatch(StringRange const&, Tokens const&,
                 bool beginAnchor, bool endAnchor) const;

private:
    // @override Pattern
    void print(std::ostream&) const override;

private:
    StringRange m_str;
    mutable Compare m_compare;
};

} // namespace adblock

#endif // BASE_MATCH_PATTERN_HPP
