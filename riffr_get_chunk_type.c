/*
   riffr_get_chunk_type.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include "riffr.h"
#include "riffr_internal.h"

#define CHAR_AT(c, bitpos) (((c) >> (bitpos)) & 0xff)

int riffr_get_chunk_type(struct riffr *handle,
                         uint32_t chunk_id,
                         struct riffr_chunk_type *type)
{
    int err = -1;

    (void)handle; /* Will need eventually for endianness. */
    if (type) {
        type->str[0] = CHAR_AT(chunk_id,  0);
        type->str[1] = CHAR_AT(chunk_id,  8);
        type->str[2] = CHAR_AT(chunk_id, 16);
        type->str[3] = CHAR_AT(chunk_id, 24);
        type->str[4] = 0;
        err = 0;
    }

    return err;
}
