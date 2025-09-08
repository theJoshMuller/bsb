#pragma once

#include "bsb_config.h"
#include "bsb_ref.h"

typedef struct {
    int start;
    int end;
} bsb_range;

typedef struct {
    int current;
    int next_match;
    bsb_range matches[2];
} bsb_next_data;

int
bsb_next_verse(const bsb_ref *ref, const bsb_config *config, bsb_next_data *next);
