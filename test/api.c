#include "api.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROJECT_ROOT "/home/stream/tmp/wildcard/"

int main()
{
    const char project_root[] = PROJECT_ROOT;
    const char relative_path[] = "test/data/easylist.txt";

    const size_t path_len = strlen(project_root) + strlen(relative_path) + 2;
    char *path = malloc(path_len);
    snprintf(path, path_len, "%s/%s", project_root, relative_path);

    adblock_add_subscription(path, path_len);

    adblock_context_t context;
    memset(&context, 0, sizeof(context));

    context.origin = "http://www.adblock.org";
    context.origin_len = strlen(context.origin);

    const char uri1[] = "http://a.kickass.to/sc-b98b537.js";
    bool rv = adblock_should_block(uri1, strlen(uri1), &context);
    if (rv) {
        printf("TEST1: pass\n");
    }

    const char uri2[] = "http://barhbarhbarh.com";
    rv = adblock_should_block(uri2, strlen(uri2), &context);
    if (!rv) {
        printf("TEST2: pass\n");
    }

    free(path);
}
