#ifndef ADBLOCK_FILTER_SET_HPP
#define ADBLOCK_FILTER_SET_HPP

#include "file.hpp"
#include "type.hpp"
#include "rule/rule.hpp"

#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include <boost/container/flat_map.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <boost/range/adaptor/indirected.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/iterator_range.hpp>

namespace adblock {

class FilterSet
{
public:
    using RulePtr = std::unique_ptr<Rule>;
    using Rules = std::vector<RulePtr>;
    using RulesRange = boost::indirected_range<Rules const>;
    using iterator = StringRange::iterator;
    using const_iterator = StringRange::const_iterator;
    using Path = boost::filesystem::path;
    using Parameters = boost::container::flat_map<StringRange, StringRange>;

    class ParseError;

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

    static StringRange supportedVersion();

    void validate() const
    {
        assert(boost::count(m_rules, nullptr) == 0);
    }

    // modifier
    void reload();

private:
    void parse(char const* buffer, size_t size);

private:
    File m_file;
    Rules m_rules;
};


//TODO derive from application specific base exception class
class FilterSet::ParseError : public std::exception
{
    const char* what() const noexcept override;
};

} // namespace adblock

#endif // ADBLOCK_FILTER_SET_HPP
