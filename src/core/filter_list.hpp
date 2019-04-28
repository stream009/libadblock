#ifndef ADBLOCK_FILTER_LIST_HPP
#define ADBLOCK_FILTER_LIST_HPP

#include "file.hpp"
#include "json_fwd.hpp"
#include "string_range.hpp"

#include "parser/parse_error.hpp"
#include "rule/rule.hpp"

#include <filesystem>
#include <memory>
#include <vector>

#include <boost/container/flat_map.hpp>
#include <boost/range/adaptor/indirected.hpp>

namespace adblock {

class FilterList
{
public:
    using RulePtr = std::unique_ptr<Rule>;
    using Rules = std::vector<RulePtr>;
    using RulesRange = boost::indirected_range<Rules const>;
    using const_iterator = StringRange::const_iterator;
    using Path = std::filesystem::path;
    using Parameters = boost::container::flat_map<StringRange, StringRange>;

public:
    // constructors
    FilterList(Path const& filePath);

    // accessors
    const_iterator begin() const;
    const_iterator end() const;

    RulesRange rules() const;
    Path const& path() const { return m_file.path(); }

    // query
    std::vector<ParseError> errors() const { return m_errors; }
    Parameters parameters() const;

    json::object statistics() const;

    // modifier
    void reload();

private:
    void parse();

private:
    File m_file;
    Rules m_rules;
    std::vector<ParseError> m_errors;
};

} // namespace adblock

#endif // ADBLOCK_FILTER_LIST_HPP
