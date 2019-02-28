#include "ui.h"
#include "model.h"

typedef struct check_data_s {
    GPtrArray *radio_buttons;
    int correct_answer_index;
} CheckData;

static void check_answer(GtkWidget *widget, gpointer data) {
    CheckData *check_data = (CheckData*) data;
    GPtrArray *radio_buttons = check_data->radio_buttons;
    int actual_answer_index;

    for(int i = 0; i < radio_buttons->len; i++) {
        gpointer radio_button = g_ptr_array_index(radio_buttons, i);

        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_button))) {
            actual_answer_index = i;
            break;
        }
    }

    if (check_data->correct_answer_index == actual_answer_index) {
        g_print("Win\n");
    } else {
        g_print("Fail\n");
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
            radio = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio), answer->str);
        }

        gtk_container_add(container, radio);
        g_ptr_array_add(result, radio);
    }

    return result;
}

void activate (GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Window");
    gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    Question *question = (Question*) user_data;
    GtkWidget *label = gtk_label_new(question->text->str);
    gtk_container_add(GTK_CONTAINER(vbox), label);
    //TODO free radio_buttons
    GPtrArray *radio_buttons = create_radios(GTK_CONTAINER(vbox), question->answers);
    GtkWidget *button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add (GTK_CONTAINER (vbox), button_box);
    GtkWidget *button = gtk_button_new_with_label ("Check");
    //TODO free check_data
    CheckData *check_data = malloc(sizeof(CheckData));
    check_data->radio_buttons = radio_buttons;
    check_data->correct_answer_index = question->correct_answer_index;
    g_signal_connect (button, "clicked", G_CALLBACK (check_answer), check_data);
    gtk_container_add (GTK_CONTAINER (button_box), button);
    gtk_widget_show_all (window);
}