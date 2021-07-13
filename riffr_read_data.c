/*
   riffr_read_data.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include <stdio.h>
#include "riffr.h"
#include "riffr_internal.h"

static int riffr_read_dword_internal(struct riffr *handle,
                                     size_t *length,
                                     void **buffer)
{
    int err = -1;
    uint32_t data;
    size_t datalen = sizeof(data);
    size_t num;
    uint32_t *out = *(uint32_t **)buffer;

    if (*length >= datalen) {
        num = fread(&data, 1, datalen, handle->f);
        err = (num != datalen);
        if (!err) {
            *length = *length - datalen;
            *out++ = handle->r32(data);
            *buffer = out;
        }
    }

    return err;
}

static int riffr_read_data_internal(struct riffr *handle,
                                    const char *format,
                                    size_t length,
                                    void *buffer)
{
    int err = -1;
    int c;

    while ((c = *format++)) {
        switch (c) {
        case 'D':
            err = riffr_read_dword_internal(handle, &length, &buffer);
            break;
        default:
            break;
        }
    }

    return err;
}

int riffr_read_data(struct riffr *handle,
                    const char *format,
                    size_t length,
                    void *buffer)
{
    int err = -1;

    if (handle && buffer && format) {
        err = riffr_read_data_internal(handle, format, length, buffer);
    }

    return err;
}
