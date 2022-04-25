/*
   riffr_get_chunk_type.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include "riffr.h"
#include "riffr_internal.h"

int riffr_get_chunk_type(struct riffr *handle,
                         uint32_t chunk_id,
                         struct riffr_chunk_type *type)
{
    int err = -1;

    if (type && handle) {
        err = (handle->type2str)(type->str, sizeof(type->str), chunk_id);
    }

    return err;
}
