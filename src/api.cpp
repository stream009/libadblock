#include "api.h"

#include "core/adblock.hpp"
#include "core/context.hpp"
#include "core/filter_list.hpp"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/range/iterator_range.hpp>

class Context : public adblock::Context
{
public:
    using StringRange = adblock::StringRange;
public:
    Context(struct adblock_context const& cxt)
        : m_cxt { cxt },
          m_origin { m_cxt.origin, m_cxt.origin_len },
          m_siteKey { m_cxt.site_key, m_cxt.site_key + m_cxt.site_key_len }
    {}

    adblock::Uri const& origin() const override
    {
        return m_origin; // origin could be invalid.
    }

    bool fromScriptTag() const override
    {
        return m_cxt.content_type == TYPE_SCRIPT;
    }

    bool fromImageTag() const override
    {
        return m_cxt.content_type == TYPE_IMAGE;
    }

    bool fromAudioVideoTag() const override
    {
        return m_cxt.content_type == TYPE_MEDIA;
    }

    bool isExternalStyleSheet() const override
    {
        return m_cxt.content_type == TYPE_STYLESHEET;
    }

    bool fromObjectTag() const override
    {
        return m_cxt.content_type == TYPE_OBJECT;
    }

    bool fromXmlHttpRequest() const override
    {
        return m_cxt.content_type == TYPE_XMLHTTPREQUEST;
    }

    bool fromPlugins() const override
    {
        return m_cxt.content_type == TYPE_OBJECT_SUBREQUEST;
    }

    bool isSubdocument() const override
    {
        return m_cxt.content_type == TYPE_SUBDOCUMENT;
    }

    bool isPopUp() const override
    {
        return m_cxt.is_popup;
    }

    bool isFont() const override
    {
        return m_cxt.content_type == TYPE_FONT;
    }

    bool isWebSocket() const override
    {
        return m_cxt.content_type == TYPE_WEBSOCKET;
    }

    bool isPing() const override
    {
        return m_cxt.content_type == TYPE_PING;
    }

    bool isWebRtc() const override
    {
        return m_cxt.content_type == TYPE_WEBRTC;
    }

    adblock::StringRange siteKey() const override
    {
        return m_siteKey;
    }

private:
    struct adblock_context const& m_cxt;
    adblock::Uri m_origin;
    adblock::StringRange m_siteKey;
};

static std::vector<std::vector<char>> s_strings;
static std::vector<std::unique_ptr<adblock::AdBlock>> s_adBlocks;
static std::vector<std::vector<adblock_string_t>> s_stringVectors;

adblock_t
adblock_create()
{
    s_adBlocks.emplace_back(std::make_unique<adblock::AdBlock>());

    return reinterpret_cast<adblock_t>(s_adBlocks.back().get());
}

bool
adblock_destroy(adblock_t adblock)
{
    const auto it = std::find_if(s_adBlocks.begin(), s_adBlocks.end(),
        [&adblock](const std::unique_ptr<adblock::AdBlock> &adBlockPtr) {
            return reinterpret_cast<adblock_t>(adBlockPtr.get())
                                                            == adblock;
        });
    if (it == s_adBlocks.end()) return false;

    s_adBlocks.erase(it);

    return true;
}

void
adblock_add_filter_set(adblock_t adblock,
                       char const* path_in_utf8, size_t const len)
{
    auto* const adBlock = reinterpret_cast<adblock::AdBlock*>(adblock);
    assert(adBlock);

    namespace bfs = boost::filesystem;
    try {
        adblock::AdBlock::Path const path {
                                    path_in_utf8, path_in_utf8 + len };
        if (!bfs::exists(path)) {
            std::cerr << __func__ << ": File doesn't exists: "
                                  << path << "\n";
            return;
        }
        adBlock->addFilterList(path);
    }
    catch (std::exception const& e) {
        std::cerr << __func__ << ": "
                  << "Uncaught exception: " <<  e.what() << "\n";
    }
    catch (...) {
        std::cerr << __func__ << ": Unknown exception is caught.\n";
    }
}

bool
adblock_should_block(adblock_t adblock,
                     char const* uri_in_utf8, size_t const len,
                     struct adblock_context const* cxt,
                     char const** filter_set, size_t *filter_set_len,
                     char const** reason, size_t *reason_len)
{
    auto* const adBlock = reinterpret_cast<adblock::AdBlock*>(adblock);
    assert(adBlock);

    try {
        *filter_set = nullptr;
        *filter_set_len = 0;
        *reason = nullptr;
        *reason_len = 0;

        adblock::Uri const uri { uri_in_utf8, len };

        if (cxt == nullptr) {
            std::cerr << __func__ << ": Null context is given.\n";
            return false;
        }

        Context const context { *cxt };

        auto const& rv = adBlock->shouldBlock(uri, context);
        auto const& shouldBlock = rv.first;
        auto const& reasonRule = rv.second;

        if (reasonRule) {
            auto* const filterListP = reasonRule->filterList();
            if (filterListP) {
                auto const& path = filterListP->path();
                *filter_set = path.c_str();
                *filter_set_len = std::strlen(path.c_str());
            }

            *reason = reasonRule->line().begin();
            *reason_len = reasonRule->line().size();
        }

        return shouldBlock;
    }
    catch (std::exception const& e) {
        std::cerr << __func__ << ": "
                  << "Uncaught exception: " <<  e.what() << "\n";
    }
    catch (...) {
        std::cerr << __func__ << ": Unknown exception is caught.\n";
    }

    return false;
}

bool
adblock_remove_filter_set(adblock_t adblock,
                          char const* path, size_t const len)
{
    auto* const adBlock = reinterpret_cast<adblock::AdBlock*>(adblock);
    assert(adBlock);
    assert(path);

    auto const& filterLists = adBlock->filterLists();

    auto const it = boost::find_if(filterLists,
        [&path, &len](auto& filterList) {
            auto* const c_str = filterList.path().c_str();
            return c_str[len] == '\0' &&
                   std::strncmp(c_str, path, len) == 0;
        });
    if (it == filterLists.end()) return false;

    adBlock->removeFilterList(*it);

    return true;
}

void
adblock_element_hide_css(adblock_t adblock,
        char const* uri_in_utf8, size_t const uri_len,
        char const** css, size_t* css_len)
{
    *css = nullptr;
    *css_len = 0;

    auto* const adBlock = reinterpret_cast<adblock::AdBlock*>(adblock);
    assert(adBlock);

    try {
        adblock::Uri const uri { uri_in_utf8, uri_len };

        auto &&cssStr = adBlock->elementHideCss(uri);

        if (!cssStr.empty()) {
            s_strings.emplace_back(cssStr.begin(), cssStr.end());
            auto const& str = s_strings.back();
            *css = str.data();
            *css_len = str.size();
        }
    }
    catch (std::exception const& e) {
        std::cerr << __func__ << ": "
                  << "Uncaught exception: " <<  e.what() << "\n";
    }
    catch (...) {
        std::cerr << __func__ << ": Unknown exception is caught.\n";
    }
}

void
adblock_extended_element_hide_selectors(adblock_t adblock,
        adblock_string_t const* uri_in_utf8,
        adblock_string_t** out_selectors,
        size_t* out_selector_count)
{
    *out_selectors = nullptr;
    *out_selector_count = 0;

    auto* const adBlock = reinterpret_cast<adblock::AdBlock*>(adblock);
    assert(adBlock);

    try {
        adblock::Uri const uri { uri_in_utf8->ptr, uri_in_utf8->length };

        auto results = adBlock->extendedElementHideSelector(uri);

        if (!results.empty()) {
            std::vector<adblock_string_t> strings;
            strings.reserve(results.size());

            std::transform(results.begin(), results.end(),
                           std::back_inserter(strings),
                [](auto const& selector) -> adblock_string_t {
                    return { selector.begin(), selector.size() };
                }
            );

            s_stringVectors.push_back(std::move(strings));

            *out_selectors = s_stringVectors.back().data();
            *out_selector_count = s_stringVectors.back().size();
        }
    }
    catch (std::exception const& e) {
        std::cerr << __func__ << ": "
                  << "Uncaught exception: " <<  e.what() << "\n";
    }
    catch (...) {
        std::cerr << __func__ << ": Unknown exception is caught.\n";
    }
}

void
adblock_content_security_policy(adblock_t handle,
            adblock_string_t const* uri_in_utf8,
            adblock_string_t* policy /* OUT */)
{
    try {
        auto* const adBlock = reinterpret_cast<adblock::AdBlock*>(handle);
        if (!adBlock) throw "wrong handle";

        adblock::Uri const uri { uri_in_utf8->ptr, uri_in_utf8->length };

        auto const p = adBlock->contentSecurityPolicy(uri);

        policy->ptr = p.begin();
        policy->length = p.size();
    }
    catch (std::exception const& e) {
        std::cerr << __func__ << ": "
                  << "Uncaught exception: " <<  e.what() << "\n";
    }
    catch (char const* const msg) {
        std::cerr << __func__ << ": "
                  << "Uncaught exception: " <<  msg << "\n";
    }
    catch (...) {
        std::cerr << __func__ << ": Unknown exception is caught.\n";
    }
}

bool
adblock_filter_set_parameters(
        adblock_t handle,
        adblock_string_t const* const filepath_in_utf8,
        adblock_string_t** keys, /* OUT */
        size_t* keys_len, /* OUT */
        adblock_string_t** values /* OUT */,
        size_t* values_len /* OUT */)
{
    try {
        auto* const adBlock = reinterpret_cast<adblock::AdBlock*>(handle);
        if (!adBlock) throw "wrong handle";

        adblock::AdBlock::Path const filePath {
            filepath_in_utf8->ptr,
            filepath_in_utf8->ptr + filepath_in_utf8->length };

        auto const& filterList = adBlock->filterList(filePath);
        if (!filterList) throw "invalid path";

        auto const& params = filterList->parameters();

        std::vector<adblock_string_t> keysVector;
        keysVector.reserve(params.size());

        std::vector<adblock_string_t> valuesVector;
        valuesVector.reserve(params.size());

        for (auto const& [key, value]: params) {
            keysVector.push_back({ key.begin(), key.size() });
            valuesVector.push_back({ value.begin(), value.size() });
        }

        s_stringVectors.push_back(std::move(keysVector));
        auto& keysRef = s_stringVectors.back();

        *keys = keysRef.data();
        *keys_len = keysRef.size();

        s_stringVectors.push_back(std::move(valuesVector));
        auto& valuesRef = s_stringVectors.back();

        *values = valuesRef.data();
        *values_len = valuesRef.size();

        return true;
    }
    catch (std::exception const& e) {
        std::cerr << __func__ << ": "
                  << "Uncaught exception: " <<  e.what() << "\n";
        return false;
    }
    catch (char const* const msg) {
        std::cerr << __func__ << ": "
                  << "Uncaught exception: " <<  msg << "\n";
        return false;
    }
    catch (...) {
        std::cerr << __func__ << ": Unknown exception is caught.\n";
        return false;
    }
}

void
adblock_reload(adblock_t adblock)
{
    auto* const adBlock = reinterpret_cast<adblock::AdBlock*>(adblock);
    assert(adBlock);

    adBlock->reload();
}

void
adblock_statistics(adblock_t adblock, char const** json, size_t* json_len)
{
    auto* const adBlock = reinterpret_cast<adblock::AdBlock*>(adblock);
    assert(adBlock);

    auto const& stats = adBlock->statistics();

    std::ostringstream oss;
    boost::property_tree::write_json(oss, stats, false);

    auto &&string = oss.str();
    assert(!string.empty());

    s_strings.emplace_back(string.begin(), string.end());

    auto const& saved = s_strings.back();

    *json = saved.data();
    *json_len = saved.size();
}

bool
adblock_free(char const* ptr)
{
    try {
        auto const begin = s_strings.begin(), end = s_strings.end();
        auto const it = std::remove_if(begin, end,
            [ptr](auto& str) {
                return str.data() == ptr;
            }
        );
        s_strings.erase(it, end);

        return it != end;
    }
    catch (std::exception const& e) {
        std::cerr << __func__ << ": "
                  << "Uncaught exception: " <<  e.what() << "\n";
    }
    catch (...) {
        std::cerr << __func__ << ": Unknown exception is caught.\n";
    }
    return false;
}

static bool
removeStringVector(adblock_string_t* const ptr)
{
    try {
        if (ptr == nullptr) return false;

        auto const end = s_stringVectors.end();
        auto const it = std::remove_if(
            s_stringVectors.begin(), end,
            [&](auto& v) {
                return !v.empty() && &v.front() == ptr;
            }
        );
        s_stringVectors.erase(it, end);

        return it != end;
    }
    catch (std::exception const& e) {
        std::cerr << __func__ << ": "
                  << "Uncaught exception: " <<  e.what() << "\n";
    }
    catch (...) {
        std::cerr << __func__ << ": Unknown exception is caught.\n";
    }
    return false;
}

bool
adblock_free_selectors(adblock_string_t* const ptr)
{
    return removeStringVector(ptr);
}

bool
adblock_free_keys(adblock_string_t* const ptr)
{
    return removeStringVector(ptr);
}

bool
adblock_free_values(adblock_string_t* const ptr)
{
    return removeStringVector(ptr);
}
