/*
   riffr_open.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "riffr_internal.h"

/*  */
static int riffr_valid(struct riffr *handle)
{
    int err = -1;
    struct riffr_chunk_header riff_header;
    uint32_t form_id;

    do {
        err = riffr_read_chunk_header(handle, &riff_header);
        if (err) {
            break;
        }

        /* TODO: handle host/target endianness. */
        err = (riff_header.id != RIFFR_RIFF_TAG);
        if (err) {
            break;
        }

        err = riffr_read_dword(handle,
                               1,
                               &form_id);
        if (err) {
            break;
        }

        err = riffr_get_chunk_type(handle, form_id, &handle->form);

    } while (0);

    return err;
}

/* Attempt to open RIFF file 'filename' */
static int riffr_open_internal(struct riffr *handle,
                               const char *filename,
                               const char *mode)
{
    int err = -1;

    handle->f = fopen(filename, mode);
    if (handle->f) {
        err = riffr_valid(handle);
    } else {
        fprintf(stderr, "%s: %s\n",filename,strerror(errno));
    }

    return err;
}

struct riffr *riffr_open(const char *filename,
                         const char *mode,
                         struct riffr_chunk_type *form)
{
    struct riffr *handle = NULL;

    if (filename && mode) {
        handle = calloc(1, sizeof(*handle));

        if (handle) {
            if (riffr_open_internal(handle, filename, mode)) {
                free(handle);
                handle = NULL;
            } else if (form) {
                *form = handle->form;
            }
        }
    }

    return handle;
}
