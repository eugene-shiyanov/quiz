#include "ui.h"
#include "question.h"
#include "question_generator.h"

static void clear_form(GtkWidget*);
static void render_question(GtkWidget*, Question*);

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

void activate (GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW(window), "Window");
    gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    GtkWidget *button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(vbox), button_box, TRUE, TRUE, 5);
    GtkWidget *button = gtk_button_new_with_label ("Start");
    //TODO free
    QuestionGenerator *question_generator = question_generator_new();
    g_signal_connect (button, "clicked", G_CALLBACK(check_answer), question_generator);
    gtk_container_add (GTK_CONTAINER(button_box), button);
    gtk_widget_show_all (window);
}

static void clear_form(GtkWidget *vbox) {
    GList *child = gtk_container_get_children(GTK_CONTAINER(vbox));
    GList *first_child = child;
    
    while (child != NULL) {
        GList *next_child = g_list_next(child);
        GtkWidget *widget = (GtkWidget*) child->data;
        gtk_widget_destroy(widget);
        child = next_child;
    }

    g_list_free(first_child);
}

static void render_question(GtkWidget *vbox, Question *question) {
    GtkWidget *label = gtk_label_new(question->text->str);
    gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 5);

    for (int i = 0; i < question->answers->len; i++) {
        GString *answer = (GString*) g_ptr_array_index(question->answers, i);
        GtkWidget *radio;

        if (i == 0) {
            radio = gtk_radio_button_new_with_label(NULL, answer->str);
        } else {
            radio = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio), answer->str);
        }

        gtk_box_pack_start(GTK_BOX(vbox), radio, TRUE, TRUE, 5);
    }

    GtkWidget *button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(vbox), button_box, TRUE, TRUE, 5);
    GtkWidget *button = gtk_button_new_with_label ("Verify");
    gtk_container_add (GTK_CONTAINER(button_box), button);
}