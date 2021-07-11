/*
   riffr_read_chunk_body.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include <stdio.h>
#include "riffr.h"
#include "riffr_internal.h"

int riffr_read_chunk_body(struct riffr *handle,
                          size_t size,
                          void *buffer)
{
    int err = -1;

    if (handle) {
        if (buffer) {
            size_t num;

            num = fread(buffer, 1, size, handle->f);
            err = (num != size);
        } else {
            err = fseek(handle->f, size, SEEK_CUR);
        }
    }

    return err;
}
