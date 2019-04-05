#include "rule_builder.hpp"

#include "option/collapse_option.hpp"
#include "option/csp_option.hpp"
#include "option/do_not_track_option.hpp"
#include "option/domain_option.hpp"
#include "option/font_option.hpp"
#include "option/image_option.hpp"
#include "option/match_case_option.hpp"
#include "option/media_option.hpp"
#include "option/object_option.hpp"
#include "option/object_subrequest_option.hpp"
#include "option/other_option.hpp"
#include "option/ping_option.hpp"
#include "option/popup_option.hpp"
#include "option/script_option.hpp"
#include "option/site_key_option.hpp"
#include "option/style_sheet_option.hpp"
#include "option/subdocument_option.hpp"
#include "option/third_party_option.hpp"
#include "option/web_rtc_option.hpp"
#include "option/web_socket_option.hpp"
#include "option/xml_http_request_option.hpp"
#include "option/document_option.hpp"
#include "option/elem_hide_option.hpp"
#include "option/generic_hide_option.hpp"
#include "option/generic_block_option.hpp"
#include "pattern/basic_match_pattern.hpp"
#include "pattern/domain_match_pattern.hpp"
#include "pattern/regex_pattern.hpp"
#include "rule/basic_element_hide_rule.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/comment_rule.hpp"
#include "rule/exception_element_hide_rule.hpp"
#include "rule/exception_filter_rule.hpp"
#include "rule/extended_element_hide_rule.hpp"

#include <iostream>
#include <string>
#include <string_view>

#include <boost/optional.hpp>

namespace adblock::parser {

template<typename T>
boost::optional<std::vector<T>>
make_optional_vector(std::vector<T>&& v)
{
    if (v.empty()) {
        return boost::none;
    }
    else {
        return std::move(v);
    }
}

template<typename It>
auto
find_separator(It const begin, It const end)
{
    return std::find_if(begin, end,
        [](auto c) {
            return c == '/'
                || c == '^';
        });
}

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
        std::make_shared<BasicFilterRule>(
            m_pattern,
            make_optional_vector(std::move(m_options))
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
        std::make_shared<ExceptionFilterRule>(
            m_pattern,
            make_optional_vector(std::move(m_options))
        )
    );
}

void RuleBuilder::
basic_address_pattern(iterator const begin, iterator const end,
                      bool const prefix, bool const suffix)
{
    m_pattern = std::make_shared<BasicMatchPattern>(
        StringRange { begin, end },
        prefix,
        suffix
    );
}

void RuleBuilder::
domain_address_pattern(iterator const begin, iterator const end,
                       bool const suffix)
{
    auto const it = find_separator(begin, end);
    StringRange domain { begin, it };
    StringRange path { it, end };

    m_pattern = std::make_shared<DomainMatchPattern>(
        domain,
        path,
        suffix
    );
}

void RuleBuilder::
regex_address_pattern(iterator const begin, iterator const end)
{
    m_pattern = std::make_shared<RegexPattern>(StringRange { begin, end });
}

void RuleBuilder::
script_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    m_options.push_back(
        std::make_shared<ScriptOption>(inverse)
    );
}

void RuleBuilder::
image_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    m_options.push_back(
        std::make_shared<ImageOption>(inverse)
    );
}

void RuleBuilder::
stylesheet_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    m_options.push_back(
        std::make_shared<StyleSheetOption>(inverse)
    );
}

void RuleBuilder::
object_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    m_options.push_back(
        std::make_shared<ObjectOption>(inverse)
    );
}

void RuleBuilder::
xmlhttprequest_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    m_options.push_back(
        std::make_shared<XmlHttpRequestOption>(inverse)
    );
}

void RuleBuilder::
object_subrequest_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    m_options.push_back(
        std::make_shared<ObjectSubRequestOption>(inverse)
    );
}

void RuleBuilder::
subdocument_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    m_options.push_back(
        std::make_shared<SubDocumentOption>(inverse)
    );
}

void RuleBuilder::
ping_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    m_options.push_back(
        std::make_shared<PingOption>(inverse)
    );
}

void RuleBuilder::
websocket_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    m_options.push_back(
        std::make_shared<WebSocketOption>(inverse)
    );
}

void RuleBuilder::
webrtc_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    m_options.push_back(
        std::make_shared<WebRtcOption>(inverse)
    );
}

void RuleBuilder::
popup_option(iterator /*begin*/, iterator /*end*/)
{
    m_options.push_back(
        std::make_shared<PopUpOption>()
    );
}

void RuleBuilder::
media_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    m_options.push_back(
        std::make_shared<MediaOption>(inverse)
    );
}

void RuleBuilder::
font_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    m_options.push_back(
        std::make_shared<FontOption>(inverse)
    );
}

void RuleBuilder::
other_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    m_options.push_back(
        std::make_shared<OtherOption>(inverse)
    );
}

void RuleBuilder::
third_party_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    m_options.push_back(
        std::make_shared<ThirdPartyOption>(inverse)
    );
}

void RuleBuilder::
match_case_option(iterator /*begin*/, iterator /*end*/)
{
    m_options.push_back(
        std::make_shared<MatchCaseOption>()
    );
}

void RuleBuilder::
collapse_option(iterator /*begin*/, iterator /*end*/, bool const inverse)
{
    m_options.push_back(
        std::make_shared<CollapseOption>(inverse)
    );
}

void RuleBuilder::
do_not_track_option(iterator /*begin*/, iterator /*end*/)
{
    m_options.push_back(
        std::make_shared<DoNotTrackOption>()
    );
}

void RuleBuilder::
begin_domain_option(iterator /*begin*/, iterator /*end*/)
{
    m_domains = {};
}

void RuleBuilder::
filter_domain(iterator const begin, iterator const end)
{
    m_domains.push_back({ begin, end });
}

void RuleBuilder::
end_domain_option(iterator /*begin*/, iterator /*end*/)
{
    m_options.push_back(
        std::make_shared<DomainOption>(m_domains)
    );
}

void RuleBuilder::
site_key_option(iterator /*begin*/, iterator /*end*/,
                iterator const value_begin, iterator const value_end)
{
    std::vector<StringRange> keys; //TODO

    keys.push_back({ value_begin, value_end });

    m_options.push_back(
        std::make_shared<SiteKeyOption>(keys)
    );
}

void RuleBuilder::
csp_option(iterator /*begin*/, iterator /*end*/,
           iterator const value_begin, iterator const value_end)
{
    m_options.push_back(
        std::make_shared<CspOption>(StringRange { value_begin, value_end })
    );
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
    m_options.push_back(
        std::make_shared<DocumentOption>(inverse)
    );
}

void RuleBuilder::
elem_hide_option(iterator /*begin*/, iterator /*end*/, bool inverse)
{
    m_options.push_back(
        std::make_shared<ElemHideOption>(inverse)
    );
}

void RuleBuilder::
generic_hide_option(iterator /*begin*/, iterator /*end*/)
{
    m_options.push_back(
        std::make_shared<GenericHideOption>()
    );
}

void RuleBuilder::
generic_block_option(iterator /*begin*/, iterator /*end*/)
{
    m_options.push_back(
        std::make_shared<GenericBlockOption>()
    );
}

//
// element hide rule
//
void RuleBuilder::
begin_basic_element_hiding_rule(iterator /*begin*/, iterator /*end*/)
{
    m_domains = {};
}

void RuleBuilder::
end_basic_element_hiding_rule(iterator /*begin*/, iterator /*end*/)
{
    add_rule(
        std::make_shared<BasicElementHideRule>(
            m_selector,
            make_optional_vector(std::move(m_domains))
        )
    );
}

void RuleBuilder::
begin_exception_element_hiding_rule(iterator /*begin*/, iterator /*end*/)
{
    m_domains = {};
}

void RuleBuilder::
end_exception_element_hiding_rule(iterator /*begin*/, iterator /*end*/)
{
    add_rule(
        std::make_shared<ExceptionElementHideRule>(
            m_selector,
            make_optional_vector(std::move(m_domains))
        )
    );
}

void RuleBuilder::
begin_extended_element_hiding_rule(iterator /*begin*/, iterator /*end*/)
{
    m_domains = {};
}

void RuleBuilder::
end_extended_element_hiding_rule(iterator /*begin*/, iterator /*end*/)
{
    add_rule(
        std::make_shared<ExtendedElementHideRule>(
            m_selector,
            make_optional_vector(std::move(m_domains))
        )
    );
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
        std::make_shared<CommentRule>(StringRange { bol, eol })
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
add_rule(RulePtr const rule)
{
    if (m_filterSet) {
        rule->setFilterSet(*m_filterSet);
    }

    rule->setLine(m_line);

    m_rules.push_back(rule);
}

} // namespace adblock::parser
