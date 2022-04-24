/*
   riffr-smf.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stddef.h>
#include "riffr.h"

#if 0
static int is_smf_fmt(const char *form_str,
                       const char *chunk_str)
{
    return !strcmp(form_str, "WAVE") &&  !strcmp(chunk_str, "fmt ");
}

static void show_guid(const char *label, const uint8_t *guid, size_t len)
{
    printf("%s (%ld):", label, len);
    while(len--) {
        printf(" %02x", *guid++);
    }
    printf("\n");
}

/* 'len' may be larger, the same, or smaller than the size of 'fmt' */
static void show_wave_fmt_data(struct riffr_wave_fmt *fmt, size_t len)
{
    if (len >= offsetof(struct riffr_wave_fmt, nChannels)) {
        printf("wFormatTag: 0x%x\n", fmt->wFormatTag);
    }
    if (len >= offsetof(struct riffr_wave_fmt, nSamplesPerSec)) {
        printf("nChannels: %d\n", fmt->nChannels);
    }
    if (len >= offsetof(struct riffr_wave_fmt, nAvgBytesPerSec)) {
        printf("nSamplesPerSec: %d\n", fmt->nSamplesPerSec);
    }
    if (len >= offsetof(struct riffr_wave_fmt, nBlockAlign)) {
        printf("nAvgBytesPerSec: %d\n", fmt->nAvgBytesPerSec);
    }
    if (len >= offsetof(struct riffr_wave_fmt, wBitsPerSample)) {
        printf("nBlockAlign: %d\n", fmt->nBlockAlign);
    }
    if (len >= offsetof(struct riffr_wave_fmt, cbSize)) {
        printf("wBitsPerSample: %d\n", fmt->wBitsPerSample);
    }
    if (len >= offsetof(struct riffr_wave_fmt, wValidBitsPerSample)) {
        printf("cbSize: %d\n", fmt->cbSize);
    }
    if (len >= offsetof(struct riffr_wave_fmt, dwChannelMask)) {
        printf("wValidBitsPerSample: %d\n", fmt->wValidBitsPerSample);
    }
    if (len >= offsetof(struct riffr_wave_fmt, SubFormat)) {
        printf("dwChannelMask: 0x%08x\n", fmt->dwChannelMask);
    }
    if (len >= offsetof(struct riffr_wave_fmt, SubFormat)) {
        size_t remaining = sizeof(*fmt);
        remaining -= offsetof(struct riffr_wave_fmt, SubFormat);

        show_guid("SubFormat", fmt->SubFormat, remaining);
    }
}

/* 'len' may be larger, the same, or smaller than the size of 'fmt' */
static int show_wave_fmt_leftover(struct riffr *handle,
                                  const char *label,
                                  size_t len)
{
    int err = -1;
    int rc = 0;
    int i = 0;
    uint8_t data;
    int outlen;

    outlen = printf("%s (%ld):", label, len);
    while (len--) {
        rc = riffr_read_data(handle, "C", sizeof(data), &data);
        if (rc < 0) {
            err = rc;
            break;
        } else {
            err = 0;
        }
        if (i == 8) {
            printf("\n%*s", outlen, "");
            i = 0;
        }
        printf(" %02x", data);
        i++;
    }
    printf("\n");

    return err;
}

static int show_wave_fmt(struct riffr *handle,
                         struct riffr_chunk_header *header)
{
    int err = -1;
    struct riffr_wave_fmt fmt;
    int actual;

    memset(&fmt, 0, sizeof(fmt));

    /* Read as much of fmt as possible up to the size of the header. */
    actual = riffr_read_data(handle,
                             RIFFR_WAVE_FMT_FORMAT,
                             header->size,
                             &fmt);

    if (actual > 0) {
        ssize_t leftover;
        leftover = header->size - actual;
        show_wave_fmt_data(&fmt, actual);
        if (leftover > 0) {
            err = show_wave_fmt_leftover(handle, "More...", leftover);
        }
    } else {
        err = -1;
        fprintf(stderr,
                "%s: Error handling WAVE fmt\n",
                riffr_filename(handle));
    }

    return err;
}
#endif

static int show_chunk(struct riffr *handle,
                      struct riffr_chunk_header *header)
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

    err = riffr_read_chunk_body(handle, header->size, NULL);
    if (err) {
        fprintf(stderr, "%s: Error skipping chunk body\n", file);
        return -1;
    }

    return err;
}

static void riffr_smf(const char *file)
{
    struct riffr_chunk_header header;
    struct riffr *handle;
    int err = -1;

    handle = riffr_open_smf(file, "r");
    if (handle) {
        for (;;) {
            err = riffr_read_chunk_header(handle, &header);
            if (err) {
                /* Assume EOF */
                break;
            }
            err = show_chunk(handle, &header);
            if (err) {
                break;
            }
        }
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
