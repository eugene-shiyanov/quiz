#include <gtk/gtk.h>
#include "ui.h"
#include "question_generator.h"

static void shutdown(GApplication *app, gpointer user_data) {
    QuestionGenerator *question_generator = (QuestionGenerator*) user_data;
    question_generator_free(question_generator);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("org.quiz", G_APPLICATION_FLAGS_NONE);
    QuestionGenerator *question_generator = question_generator_new();
    g_signal_connect(app, "activate", G_CALLBACK(activate), question_generator);
    g_signal_connect(app, "shutdown", G_CALLBACK(shutdown), question_generator);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
