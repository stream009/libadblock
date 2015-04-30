#ifndef ADBLOCK_SUBSCRIPTION_HPP
#define ADBLOCK_SUBSCRIPTION_HPP

#include "type.hpp"
#include "rule/rule.hpp"

#include <memory>
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

public:
    // constructors
    Subscription(const Path &filePath);

    // accessors
    const_iterator begin() const;
    const_iterator end() const;

    RulesRange rules() const;

private:
    Path m_filePath;
    boost::iostreams::mapped_file_source m_file;
    Rules m_rules;
};

} // namespace adblock

#endif // ADBLOCK_SUBSCRIPTION_HPP
