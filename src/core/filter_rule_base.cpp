#include "filter_rule_base.hpp"

#include "context.hpp"
#include "json.hpp"
#include "string_range.hpp"
#include "uri.hpp"
#include "white_list_query_context.hpp"

#include "rule/basic_filter_rule.hpp"
#include "rule/exception_filter_rule.hpp"

#include <cassert>
#include <utility>

namespace adblock {

class WhiteListQueryContextAdaptor : public WhiteListQueryContext
{
public:
    WhiteListQueryContextAdaptor(Uri const& uri, StringRange const siteKey)
        : m_uri { uri }
        , m_siteKey { siteKey }
    {}

    Uri const& origin() const override { return m_uri; }
    StringRange siteKey() const override { return m_siteKey; }

private:
    Uri const& m_uri;
    StringRange const m_siteKey;
};

class BlockWhiteListQueryContext : public WhiteListQueryContextAdaptor
{
public:
    using WhiteListQueryContextAdaptor::WhiteListQueryContextAdaptor;

    // @override WhiteListQueryContext
    bool blockDisablerMode() const override { return true; }
};

class GenericBlockWhiteListQueryContext : public WhiteListQueryContextAdaptor
{
public:
    using WhiteListQueryContextAdaptor::WhiteListQueryContextAdaptor;

    // @override WhiteListQueryContext
    bool genericBlockDisablerMode() const override { return true; }
};

class HideWhiteListQueryContext : public WhiteListQueryContextAdaptor
{
public:
    using WhiteListQueryContextAdaptor::WhiteListQueryContextAdaptor;

    // @override WhiteListQueryContext
    bool hideDisablerMode() const override { return true; }
};

class GenericHideWhiteListQueryContext : public WhiteListQueryContextAdaptor
{
public:
    using WhiteListQueryContextAdaptor::WhiteListQueryContextAdaptor;

    // @override WhiteListQueryContext
    bool genericHideDisablerMode() const override { return true; }
};


void FilterRuleBase::
put(FilterRule const& rule)
{
    if (typeid(rule) == typeid(BasicFilterRule)) {
        m_normal.put(rule);
    }
    else if (typeid(rule) == typeid(ExceptionFilterRule)) {
        m_exception.put(rule);
    }
    else {
        assert(false && "unknown type of filter rule");
    }
}

void FilterRuleBase::
clear()
{
    m_normal.clear();
    m_exception.clear();
}

std::pair<bool, FilterRule const*> FilterRuleBase::
query(Uri const& uri, Context const& context) const
{
    if (auto* const rule = getFrameBlockDisabler(
                            context.origin(), context.siteKey()))
    {
        return std::make_pair(false, rule);
    }
    else if (auto* rule = m_exception.query(uri, context)) {
        return std::make_pair(false, rule);
    }
    else {
        auto* const disabler = getGenericBlockDisabler(
                            context.origin(), context.siteKey());

        rule = m_normal.query(uri, context, disabler != nullptr);

        return std::make_pair(rule != nullptr, rule);
    }
}

FilterRule const* FilterRuleBase::
getFrameBlockDisabler(Uri const& uri, StringRange const siteKey) const
{
    BlockWhiteListQueryContext const context { uri, siteKey };
    return m_exception.query(uri, context);
}

FilterRule const* FilterRuleBase::
getGenericBlockDisabler(Uri const& uri, StringRange const siteKey) const
{
    GenericBlockWhiteListQueryContext const context { uri, siteKey };
    return m_exception.query(uri, context);
}

FilterRule const* FilterRuleBase::
getFrameHideDisabler(Uri const& uri, StringRange const siteKey) const
{
    HideWhiteListQueryContext const context { uri, siteKey };
    return m_exception.query(uri, context);
}

FilterRule const* FilterRuleBase::
getGenericHideDisabler(Uri const& uri, StringRange const siteKey) const
{
    GenericHideWhiteListQueryContext const context { uri, siteKey };
    return m_exception.query(uri, context);
}

json::object FilterRuleBase::
statistics() const
{
    json::object result, detail;

    double total = 0;

    auto stats = m_normal.statistics();
    auto num = to_number(stats["Total"]);
    total += num;
    result["Basic match pattern"] = num;
    detail["Basic match pattern"] = stats;

    stats = m_exception.statistics();
    num = to_number(stats["Total"]);
    total += num;
    result["Exception match pattern"] = num;
    detail["Exception match pattern"] = stats;

    result["Total"] = total;
    result["detail"] = detail;

    return result;
}

} // namespace adblock
