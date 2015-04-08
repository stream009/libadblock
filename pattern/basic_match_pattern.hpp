#ifndef BASIC_MATCH_PATTERN_HPP
#define BASIC_MATCH_PATTERN_HPP

#include "pattern.hpp"

#include <algorithm>
#include <cassert>
#include <iosfwd>
#include <vector>

#include <boost/range/iterator_range.hpp>

namespace adblock {

class BasicMatchPattern : public Pattern
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
    template<typename Str>
    BasicMatchPattern(Str &&str);

    BasicMatchPattern(const StringRange &range);

    bool match(const Uri &url) const override
    {
        return doMatchUrl(url);
    }

protected:
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

template<typename Str>
BasicMatchPattern::
BasicMatchPattern(Str &&str)
    : m_str { std::forward<Str>(str) }
{
    init();

    validate();
}

} // namespace adblock

#endif // BASIC_MATCH_PATTERN_HPP
