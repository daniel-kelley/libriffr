/*
   riffr-info.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "riffr.h"

static void riffr_info(const char *file)
{
    struct riffr_chunk_header header;
    struct riffr_chunk_type type;
    struct riffr_chunk_type form;
    struct riffr *handle;
    uint32_t list_form;
    int err = -1;

    handle = riffr_open(file, "r", &form);
    if (handle) {
        printf("%s: %s\n", file, riffr_type_str(form));
        for (;;) {
            const char *str;
            err = riffr_read_chunk_header(handle, &header);
            if (err) {
                break;
            }
            err = riffr_get_chunk_type(handle, header.id, &type);
            if (err) {
                fprintf(stderr, "%s: Error getting chunk type\n", file);
                break;
            }
            str = riffr_type_str(type);
            printf("%s: %u\n", str, header.size);
            if (!strcmp(str, "LIST")) {
                /* LIST contains a type and is nested with more chunks. */
                err = riffr_read_dword(handle, 1, &list_form);
                if (!err) {
                    err = riffr_get_chunk_type(handle, list_form, &type);
                    if (err) {
                        fprintf(stderr, "%s: Error getting chunk type\n", file);
                        break;
                    }
                    str = riffr_type_str(type);
                    printf("LIST: %s\n", str);
                } else {
                    fprintf(stderr, "%s: Error handling LIST\n", file);
                    break;
                }
            } else {
                err = riffr_read_chunk_body(handle, header.size, NULL);
                if (err) {
                    fprintf(stderr, "%s: Error skipping chunk body\n", file);
                    break;
                }
            }
        }
    } else {
        fprintf(stderr, "%s: %s\n", file, strerror(errno));
    }
    riffr_close(handle);
}

int main(int argc, char *argv[])
{
    int i;

    for (i=1; i<argc; i++) {
        riffr_info(argv[i]);
    }

    return 0;
}
