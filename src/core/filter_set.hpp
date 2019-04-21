#ifndef ADBLOCK_FILTER_SET_HPP
#define ADBLOCK_FILTER_SET_HPP

#include "file.hpp"
#include "string_range.hpp"

#include "rule/rule.hpp"

#include <memory>
#include <vector>

#include <boost/container/flat_map.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <boost/range/adaptor/indirected.hpp>

namespace adblock {

class FilterSet //TODO change name to FilterList
{
public:
    using RulePtr = std::unique_ptr<Rule>;
    using Rules = std::vector<RulePtr>;
    using RulesRange = boost::indirected_range<Rules const>;
    using const_iterator = StringRange::const_iterator;
    using Path = boost::filesystem::path;
    using Parameters = boost::container::flat_map<StringRange, StringRange>;

public:
    // constructors
    FilterSet(Path const& filePath);

    // accessors
    const_iterator begin() const;
    const_iterator end() const;

    RulesRange rules() const;
    Path const& path() const { return m_file.path(); }

    // query
    Parameters parameters() const;

    boost::property_tree::ptree statistics() const;

    // modifier
    void reload();

private:
    void parse(char const* buffer, size_t size);

private:
    File m_file;
    Rules m_rules;
};

} // namespace adblock

#endif // ADBLOCK_FILTER_SET_HPP
