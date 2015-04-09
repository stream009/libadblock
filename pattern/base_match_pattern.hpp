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
    using TokenRange = boost::iterator_range<Tokens::const_iterator>;
    using UriRange = Uri::const_range_type;

    struct Compare {
        bool operator()(const char left, const char right) const;
    };

public:
    bool match(const Uri &url) const override
    {
        return doMatchUrl(url);
    }

    const StringRange &wholePattern() const { return m_str; }
    const StringRange &firstToken() const { return m_tokens.at(0); }

protected:
    BaseMatchPattern(const StringRange &range);

    const Tokens &tokens() const { return m_tokens; }

    virtual bool doMatchUrl(const Uri&) const;
    virtual bool doMatch(const UriRange&, const TokenRange&) const;

private:
    void validate() const
    {
        assert(!m_str.empty());
        assert(!m_tokens.empty());
        assert(std::none_of(m_tokens.begin(), m_tokens.end(),
            [] (const Token &token) {
                return token.empty();
            }
        ));
    }

    void init();

    // @override Pattern
    void print(std::ostream&) const override;

private:
    StringRange m_str;
    Tokens m_tokens;
};

} // namespace adblock

#endif // BASE_MATCH_PATTERN_HPP
