/*
   riffr_open_smf.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include <stdlib.h>
#include "riffr_internal.h"

struct riffr *riffr_open_smf(const char *filename,
                             const char *mode,
                             struct riffr_smf_header *smf_header)
{
    struct riffr *handle = NULL;
    int err = -1;

    do {

        if (!filename || !mode) {
            break;
        }

        handle = calloc(1, sizeof(*handle));
        if (!handle) {
            break;
        }

        err = riffr_open_internal(handle, filename, mode, ENDIAN_ALWAYS_BE);
        if (err) {
            break;
        }

        if (!riffr_valid(handle, RIFFR_MTHD_TAG)) {
            err = -1;
            break;
        }

        err = riffr_read_data(handle,
                              "WWW",
                              sizeof(handle->u.smf),
                              &handle->u.smf);
        if (err <= 0) {
            break;
        }

        if (smf_header) {
            *smf_header = handle->u.smf;
        }

        err = 0;

    } while (0);

    if (handle && err) {
        free(handle);
        handle = NULL;
    }

    return handle;
}
