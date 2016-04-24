#ifndef FILTER_RULE_HPP
#define FILTER_RULE_HPP

#include "context.hpp"
#include "rule.hpp"
#include "type.hpp"

#include <cassert>
#include <iosfwd>
#include <memory>
#include <vector>

#include <boost/algorithm/cxx11/any_of.hpp>
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
    bool match(const Uri&, const Context* const) const;

    const Pattern &pattern() const;
    OptionsRange options() const;

    template<typename OptionT>
    bool hasOption() const
    {
        if (!m_options) return false;

        return boost::algorithm::any_of(*m_options,
            [](const Options::value_type &option) {
                assert(option);
                const auto &theOption = *option;
                return typeid(theOption) == typeid(OptionT);
            }
        );
    }

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

private:
    std::shared_ptr<Pattern> m_pattern;
    boost::optional<Options> m_options;
};

} // namespace adblock

#endif // FILTER_RULE_HPP
