#include <gtk/gtk.h>
#include "model.h"
#include "ui.h"

static Question* create_question() {
    Question *result = question_new("Fourth planet?");
    question_add_answer(result, "Jupiter");
    question_add_answer(result, "Saturn");
    question_add_answer(result, "Mars");
    question_add_answer(result, "Earth");
    question_set_correct_answer_index(result, 2);
    return result;
}

int main (int argc, char **argv) {
    GtkApplication *app = gtk_application_new ("org.quiz", G_APPLICATION_FLAGS_NONE);
    Question *question = create_question();
    g_signal_connect (app, "activate", G_CALLBACK (activate), question);
    int status = g_application_run (G_APPLICATION (app), argc, argv);
    question_free(question);
    g_object_unref (app);
    return status;
}