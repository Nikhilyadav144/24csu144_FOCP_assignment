#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char question[300];
    char options[4][100];
    char correct;
    int score;
    int timeout;
} Question;

typedef struct {
    GtkWidget *window;
    GtkWidget *question_label;
    GtkWidget *option_buttons[4];
    GtkWidget *score_label;
    GtkWidget *next_button;
    GtkWidget *timer_label;
    GtkWidget *lifeline_buttons[2];
    GtkWidget *progress_label;  // Added to show question progress
    Question *questions;
    int current_question;
    int total_questions;
    int total_score;
    int timer_id;
    int seconds_left;
    int lifelines_available[2];
} QuizApp;

// Function declarations
static void activate(GtkApplication *app, gpointer user_data);
static void check_answer(GtkButton *button, gpointer data);
static void next_question(GtkButton *button, gpointer data);
static int load_questions(Question **questions);
static gboolean update_timer(gpointer data);
static void use_lifeline(GtkButton *button, gpointer data);
static void reset_question_state(QuizApp *quiz_app);
static void apply_custom_styles(QuizApp *quiz_app);

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;
    
    srand(time(NULL));

    app = gtk_application_new("org.quiz.basic", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

static void apply_custom_styles(QuizApp *quiz_app) {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "button { padding: 10px; margin: 5px; border-radius: 5px; }"
        "button:hover { background: #4a90e2; color: white; }"
        "label { padding: 5px; }"
        ".question-label { font-size: 16px; font-weight: bold; padding: 15px; }"
        ".timer-label { font-size: 14px; color: #e74c3c; font-weight: bold; }"
        ".score-label { font-size: 14px; color: #27ae60; font-weight: bold; }"
        ".progress-label { font-size: 12px; color: #7f8c8d; }"
        ".option-button { background: #ecf0f1; }"
        ".lifeline-button { background: #3498db; color: white; }"
        ".next-button { background: #2ecc71; color: white; font-weight: bold; }",
        -1, NULL);

    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );

    GtkStyleContext *context;
    
    context = gtk_widget_get_style_context(quiz_app->question_label);
    gtk_style_context_add_class(context, "question-label");
    
    context = gtk_widget_get_style_context(quiz_app->timer_label);
    gtk_style_context_add_class(context, "timer-label");
    
    context = gtk_widget_get_style_context(quiz_app->score_label);
    gtk_style_context_add_class(context, "score-label");
    
    context = gtk_widget_get_style_context(quiz_app->progress_label);
    gtk_style_context_add_class(context, "progress-label");
    
    context = gtk_widget_get_style_context(quiz_app->next_button);
    gtk_style_context_add_class(context, "next-button");

    for (int i = 0; i < 4; i++) {
        context = gtk_widget_get_style_context(quiz_app->option_buttons[i]);
        gtk_style_context_add_class(context, "option-button");
    }

    for (int i = 0; i < 2; i++) {
        context = gtk_widget_get_style_context(quiz_app->lifeline_buttons[i]);
        gtk_style_context_add_class(context, "lifeline-button");
    }

    g_object_unref(provider);
}

static void activate(GtkApplication *app, gpointer user_data) {
    QuizApp *quiz_app = g_new(QuizApp, 1);
    quiz_app->total_score = 0;
    quiz_app->current_question = 0;
    quiz_app->timer_id = 0;
    quiz_app->lifelines_available[0] = 1;
    quiz_app->lifelines_available[1] = 1;
    
    quiz_app->window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(quiz_app->window), "Knowledge Quest");
    gtk_window_set_default_size(GTK_WINDOW(quiz_app->window), 600, 450);
    gtk_container_set_border_width(GTK_CONTAINER(quiz_app->window), 20);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_container_add(GTK_CONTAINER(quiz_app->window), main_box);

    GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_container_add(GTK_CONTAINER(main_box), header_box);

    quiz_app->timer_label = gtk_label_new("Time: --");
    gtk_box_pack_start(GTK_BOX(header_box), quiz_app->timer_label, TRUE, TRUE, 0);

    quiz_app->progress_label = gtk_label_new("");
    gtk_box_pack_end(GTK_BOX(header_box), quiz_app->progress_label, TRUE, TRUE, 0);

    quiz_app->question_label = gtk_label_new("");
    gtk_label_set_line_wrap(GTK_LABEL(quiz_app->question_label), TRUE);
    gtk_label_set_justify(GTK_LABEL(quiz_app->question_label), GTK_JUSTIFY_CENTER);
    gtk_widget_set_margin_top(quiz_app->question_label, 10);
    gtk_widget_set_margin_bottom(quiz_app->question_label, 10);
    gtk_container_add(GTK_CONTAINER(main_box), quiz_app->question_label);

    GtkWidget *lifeline_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_margin_bottom(lifeline_box, 10);
    gtk_container_add(GTK_CONTAINER(main_box), lifeline_box);

    GtkWidget *centered_lifeline_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(lifeline_box), centered_lifeline_box, TRUE, FALSE, 0);

    quiz_app->lifeline_buttons[0] = gtk_button_new_with_label("50:50");
    quiz_app->lifeline_buttons[1] = gtk_button_new_with_label("Skip");
    for (int i = 0; i < 2; i++) {
        gtk_container_add(GTK_CONTAINER(centered_lifeline_box), quiz_app->lifeline_buttons[i]);
        g_signal_connect(quiz_app->lifeline_buttons[i], "clicked", G_CALLBACK(use_lifeline), quiz_app);
    }

    GtkWidget *options_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_add(GTK_CONTAINER(main_box), options_box);

    char option_labels[4] = {'A', 'B', 'C', 'D'};
    for (int i = 0; i < 4; i++) {
        GtkWidget *option_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        char prefix[5];
        snprintf(prefix, sizeof(prefix), "%c) ", option_labels[i]);
        GtkWidget *prefix_label = gtk_label_new(prefix);
        gtk_container_add(GTK_CONTAINER(option_box), prefix_label);
        
        quiz_app->option_buttons[i] = gtk_button_new_with_label("");
        gtk_container_add(GTK_CONTAINER(option_box), quiz_app->option_buttons[i]);
        gtk_widget_set_hexpand(quiz_app->option_buttons[i], TRUE);
        g_signal_connect(quiz_app->option_buttons[i], "clicked", G_CALLBACK(check_answer), quiz_app);
        
        gtk_container_add(GTK_CONTAINER(options_box), option_box);
    }

    char initial_score[32];
    snprintf(initial_score, sizeof(initial_score), "Score: %d", quiz_app->total_score);
    quiz_app->score_label = gtk_label_new(initial_score);
    gtk_widget_set_margin_top(quiz_app->score_label, 10);
    gtk_container_add(GTK_CONTAINER(main_box), quiz_app->score_label);

    quiz_app->next_button = gtk_button_new_with_label("Next Question");
    g_signal_connect(quiz_app->next_button, "clicked", G_CALLBACK(next_question), quiz_app);
    gtk_container_add(GTK_CONTAINER(main_box), quiz_app->next_button);
    
    quiz_app->total_questions = load_questions(&quiz_app->questions);
    
    apply_custom_styles(quiz_app);
    gtk_widget_show_all(quiz_app->window);

    next_question(NULL, quiz_app);  // Start the first question
}

static int load_questions(Question **questions) {
    FILE *file = fopen("questions.txt", "r");
    if (!file) {
        perror("Error opening file");
        return 0;
    }

    char line[1024];
    int question_count = 0;
    *questions = malloc(sizeof(Question) * 10);  // Allocate memory for 10 questions initially

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "Question", 8) == 0) {
            sscanf(line, "Question %*d: %[^\n]", (*questions)[question_count].question);

            for (int i = 0; i < 4; i++) {
                fgets(line, sizeof(line), file);
                sscanf(line, "%*c) %[^\n]", (*questions)[question_count].options[i]);
            }

            fgets(line, sizeof(line), file);
            sscanf(line, "Answer: %c", &(*questions)[question_count].correct);

            fgets(line, sizeof(line), file);
            sscanf(line, "Score: %d", &(*questions)[question_count].score);

            fgets(line, sizeof(line), file);
            sscanf(line, "Timeout: %d", &(*questions)[question_count].timeout);

            question_count++;
        }
    }

    fclose(file);
    return question_count;
}

// The rest of your functions (`check_answer`, `next_question`, `use_lifeline`, etc.) stay the same.

