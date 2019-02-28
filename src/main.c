#include <gtk/gtk.h>
#include <stdio.h>
#include "model.h"

static void check_answer(GtkWidget *widget, gpointer data) {
    GPtrArray *radio_buttons = (GPtrArray*) data;

    for(int i = 0; i < radio_buttons->len; i++) {
        gpointer radio_button = g_ptr_array_index(radio_buttons, i);

        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_button))) {
            g_print("index is %d\n", i);
        }
    }
}

static GPtrArray* create_radios(GtkContainer *container, GPtrArray *answers) {
    GPtrArray *result = g_ptr_array_new();

    for (int i = 0; i < answers->len; i++) {
        GString *answer = (GString*) g_ptr_array_index(answers, i);
        GtkWidget *radio;

        if (i == 0) {
            radio = gtk_radio_button_new_with_label(NULL, answer->str);
        } else {
            radio = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(g_ptr_array_index(result, 0)), answer->str);
        }

        gtk_container_add(container, radio);
        g_ptr_array_add(result, radio);
    }

    return result;
}

static void activate (GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Window");
    gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    Question *question = (Question*) user_data;
    GtkWidget *label = gtk_label_new(question->text->str);
    gtk_container_add(GTK_CONTAINER(vbox), label);
    GPtrArray *radio_buttons = create_radios(GTK_CONTAINER(vbox), question->answers);
    GtkWidget *button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add (GTK_CONTAINER (vbox), button_box);
    GtkWidget *button = gtk_button_new_with_label ("Check");
    g_signal_connect (button, "clicked", G_CALLBACK (check_answer), radio_buttons);
    gtk_container_add (GTK_CONTAINER (button_box), button);
    gtk_widget_show_all (window);
}

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