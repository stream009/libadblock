#ifndef ADBLOCK_DATABASE_H
#define ADBLOCK_DATABASE_H

#include "dll.h"

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* type declaration */
struct adblock_db;
typedef struct adblock_db adblock_db_t;

struct adblock_string;
typedef struct adblock_string adblock_string_t;

struct adblock_str_array;
typedef struct adblock_str_array adblock_str_array_t;

struct adblock_error;
typedef struct adblock_error adblock_error_t;

enum request_type {
    ADBLOCK_REQUEST_OTHER = 0,
    ADBLOCK_REQUEST_SCRIPT = 1,
    ADBLOCK_REQUEST_IMAGE = 2,
    ADBLOCK_REQUEST_STYLE_SHEET = 3,
    ADBLOCK_REQUEST_FONT = 4,
    ADBLOCK_REQUEST_MEDIA = 5,
    ADBLOCK_REQUEST_OBJECT = 6,
    ADBLOCK_REQUEST_POPUP = 7,
    ADBLOCK_REQUEST_SUBDOCUMENT = 8,
    ADBLOCK_REQUEST_PING = 9,
    ADBLOCK_REQUEST_XML_HTTP_REQUEST = 10,
    ADBLOCK_REQUEST_OBJECT_SUBREQUEST = 11,
    ADBLOCK_REQUEST_WEB_SOCKET = 12,
    ADBLOCK_REQUEST_WEB_RTC = 13,
};

/* function declaration */

/*
 * Create new ADBlock database instance
 *
 * @return pointer to the instance
 */
ADBLOCK_DLL_EXPORT adblock_db_t *
    adblock_db_new();

/*
 * Destroy ADBlock database instance
 *
 * @param db [IN]  pointer to the database
 */
ADBLOCK_DLL_EXPORT void
    adblock_db_free(adblock_db_t *db);

/*
 * Add filter list to the database
 *
 * @param db   [IN]  pointer to the database
 * @param path [IN]  path to the filter list file
 * @param err  [OUT] pointer to an error info.
 *                   report error to the console if err is nullptr.
 */
ADBLOCK_DLL_EXPORT void
    adblock_add_filter_list(adblock_db_t *db,
            const adblock_string_t *path,
            adblock_error_t **err /* OUT */);

/*
 * Remove filter list from the database
 *
 * @param db   [IN]  pointer to the database
 * @param path [IN]  path to the filter list file
 * @param err  [OUT] pointer to an error info.
 *                   report error to the console if err is nullptr.
 */
ADBLOCK_DLL_EXPORT void
    adblock_remove_filter_list(adblock_db_t *db,
            const adblock_string_t *path,
            adblock_error_t **err /* OUT */);

/*
 * Query if you should block given request.
 *
 * @param db          [IN]  pointer to the database
 * @param url         [IN]  URL of the request
 * @param type        [IN]  type of request
 * @param origin      [IN]  origin URL of the request
 * @param site_key    [IN]  (optional) site key of the request
 * @param filter_list [OUT] path to the matched filter list file
 * @param reason      [OUT] matched filter rule
 * @param err         [OUT] pointer to an error info.
 *                          report error to the console if err is nullptr.
 *
 * @return true if you should block given request
 */
ADBLOCK_DLL_EXPORT bool
    adblock_should_block(adblock_db_t *db,
            const adblock_string_t *url,
            enum request_type type,
            const adblock_string_t *origin,
            const adblock_string_t *site_key,
            adblock_string_t *filter_list /* OUT */,
            adblock_string_t *reason /* OUT */,
            adblock_error_t **err /* OUT */);

/*
 * Query element hiding CSS selectors for given request
 *
 * @param db        [IN]  pointer to the database
 * @param url       [IN]  URL of the request
 * @param site_key  [IN]  (optional) site key of the request
 * @param selectors [OUT] array of CSS selectors
 * @param err       [OUT] pointer to an error info.
 *                        report error to the console if err is nullptr.
 */
ADBLOCK_DLL_EXPORT void
    adblock_element_hiding_selectors(adblock_db_t *db,
            const adblock_string_t *url,
            const adblock_string_t *site_key,
            adblock_str_array_t *selectors /* OUT */,
            adblock_error_t **err /* OUT */);

/*
 * Query extended element hiding CSS selectors for given request
 *
 * @param db        [IN]  pointer to the database
 * @param url       [IN]  URL of the request
 * @param site_key  [IN]  (optional) site key of the request
 * @param selectors [OUT] array of CSS selectors
 * @param err       [OUT] pointer to an error info.
 *                        report error to the console if err is nullptr.
 */
ADBLOCK_DLL_EXPORT void
    adblock_extended_element_hiding_selectors(adblock_db_t *db,
            const adblock_string_t *url,
            const adblock_string_t *site_key,
            adblock_str_array_t *selectors /* OUT */,
            adblock_error_t **err /* OUT */);

/*
 * Query content security policy to inject in the HTTP response
 *
 * @param db       [IN]  pointer to the database
 * @param url      [IN]  URL of the request
 * @param site_key [IN]  (optional) site key of the request
 * @param policy   [OUT] content security policy
 * @param err      [OUT] pointer to an error info.
 *                       report error to the console if err is nullptr.
 */
ADBLOCK_DLL_EXPORT void
    adblock_content_security_policy(adblock_db_t *db,
            const adblock_string_t *url,
            const adblock_string_t *site_key,
            adblock_string_t *policy /* OUT */,
            adblock_error_t **err /* OUT */);

/*
 * Query snippet for the given request
 *
 * @param db       [IN]  pointer to the database
 * @param url      [IN]  URL of the request
 * @param site_key [IN]  (optional) site key of the request
 * @param snippets [OUT] array of snippets
 * @param err      [OUT] pointer to an error info.
 *                       report error to the console if err is nullptr.
 */
ADBLOCK_DLL_EXPORT void
    adblock_snippets(adblock_db_t *db,
            const adblock_string_t *url,
            const adblock_string_t *site_key,
            adblock_str_array_t *snippets /* OUT */,
            adblock_error_t **err /* OUT */);

/*
 * Query parameters of the filter list
 *
 * @param db     [IN]  pointer to the database
 * @param path   [IN]  path to the filter list file
 * @param keys   [OUT] array of parameter keys
 * @param values [OUT] array of parameter values
 * @param err    [OUT] pointer to an error info.
 *                     report error to the console if err is nullptr.
 */
ADBLOCK_DLL_EXPORT void
    adblock_filter_list_parameters(adblock_db_t *db,
            const adblock_string_t *path,
            adblock_str_array_t *keys /* OUT */,
            adblock_str_array_t *values /* OUT */,
            adblock_error_t **err /* OUT */);

/*
 * Reload filter lists
 *
 * @param db  [IN]  pointer to the database
 * @param err [OUT] pointer to an error info.
 *                  report error to the console if err is nullptr.
 */
ADBLOCK_DLL_EXPORT void
    adblock_reload(adblock_db_t *db,
            adblock_error_t **err /* OUT */);

/*
 * Get internal statistics
 *
 * @param db   [IN]  pointer to the database
 * @param json [OUT] statistics in JSON format
 * @param err  [OUT] pointer to an error info.
 *                   report error to the console if err is nullptr.
 */
ADBLOCK_DLL_EXPORT void
    adblock_statistics(adblock_db_t *db,
            adblock_string_t* json /* OUT */,
            adblock_error_t **err /* OUT */);

struct adblock_string
{
    const char *ptr;
    size_t length;
};

/*
 * Destroy string
 *
 * @param s [IN]  pointer to the string
 */
ADBLOCK_DLL_EXPORT void adblock_string_free(adblock_string_t *s);

struct adblock_str_array
{
    adblock_string_t *ptr;
    size_t length;
};

/*
 * Destroy string array
 *
 * @param arr [IN]  pointer to the string array
 */
ADBLOCK_DLL_EXPORT void adblock_str_array_free(adblock_str_array_t *arr);

struct adblock_error
{
    adblock_string_t message;
};

/*
 * Destroy error
 *
 * @param err [IN]  pointer to the error
 */
ADBLOCK_DLL_EXPORT void adblock_error_free(adblock_error_t *);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // ADBLOCK_DATABASE_H
