#pragma once

#include <gmodule.h>

typedef struct question_generator_s {
    GPtrArray *questions;
    guint32 next_question_index;
} QuestionGenerator;

QuestionGenerator* question_generator_new(void);
void question_generator_free(QuestionGenerator*);
Question* question_generator_next(void);