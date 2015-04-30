#ifndef FILTER_RULE_HPP
#define FILTER_RULE_HPP

#include "context.hpp"
#include "rule.hpp"
#include "type.hpp"

#include <cassert>
#include <iosfwd>
#include <memory>
#include <vector>

#include <boost/optional.hpp>

namespace adblock {

class Option;
class Pattern;

class FilterRule : public Rule
{
    using Base = Rule;
public:
    using Options = std::vector<std::shared_ptr<Option>>;
    using OptionsRange = boost::iterator_range<Options::const_iterator>;

public:
    virtual bool match(const Uri&, const Context&) const;

    const Pattern &pattern() const;
    OptionsRange options() const;

protected:
    FilterRule(const std::shared_ptr<Pattern>&,
               const boost::optional<Options>&);

private:
    // @override Rule
    void print(std::ostream&) const override;

    void validate() const
    {
        assert(m_pattern);
    }

    bool hasMatchCaseOption() const;

private:
    std::shared_ptr<Pattern> m_pattern;
    boost::optional<Options> m_options;
};

} // namespace adblock

#endif // FILTER_RULE_HPP
