/*
   riffr_valid.c

   Copyright (c) 2022 by Daniel Kelley

*/

#define _DEFAULT_SOURCE
#include <endian.h>
#include "riffr_internal.h"

static uint16_t riffr_le16toh(uint16_t data)
{
    return le16toh(data);
}

static uint32_t riffr_le32toh(uint32_t data)
{
    return le32toh(data);
}

int riffr_valid(struct riffr *handle, uint32_t tag)
{
    int err = -1;
    struct riffr_chunk_header riff_header;

    do {
        /* Little bit of a chicken and egg problem. Start with this for now. */
#if __BYTE_ORDER == __LITTLE_ENDIAN
        handle->r16 = riffr_le16toh;
        handle->r32 = riffr_le32toh;
#elif __BYTE_ORDER == __BIG_ENDIAN
        handle->r16 = riffr_be16toh;
        handle->r32 = riffr_be32toh;
#else
#error "Needs endian support"
#endif

        err = riffr_read_chunk_header(handle, &riff_header);
        if (err) {
            break;
        }

        /* TODO: handle target endianness. */
        err = (riff_header.id != tag);

    } while (0);

    return err == 0;
}

