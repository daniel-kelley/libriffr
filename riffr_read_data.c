/*
   riffr_read_data.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include <stdio.h>
#include "riffr.h"
#include "riffr_internal.h"

static int riffr_read_dword_internal(struct riffr *handle,
                                     size_t *length,
                                     void **buffer,
                                    size_t *actual)
{
    int err = -1;
    uint32_t data;
    size_t datalen = sizeof(data);
    size_t num;
    uint32_t *out = *(uint32_t **)buffer;

    if (*length >= datalen) {
        num = fread(&data, 1, datalen, handle->f);
        err = (num != datalen) ? -1 : 0;
        if (!err) {
            *length = *length - datalen;
            *out++ = handle->r32(data);
            *buffer = out;
            *actual += datalen;
        }
    } else {
        err = 0;
    }

    return err;
}

static int riffr_read_word_internal(struct riffr *handle,
                                    size_t *length,
                                    void **buffer,
                                    size_t *actual)
{
    int err = -1;
    uint16_t data;
    size_t datalen = sizeof(data);
    size_t num;
    uint16_t *out = *(uint16_t **)buffer;

    if (*length >= datalen) {
        num = fread(&data, 1, datalen, handle->f);
        err = (num != datalen) ? -1 : 0;
        if (!err) {
            *length = *length - datalen;
            *out++ = handle->r16(data);
            *buffer = out;
            *actual += datalen;
        }
    } else {
        err = 0;
    }

    return err;
}

static int riffr_read_char_internal(struct riffr *handle,
                                    size_t *length,
                                    void **buffer,
                                    size_t *actual)
{
    int err = -1;
    size_t num;
    uint8_t *out = *(uint8_t **)buffer;
    size_t datalen = sizeof(*out);

    if (*length >= datalen) {
        num = fread(out, 1, datalen, handle->f);
        err = (num != datalen) ? -1 : 0;
        if (!err) {
            *length = *length - datalen;
            *buffer = ++out;
            *actual += datalen;
        }
    } else {
        err = 0;
    }

    return err;
}

static int riffr_read_guid_internal(struct riffr *handle,
                                    size_t *length,
                                    void **buffer,
                                    size_t *actual)
{
    int err = -1;
    int i;

    for (i=0; i<RIFFR_WAVE_FMT_GUID_LEN; i++) {
        err = riffr_read_char_internal(handle, length, buffer, actual);
        if (err) {
            break;
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
    size_t actual = 0;

    while ((c = *format++)) {
        switch (c) {
        case 'D':
            err = riffr_read_dword_internal(handle, &length, &buffer, &actual);
            break;
        case 'W':
            err = riffr_read_word_internal(handle, &length, &buffer, &actual);
            break;
        case 'G':
            err = riffr_read_guid_internal(handle, &length, &buffer, &actual);
            break;
        case 'C':
            err = riffr_read_char_internal(handle, &length, &buffer, &actual);
            break;
        default:
            err = -1;
            break;
        }
        if (err) {
            break;
        }
    }

    return err ? err : (int)actual;
}

/* return: <0 error other bytes read. */
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
