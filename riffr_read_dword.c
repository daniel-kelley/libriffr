/*
   riffr_read_dword.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include <stdio.h>
#include "riffr.h"
#include "riffr_internal.h"

int riffr_read_dword(struct riffr *handle,
                     size_t count,
                     uint32_t *buffer)
{
    int err = -1;
    size_t num;

    do {
        if (!handle || !buffer) {
            break;
        }
        num = fread(buffer, sizeof(*buffer), count, handle->f);
        err = (num != count);
    } while (0);

    return err;
}
