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

#define RIFFR_RIFF_TAG 0x46464952

/* handle */
struct riffr {
    struct riffr_chunk_type form;
    FILE *f;
};

#endif /* RIFFR_INTERNAL_H_ */
