#include "filter_rule.hpp"

#include "option.hpp"

FilterRule::
FilterRule(const std::shared_ptr<Pattern> &pattern,
           const boost::optional<
                        std::vector<std::shared_ptr<Option>>> &options)
    : m_pattern { pattern }
    , m_options { options }
{}

bool FilterRule::
match(const Uri &url, const Context&) const
{
    return m_pattern->match(url);
}
