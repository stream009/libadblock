#include "core/string_range.hpp"
#include "rule/rule.hpp"
#include "rule/filter_option.hpp"
#include "pattern/pattern.hpp"

#include <memory>
#include <string_view>
#include <vector>

#include <adblock_parser.hpp>

namespace adblock::parser {

using iterator = adblock_parser::iterator;

class RuleBuilder : public adblock_parser::event_handler
{
    using RulePtr = std::unique_ptr<Rule>;
    using PatternPtr = std::unique_ptr<Pattern>;

public:
    RuleBuilder(FilterList const&, std::vector<RulePtr>&);
    RuleBuilder(std::vector<RulePtr>&);

    ~RuleBuilder() override;

    //
    // filter rule
    //
    void begin_basic_filter_rule(iterator bol, iterator eol) override;
    void end_basic_filter_rule(iterator bol, iterator eol) override;

    void begin_exception_filter_rule(iterator bol, iterator eol) override;
    void end_exception_filter_rule(iterator bol, iterator eol) override;

    // address pattern
    void basic_address_pattern(
        iterator begin, iterator end, bool prefix, bool suffix) override;

    void domain_address_pattern(
        iterator begin, iterator end, bool suffix) override;

    void regex_address_pattern(iterator begin, iterator end) override;

    // filter option
    //void begin_filter_options(iterator begin, iterator end) override;
    //void end_filter_options(iterator begin, iterator end) override;

    void script_option(iterator begin, iterator end, bool inverse) override;
    void image_option(iterator begin, iterator end, bool inverse) override;
    void stylesheet_option(iterator begin, iterator end, bool inverse) override;
    void object_option(iterator begin, iterator end, bool inverse) override;
    void xmlhttprequest_option(iterator begin, iterator end, bool inverse) override;
    void object_subrequest_option(iterator begin, iterator end, bool inverse) override;
    void subdocument_option(iterator begin, iterator end, bool inverse) override;
    void ping_option(iterator begin, iterator end, bool inverse) override;
    void websocket_option(iterator begin, iterator end, bool inverse) override;
    void webrtc_option(iterator begin, iterator end, bool inverse) override;
    void popup_option(iterator begin, iterator end) override;
    void media_option(iterator begin, iterator end, bool inverse) override;
    void font_option(iterator begin, iterator end, bool inverse) override;
    void other_option(iterator begin, iterator end, bool inverse) override;
    void third_party_option(iterator begin, iterator end, bool inverse) override;
    void match_case_option(iterator begin, iterator end) override;
    void collapse_option(iterator begin, iterator end, bool inverse) override;
    //void do_not_track_option(iterator begin, iterator end) override;

    void begin_domain_option(iterator begin, iterator end) override;
    void filter_domain(iterator begin, iterator end) override;
    void end_domain_option(iterator begin, iterator end) override;

    void site_key_option(iterator begin, iterator end,
                         iterator value_begin, iterator value_end) override;
    void csp_option(iterator begin, iterator end,
                    iterator value_begin, iterator value_end) override;
    void rewrite_option(iterator begin, iterator end,
                        iterator value_begin, iterator value_end) override;

    void document_option(iterator begin, iterator end, bool inverse) override;
    void elem_hide_option(iterator begin, iterator end, bool inverse) override;
    void generic_hide_option(iterator begin, iterator end) override;
    void generic_block_option(iterator begin, iterator end) override;

    //
    // element hide rule
    //

    // basic element hiding rule
    //void begin_basic_element_hiding_rule(
    //                                iterator begin, iterator end) override;
    void end_basic_element_hiding_rule(
                                    iterator begin, iterator end) override;

    // exception element hiding rule
    //void begin_exception_element_hiding_rule(
    //                                iterator begin, iterator end) override;
    void end_exception_element_hiding_rule(
                                    iterator begin, iterator end) override;

    // extended element hiding rule
    //void begin_extended_element_hiding_rule(
    //                                iterator begin, iterator end) override;
    void end_extended_element_hiding_rule(
                                    iterator begin, iterator end) override;

    // element hiding domain(s)
    void begin_filter_domains(iterator begin, iterator end) override;
    //void end_filter_domains(iterator begin, iterator end) override;

    void css_selector(iterator begin, iterator end) override;

    //
    // snippet filter rule
    //
    void begin_snippet_filter_rule(iterator begin, iterator end) override;
    void end_snippet_filter_rule(iterator begin, iterator end) override;

    void snippet(iterator begin, iterator end) override;

    //
    // miscellaneous
    //
    void new_line(iterator bol, iterator eol) override;

    void comment(iterator bol, iterator eol) override;

    void error(iterator begin, iterator end, std::string_view msg) override;

private:
    void add_rule(RulePtr);

private:
    FilterList const* const m_filterList = nullptr;
    std::vector<RulePtr>& m_rules;

    PatternPtr m_pattern;
    FilterOptionSet m_options;
    std::unique_ptr<std::vector<StringRange>> m_domains;
    std::unique_ptr<std::vector<StringRange>> m_siteKeys;
    std::unique_ptr<StringRange> m_cspValue;
    StringRange m_selector;

    int64_t m_line_no = 0;
    StringRange m_line;
    bool m_skip = false;
};

} // namespace adblock::parser
