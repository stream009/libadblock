#ifndef FILTER_RULE_HPP
#define FILTER_RULE_HPP

#include "rule.hpp"
#include "pattern/pattern.hpp"

#include <memory>
#include <vector>

#include <boost/optional.hpp>

class Option;

class FilterRule : public Rule
{
    using Base = Rule;
public:
    using Base::String;
    using Base::StringRange;
    using Base::Uri;
    using Context = String; //TODO

public:
    virtual bool match(const Uri&, const Context&) const;

protected:
    FilterRule(const std::shared_ptr<Pattern>&,
               const boost::optional<std::vector<std::shared_ptr<Option>>>&);

private:
    std::shared_ptr<Pattern> m_pattern;
    boost::optional<std::vector<std::shared_ptr<Option>>> m_options;

    // @override Rule
    void print(std::ostream&) const override;
};

#endif // FILTER_RULE_HPP
