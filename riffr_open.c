/*
   riffr_open.c

   Copyright (c) 2021 by Daniel Kelley

*/

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <endian.h>
#include "riffr_internal.h"

static uint16_t riffr_le16toh(uint16_t data)
{
    return le16toh(data);
}

static uint32_t riffr_le32toh(uint32_t data)
{
    return le32toh(data);
}

static int riffr_valid(struct riffr *handle)
{
    int err = -1;
    int rc = 0;
    struct riffr_chunk_header riff_header;
    uint32_t form_id;

    do {
        /* Little bit of a chicken and egg problem. Start with this for now. */
#if __BYTE_ORDER == __LITTLE_ENDIAN
        handle->r16 = riffr_le16toh;
        handle->r32 = riffr_le32toh;
#elif __BYTE_ORDER == __BIG_ENDIAN
        handle->r16 = riffr_be16toh;
        handle->r32 = riffr_be32toh;
#else
#error "Needs endian support"
#endif

        err = riffr_read_chunk_header(handle, &riff_header);
        if (err) {
            break;
        }

        /* TODO: handle target endianness. */
        err = (riff_header.id != RIFFR_RIFF_TAG);
        if (err) {
            break;
        }

        rc = riffr_read_data(handle,
                             "D",
                             sizeof(form_id),
                             &form_id);
        if (rc < 0) {
            err = rc;
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
    handle->filename = filename;
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
