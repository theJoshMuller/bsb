#pragma once

typedef struct {
    int number;
    char *name;
    char *abbr;
} bsb_book;

typedef struct {
    int book;
    int chapter;
    int verse;
    char *text;
} bsb_verse;

extern bsb_verse bsb_verses[];

extern int bsb_verses_length;

extern bsb_book bsb_books[];

extern int bsb_books_length;
