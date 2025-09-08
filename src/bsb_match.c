#include <assert.h>
#include <regex.h>
#include <stdbool.h>
#include <stdlib.h>

#include "bsb_data.h"
#include "bsb_match.h"
#include "intset.h"

static bool
bsb_verse_matches(const bsb_ref *ref, const bsb_verse *verse)
{
    switch (ref->type) {
        case BSB_REF_SEARCH:
            return (ref->book == 0 || ref->book == verse->book) &&
                (ref->chapter == 0 || verse->chapter == ref->chapter) &&
                regexec(&ref->search, verse->text, 0, NULL, 0) == 0;

        case BSB_REF_EXACT:
            return ref->book == verse->book &&
                (ref->chapter == 0 || ref->chapter == verse->chapter) &&
                (ref->verse == 0 || ref->verse == verse->verse);

        case BSB_REF_EXACT_SET:
            return ref->book == verse->book &&
                (ref->chapter == 0 || verse->chapter == ref->chapter) &&
                intset_contains(ref->verse_set, verse->verse);

        case BSB_REF_RANGE:
            return ref->book == verse->book &&
                ((ref->chapter_end == 0 && ref->chapter == verse->chapter) ||
                    (verse->chapter >= ref->chapter && verse->chapter <= ref->chapter_end)) &&
                (ref->verse == 0 || verse->verse >= ref->verse) &&
                (ref->verse_end == 0 || verse->verse <= ref->verse_end);

        case BSB_REF_RANGE_EXT:
            return ref->book == verse->book &&
                (
                    (verse->chapter == ref->chapter && verse->verse >= ref->verse && ref->chapter != ref->chapter_end) ||
                    (verse->chapter > ref->chapter && verse->chapter < ref->chapter_end) ||
                    (verse->chapter == ref->chapter_end && verse->verse <= ref->verse_end && ref->chapter != ref->chapter_end) ||
                    (ref->chapter == ref->chapter_end && verse->chapter == ref->chapter && verse->verse >= ref->verse && verse->verse <= ref->verse_end)
                );

        default:
            return false;
    }
}

#define BSB_DIRECTION_BEFORE -1
#define BSB_DIRECTION_AFTER 1

static int
bsb_chapter_bounds(int i, int direction, int maximum_steps)
{
    assert(direction == BSB_DIRECTION_BEFORE || direction == BSB_DIRECTION_AFTER);

    int steps = 0;
    for ( ; 0 <= i && i < bsb_verses_length; i += direction) {
        bool step_limit = (maximum_steps != -1 && steps >= maximum_steps) ||
            (direction == BSB_DIRECTION_BEFORE && i == 0) ||
            (direction == BSB_DIRECTION_AFTER && i + 1 == bsb_verses_length);
        if (step_limit) {
            break;
        }

        const bsb_verse *current = &bsb_verses[i], *next = &bsb_verses[i + direction];
        if (current->book != next->book || current->chapter != next->chapter) {
            break;
        }
        steps++;
    }
    return i;
}

static int
bsb_next_match(const bsb_ref *ref, int i)
{
    for ( ; i < bsb_verses_length; i++) {
        const bsb_verse *verse = &bsb_verses[i];
        if (bsb_verse_matches(ref, verse)) {
            return i;
        }
    }
    return -1;
}

static void
bsb_next_addrange(bsb_next_data *next, bsb_range range) {
    if (next->matches[0].start == -1 && next->matches[0].end == -1) {
        next->matches[0] = range;
    } else if (range.start < next->matches[0].end) {
        next->matches[0] = range;
    } else {
        next->matches[1] = range;
    }
}

int
bsb_next_verse(const bsb_ref *ref, const bsb_config *config, bsb_next_data *next)
{
    if (next->current >= bsb_verses_length) {
        return -1;
    }

    if (next->matches[0].start != -1 && next->matches[0].end != -1 && next->current >= next->matches[0].end) {
        next->matches[0] = next->matches[1];
        next->matches[1] = (bsb_range){-1, -1};
    }

    if ((next->next_match == -1 || next->next_match < next->current) && next->next_match < bsb_verses_length) {
        int next_match = bsb_next_match(ref, next->current);
        if (next_match >= 0) {
            next->next_match = next_match;
            bsb_range bounds = {
                .start = bsb_chapter_bounds(next_match, BSB_DIRECTION_BEFORE, config->context_chapter ? -1 : config->context_before),
                .end = bsb_chapter_bounds(next_match, BSB_DIRECTION_AFTER, config->context_chapter ? -1 : config->context_after) + 1,
            };
            bsb_next_addrange(next, bounds);
        } else {
            next_match = bsb_verses_length;
        }
    }

    if (next->matches[0].start == -1 && next->matches[0].end == -1) {
        return -1;
    }

    if (next->current < next->matches[0].start) {
        next->current = next->matches[0].start;
    }

    return next->current++;
}
