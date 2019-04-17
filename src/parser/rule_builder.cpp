#include "rule_builder.hpp"

#include "pattern/basic_match_pattern.hpp"
#include "pattern/domain_match_pattern.hpp"
#include "pattern/regex_pattern.hpp"
#include "rule/basic_element_hide_rule.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/comment_rule.hpp"
#include "rule/exception_element_hide_rule.hpp"
#include "rule/exception_filter_rule.hpp"
#include "rule/extended_element_hide_rule.hpp"
#include "rule/filter_option.hpp"

#include <iostream>
#include <string>
#include <string_view>

#include <boost/algorithm/string/split.hpp>

namespace adblock::parser {

static void
print_error_line(std::ostream& os, StringRange const line,
                 int64_t const column, int64_t const length)
{
    auto const c = static_cast<size_t>(column);
    auto const l = static_cast<size_t>(std::max(length - 1, 0l));

    os << '\t' << line << '\n'
       << '\t' << std::string(c, ' ')
       << '^' << std::string(l, '-') << '\n';
}

RuleBuilder::
RuleBuilder(FilterSet const& filterSet, std::vector<RulePtr>& rules)
    : m_filterSet { &filterSet }
    , m_rules { rules }
{}

RuleBuilder::
RuleBuilder(std::vector<RulePtr>& rules)
    : m_rules { rules }
{}

RuleBuilder::~RuleBuilder() = default;

//
// filter rule
//
void RuleBuilder::
begin_basic_filter_rule(iterator /*bol*/, iterator /*eol*/)
{
    m_options = {};
}

void RuleBuilder::
end_basic_filter_rule(iterator /*bol*/, iterator /*eol*/)
{
    add_rule(
        std::make_unique<BasicFilterRule>(
            std::move(m_pattern),
            std::move(m_options),
            std::move(m_domains),
            std::move(m_siteKeys),
            std::move(m_cspValue)
        )
    );
}

void RuleBuilder::
begin_exception_filter_rule(iterator /*bol*/, iterator /*eol*/)
{
    m_options = {};
}

void RuleBuilder::
end_exception_filter_rule(iterator /*bol*/, iterator /*eol*/)
{
    add_rule(
        std::make_unique<ExceptionFilterRule>(
            std::move(m_pattern),
            std::move(m_options),
            std::move(m_domains),
            std::move(m_siteKeys),
            std::move(m_cspValue)
        )
    );
}

void RuleBuilder::
basic_address_pattern(iterator const begin, iterator const end,
                      bool const prefix, bool const suffix)
{
    m_pattern = std::make_unique<BasicMatchPattern>(
        StringRange { begin, end },
        prefix,
        suffix
    );
}

void RuleBuilder::
domain_address_pattern(iterator const begin, iterator const end,
                       bool const suffix)
{
    m_pattern = std::make_unique<DomainMatchPattern>(
        StringRange { begin, end },
        suffix
    );
}

void RuleBuilder::
regex_address_pattern(iterator const begin, iterator const end)
{
    m_pattern = std::make_unique<RegexPattern>(StringRange { begin, end });
}

void RuleBuilder::
script_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    if (inverse) {
        m_options.set(FilterOption::ScriptInv);
    }
    else {
        m_options.set(FilterOption::Script);
    }
}

void RuleBuilder::
image_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    if (inverse) {
        m_options.set(FilterOption::ImageInv);
    }
    else {
        m_options.set(FilterOption::Image);
    }
}

void RuleBuilder::
stylesheet_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    if (inverse) {
        m_options.set(FilterOption::StyleSheetInv);
    }
    else {
        m_options.set(FilterOption::StyleSheet);
    }
}

void RuleBuilder::
object_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    if (inverse) {
        m_options.set(FilterOption::ObjectInv);
    }
    else {
        m_options.set(FilterOption::Object);
    }
}

void RuleBuilder::
xmlhttprequest_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    if (inverse) {
        m_options.set(FilterOption::XmlHttpRequestInv);
    }
    else {
        m_options.set(FilterOption::XmlHttpRequest);
    }
}

void RuleBuilder::
object_subrequest_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    if (inverse) {
        m_options.set(FilterOption::ObjectSubRequestInv);
    }
    else {
        m_options.set(FilterOption::ObjectSubRequest);
    }
}

void RuleBuilder::
subdocument_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    if (inverse) {
        m_options.set(FilterOption::SubDocumentInv);
    }
    else {
        m_options.set(FilterOption::SubDocument);
    }
}

void RuleBuilder::
ping_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    if (inverse) {
        m_options.set(FilterOption::PingInv);
    }
    else {
        m_options.set(FilterOption::Ping);
    }
}

void RuleBuilder::
websocket_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    if (inverse) {
        m_options.set(FilterOption::WebSocketInv);
    }
    else {
        m_options.set(FilterOption::WebSocket);
    }
}

void RuleBuilder::
webrtc_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    if (inverse) {
        m_options.set(FilterOption::WebRtcInv);
    }
    else {
        m_options.set(FilterOption::WebRtc);
    }
}

void RuleBuilder::
popup_option(iterator /*begin*/, iterator /*end*/)
{
    m_options.set(FilterOption::Popup);
}

void RuleBuilder::
media_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    if (inverse) {
        m_options.set(FilterOption::MediaInv);
    }
    else {
        m_options.set(FilterOption::Media);
    }
}

void RuleBuilder::
font_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    if (inverse) {
        m_options.set(FilterOption::FontInv);
    }
    else {
        m_options.set(FilterOption::Font);
    }
}

void RuleBuilder::
other_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    if (inverse) {
        m_options.set(FilterOption::OtherInv);
    }
    else {
        m_options.set(FilterOption::Other);
    }
}

void RuleBuilder::
third_party_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    if (inverse) {
        m_options.set(FilterOption::SameOrigin);
    }
    else {
        m_options.set(FilterOption::ThirdParty);
    }
}

void RuleBuilder::
match_case_option(iterator /*begin*/, iterator /*end*/)
{
    m_options.set(FilterOption::MatchCase);
}

void RuleBuilder::
collapse_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    if (inverse) {
        m_options.set(FilterOption::NeverCollapse);
    }
    else {
        m_options.set(FilterOption::AlwaysCollapse);
    }
}

void RuleBuilder::
begin_domain_option(iterator /*begin*/, iterator /*end*/)
{
    m_domains = std::make_unique<std::vector<StringRange>>();
}

void RuleBuilder::
filter_domain(iterator const begin, iterator const end)
{
    m_domains->push_back({ begin, end });
}

void RuleBuilder::
end_domain_option(iterator /*begin*/, iterator /*end*/)
{
    m_options.set(FilterOption::Domain);
}

void RuleBuilder::
site_key_option(iterator /*begin*/, iterator /*end*/,
                iterator const value_begin, iterator const value_end)
{
    m_options.set(FilterOption::SiteKey);

    m_siteKeys = std::make_unique<std::vector<StringRange>>();
    StringRange value { value_begin, value_end };

    boost::algorithm::split(
        *m_siteKeys,
        value,
        [](auto c) { return c == '|'; }
    );
}

void RuleBuilder::
csp_option(iterator /*begin*/, iterator /*end*/,
           iterator const value_begin, iterator const value_end)
{
    m_options.set(FilterOption::Csp);
    m_cspValue = std::make_unique<StringRange>(value_begin, value_end);
}

void RuleBuilder::
rewrite_option(iterator /*begin*/, iterator /*end*/,
               iterator /*value_begin*/, iterator /*value_end*/)
{
    //TODO
}

void RuleBuilder::
document_option(iterator /*begin*/, iterator /*end*/, bool inverse)
{
    if (inverse) {
        m_options.set(FilterOption::DocumentInv);
    }
    else {
        m_options.set(FilterOption::Document);
    }
}

void RuleBuilder::
elem_hide_option(iterator /*begin*/, iterator /*end*/, bool inverse)
{
    if (inverse) {
        m_options.set(FilterOption::ElemHideInv);
    }
    else {
        m_options.set(FilterOption::ElemHide);
    }
}

void RuleBuilder::
generic_hide_option(iterator /*begin*/, iterator /*end*/)
{
    m_options.set(FilterOption::GenericHide);
}

void RuleBuilder::
generic_block_option(iterator /*begin*/, iterator /*end*/)
{
    m_options.set(FilterOption::GenericBlock);
}

//
// element hide rule
//
void RuleBuilder::
end_basic_element_hiding_rule(iterator /*begin*/, iterator /*end*/)
{
    add_rule(
        std::make_unique<BasicElementHideRule>(
            m_selector, std::move(m_domains)
        )
    );
}

void RuleBuilder::
end_exception_element_hiding_rule(iterator /*begin*/, iterator /*end*/)
{
    add_rule(
        std::make_unique<ExceptionElementHideRule>(
            m_selector, std::move(m_domains)
        )
    );
}

void RuleBuilder::
end_extended_element_hiding_rule(iterator /*begin*/, iterator /*end*/)
{
    add_rule(
        std::make_unique<ExtendedElementHideRule>(
            m_selector, std::move(m_domains)
        )
    );
}

void RuleBuilder::
begin_filter_domains(iterator /*begin*/, iterator /*end*/)
{
    m_domains = std::make_unique<std::vector<StringRange>>();
}

void RuleBuilder::
css_selector(iterator const begin, iterator const end)
{
    m_selector = { begin, end };
}

//
// snippet filter rule
//
void RuleBuilder::
begin_snippet_filter_rule(iterator /*begin*/, iterator /*end*/)
{
    //TODO
}

void RuleBuilder::
end_snippet_filter_rule(iterator /*begin*/, iterator /*end*/)
{
    //TODO
}

void RuleBuilder::
snippet(iterator /*begin*/, iterator /*end*/)
{
    //TODO
}

//
// miscellaneous
//
void RuleBuilder::
new_line(iterator const bol, iterator const eol)
// [[expects: bol <= eol]]
{
    ++m_line_no;
    m_line = { bol, eol };
}

void RuleBuilder::
comment(iterator const bol, iterator const eol)
{
    add_rule(
        std::make_unique<CommentRule>(StringRange { bol, eol })
    );
}

void RuleBuilder::
error(iterator const begin, iterator const end, std::string_view const msg)
{
    auto const column = begin - m_line.begin();
    auto const len = end - begin;

    std::cerr << '[' << m_line_no << ':' << column << ']'
              << ": " << msg << '\n';

    print_error_line(std::cerr, m_line, column, len);
}

void RuleBuilder::
add_rule(RulePtr rule)
{
    if (m_filterSet) {
        rule->setFilterSet(*m_filterSet);
    }

    rule->setLine(m_line);

    m_rules.push_back(std::move(rule));
}

} // namespace adblock::parser
