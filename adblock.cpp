#include "adblock.hpp"

#include "parser/parser.hpp"
#include "rule/comment_rule.hpp"
#include "rule/element_hide_rule.hpp"
#include "rule/filter_rule.hpp"
#include "rule/rule.hpp"

#include <memory>

#include <boost/range/algorithm.hpp>
#include <boost/algorithm/string.hpp>

namespace adblock {

void AdBlock::
addSubscription(const Path &filePath)
{
    const Subscription subscription { filePath };

    for (const auto &rule: subscription.rules()) {
        if (const auto &ptr = std::dynamic_pointer_cast<FilterRule>(rule)) {
            m_filterRuleBase.put(ptr);
        }
        else if (const auto &ptr =
                    std::dynamic_pointer_cast<ElementHideRule>(rule))
        {
            m_elementHideRuleBase.put(ptr);
        }
        else if (std::dynamic_pointer_cast<CommentRule>(rule)) {
            // comment rule will be skipped
        }
        else {
            assert(false && "unknown type of rule");
        }
    }

    m_subscriptions.push_back(subscription);
}

bool AdBlock::
shouldBlock(const Uri &uri, const Context &context) const
{
    return m_filterRuleBase.query(uri, context);
}

std::string AdBlock::
elementHideCss(const Uri &uri) const
{
    return m_elementHideRuleBase.query(uri);
}

} // namespace adblock
