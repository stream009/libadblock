#ifndef ADBLOCK_SUBSCRIPTION_HPP
#define ADBLOCK_SUBSCRIPTION_HPP

#include "type.hpp"
#include "rule/rule.hpp"

#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/range/iterator_range.hpp>

namespace adblock {

class Subscription
{
public:
    using RulePtr = std::shared_ptr<Rule>;
    using Rules = std::vector<RulePtr>;
    using RulesRange = boost::iterator_range<Rules::const_iterator>;
    using iterator = StringRange::iterator;
    using const_iterator = StringRange::const_iterator;
    using Path = boost::filesystem::path;

    class ParseError;
    struct Statistics;

public:
    // constructors
    Subscription(const Path &filePath);

    // accessors
    const_iterator begin() const;
    const_iterator end() const;

    RulesRange rules() const;

    // query
    Statistics statistics() const;

    static const StringRange supportedVersion();

private:
    boost::iostreams::mapped_file_source m_file;
    Rules m_rules;
};


//TODO derive from application specific base exception class
class Subscription::ParseError : public std::exception
{
    const char* what() const noexcept override;
};


struct Subscription::Statistics {
    struct FilterRule {
        size_t total;
        size_t basicMatchPattern;
        size_t domainMatchPattern;
        size_t regexPattern;
    };

    struct ElementHideRule {
        size_t total;
        size_t basic;
        size_t domainRestricted;
    };

    FilterRule      basicFilterRule;
    FilterRule      exceptionFilterRule;
    ElementHideRule basicElementHideRule;
    ElementHideRule exceptionElementHideRule;
    size_t          commentRule;
};

std::ostream &operator<<(std::ostream&, const Subscription::Statistics&);

} // namespace adblock

#endif // ADBLOCK_SUBSCRIPTION_HPP
