/*
   riffr_close.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include <stdlib.h>
#include <string.h>
#include "riffr_internal.h"

int riffr_close(struct riffr *handle)
{
    int rc = 0;

    if (handle) {
        fclose(handle->f);
        memset(handle, 0, sizeof(*handle));
        free(handle);
    }

    return rc ? 1 : 0;
}
