/*
   riffr_open_internal.c

   Copyright (c) 2022 by Daniel Kelley

*/

#include <stdio.h>
#include "riffr_internal.h"

/* Attempt to open RIFF file 'filename' */
int riffr_open_internal(struct riffr *handle,
                        const char *filename,
                        const char *mode)
{
    handle->f = fopen(filename, mode);
    handle->filename = filename;

    return (handle->f == NULL) ? -1 : 0;
}
