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

extern struct riffr *riffr_open(const char *filename,
                                const char *mode,
                                struct riffr_chunk_type *form);

extern int riffr_read_chunk_header(struct riffr *handle,
                                   struct riffr_chunk_header *header);
extern int riffr_read_chunk_body(struct riffr *handle,
                                 size_t size,
                                 void *buffer);
extern int riffr_read_dword(struct riffr *handle,
                            size_t count,
                            uint32_t *buffer);
extern int riffr_get_chunk_type(struct riffr *handle,
                                uint32_t chunk_id,
                                struct riffr_chunk_type *chunk_type);
extern int riffr_close(struct riffr *handle);

#define riffr_type_str(form) ((form).str)

#endif /* RIFFR_H_ */
