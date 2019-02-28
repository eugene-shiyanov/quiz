#include <stdlib.h>
#include "model.h"

Question* question_new(gchar *text) {
    Question *result = calloc(1, sizeof(Question));
    result->text = g_string_new(text);
    result->answers = g_ptr_array_new();
    return result;
}

static void answer_free(gpointer answer, gpointer user_data) {
    GString *text = (GString*) answer;
    g_string_free(text, TRUE);
}

void question_free(Question *question) {
    g_string_free(question->text, TRUE);
    g_ptr_array_foreach(question->answers, answer_free, NULL);
    g_ptr_array_free(question->answers, TRUE);
    free(question);
}

void question_add_answer(Question *question, gchar *text) {
    g_ptr_array_add(question->answers, g_string_new(text));
}

void question_set_correct_answer_index(Question *question, int index) {
    question->correct_answer_index = index;
}
