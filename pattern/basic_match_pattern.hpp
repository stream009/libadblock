#ifndef BASIC_MATCH_PATTERN_HPP
#define BASIC_MATCH_PATTERN_HPP

#include "pattern.hpp"

#include <algorithm>
#include <cassert>
#include <iosfwd>
#include <vector>

class BasicMatchPattern : public Pattern
{
protected:
    using Token = Range;
    using Tokens = std::vector<Token>;
    using TokenRange = boost::iterator_range<Tokens::const_iterator>;

    struct Compare {
        bool operator()(const char left, const char right) const;
    };

public:
    template<typename Str>
    BasicMatchPattern(Str &&str);

    BasicMatchPattern(const Range &range);

    bool match(const Uri &url) const override
    {
        return doMatchUrl(url);
    }

protected:
    const Tokens &tokens() const { return m_tokens; }

    virtual bool doMatchUrl(const Uri&) const;
    virtual bool doMatch(const Range&, const TokenRange&) const;

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
    std::string m_str;
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

#endif // BASIC_MATCH_PATTERN_HPP
