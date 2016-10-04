#ifndef ADBLOCK_API_HPP
#define ADBLOCK_API_HPP

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DLL_API __attribute__((visibility ("default")))

typedef void * adblock_t;

struct adblock_context;

adblock_t DLL_API adblock_create();

bool DLL_API adblock_destroy(adblock_t);

void DLL_API
    adblock_add_filter_set(adblock_t,
            const char *path_in_utf8, const size_t len);

bool DLL_API
    adblock_remove_filter_set(adblock_t,
            const char *path_in_utf8, const size_t len);

bool DLL_API
    adblock_should_block(adblock_t,
            const char *uri_in_utf8, const size_t len,
            const struct adblock_context*,
            const char **filter_set, size_t *filter_set_len,
            const char **reason, size_t *reason_len);

void DLL_API
    adblock_element_hide_css(adblock_t,
            const char *uri_in_utf8, const size_t uri_len,
            const char **css, size_t *css_len);

void DLL_API
    adblock_reload(adblock_t);

void DLL_API
    adblock_statistics(adblock_t, const char **json, size_t *json_len);

bool DLL_API
    adblock_free(const char *);

/*
 * https://developer.mozilla.org/en-US/docs/Mozilla/Tech/XPCOM/Reference/Interface/nsIContentPolicy
 */
enum content_type {
    TYPE_OTHER = 1,
    TYPE_SCRIPT = 2,
    TYPE_IMAGE = 3,
    TYPE_STYLESHEET = 4,
    TYPE_OBJECT = 5,
    TYPE_DOCUMENT = 6,
    TYPE_SUBDOCUMENT = 7,
    TYPE_XMLHTTPREQUEST = 11,
    TYPE_OBJECT_SUBREQUEST = 12,
    TYPE_FONT = 14,
    TYPE_MEDIA = 15,
    TYPE_WEBSOCKET = 16,
};

typedef struct adblock_context
{
    const char *origin;
    size_t origin_len;

    const char *site_key;
    size_t site_key_len;

    enum content_type content_type;

    bool is_popup;
} adblock_context_t;

#ifdef __cplusplus
} // extern "C"
#endif

#endif // ADBLOCK_API_HPP
