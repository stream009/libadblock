#ifndef ADBLOCK_HPP
#define ADBLOCK_HPP

#include "context.hpp"
#include "element_hide_rule_base.hpp"
#include "filter_rule_base.hpp"
#include "subscription.hpp"
#include "type.hpp"

#include <vector>

#include <boost/filesystem.hpp>

namespace adblock {

class AdBlock
{
    using Path = boost::filesystem::path;
public:
    // modifier
    void addSubscription(const Path &filePath);

    // query
    bool shouldBlock(const Uri&, const Context&) const;
    std::string elementHideCss(const Uri&) const;

private:
    std::vector<Subscription> m_subscriptions;
    FilterRuleBase m_filterRuleBase;
    ElementHideRuleBase m_elementHideRuleBase;
};

} // namespace adblock

#endif // ADBLOCK_HPP
