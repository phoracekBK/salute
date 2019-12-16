/*
 * salute_ui.h
 *
 *  Created on: 10. 12. 2019
 *      Author: HORACEK
 */

#ifndef SALUTE_UI_H_
#define SALUTE_UI_H_

#include <aclib.h>
#include "network.h"


#define UI_MAIN_SCREEN_NAME "main_screen"
#define UI_NETWORK_SETTINGS_SCREEN_NAME "network_settings_screen"

enum _test_tree_list_columns_
{
	TEST_SUITE_NAME = 0,
	TEST_CASE_NAME,
	TEST_TIME_DURATION,
	TEST_EVALUATION,

	TEST_N
};

struct _salute_ui_;
typedef struct _salute_ui_ salute_ui;

struct _private_ui_;
typedef struct _private_ui_ private_ui;

struct _salute_ui_
{
	GtkWidget * window;

	GtkWidget * screen_stack;

	GtkWidget * btn_run_tests;
	GtkWidget * btn_initialize_tests;
	GtkWidget * btn_save_report;

	GtkWidget * test_tree_list;
	GtkTreeStore * test_list_tree_store;

	GtkWidget * btn_show_main_screen;
	GtkWidget * btn_show_tcp_settings_screen;
	GtkWidget * entry_ip_address;
	GtkWidget * entry_tcp_port;

	gtk_chart * pass_test_chart;
	gtk_chart * fail_test_chart;

	GtkWidget * spinner;

	private_ui * private;
};



salute_ui * salute_ui_new();
void salute_ui_put_widget(salute_ui *, GtkWidget *, GtkWidget *, double, double);
GtkWidget * salute_ui_create_screen(salute_ui *, char *);
double salute_ui_count_horizontal_ratio(salute_ui *, double);
double salute_ui_count_vertical_ratio(salute_ui *, double);
void salute_ui_finalize(salute_ui *);



#endif /* SALUTE_UI_H_ */






















