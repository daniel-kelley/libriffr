/*
   riffr_read_chunk_header.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include "riffr.h"
#include "riffr_internal.h"

int riffr_read_chunk_header(struct riffr *handle,
                            struct riffr_chunk_header *header)

{
    int err = -1;

    do {
        if (!handle || !header) {
            break;
        }
        err = riffr_read_dword(handle, 2, &header->id);
    } while (0);

    return err;
}
