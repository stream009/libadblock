#include <adblock/database.hpp>

#include "core/adblock.hpp"
#include "core/context.hpp"
#include "core/filter_list.hpp"
#include "core/json.hpp"
#include "core/string_range.hpp"
#include "core/uri.hpp"
#include "rule/snippet_rule.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace adblock {

class request_context : public Context
{
public:
    request_context(request_type const type,
                    std::string_view const origin,
                    std::string_view const site_key)
        : m_type { type }
        , m_origin { origin }
        , m_site_key { site_key }
    {}

    Uri const& origin() const override { return m_origin; }

    bool fromScriptTag() const override { return m_type == request_type::script; }
    bool fromImageTag() const  override { return m_type == request_type::image; }
    bool fromAudioVideoTag() const override { return m_type == request_type::media; }
    bool isExternalStyleSheet() const override { return m_type == request_type::style_sheet; }
    bool fromObjectTag() const override { return m_type == request_type::object; }
    bool fromXmlHttpRequest() const override { return m_type == request_type::xml_http_request; }
    bool fromPlugins() const override { return m_type == request_type::object; }
    bool isSubdocument() const override { return m_type == request_type::subdocument; }
    bool isPopup() const override { return m_type == request_type::popup; }
    bool isFont() const override { return m_type == request_type::font; }
    bool isWebSocket() const override { return m_type == request_type::web_socket; }
    bool isPing() const override { return m_type == request_type::ping; }
    bool isWebRtc() const override { return m_type == request_type::web_rtc; }

    StringRange siteKey() const override { return m_site_key; }

private:
    request_type m_type;
    Uri m_origin;
    StringRange m_site_key;
};

//
// database
//
database::
database()
    : m_adblock { std::make_unique<AdBlock>() }
{}

database::~database() = default;

block_result database::
should_block_request(std::string_view const url,
                     request_type const type,
                     std::string_view const origin,
                     std::string_view const site_key/*= {}*/) const
{
    try {
        request_context const cxt { type, origin, site_key };

        auto const& [block, rule] = m_adblock->shouldBlock(url, cxt);

        block_result result {};

        result.block = block;

        if (rule) {
            auto* const filter_list = rule->filterList();
            if (filter_list) {
                result.filter_list = &filter_list->path();
            }

            result.filter = rule->line();
        }

        return result;
    }
    catch (adblock::uri_error const& e) {
        throw url_parse_error { e.url(), e.what(), e.location() };
    }
}

std::vector<std::string_view> database::
element_hiding_selectors(std::string_view const url,
                         std::string_view const site_key/*= {}*/) const
{
    try {
        auto const& selectors = m_adblock->elementHideSelectors(url, site_key);

        std::vector<std::string_view> result;
        result.reserve(selectors.size());

        for (auto const selector: selectors) {
            result.emplace_back(selector);
        }

        return result;
    }
    catch (adblock::uri_error const& e) {
        throw url_parse_error { e.url(), e.what(), e.location() };
    }
}

std::vector<std::string_view> database::
extended_element_hiding_selectors(std::string_view const url,
                                  std::string_view const site_key/*= {}*/) const
{
    try {
        auto const& selectors =
            m_adblock->extendedElementHideSelectors(url, site_key);

        std::vector<std::string_view> result;
        result.reserve(selectors.size());

        for (auto const selector: selectors) {
            result.emplace_back(selector);
        }

        return result;
    }
    catch (adblock::uri_error const& e) {
        throw url_parse_error { e.url(), e.what(), e.location() };
    }
}

std::string_view database::
content_security_policy(std::string_view const url,
                        std::string_view const site_key/*= {}*/) const
{
    try {
        return m_adblock->contentSecurityPolicy(url, site_key);
    }
    catch (adblock::uri_error const& e) {
        throw url_parse_error { e.url(), e.what(), e.location() };
    }
}

std::vector<std::string_view> database::
snippets(std::string_view const url, std::string_view site_key/*= {}*/) const
{
    try {
        std::vector<std::string_view> result;

        for (auto* const s: m_adblock->snippets(url, site_key)) {
            result.push_back(s->snippet());
        }

        return result;
    }
    catch (adblock::uri_error const& e) {
        throw url_parse_error { e.url(), e.what(), e.location() };
    }
}

std::vector<parameter> database::
filter_list_parameters(std::filesystem::path const& path) const
{
    std::vector<parameter> result;

    auto* const filter_list = m_adblock->filterList(path);

    if (filter_list) {
        for (auto const& p: filter_list->parameters()) {
            result.push_back({ p.first, p.second });
        }
    }

    return result;
}

std::vector<parse_error> database::
filter_list_errors(std::filesystem::path const& path) const
{
    std::vector<parse_error> result;

    auto* const filter_list = m_adblock->filterList(path);

    if (filter_list) {
        for (auto const& err: filter_list->errors()) {
            result.push_back({
                err.line_no,
                err.from,
                err.to,
                err.line,
                err.message
            });
        }
    }

    return result;
}

std::string database::
statistics() const
{
    auto const& stats = m_adblock->statistics();

    return stats.to_string();
}

void database::
add_filter_list(std::filesystem::path const& path)
{
    m_adblock->addFilterList(path);
}

void database::
remove_filter_list(std::filesystem::path const& path)
{
    m_adblock->removeFilterList(path);
}

void database::
reload()
{
    m_adblock->reload();
}

//
// url_parse_error
//
url_parse_error::
url_parse_error(std::string_view const url,
                std::string const& message,
                int64_t const location)
    : std::runtime_error { message }
    , m_url { url }
    , m_location { location }
{}

url_parse_error::~url_parse_error() = default;

} // namespace adblock
