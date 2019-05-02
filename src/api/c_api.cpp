#include <adblock/database.h>

#include "core/adblock.hpp"
#include "core/context.hpp"
#include "core/filter_list.hpp"
#include "core/json.hpp"
#include "core/string_range.hpp"
#include "lib/uri/parser.hpp"
#include "rule/snippet_rule.hpp"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace fs = std::filesystem;

static adblock::StringRange to_string_range(adblock_string_t const*);

class adblock_db : public adblock::AdBlock {};

class context : public adblock::Context
{
public:
    context(request_type const type,
            adblock_string_t const* const origin,
            adblock_string_t const* const site_key)
        : m_request_type { type }
        , m_origin { to_string_range(origin) }
        , m_site_key { to_string_range(site_key) }
    {}

    adblock::Uri const& origin() const override
    {
        return m_origin; // origin could be invalid.
    }

    bool fromScriptTag() const override
    {
        return m_request_type == ADBLOCK_REQUEST_SCRIPT;
    }

    bool fromImageTag() const override
    {
        return m_request_type == ADBLOCK_REQUEST_IMAGE;
    }

    bool fromAudioVideoTag() const override
    {
        return m_request_type == ADBLOCK_REQUEST_MEDIA;
    }

    bool isExternalStyleSheet() const override
    {
        return m_request_type == ADBLOCK_REQUEST_STYLE_SHEET;
    }

    bool fromObjectTag() const override
    {
        return m_request_type == ADBLOCK_REQUEST_OBJECT;
    }

    bool fromXmlHttpRequest() const override
    {
        return m_request_type == ADBLOCK_REQUEST_XML_HTTP_REQUEST;
    }

    bool fromPlugins() const override
    {
        return m_request_type == ADBLOCK_REQUEST_OBJECT_SUBREQUEST;
    }

    bool isSubdocument() const override
    {
        return m_request_type == ADBLOCK_REQUEST_SUBDOCUMENT;
    }

    bool isPopup() const override
    {
        return m_request_type == ADBLOCK_REQUEST_POPUP;
    }

    bool isFont() const override
    {
        return m_request_type == ADBLOCK_REQUEST_FONT;
    }

    bool isWebSocket() const override
    {
        return m_request_type == ADBLOCK_REQUEST_WEB_SOCKET;
    }

    bool isPing() const override
    {
        return m_request_type == ADBLOCK_REQUEST_PING;
    }

    bool isWebRtc() const override
    {
        return m_request_type == ADBLOCK_REQUEST_WEB_RTC;
    }

    adblock::StringRange siteKey() const override
    {
        return m_site_key;
    }

private:
    request_type m_request_type;
    adblock::Uri m_origin;
    adblock::StringRange m_site_key;
};

static std::vector<std::unique_ptr<adblock_db_t>> s_databases;
static std::vector<std::vector<char>> s_strings;
static std::vector<std::vector<adblock_string_t>> s_stringVectors;
static std::vector<std::unique_ptr<adblock_error_t>> s_errors;

static adblock::StringRange
to_string_range(adblock_string_t const* const s)
{
    if (s == nullptr) {
        return {};
    }
    else {
        return { s->ptr, s->length };
    }
}

static adblock_string_t
to_string_t(adblock::StringRange const s)
{
    return { s.data(), s.size() };
}

static fs::path
to_path(adblock_string_t const* const s) {
    if (s == nullptr) {
        return {};
    }
    else {
        return { s->ptr, s->ptr + s->length };
    }
}

static adblock_string_t
adblock_string_new(std::string_view const s)
{
    s_strings.emplace_back(s.begin(), s.end());
    auto& v = s_strings.back();

    return { v.data(), v.size() };
}

static adblock_str_array_t
adblock_str_array_new(size_t const size)
{
    s_stringVectors.emplace_back();
    auto& v = s_stringVectors.back();

    v.reserve(size);

    return { v.data(), v.size() };
}

static std::ostream&
operator<<(std::ostream& os, adblock_string_t const* const s)
{
    return os << to_string_range(s);
}

template<typename... S>
adblock_error_t*
adblock_error_new(S&&... s)
{
    std::ostringstream oss;
    (oss << ... << s);

    s_errors.emplace_back();
    auto& e = s_errors.back();
    e->message = adblock_string_new(oss.str());

    return e.get();
}

template<typename... S>
void
report_error(adblock_error_t** const error, S&&... s)
{
    if (error) {
        *error = adblock_error_new(std::forward<S>(s)...);
    }
    else {
        (std::cerr << ... << s) << "\n";
    }
}

adblock_db_t *
adblock_db_new()
{
    s_databases.push_back(std::make_unique<adblock_db_t>());
    return s_databases.back().get();
}

void
adblock_db_free(adblock_db_t* const db)
{
    auto const it = std::remove_if(
        s_databases.begin(), s_databases.end(),
        [&](auto& ptr) { return ptr.get() == db; }
    );

    s_databases.erase(it, s_databases.end());
}

void
adblock_add_filter_list(adblock_db_t* const db,
        adblock_string_t const* const path,
        adblock_error_t** error)
{
    try {
        if (db == nullptr) {
            report_error(error, __func__, ": Invalid parameter: db");
            return;
        }
        if (path == nullptr) {
            report_error(error, __func__, ": Invalid parameter: path");
            return;
        }
        if (error != nullptr) {
            *error = nullptr;
        }

        auto const p = to_path(path);

        if (!fs::exists(p)) {
            report_error(error, __func__, ": File doesn't exists: ", p);
            return;
        }

        db->addFilterList(p);
    }
    catch (std::exception const& e) {
        report_error(error, __func__, ": Uncaught exception: ", e.what());
    }
    catch (...) {
        report_error(error, __func__, ": Unknown exception is caught.");
    }
}

void
adblock_remove_filter_list(adblock_db_t* const db,
        adblock_string_t const* const path,
        adblock_error_t** error)
{
    try {
        if (db == nullptr) {
            report_error(error, __func__, ": Invalid parameter: db");
            return;
        }
        if (path == nullptr) {
            report_error(error, __func__, ": Invalid parameter: path");
            return;
        }
        if (error != nullptr) {
            *error = nullptr;
        }

        auto const p = to_path(path);

        db->removeFilterList(p);
    }
    catch (std::exception const& e) {
        report_error(error, __func__, ": Uncaught exception: ", e.what());
    }
    catch (...) {
        report_error(error, __func__, ": Unknown exception is caught.");
    }
}

bool
adblock_should_block(adblock_db_t* const db,
        adblock_string_t const* const url,
        request_type const type,
        adblock_string_t const* const origin,
        adblock_string_t const* const site_key,
        adblock_string_t* const filter_list /* OUT */,
        adblock_string_t* const reason /* OUT */,
        adblock_error** error/* OUT */)
{
    try {
        if (db == nullptr) {
            report_error(error, __func__, ": Invalid parameter: db");
            return false;
        }
        if (url == nullptr) {
            report_error(error, __func__, ": Invalid parameter: url");
            return false;
        }
        if (origin == nullptr) {
            report_error(error, __func__, ": Invalid parameter: origin");
            return false;
        }
        if (filter_list != nullptr) {
            *filter_list = {};
        }
        if (reason != nullptr) {
            *reason = {};
        }
        if (error != nullptr) {
            *error = nullptr;
        }

        adblock::Uri const uri { url->ptr, url->length };

        context const cxt { type, origin, site_key };

        auto const& [should_block, reason_rule] = db->shouldBlock(uri, cxt);

        if (reason_rule) {
            auto* const filter_list_p = reason_rule->filterList();
            if (filter_list_p && filter_list) {
                auto const& path = filter_list_p->path();

                filter_list->ptr = path.c_str();
                filter_list->length = std::strlen(path.c_str());
            }

            if (reason) {
                auto const& line = reason_rule->line();

                reason->ptr = line.begin();
                reason->length = line.size();
            }
        }

        return should_block;
    }
    catch (uri::parse_error const& e) {
        report_error(error, __func__, ": URL parse error: ", url);
    }
    catch (std::exception const& e) {
        report_error(error, __func__, ": Uncaught exception: ", e.what());
    }
    catch (...) {
        report_error(error, __func__, ": Unknown exception is caught.");
    }

    return false;
}

void
adblock_element_hiding_selectors(adblock_db_t* const db,
        adblock_string_t const* const url,
        adblock_string_t const* const site_key,
        adblock_str_array_t* const selectors /* OUT */,
        adblock_error_t** error /* OUT */)
{
    try {
        if (db == nullptr) {
            report_error(error, __func__, ": Invalid parameter: db");
            return;
        }
        if (url == nullptr) {
            report_error(error, __func__, ": Invalid parameter: url");
            return;
        }
        if (selectors == nullptr) {
            report_error(error, __func__, ": Invalid parameter: selectors");
            return;
        }
        if (error != nullptr) {
            *error = nullptr;
        }

        adblock::Uri const uri { url->ptr, url->length };
        *selectors = {};

        auto const sels = db->elementHideSelectors(uri, to_string_range(site_key));

        if (!sels.empty()) {
            *selectors = adblock_str_array_new(sels.size());

            std::transform(sels.begin(), sels.end(),
                           selectors->ptr,
                [](auto const s) -> adblock_string_t {
                    return { s.begin(), s.size() };
                }
            );

            selectors->length = sels.size();
        }
    }
    catch (uri::parse_error const& e) {
        report_error(error, __func__, ": URL parse error: ", url);
    }
    catch (std::exception const& e) {
        report_error(error, __func__, ": Uncaught exception: ", e.what());
    }
    catch (...) {
        report_error(error, __func__, ": Unknown exception is caught.");
    }
}

void
adblock_extended_element_hiding_selectors(adblock_db_t* const db,
        adblock_string_t const* const url,
        adblock_string_t const* const site_key,
        adblock_str_array_t* const selectors /* OUT */,
        adblock_error_t** error /* OUT */)
{
    try {
        if (db == nullptr) {
            report_error(error, __func__, ": Invalid parameter: db");
            return;
        }
        if (url == nullptr) {
            report_error(error, __func__, ": Invalid parameter: url");
            return;
        }
        if (selectors == nullptr) {
            report_error(error, __func__, ": Invalid parameter: selectors");
            return;
        }
        if (error != nullptr) {
            *error = nullptr;
        }

        adblock::Uri const uri { url->ptr, url->length };
        *selectors = {};

        auto const v =
            db->extendedElementHideSelectors(uri, to_string_range(site_key));

        if (!v.empty()) {
            *selectors = adblock_str_array_new(v.size());

            std::transform(v.begin(), v.end(), selectors->ptr, to_string_t);

            selectors->length = v.size();
        }
    }
    catch (uri::parse_error const& e) {
        report_error(error, __func__, ": URL parse error: ", url);
    }
    catch (std::exception const& e) {
        report_error(error, __func__, ": Uncaught exception: ", e.what());
    }
    catch (...) {
        report_error(error, __func__, ": Unknown exception is caught.");
    }
}

void
adblock_content_security_policy(adblock_db_t* const db,
        adblock_string_t const* const url,
        adblock_string_t const* const site_key,
        adblock_string_t* policy /* OUT */,
        adblock_error_t** error /* OUT */)
{
    try {
        if (db == nullptr) {
            report_error(error, __func__, ": Invalid parameter: db");
            return;
        }
        if (url == nullptr) {
            report_error(error, __func__, ": Invalid parameter: url");
            return;
        }
        if (policy == nullptr) {
            report_error(error, __func__, ": Invalid parameter: policy");
            return;
        }
        if (error != nullptr) {
            *error = nullptr;
        }

        adblock::Uri const uri { url->ptr, url->length };

        auto const p = db->contentSecurityPolicy(uri, to_string_range(site_key));

        policy->ptr = p.data();
        policy->length = p.size();
    }
    catch (uri::parse_error const& e) {
        report_error(error, __func__, ": URL parse error: ", url);
    }
    catch (std::exception const& e) {
        report_error(error, __func__, ": Uncaught exception: ", e.what());
    }
    catch (...) {
        report_error(error, __func__, ": Unknown exception is caught.");
    }
}

void
adblock_snippets(adblock_db_t* const db,
        adblock_string_t const* const url,
        adblock_string_t const* const site_key,
        adblock_str_array_t* snippets /* OUT */,
        adblock_error_t** error /* OUT */)
{
    try {
        if (db == nullptr) {
            report_error(error, __func__, ": Invalid parameter: db");
            return;
        }
        if (url == nullptr) {
            report_error(error, __func__, ": Invalid parameter: url");
            return;
        }
        if (snippets == nullptr) {
            report_error(error, __func__, ": Invalid parameter: snippets");
            return;
        }
        if (error != nullptr) {
            *error = nullptr;
        }

        adblock::Uri const uri { url->ptr, url->length };
        *snippets = {};

        auto const v = db->snippets(uri, to_string_range(site_key));

        if (!v.empty()) {
            *snippets = adblock_str_array_new(v.size());

            std::transform(v.begin(), v.end(), snippets->ptr,
                [&](auto s) { return to_string_t(s->snippet()); }
            );

            snippets->length = v.size();
        }
    }
    catch (uri::parse_error const& e) {
        report_error(error, __func__, ": URL parse error: ", url);
    }
    catch (std::exception const& e) {
        report_error(error, __func__, ": Uncaught exception: ", e.what());
    }
    catch (...) {
        report_error(error, __func__, ": Unknown exception is caught.");
    }
}

void
adblock_filter_list_parameters(adblock_db_t* const db,
        adblock_string_t const* const path,
        adblock_str_array_t* keys /* OUT */,
        adblock_str_array_t* values /* OUT */,
        adblock_error_t** error /* OUT */)
{
    try {
        if (db == nullptr) {
            report_error(error, __func__, ": Invalid parameter: db");
            return;
        }
        if (path == nullptr) {
            report_error(error, __func__, ": Invalid parameter: path");
            return;
        }
        if (keys == nullptr) {
            report_error(error, __func__, ": Invalid parameter: keys");
            return;
        }
        if (values == nullptr) {
            report_error(error, __func__, ": Invalid parameter: values");
            return;
        }
        if (error != nullptr) {
            *error = nullptr;
        }

        auto const p = to_path(path);

        auto* const filter_list = db->filterList(p);
        if (filter_list == nullptr) {
            report_error(error, __func__, ": Invalid path: ", p);
            return;
        }

        auto const& params = filter_list->parameters();
        *keys = {};
        *values = {};

        if (!params.empty()) {
            *keys = adblock_str_array_new(params.size());
            *values = adblock_str_array_new(params.size());

            size_t i = 0;
            for (auto const [key, value]: params) {
                keys->ptr[i] = to_string_t(key);
                values->ptr[i] = to_string_t(value);

                ++i;
            }

            keys->length = params.size();
            values->length = params.size();
        }
    }
    catch (std::exception const& e) {
        report_error(error, __func__, ": Uncaught exception: ", e.what());
    }
    catch (...) {
        report_error(error, __func__, ": Unknown exception is caught.");
    }
}

void
adblock_reload(adblock_db_t* const db,
    adblock_error_t** error /* OUT */)
{
    try {
        if (db == nullptr) {
            report_error(error, __func__, ": Invalid parameter: db");
            return;
        }
        if (error != nullptr) {
            *error = nullptr;
        }

        db->reload();
    }
    catch (std::exception const& e) {
        report_error(error, __func__, ": Uncaught exception: ", e.what());
    }
    catch (...) {
        report_error(error, __func__, ": Unknown exception is caught.");
    }
}

void
adblock_statistics(adblock_db_t* const db,
    adblock_string_t* const json /* OUT */,
    adblock_error_t** error /* OUT */)
{
    try {
        if (db == nullptr) {
            report_error(error, __func__, ": Invalid parameter: db");
            return;
        }
        if (json == nullptr) {
            report_error(error, __func__, ": Invalid parameter: json");
            return;
        }
        if (error != nullptr) {
            *error = nullptr;
        }

        auto const& stats = db->statistics();

        auto s = json::stringify(stats);

        *json = adblock_string_new(s);
    }
    catch (std::exception const& e) {
        report_error(error, __func__, ": Uncaught exception: ", e.what());
    }
    catch (...) {
        report_error(error, __func__, ": Unknown exception is caught.");
    }
}

void
adblock_string_free(adblock_string_t* const s)
{
    if (s == nullptr) return;

    auto const it = std::remove_if(
        s_strings.begin(), s_strings.end(),
        [&](auto& v) { return v.data() == s->ptr; }
    );

    s_strings.erase(it, s_strings.end());
}

void
adblock_str_array_free(adblock_str_array_t* const array)
{
    if (array == nullptr) return;

    auto const it = std::remove_if(
        s_stringVectors.begin(), s_stringVectors.end(),
        [&](auto& v) { return v.data() == array->ptr; }
    );

    s_stringVectors.erase(it, s_stringVectors.end());
}

void
adblock_error_free(adblock_error_t* const e)
{
    if (e == nullptr) return;

    adblock_string_free(&e->message);

    auto const it = std::remove_if(
        s_errors.begin(), s_errors.end(),
        [&](auto& ptr) { return ptr.get() == e; }
    );

    s_errors.erase(it, s_errors.end());
}
