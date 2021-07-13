/*
   riffr.h

   Copyright (c) 2021 by Daniel Kelley

   RIFF file reader.

*/

#ifndef   RIFFR_H_
#define   RIFFR_H_

#include <stdio.h>
#include <inttypes.h>

struct riffr;

struct riffr_chunk_header {
    uint32_t id;                /* Chunk ID */
    uint32_t size;              /* Size in bytes */
};

#define RIFFR_TYPE_LEN 5 /* 4 chars plus trailing NUL */

struct riffr_chunk_type {
    char str[RIFFR_TYPE_LEN];
};

/* WAVE fmt chunk */

#define RIFFR_WAVE_FMT_GUID_LEN 16

struct riffr_wave_fmt {
    uint16_t wFormatTag;          /*  0 Format code                       */
    uint16_t nChannels;           /*  2 Number of interleaved channels    */
    uint32_t nSamplesPerSec;      /*  4 Sampling rate (blocks per second) */
    uint32_t nAvgBytesPerSec;     /*  8 Data rate                         */
    uint16_t nBlockAlign;         /* 12 Data block size (bytes)           */
    uint16_t wBitsPerSample;      /* 14 Bits per sample                   */
    uint16_t cbSize;              /* 16 Size of the extension             */
    uint16_t wValidBitsPerSample; /* 18 Number of valid bits              */
    uint32_t dwChannelMask;       /* 20 Speaker position mask             */
                                  /* 24 GUID                              */
    uint8_t  SubFormat[RIFFR_WAVE_FMT_GUID_LEN];
};                                /* 40 (maximum) */

#define RIFFR_WAVE_FMT_FORMAT_16 "WWDDWW"
#define RIFFR_WAVE_FMT_FORMAT_18 RIFFR_WAVE_FMT_FORMAT_16 "W"
#define RIFFR_WAVE_FMT_FORMAT RIFFR_WAVE_FMT_FORMAT_18 "WDG"

extern struct riffr *riffr_open(const char *filename,
                                const char *mode,
                                struct riffr_chunk_type *form);

extern int riffr_read_chunk_header(struct riffr *handle,
                                   struct riffr_chunk_header *header);
extern int riffr_read_chunk_body(struct riffr *handle,
                                 size_t size,
                                 void *buffer);
extern int riffr_get_chunk_type(struct riffr *handle,
                                uint32_t chunk_id,
                                struct riffr_chunk_type *chunk_type);
extern int riffr_close(struct riffr *handle);
extern int riffr_read_data(struct riffr *handle,
                           const char *format,
                           size_t length,
                           void *buffer);
extern const char *riffr_filename(const struct riffr *handle);

#define riffr_type_str(form) ((form).str)

#endif /* RIFFR_H_ */
