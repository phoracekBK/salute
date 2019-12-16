/*
 * salute_ui.c
 *
 *  Created on: 10. 12. 2019
 *      Author: HORACEK
 */

#include "include/salute_ui.h"

struct _private_ui_
{
	GtkWidget * test_tree_list_scroll;
	GtkWidget * main_screen;
	GtkWidget * network_settings_screen;

	GtkWidget * lbl_ip_address;
	GtkWidget * lbl_tcp_port;

	GdkRectangle * geometry;

	double horizontal_ratio;
	double vertical_ratio;
};

static void salute_ui_build_widgets(salute_ui *);
static void salute_ui_load_window_dimension(salute_ui *);
static void salute_ui_setup_window(salute_ui *);
static void salute_ui_setup_widgets(salute_ui *);
static void salute_ui_setup_test_tree_list_column(salute_ui *);
static void salute_ui_pack_widgets(salute_ui *);
static GtkTreeViewColumn * salute_ui_add_test_tree_list_column(int , char * );
static void salute_ui_signals(salute_ui *);
static void salute_ui_pack_main_screen(salute_ui *);
static void salute_ui_pack_network_settings_screen(salute_ui *);
static void salute_ui_build_main_window_widgets(salute_ui *);
static void salute_ui_build_network_settings_screen_widgets(salute_ui *);
static void salute_ui_btn_tcp_settings_click_callback(GtkWidget*, gpointer);
static void salute_ui_tcp_settings_btn_back_click_callback(GtkWidget*, gpointer);





salute_ui * salute_ui_new()
{
	salute_ui * this = malloc(sizeof(salute_ui));

	this->private = malloc(sizeof(private_ui));

	salute_ui_build_widgets(this);
	salute_ui_load_window_dimension(this);
	salute_ui_setup_widgets(this);
	salute_ui_pack_widgets(this);
	salute_ui_signals(this);

	return this;
}

void salute_ui_put_widget(salute_ui * this, GtkWidget * container, GtkWidget * widget, double x, double y)
{
	gtk_fixed_put(GTK_FIXED(container), widget, salute_ui_count_horizontal_ratio(this, x), salute_ui_count_vertical_ratio(this, y));
}

GtkWidget * salute_ui_create_screen(salute_ui * this, char * screen_name)
{
	GtkWidget * screen = gtk_fixed_new();
	gtk_stack_add_named(GTK_STACK(this->screen_stack), screen, screen_name);

	return screen;
}

double salute_ui_count_horizontal_ratio(salute_ui * this, double width)
{
	return width*this->private->horizontal_ratio;
}

double salute_ui_count_vertical_ratio(salute_ui * this, double height)
{
	return height * this->private->vertical_ratio ;
}

void salute_ui_finalize(salute_ui * this)
{
	free(this->private);
	free(this);
}

static void salute_ui_build_main_window_widgets(salute_ui * this)
{
	this->screen_stack = gtk_stack_new();

	this->private->main_screen = salute_ui_create_screen(this, UI_MAIN_SCREEN_NAME);
	this->private->network_settings_screen = salute_ui_create_screen(this, UI_NETWORK_SETTINGS_SCREEN_NAME);

	this->spinner = gtk_spinner_new();

	this->test_list_tree_store = gtk_tree_store_new(TEST_N, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

	this->private->test_tree_list_scroll = gtk_scrolled_window_new(NULL, NULL);
	this->test_tree_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(this->test_list_tree_store));

	this->btn_initialize_tests = gtk_button_new_with_label("Inicializovat");
	this->btn_run_tests = gtk_button_new_with_label("Spustit testy");
	this->btn_save_report = gtk_button_new_with_label("Ulo\u017Eit report");

	double color_fail[3] = {0.6, 0.1, 0.1};
	double color_pass[3] = {0.1, 0.6, 0.1};

	this->fail_test_chart = gtk_chart_new("FAIL", color_fail);
	this->pass_test_chart = gtk_chart_new("PASS", color_pass);

	this->spinner = gtk_spinner_new();
}

static void salute_ui_build_network_settings_screen_widgets(salute_ui * this)
{
	this->btn_show_main_screen = gtk_button_new_with_label("Zp\u011Bt");

	this->btn_show_tcp_settings_screen = gtk_button_new_with_label("Nastaven\u00ED");

	this->private->lbl_ip_address = gtk_label_new("IP adresa:");
	this->private->lbl_tcp_port = gtk_label_new("TCP port:");

	this->entry_ip_address = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(this->entry_ip_address), "192.168.0.1");

	this->entry_tcp_port = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(this->entry_tcp_port), "2500");
}

static void salute_ui_build_widgets(salute_ui * this)
{
	this->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	salute_ui_build_main_window_widgets(this);
	salute_ui_build_network_settings_screen_widgets(this);
}

static GtkTreeViewColumn * salute_ui_add_test_tree_list_column(int order, char * label)
{
	GtkCellRenderer * renderer = gtk_cell_renderer_text_new ();
	g_object_set (renderer, "xalign", 0.5, NULL);

	GtkTreeViewColumn * column = gtk_tree_view_column_new_with_attributes (label,  renderer, "text", order, NULL);

	gtk_tree_view_column_set_expand (column, TRUE);
	gtk_tree_view_column_set_alignment(column, 0.5);

	return column;
}

static void salute_ui_setup_test_tree_list_column(salute_ui * this)
{
	gtk_tree_view_append_column (GTK_TREE_VIEW (this->test_tree_list),
							salute_ui_add_test_tree_list_column(TEST_SUITE_NAME, "Testovac\u00ED sc\u00E9n\u00E1\u0159"));

	gtk_tree_view_append_column (GTK_TREE_VIEW (this->test_tree_list),
								salute_ui_add_test_tree_list_column(TEST_CASE_NAME, "Testovac\u00ED p\u0159\u00EDpad"));

	gtk_tree_view_append_column (GTK_TREE_VIEW (this->test_tree_list),
								salute_ui_add_test_tree_list_column(TEST_TIME_DURATION, "Doba trv\u00E1n\u00ED"));

	gtk_tree_view_append_column (GTK_TREE_VIEW (this->test_tree_list),
								salute_ui_add_test_tree_list_column(TEST_EVALUATION, "V\u00FDsledek testu"));

	gtk_tree_view_columns_autosize (GTK_TREE_VIEW (this->test_tree_list));
}

static void salute_ui_setup_widgets(salute_ui * this)
{
	salute_ui_setup_window(this);

	gtk_widget_set_size_request(GTK_WIDGET(this->spinner), salute_ui_count_horizontal_ratio(this, 30),
															salute_ui_count_vertical_ratio(this, 30));

	gtk_widget_set_size_request(GTK_WIDGET(this->private->test_tree_list_scroll),
								salute_ui_count_horizontal_ratio(this, 700),
								salute_ui_count_vertical_ratio(this, 330));

	gtk_chart_set_dimension(this->pass_test_chart, salute_ui_count_horizontal_ratio(this, 700), salute_ui_count_vertical_ratio(this, 30));
	gtk_chart_set_dimension(this->fail_test_chart, salute_ui_count_horizontal_ratio(this, 700), salute_ui_count_vertical_ratio(this, 30));

	gtk_widget_set_size_request(GTK_WIDGET(this->btn_initialize_tests), 120, 35);
	gtk_widget_set_size_request(GTK_WIDGET(this->btn_run_tests), 120, 35);
	gtk_widget_set_size_request(GTK_WIDGET(this->btn_save_report), 120, 35);

	 salute_ui_setup_test_tree_list_column(this);

	 gtk_widget_set_size_request(GTK_WIDGET(this->btn_show_main_screen), 120, 35);
	 gtk_widget_set_size_request(GTK_WIDGET(this->btn_show_tcp_settings_screen), 120, 35);

	 gtk_widget_set_size_request(GTK_WIDGET(this->spinner), 50,50);
}

static void salute_ui_setup_window(salute_ui * this)
{
	gtk_widget_set_size_request(GTK_WIDGET(this->window), this->private->geometry->width, this->private->geometry->height);
	gtk_window_maximize(GTK_WINDOW(this->window));
}

static void salute_ui_pack_main_screen(salute_ui * this)
{
	gtk_container_add(GTK_CONTAINER(this->private->test_tree_list_scroll), this->test_tree_list);

	salute_ui_put_widget(this, this->private->main_screen, this->private->test_tree_list_scroll, 50, 70);

	salute_ui_put_widget(this, this->private->main_screen, gtk_chart_get_instance(this->fail_test_chart), 50, 430);
	salute_ui_put_widget(this, this->private->main_screen, gtk_chart_get_instance(this->pass_test_chart), 50, 500);

	salute_ui_put_widget(this, this->private->main_screen, this->btn_save_report, 50, 30);
	gtk_fixed_put(GTK_FIXED(this->private->main_screen), this->btn_run_tests,
						(this->private->horizontal_ratio*(800-50)) - 120,
						salute_ui_count_vertical_ratio(this, 30));
	gtk_fixed_put(GTK_FIXED(this->private->main_screen), this->btn_initialize_tests,
						(this->private->horizontal_ratio*(800-60)) - 240,
						salute_ui_count_vertical_ratio(this, 30));


	gtk_fixed_put(GTK_FIXED(this->private->main_screen),this->btn_show_tcp_settings_screen,
						salute_ui_count_horizontal_ratio(this, 50)+ 120+30,
						salute_ui_count_vertical_ratio(this, 30));

	salute_ui_put_widget(this, this->private->main_screen, this->spinner, 400,30);
}

static void salute_ui_pack_network_settings_screen(salute_ui * this)
{
	salute_ui_put_widget(this, this->private->network_settings_screen, this->btn_show_main_screen, 200,100);

	salute_ui_put_widget(this, this->private->network_settings_screen, this->private->lbl_ip_address, 200,150);
	salute_ui_put_widget(this, this->private->network_settings_screen, this->entry_ip_address, 400,150);

	salute_ui_put_widget(this, this->private->network_settings_screen, this->private->lbl_tcp_port, 200,250);
	salute_ui_put_widget(this, this->private->network_settings_screen, this->entry_tcp_port, 400,250);
}

static void salute_ui_pack_widgets(salute_ui * this)
{
	gtk_container_add(GTK_CONTAINER(this->window), this->screen_stack);

	salute_ui_pack_main_screen(this);
	salute_ui_pack_network_settings_screen(this);
}

static void salute_ui_load_window_dimension(salute_ui * this)
{
	this->private->geometry = gtk_base_read_display_dimension();

	this->private->horizontal_ratio = ((double)this->private->geometry->width)/BASE_WINDOW_WIDTH;
	this->private->vertical_ratio = ((double)this->private->geometry->height)/BASE_WINDOW_HEIGHT;
}

static void salute_ui_btn_tcp_settings_click_callback(GtkWidget* widget, gpointer param)
{
	salute_ui * this = (salute_ui *) param;
	gtk_stack_set_visible_child_name(GTK_STACK(this->screen_stack), UI_NETWORK_SETTINGS_SCREEN_NAME);
	network_close(3);
}

static void salute_ui_tcp_settings_btn_back_click_callback(GtkWidget* widget, gpointer param)
{
	salute_ui * this = (salute_ui *) param;
	gtk_stack_set_visible_child_name(GTK_STACK(this->screen_stack), UI_MAIN_SCREEN_NAME);
}

static void salute_ui_signals(salute_ui * this)
{
	g_signal_connect(G_OBJECT(this->btn_show_tcp_settings_screen), "clicked", G_CALLBACK(salute_ui_btn_tcp_settings_click_callback), this);
	g_signal_connect(G_OBJECT(this->btn_show_main_screen), "clicked", G_CALLBACK(salute_ui_tcp_settings_btn_back_click_callback), this);

	g_signal_connect(G_OBJECT(this->window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
}
