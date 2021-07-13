/*
   riffr_filename.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include "riffr.h"
#include "riffr_internal.h"

const char *riffr_filename(const struct riffr *handle)
{
    return handle->filename;
}
