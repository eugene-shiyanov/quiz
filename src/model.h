#pragma once

#include <gmodule.h>

typedef struct question_s {
    GString *text;
    GPtrArray *answers;
    int correct_answer_index;
} Question;

Question* question_new(gchar *text);
void question_free(Question *question);
void question_add_answer(Question *question, gchar *text);
void question_set_correct_answer_index(Question *question, int index);