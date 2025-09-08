#pragma once

#include <regex.h>

#include "intset.h"

#define BSB_REF_SEARCH 1
#define BSB_REF_EXACT 2
#define BSB_REF_EXACT_SET 3
#define BSB_REF_RANGE 4
#define BSB_REF_RANGE_EXT 5

typedef struct bsb_ref {
    int type;
    unsigned int book;
    unsigned int chapter;
    unsigned int chapter_end;
    unsigned int verse;
    unsigned int verse_end;
    intset *verse_set;
    char *search_str;
    regex_t search;
} bsb_ref;

bsb_ref *
bsb_newref();

void
bsb_freeref(bsb_ref *ref);

int
bsb_parseref(bsb_ref *ref, const char *ref_str);
