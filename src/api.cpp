#include "api.h"

#include "adblock.hpp"
#include "context.hpp"
#include "filter_set.hpp"

#include <algorithm>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/make_unique.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/range/iterator_range.hpp>

class Context : public adblock::Context
{
public:
    using StringRange = adblock::StringRange;
public:
    Context(const struct adblock_context &cxt)
        : m_cxt { cxt },
          m_origin { m_cxt.origin, m_cxt.origin_len },
          m_siteKey { m_cxt.site_key, m_cxt.site_key + m_cxt.site_key_len }
    {}

    const adblock::Uri &origin() const override
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

    bool isSubDocument() const override
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
    const struct adblock_context &m_cxt;
    adblock::Uri m_origin;
    adblock::StringRange m_siteKey;
};

static std::vector<std::vector<char>> s_strings;
static std::vector<std::unique_ptr<adblock::AdBlock>> s_adBlocks;
static std::vector<std::vector<adblock_string_t>> s_stringVectors;

adblock_t
adblock_create()
{
    s_adBlocks.emplace_back(boost::make_unique<adblock::AdBlock>());

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
                       const char *path_in_utf8, const size_t len)
{
    auto* const adBlock = reinterpret_cast<adblock::AdBlock*>(adblock);
    assert(adBlock);

    namespace bfs = boost::filesystem;
    try {
        const adblock::AdBlock::Path path {
                                    path_in_utf8, path_in_utf8 + len };
        if (!bfs::exists(path)) {
            std::cerr << __func__ << ": File doesn't exists: "
                                  << path << "\n";
            return;
        }
        adBlock->addFilterSet(path);
    }
    catch (const std::exception &e) {
        std::cerr << __func__ << ": "
                  << "Uncaught exception: " <<  e.what() << "\n";
    }
    catch (...) {
        std::cerr << __func__ << ": Unknown exception is caught.\n";
    }
}

bool
adblock_should_block(adblock_t adblock,
                     const char *uri_in_utf8, const size_t len,
                     const struct adblock_context *cxt,
                     const char **filter_set, size_t *filter_set_len,
                     const char **reason, size_t *reason_len)
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

        const Context context { *cxt };

        const auto &rv = adBlock->shouldBlock(uri, context);
        const auto &shouldBlock = rv.first;
        const auto &reasonRule = rv.second;

        if (reasonRule) {
            const auto* const filterSetP = reasonRule->filterSet();
            if (filterSetP) {
                const auto &path = filterSetP->path();
                *filter_set = path.c_str();
                *filter_set_len = std::strlen(path.c_str());
            }

            *reason = reasonRule->line().begin();
            *reason_len = reasonRule->line().size();
        }

        return shouldBlock;
    }
    catch (const std::exception &e) {
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
                          const char *path, const size_t len)
{
    auto* const adBlock = reinterpret_cast<adblock::AdBlock*>(adblock);
    assert(adBlock);
    assert(path);

    const auto &filterSets = adBlock->filterSets();

    const auto &it = boost::find_if(filterSets,
        [&path, &len](const adblock::FilterSet &filterSet) {
            auto* const c_str = filterSet.path().c_str();
            return c_str[len] == '\0' &&
                   std::strncmp(c_str, path, len) == 0;
        });
    if (it == filterSets.end()) return false;

    adBlock->removeFilterSet(*it);

    return true;
}

void
adblock_element_hide_css(adblock_t adblock,
        const char *uri_in_utf8, const size_t uri_len,
        const char **css, size_t *css_len)
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
    catch (const std::exception &e) {
        std::cerr << __func__ << ": "
                  << "Uncaught exception: " <<  e.what() << "\n";
    }
    catch (...) {
        std::cerr << __func__ << ": Unknown exception is caught.\n";
    }
}

void
adblock_extended_element_hide_selectors(adblock_t adblock,
        const adblock_string_t *uri_in_utf8,
        adblock_string_t **out_selectors,
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
adblock_reload(adblock_t adblock)
{
    auto* const adBlock = reinterpret_cast<adblock::AdBlock*>(adblock);
    assert(adBlock);

    adBlock->reload();
}

void
adblock_statistics(adblock_t adblock, const char **json, size_t *json_len)
{
    auto* const adBlock = reinterpret_cast<adblock::AdBlock*>(adblock);
    assert(adBlock);

    const auto &stats = adBlock->statistics();

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
adblock_free(const char *ptr)
{
    try {
        const auto begin = s_strings.begin(), end = s_strings.end();
        const auto &it = std::remove_if(begin, end,
            [ptr](const std::vector<char> &str) {
                return str.data() == ptr;
            }
        );
        s_strings.erase(it, end);

        return it != end;
    }
    catch (const std::exception &e) {
        std::cerr << __func__ << ": "
                  << "Uncaught exception: " <<  e.what() << "\n";
    }
    catch (...) {
        std::cerr << __func__ << ": Unknown exception is caught.\n";
    }
    return false;
}

bool
adblock_free_selectors(adblock_string_t *ptr)
{
    try {
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
    catch (const std::exception &e) {
        std::cerr << __func__ << ": "
                  << "Uncaught exception: " <<  e.what() << "\n";
    }
    catch (...) {
        std::cerr << __func__ << ": Unknown exception is caught.\n";
    }
    return false;
}
