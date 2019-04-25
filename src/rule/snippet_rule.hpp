#ifndef ADBLOCK_RULE_SNIPPET_HPP
#define ADBLOCK_RULE_SNIPPET_HPP

#include "rule.hpp"

#include "core/string_range.hpp"

#include <iosfwd>
#include <memory>
#include <vector>

namespace adblock {

class Uri;

class SnippetRule : public Rule
{
    using Base = Rule;
public:
    using Domains = std::vector<StringRange>;

public:
    SnippetRule(StringRange snippet,
                std::unique_ptr<Domains> domains);

    // accessor
    StringRange snippet() const { return m_snippet; }
    Domains const* domains() const { return m_domains.get(); }

    // query
    bool match(Uri const&) const;

private:
    // @override Rule
    void print(std::ostream&) const override;

private:
    StringRange const m_snippet;
    std::unique_ptr<Domains> m_domains;
};

} // namespace adblock

#endif // ADBLOCK_RULE_SNIPPET_HPP
