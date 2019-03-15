#include <stdlib.h>
#include "question_generator.h"

static void question_free_callback(gpointer, gpointer);
static void fill_questions(GPtrArray*);
static Question* question_clone(Question*);

QuestionGenerator* question_generator_new(void) {
    QuestionGenerator *result = calloc(1, sizeof(QuestionGenerator));
    result->next_question_index = 0;
    result->questions = g_ptr_array_new();
    fill_questions(result->questions);
    return result;
}

void question_generator_free(QuestionGenerator* question_generator) {
    g_ptr_array_foreach(question_generator->questions, question_free_callback, NULL);
    g_ptr_array_free(question_generator->questions, TRUE);
    free(question_generator);
}

Question* question_generator_next(QuestionGenerator *question_generator) {
    Question *src = (Question*) g_ptr_array_index(question_generator->questions, question_generator->next_question_index);
    return question_clone(src);
}

static void fill_questions(GPtrArray *questions) {
    Question *question = question_new("Fourth planet?");
    question_add_answer(question, "Jupiter");
    question_add_answer(question, "Saturn");
    question_add_answer(question, "Mars");
    question_add_answer(question, "Earth");
    question_set_correct_answer_index(question, 2);
    g_ptr_array_add(questions, question);

    question = question_new("The fastest mammal?");
    question_add_answer(question, "Horse");
    question_add_answer(question, "Deer");
    question_add_answer(question, "Wolf");
    question_add_answer(question, "Cheetah");
    question_add_answer(question, "Hare");
    question_set_correct_answer_index(question, 3);
    g_ptr_array_add(questions, question);
}

static void question_free_callback(gpointer question, gpointer user_data) {
    question_free((Question*) question);
}

static Question* question_clone(Question *source) {
    Question *dest = question_new(source->text->str);

    for (int i = 0; i < source->answers->len; i++) {
        question_add_answer(dest, ((GString*) g_ptr_array_index(source->answers, i))->str);
    }

    question_set_correct_answer_index(dest, src->correct_answer_index);
    return result;
}