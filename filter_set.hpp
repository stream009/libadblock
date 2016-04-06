#ifndef ADBLOCK_FILTER_SET_HPP
#define ADBLOCK_FILTER_SET_HPP

#include "type.hpp"
#include "rule/rule.hpp"

#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <boost/range/adaptor/indirected.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/iterator_range.hpp>

namespace adblock {

class FilterSet
{
public:
    using RulePtr = std::shared_ptr<Rule>;
    using Rules = std::vector<RulePtr>;
    using RulesRange = boost::indirected_range<const Rules>;
    using iterator = StringRange::iterator;
    using const_iterator = StringRange::const_iterator;
    using Path = boost::filesystem::path;

    class ParseError;

public:
    // constructors
    FilterSet(const Path &filePath);

    // accessors
    const_iterator begin() const;
    const_iterator end() const;

    RulesRange rules() const;
    const Path &path() const { return m_path; }

    // query
    boost::property_tree::ptree statistics() const;

    static const StringRange supportedVersion();

    void validate() const
    {
        assert(m_file.is_open());
        assert(boost::count(m_rules, nullptr) == 0);
    }

    // modifier
    void reload();

private:
    void parse(const char *buffer, const size_t size);

private:
    boost::iostreams::mapped_file_source m_file;
    Path m_path;
    Rules m_rules;
};


//TODO derive from application specific base exception class
class FilterSet::ParseError : public std::exception
{
    const char* what() const noexcept override;
};

} // namespace adblock

#endif // ADBLOCK_FILTER_SET_HPP
