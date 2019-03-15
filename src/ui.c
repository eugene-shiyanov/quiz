#include "ui.h"
#include "question.h"
#include "question_generator.h"

typedef struct callback_data_s {
    GtkWidget *vbox;
    QuestionGenerator *question_generator;
    int correct_answer_index;
} CallbackData;

static void clear_form(GtkWidget*);
static void render_question(GtkWidget*, QuestionGenerator*);
static void render_result(GtkWidget*, int, QuestionGenerator*);
static void start_callback(GtkWidget*, gpointer);
static void verify_callback(GtkWidget*, gpointer);
static gboolean check_answer(GSList*, int);
static GSList* extract_rbuttons(GtkWidget*);

void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Window");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    GtkWidget *button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(vbox), button_box, TRUE, TRUE, 5);
    GtkWidget *button = gtk_button_new_with_label("Start");

    {
        CallbackData *callback_data = calloc(1, sizeof(CallbackData));
        callback_data->vbox = vbox;
        //TODO free
        QuestionGenerator *question_generator = question_generator_new();
        callback_data->question_generator = question_generator;
        g_signal_connect(button, "clicked", G_CALLBACK(start_callback), callback_data);
    }

    gtk_container_add(GTK_CONTAINER(button_box), button);
    gtk_widget_show_all(window);
}

static void start_callback(GtkWidget *widget, gpointer data) {
    CallbackData* callback_data = (CallbackData*) data;
    render_question(callback_data->vbox, callback_data->question_generator);
    gtk_widget_show_all(callback_data->vbox);
    free(callback_data);
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

static inline void render_question(GtkWidget *vbox, QuestionGenerator *question_generator) {
    clear_form(vbox);
    Question *question = question_generator_next(question_generator);
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

    GtkWidget *button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(vbox), button_box, TRUE, TRUE, 5);
    GtkWidget *button = gtk_button_new_with_label("Verify");

    {
        CallbackData *callback_data = calloc(1, sizeof(CallbackData));
        callback_data->vbox = vbox;
        callback_data->correct_answer_index = question->correct_answer_index;
        callback_data->question_generator = question_generator;
        g_signal_connect(button, "clicked", G_CALLBACK(verify_callback), callback_data);
    }

    gtk_container_add(GTK_CONTAINER(button_box), button);
    question_free(question);
}

static void verify_callback(GtkWidget *widget, gpointer data) {
    CallbackData *callback_data = (CallbackData*) data;
    render_result(callback_data->vbox, callback_data->correct_answer_index, callback_data->question_generator);
    gtk_widget_show_all(callback_data->vbox);
    free(callback_data);
}

static inline void render_result(GtkWidget *vbox, int correct_answer_index, QuestionGenerator *question_generator) {
    GSList *radio_buttons = extract_rbuttons(vbox);
    gboolean success = check_answer(radio_buttons, correct_answer_index);
    gchar *text = NULL;

    if (success) {
        text = "Win!";
    } else {
        text = "Fail";
    }

    clear_form(vbox);
    GtkWidget *label = gtk_label_new(text);
    gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 5);
    GtkWidget *button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(vbox), button_box, TRUE, TRUE, 5);
    GtkWidget *button = gtk_button_new_with_label("Start");

    {
        CallbackData *callback_data = calloc(1, sizeof(CallbackData));
        callback_data->vbox = vbox;
        callback_data->question_generator = question_generator;
        g_signal_connect(button, "clicked", G_CALLBACK(start_callback), callback_data);
    }

    gtk_container_add(GTK_CONTAINER(button_box), button);
}

static inline gboolean check_answer(GSList *radio_button_list, int correct_answer_index) {
    return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(g_slist_nth_data(radio_button_list, correct_answer_index)));
}

static inline GSList* extract_rbuttons(GtkWidget *vbox) {
    GList *child = gtk_container_get_children(GTK_CONTAINER(vbox));
    GList *first_child = child;
    GSList *result = NULL;

    while (child != NULL) {
        GList *next_child = g_list_next(child);

        if (GTK_IS_RADIO_BUTTON(child->data)) {
            result = gtk_radio_button_get_group(GTK_RADIO_BUTTON(child->data));
            break;
        }

        child = next_child;
    }

    g_list_free(first_child);
    return result;
}
