/*
   riffr-smf.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stddef.h>
#include "riffr.h"

enum dump_state {
    TIME,
    TIMECONT,
    OP,
    META_OP,
    META_LEN,
    DATA,
    SYSEX,
};

static enum dump_state time_check(unsigned int c,
                                  unsigned long long t)
{
    enum dump_state state;

    if ((c & 0x80) == 0) {
        printf("%10llu:",t);
        state = OP;
    } else {
        state = TIMECONT;
    }

    return state;
}

static void dump_chunk(const unsigned char *chunk, size_t chunk_size)
{
    enum dump_state state = TIME;
    unsigned int c;
    unsigned int cx;
    unsigned long long t = 0;
    int len = 0;
    size_t count = 0;

    while (count < chunk_size) {
        c = *chunk++;
        cx = c & 0xf0;
        count++;
        switch (state) {
        case TIME:
            t = (c & 0x7f);
            state = time_check(c, t);
            break;

        case TIMECONT:
            t <<= 7;
            t |= (c&0x7f);
            state = time_check(c, t);
            break;

        case OP:
            printf(" %02x", c);
            if (c == 0xff) {
                state = META_OP;
            } else if (cx == 0xc0 || cx == 0xd0) {
                state = DATA;
                len = 1;
            } else if (cx == 0x80 || cx == 0x90 || cx == 0xa0 || cx == 0xe0) {
                state = DATA;
                len = 2;
            } else if (cx == 0xf0) {
                state = SYSEX;
            } else {
                assert(0);
            }
            break;
        case META_OP:
            printf(" %02x", c);
            state = META_LEN;
            break;
        case META_LEN:
            printf(" %02x", c);
            len = c;
            state = (len == 0) ? TIME : DATA;
            break;
        case DATA:
            printf(" %02x", c);
            if (--len == 0) {
                state = TIME;
                printf("\n");
            }
            break;
        case SYSEX:
            printf(" %02x", c);
            if ((c & 0x80) != 0) {
                state = TIME;
                printf("\n");
            }
            break;
        default:
            assert(0);
        }
    }
    printf("\n");
}

static int show_chunk(struct riffr *handle,
                      struct riffr_chunk_header *header,
                      unsigned char **body,
                      size_t *body_size)
{
    int err = -1;
    const char *str;
    struct riffr_chunk_type type;
    const char *file = riffr_filename(handle);
    err = riffr_get_chunk_type(handle, header->id, &type);
    if (err) {
        fprintf(stderr, "%s: Error getting chunk type\n", file);
        return err;
    }
    str = riffr_type_str(type);
    printf("%s: %u\n", str, header->size);

    if (*body_size < header->size) {
        *body_size = header->size;
        *body = realloc(*body, *body_size);
        assert(*body);
    }

    err = riffr_read_chunk_body(handle, header->size, *body);
    if (err) {
        fprintf(stderr, "%s: Error skipping chunk body\n", file);
        return -1;
    }

    dump_chunk(*body, header->size);

    return err;
}

static void riffr_smf_dump(struct riffr *handle)
{
    struct riffr_chunk_header header;
    unsigned char *body = NULL;
    size_t body_size = 0;
    int err = -1;

    for (;;) {
        err = riffr_read_chunk_header(handle, &header);
        if (err) {
            /* Assume EOF */
            break;
        }
        err = show_chunk(handle, &header, &body, &body_size);
        if (err) {
            break;
        }
    }

    free(body);
}

static void riffr_smf(const char *file)
{
    struct riffr *handle;

    handle = riffr_open_smf(file, "r");
    if (handle) {
        riffr_smf_dump(handle);
    } else {
        fprintf(stderr, "%s: %s\n", file, strerror(errno));
    }
    riffr_close(handle);
}

int main(int argc, char *argv[])
{
    int i;

    for (i=1; i<argc; i++) {
        riffr_smf(argv[i]);
    }

    return 0;
}
