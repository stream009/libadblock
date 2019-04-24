#ifndef ADBLOCK_DATABASE_HPP
#define ADBLOCK_DATABASE_HPP

#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#if defined _WIN32 || defined __CYGWIN__
    #define ADBLOCK_DLL_IMPORT __declspec(dllimport)
    #define ADBLOCK_DLL_EXPORT __declspec(dllexport)
    #define ADBLOCK_DLL_LOCAL
#else
    #define ADBLOCK_DLL_IMPORT __attribute__ ((visibility ("default")))
    #define ADBLOCK_DLL_EXPORT __attribute__ ((visibility ("default")))
    #define ADBLOCK_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#endif

namespace adblock {

enum class ADBLOCK_DLL_EXPORT request_type {
    other,
    script,
    image,
    style_sheet,
    font,
    media,
    object,
    popup,
    subdocument,
    ping,
    xml_http_request,
    object_subrequest,
    web_socket,
    web_rtc,
};

struct ADBLOCK_DLL_EXPORT parameter {
    std::string_view key;
    std::string_view value;
};

struct ADBLOCK_DLL_EXPORT block_result {
    bool block;
    std::filesystem::path const* filter_list;
    std::string_view filter;
};

class AdBlock;

class ADBLOCK_DLL_EXPORT database
{
public:
    database();
    ~database();

    // query

    // Query if you should block a request
    //
    // @param url request URL
    // @param request_type type of request
    // @param origin URL to the origin of request (ex. page address)
    // @param site_key (optional) site key of the page
    //
    // @return block_result.block true if you should block request
    //         block_result.filter filter line which blocking decision is based on.
    //                               empty, when no filter matches request URL.
    //         block_result.filter_list path to the filter list which filter
    //                                  is belong to. could be nullptr.
    // @throw url_parse_error
    block_result
        should_block_request(std::string_view url,
                             request_type,
                             std::string_view origin,
                             std::string_view site_key = {}) const;

    // Query element hiding CSS selectors for given URL
    //
    // @param url page URL
    // @param site_key (optional) site key of the page
    //
    // @return selectors. could be empty.
    //
    // @throw url_parse_error
    std::vector<std::string_view>
        element_hiding_selectors(std::string_view url,
                                 std::string_view site_key = {}) const;

    // Query extended element hiding CSS selectors for given URL
    //
    // @param url page URL
    // @param site_key (optional) site key of the page
    //
    // @return selectors. could be empty.
    //
    // @throw url_parse_error
    std::vector<std::string_view>
        extended_element_hiding_selectors(std::string_view url,
                                          std::string_view site_key = {}) const;

    // Query content security policy to inject in the HTTP response
    //
    // @param url request URL
    // @param site_key (optional) site key of the page
    //
    // @return content security policy. could be empty.
    //
    // @throw url_parse_error
    std::string_view
        content_security_policy(std::string_view url,
                                std::string_view site_key = {}) const;

    // Query parameters of the filter list
    //
    // @param path path to the filter list
    //
    // @return vector of key, value pair. could be empty.
    std::vector<parameter>
        filter_list_parameters(std::filesystem::path const&) const;

    // Get internal statistics
    //
    // @return internal statistics in JSON format
    std::string statistics() const;

    // modifier

    // Add filter list
    //
    // @param path path to the filter list
    void add_filter_list(std::filesystem::path const&);

    // Remove filter list
    //
    // @param path path to the filter list
    void remove_filter_list(std::filesystem::path const&);

    // Reload filter lists
    void reload();

private:
    std::unique_ptr<AdBlock> m_adblock;
};

class ADBLOCK_DLL_EXPORT url_parse_error : public std::runtime_error
{
public:
    url_parse_error(std::string_view url,
                    std::string const& message, int64_t location);
    ~url_parse_error();

    // accessor
    std::string const& url() const { return m_url; }
    int64_t location() const { return m_location; }

private:
    std::string m_url;
    int64_t m_location;
};

} // namespace adblock

#endif // ADBLOCK_DATABASE_HPP
