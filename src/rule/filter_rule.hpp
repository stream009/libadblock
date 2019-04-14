#ifndef FILTER_RULE_HPP
#define FILTER_RULE_HPP

#include "context.hpp"
#include "rule.hpp"
#include "type.hpp"
#include "option.hpp"

#include "filter_option.hpp"

#include <cassert>
#include <iosfwd>
#include <memory>
#include <vector>

#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/range/algorithm.hpp>

namespace adblock {

class Pattern;

class FilterRule : public Rule
{
    using Base = Rule;
public:
    using Options = std::vector<std::unique_ptr<Option>>;
    using OptionRefs = std::vector<Option const*>;

public:
    ~FilterRule() override;

    bool match(Uri const&, Context const&,
                           bool const specificOnly = false) const;

    Pattern const& pattern() const;

    FilterOption::Domains* domains() const { return m_option.domains(); }
    FilterOption::SiteKeys* siteKeys() const { return m_option.siteKeys(); }
    StringRange* cspValue() const { return m_option.cspValue(); }

    bool hasOption(uint32_t mask) const { return m_option.hasOption(mask); }
    size_t numOptions() const { return m_option.numOptions(); }

protected:
    FilterRule(std::unique_ptr<Pattern>,
               Options);

private:

    // @override Rule
    void print(std::ostream&) const override;

    void validate() const
    {
        assert(m_pattern);
    }

private:
    std::unique_ptr<Pattern> m_pattern;
    FilterOption m_option;
};

#if 0
template<typename OptionT>
OptionT const* FilterRule::
option() const
{
    namespace br = boost::range;

    auto isSameType =
        [](auto const& opt) {
            assert(opt);
            return typeid(*opt) == typeid(OptionT);
        };

    auto it = br::find_if(m_typeOptions, isSameType);
    if (it != m_typeOptions.end()) {
        return static_cast<OptionT*>(it->get());
    }

    it = br::find_if(m_restrictionOptions, isSameType);
    if (it != m_restrictionOptions.end()) {
        return static_cast<OptionT*>(it->get());
    }

    it = br::find_if(m_whiteListOptions, isSameType);
    if (it != m_whiteListOptions.end()) {
        return static_cast<OptionT*>(it->get());
    }

    it = br::find_if(m_otherOptions, isSameType);
    if (it != m_otherOptions.end()) {
        return static_cast<OptionT*>(it->get());
    }

    return nullptr;
}
#endif

} // namespace adblock

#endif // FILTER_RULE_HPP
