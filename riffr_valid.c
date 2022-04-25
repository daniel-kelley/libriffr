/*
   riffr_valid.c

   Copyright (c) 2022 by Daniel Kelley

*/

#include "riffr_internal.h"

int riffr_valid(struct riffr *handle, uint32_t tag)
{
    int err = -1;

    do {
        err = riffr_read_chunk_header(handle, &handle->header);
        if (err) {
            break;
        }

        /* TODO: handle target endianness. */
        err = (handle->header.id != tag);

    } while (0);

    return err == 0;
}

