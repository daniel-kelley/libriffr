/*
   riffr_open.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include <stdlib.h>
#include "riffr_internal.h"

struct riffr *riffr_open(const char *filename,
                         const char *mode,
                         struct riffr_chunk_type *form)
{
    struct riffr *handle = NULL;
    int err = -1;
    uint32_t form_id;

    do {

        if (!filename || !mode) {
            break;
        }

        handle = calloc(1, sizeof(*handle));
        if (!handle) {
            break;
        }

        err = riffr_open_internal(handle, filename, mode, ENDIAN_HOST);
        if (err) {
            break;
        }

        if (!riffr_valid(handle, RIFFR_RIFF_TAG)) {
            break;
        }

        err = riffr_read_data(handle,
                              "D",
                              sizeof(form_id),
                              &form_id);
        if (err <= 0) {
            break;
        }

        err = riffr_get_chunk_type(handle, form_id, &handle->u.riff.form);
        if (err) {
            break;
        }

        if (form) {
            *form = handle->u.riff.form;
        }

    } while (0);

    if (handle && err) {
        free(handle);
        handle = NULL;
    }

    return handle;
}
