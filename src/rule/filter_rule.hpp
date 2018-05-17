#ifndef FILTER_RULE_HPP
#define FILTER_RULE_HPP

#include "context.hpp"
#include "rule.hpp"
#include "type.hpp"
#include "option.hpp"

#include <cassert>
#include <iosfwd>
#include <memory>
#include <vector>

#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/optional.hpp>
#include <boost/range/algorithm.hpp>

namespace adblock {

class Pattern;

class FilterRule : public Rule
{
    using Base = Rule;
public:
    using Options = std::vector<std::shared_ptr<Option>>;
    using OptionsRange = boost::iterator_range<Options::const_iterator>;

public:
    bool match(Uri const&, Context const&,
                           bool const specificOnly = false) const;

    const Pattern &pattern() const;
    Options options() const;

    template<typename OptionT>
    OptionT const* option() const;

    template<typename OptionT>
    bool hasOption() const
    {
        namespace ba = boost::algorithm;

        auto isSameType =
            [](std::shared_ptr<Option> const& opt) {
                assert(opt);
                auto const& option = *opt;
                return typeid(option) == typeid(OptionT);
            };

        auto const rv = ba::any_of(m_typeOptions, isSameType)
            || ba::any_of(m_restrictionOptions, isSameType)
            || ba::any_of(m_whiteListOptions, isSameType)
            || ba::any_of(m_otherOptions, isSameType);

        return rv;
    }

protected:
    FilterRule(const std::shared_ptr<Pattern>&,
               const boost::optional<Options>&);

private:
    bool matchWhiteListOptions(Uri const&, Context const&) const;
    bool matchTypeOptions(Uri const&, Context const&) const;
    bool matchRestrictionOptions(Uri const&, Context const&) const;

    // @override Rule
    void print(std::ostream&) const override;

    void validate() const
    {
        assert(m_pattern);
    }

private:
    std::shared_ptr<Pattern> m_pattern;
    Options m_typeOptions;
    Options m_restrictionOptions;
    Options m_whiteListOptions;
    Options m_otherOptions;
    bool m_caseSensitive = false;
    bool m_domainSpecific = false;
};

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

} // namespace adblock

#endif // FILTER_RULE_HPP
