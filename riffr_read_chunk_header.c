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
    int rc;

    do {
        if (!handle || !header) {
            break;
        }
        rc = riffr_read_data(handle, "DD", sizeof(*header), header);
        if (rc < 0) {
            err = rc;
        } else if (rc > 0) {
            err = 0;
        }
    } while (0);

    return err;
}
