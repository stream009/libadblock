#ifndef FILTER_RULE_HPP
#define FILTER_RULE_HPP

#include "rule.hpp"
#include "type.hpp"
#include "pattern/pattern.hpp"

#include <memory>
#include <vector>

#include <boost/optional.hpp>

namespace adblock {

class Option;

class FilterRule : public Rule
{
    using Base = Rule;
public:
    using Context = size_t; //TODO

public:
    virtual bool match(const Uri&, const Context&) const;

    const Pattern &pattern() const
    { assert(m_pattern); return *m_pattern; } //TODO temporary

protected:
    FilterRule(const std::shared_ptr<Pattern>&,
               const boost::optional<std::vector<std::shared_ptr<Option>>>&);

private:
    std::shared_ptr<Pattern> m_pattern;
    boost::optional<std::vector<std::shared_ptr<Option>>> m_options;

    // @override Rule
    void print(std::ostream&) const override;
};

} // namespace adblock

#endif // FILTER_RULE_HPP
