#include "filter_rule_base.hpp"

#include "context.hpp"
#include "white_list_query_context.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/exception_filter_rule.hpp"

#include <cassert>
#include <utility>

#include <boost/format.hpp>
#include <boost/property_tree/ptree.hpp>

namespace adblock {

class WhiteListQueryContextAdaptor : public WhiteListQueryContext
{
public:
    WhiteListQueryContextAdaptor(Uri const& uri, StringRange const& siteKey)
        : m_uri { uri }
        , m_siteKey { siteKey }
    {}

    Uri const& origin() const override { return m_uri; }
    StringRange siteKey() const override { return m_siteKey; }

private:
    Uri const& m_uri;
    StringRange const& m_siteKey;
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
put(const FilterRule &rule)
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
putGenericDisablerRule(const FilterRule &rule)
{
    m_genericDisabled.put(rule);
}

void FilterRuleBase::
clear()
{
    m_normal.clear();
    m_exception.clear();
}

std::pair<bool, const FilterRule*> FilterRuleBase::
query(const Uri &uri, const Context &context) const
{
    /*if (auto* rule = getFrameBlockDisabler(
                            context.origin(), context.siteKey()))
    {
        return std::make_pair(false, rule);
    }
    else*/ if (auto* rule = m_exception.query(uri, &context)) {
        return std::make_pair(false, rule);
    }
    else {
        bool const specificOnly = genericDisabled(context); //TODO
        rule = m_normal.query(uri, &context, specificOnly);
        return std::make_pair(rule != nullptr, rule);
    }
}

FilterRule const* FilterRuleBase::
getFrameBlockDisabler(Uri const& uri, StringRange const siteKey) const
{
    BlockWhiteListQueryContext const context { uri, siteKey };
    return m_exception.query(uri, &context);
}

FilterRule const* FilterRuleBase::
getGenericBlockDisabler(Uri const& uri, StringRange const siteKey) const
{
    GenericBlockWhiteListQueryContext const context { uri, siteKey };
    return m_exception.query(uri, &context);
}

FilterRule const* FilterRuleBase::
getFrameHideDisabler(Uri const& uri, StringRange const siteKey) const
{
    HideWhiteListQueryContext const context { uri, siteKey };
    return m_exception.query(uri, &context);
}

FilterRule const* FilterRuleBase::
getGenericHideDisabler(Uri const& uri, StringRange const siteKey) const
{
    GenericHideWhiteListQueryContext const context { uri, siteKey };
    return m_exception.query(uri, &context);
}

bool FilterRuleBase::
genericDisabled(const Context &context) const
{
    const auto &uri = context.origin();
    const auto* const rule = m_genericDisabled.query(uri, &context);

    return rule != nullptr;
}

boost::property_tree::ptree FilterRuleBase::
statistics() const
{
    boost::property_tree::ptree result, detail;

    size_t total = 0u;

    auto stats = m_normal.statistics();
    auto num = stats.get<size_t>("Total");
    total += num;
    result.put("Basic match pattern", num);
    detail.put_child("Basic match pattern", stats);

    stats = m_exception.statistics();
    num = stats.get<size_t>("Total");
    total += num;
    result.put("Exception match pattern", num);
    detail.put_child("Exception match pattern", stats);

    result.put("Total", total);

    result.put_child("detail", detail);

    return result;
}

} // namespace adblock
