#ifndef BASE_MATCH_PATTERN_HPP
#define BASE_MATCH_PATTERN_HPP

#include "pattern.hpp"

#include "core/string_range.hpp"

#include <iosfwd>

#include <boost/container/small_vector.hpp>

namespace adblock {

class Uri;

class BaseMatchPattern : public Pattern
{
protected:
    using Token = StringRange;
    using Tokens = boost::container::small_vector<Token, 3>;

public:
    bool match(Uri const& url, bool caseSensitive = false) const override
    {
        return doMatchUrl(url, caseSensitive);
    }

    StringRange const& pattern() const { return m_str; }

protected:
    virtual bool doMatchUrl(Uri const&, bool caseSensitive) const = 0;

    BaseMatchPattern(StringRange const& range);

    bool doMatch(StringRange const&, Tokens const&,
                 bool beginAnchor, bool endAnchor, bool caseSensitive) const;

private:
    // @override Pattern
    void print(std::ostream&) const override;

private:
    StringRange m_str;
};

} // namespace adblock

#endif // BASE_MATCH_PATTERN_HPP
