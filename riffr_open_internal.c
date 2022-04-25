/*
   riffr_open_internal.c

   Copyright (c) 2022 by Daniel Kelley

*/

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <endian.h>
#include "riffr_internal.h"

/* Little endian converters */
static uint16_t riffr_le16toh(uint16_t data)
{
    return le16toh(data);
}

static uint32_t riffr_le32toh(uint32_t data)
{
    return le32toh(data);
}

#define CHAR_AT(c, bitpos) (((c) >> (bitpos)) & 0xff)


static int riffr_letype2str(char *str, size_t len, uint32_t chunk_id)
{
    int err = -1;

    if (len == (sizeof(chunk_id)+1)) {
        err = 0;
        str[0] = CHAR_AT(chunk_id,  0);
        str[1] = CHAR_AT(chunk_id,  8);
        str[2] = CHAR_AT(chunk_id, 16);
        str[3] = CHAR_AT(chunk_id, 24);
        str[4] = 0;
    }

    return err;
}

/* Big endian converters */
static uint16_t riffr_be16toh(uint16_t data)
{
    return be16toh(data);
}

static uint32_t riffr_be32toh(uint32_t data)
{
    return be32toh(data);
}

static int riffr_betype2str(char *str, size_t len, uint32_t chunk_id)
{
    int err = -1;

    if (len == (sizeof(chunk_id)+1)) {
        err = 0;
        str[0] = CHAR_AT(chunk_id, 24);
        str[1] = CHAR_AT(chunk_id, 16);
        str[2] = CHAR_AT(chunk_id,  8);
        str[3] = CHAR_AT(chunk_id,  0);
        str[4] = 0;
    }

    return err;
}

static void riffr_setup_endian(struct riffr *handle,
                               enum file_endianness endianness)
{
    if (endianness == ENDIAN_HOST) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
        endianness = ENDIAN_ALWAYS_LE;
#elif __BYTE_ORDER == __BIG_ENDIAN
        endianness = ENDIAN_ALWAYS_BE;
#else
#error "Needs endian support"
#endif
    }

    switch (endianness) {
    case ENDIAN_ALWAYS_BE:
        handle->r16 = riffr_be16toh;
        handle->r32 = riffr_be32toh;
        handle->type2str = riffr_betype2str;
        break;
    case ENDIAN_ALWAYS_LE:
        handle->r16 = riffr_le16toh;
        handle->r32 = riffr_le32toh;
        handle->type2str = riffr_letype2str;
        break;
    default:
        /* Shouldn't happen. */
        break;
    }
}

/* Attempt to open RIFF file 'filename' */
int riffr_open_internal(struct riffr *handle,
                        const char *filename,
                        const char *mode,
                        enum file_endianness endianness)
{
    riffr_setup_endian(handle, endianness);
    handle->f = fopen(filename, mode);
    handle->filename = filename;
        /* Little bit of a chicken and egg problem. Start with this for now. */

    return (handle->f == NULL) ? -1 : 0;
}
