#ifndef FILTER_RULE_HPP
#define FILTER_RULE_HPP

#include "rule.hpp"
#include "pattern/pattern.hpp"

#include <memory>

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
    using Base::Base;

    void parse(const StringRange&);

private:
    //std::unique_ptr<Pattern> m_pattern;
    std::shared_ptr<Pattern> m_pattern;
};

#endif // FILTER_RULE_HPP
