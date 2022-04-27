/*
   riffr_internal.h

   Copyright (c) 2021 by Daniel Kelley

   See http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html

*/

#ifndef   RIFFR_INTERNAL_H_
#define   RIFFR_INTERNAL_H_

#include <stdio.h>
#include <inttypes.h>
#include "riffr.h"

#define RIFFR_RIFF_TAG 0x46464952  /* RIFF (LE) */
#define RIFFR_MTHD_TAG 0x4D546864  /* MThd (BE) */

/* handle */
struct riffr {
    struct riffr_chunk_header header;
    union {
        struct {
            struct riffr_chunk_type form;
        } riff;
        struct riffr_smf_header smf;
    } u;
    const char *filename;
    FILE *f;
    uint16_t (*r16)(uint16_t);
    uint32_t (*r32)(uint32_t);
    int (*type2str)(char *s, size_t len, uint32_t id);
};

enum file_endianness {
    ENDIAN_ALWAYS_BE,
    ENDIAN_ALWAYS_LE,
    ENDIAN_HOST
};

extern int riffr_open_internal(struct riffr *handle,
                               const char *filename,
                               const char *mode,
                               enum file_endianness endianness);

extern int riffr_valid(struct riffr *handle, uint32_t tag);

#endif /* RIFFR_INTERNAL_H_ */
