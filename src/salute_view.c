/*
 * salute_view.c
 *
 *  Created on: 10. 12. 2019
 *      Author: HORACEK
 */


#include "include/salute_view.h"



struct _salute_view_
{
	salute_ui * ui;
	cu * cu_;

	bool updating;
	bool test_evalued;
};


static void salute_view_btn_run_tests_click_callback(GtkWidget*, gpointer);
static void salute_view_btn_initialize_tests_click_callback(GtkWidget*, gpointer);
static gpointer salute_view_load_test_structure(gpointer);
static gboolean salute_view_performe_testing(gpointer);
static void salute_view_signals(salute_view *);
static void salute_view_load_test_tree_list_content(GtkTreeStore *, GtkTreeIter *, cu_test_suite *);
static void salute_view_load_test_tree_list_root(GtkTreeStore *, GtkTreeIter *, cu_test_suite *);
static void salute_view_update_charts(salute_view *);
static void salute_initialize_cu_structure(salute_view *);
static gboolean salute_view_cyclic_interupt(gpointer);



salute_view * salute_view_new()
{
	salute_view * this = malloc(sizeof(salute_view));

	this->ui = salute_ui_new();
	this->cu_ = NULL;
	this->updating = false;
	this->test_evalued = false;

	salute_view_signals(this);
	salute_view_show(this);

	g_timeout_add(100,  salute_view_cyclic_interupt, this);

	return this;
}

static gboolean salute_view_cyclic_interupt(gpointer param)
{
	salute_view * this = (salute_view *) param;

	gtk_widget_set_sensitive(GTK_WIDGET(this->ui->btn_initialize_tests), this->updating == false);
	gtk_widget_set_sensitive(GTK_WIDGET(this->ui->btn_show_tcp_settings_screen), this->updating == false);

	if(this->cu_ != NULL)
	{
		if(this->updating == true)
			salute_view_reload_test_tree_list(this);

		if((cu_get_ts_number(this->cu_) > 0) && (this->updating == false))
		{
			gtk_widget_set_sensitive(GTK_WIDGET(this->ui->btn_run_tests), this->test_evalued == false);
			gtk_widget_set_sensitive(GTK_WIDGET(this->ui->btn_save_report), TRUE);
		}
		else
		{
			gtk_widget_set_sensitive(GTK_WIDGET(this->ui->btn_run_tests), FALSE);
			gtk_widget_set_sensitive(GTK_WIDGET(this->ui->btn_save_report), FALSE);
		}
	}
	else
	{
		gtk_widget_set_sensitive(GTK_WIDGET(this->ui->btn_run_tests), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(this->ui->btn_save_report), FALSE);
	}

	if(this->updating == true)
		gtk_spinner_start (GTK_SPINNER(this->ui->spinner));
	else
		gtk_spinner_stop (GTK_SPINNER(this->ui->spinner));

	return TRUE;
}

static void salute_view_load_test_tree_list_content(GtkTreeStore * test_list_tree_store, GtkTreeIter * iter1, cu_test_suite * ts)
{
	GtkTreeIter iter2;
	array_list * tc_list = cu_test_suite_to_array_list(ts);

	for(int j = 0; j < cu_test_suite_get_size(ts); j ++)
	{
		cu_test_case * tc = array_list_get(tc_list, j);

		char string_time_duration[16];
		sprintf(string_time_duration, "%I64dms", cu_test_case_get_time_duration(tc));

		gtk_tree_store_append (test_list_tree_store, &iter2, iter1);
		gtk_tree_store_set (test_list_tree_store, &iter2,
		                    TEST_CASE_NAME, cu_test_case_get_description(tc),
							TEST_TIME_DURATION, string_time_duration,
							TEST_EVALUATION, ((cu_test_case_get_evaluation(tc) == true) ? "PASS" : "FAIL"),
		                    -1);
	}
}

static void salute_view_load_test_tree_list_root(GtkTreeStore * test_list_tree_store, GtkTreeIter * iter1, cu_test_suite * ts)
{
	gtk_tree_store_append (test_list_tree_store, iter1, NULL);
	gtk_tree_store_set (test_list_tree_store, iter1,
	                    TEST_SUITE_NAME, cu_test_suite_get_description(ts),
						TEST_EVALUATION, ((cu_test_suite_get_failed_count(ts) == 0) ? "PASS":"FAIL"),
						-1);
}

static void salute_view_update_charts(salute_view * this)
{
	uint32_t test_number = cu_get_tc_number(this->cu_);

		gtk_chart_set_parameters(this->ui->fail_test_chart,
								test_number,
								cu_get_failed_number(this->cu_),
								cu_get_failed_percentage(this->cu_));
		gtk_chart_set_parameters(this->ui->pass_test_chart,
								test_number,
								cu_get_successfull_number(this->cu_),
								cu_get_successfull_percentage(this->cu_));
}

void salute_view_reload_test_tree_list(salute_view * this)
{
	gtk_tree_store_clear (this->ui->test_list_tree_store);

	array_list * ts_list = cu_get_tests(this->cu_);
	GtkTreeIter iter1;

	for(int i = 0; i < cu_get_ts_number(this->cu_); i ++)
	{
		cu_test_suite * ts = array_list_get(ts_list, i);

		salute_view_load_test_tree_list_root(this->ui->test_list_tree_store, &iter1, ts);
		salute_view_load_test_tree_list_content(this->ui->test_list_tree_store, &iter1, ts);
	}

	salute_view_update_charts(this);
}

static void salute_view_btn_run_tests_click_callback(GtkWidget* widget, gpointer param)
{
	salute_view * this = (salute_view *) param;
	this->test_evalued = true;

	g_timeout_add(10,  salute_view_performe_testing, this);
}

static void salute_view_btn_initialize_tests_click_callback(GtkWidget* widget, gpointer param)
{

	salute_view * this = (salute_view *) param;
	this->test_evalued = false;
	g_thread_new ("init", salute_view_load_test_structure, this);
}

connection * salute_view_get_connection(salute_view * this)
{
	return connection_new(gtk_entry_get_text(GTK_ENTRY(this->ui->entry_ip_address)), atoi(gtk_entry_get_text(GTK_ENTRY(this->ui->entry_tcp_port))));
}

static void salute_initialize_cu_structure(salute_view * this)
{
	if(this->cu_ != NULL)
			cu_finalize(this->cu_);

		this->cu_ = cu_new();
}

static gpointer salute_view_load_test_structure(gpointer param)
{
	salute_view * this = (salute_view *) param;
	this->updating = true;
	connection * con = salute_view_get_connection(this);

	salute_initialize_cu_structure(this);

	uint8_t ts_number = remote_count_test_suites(con);

	for(uint8_t i = 0; i < ts_number; i++)
	{
		char * ts_name = remote_get_test_suite_name(con, i+1);
		uint8_t ts_number = remote_count_test_cases(con, i+1);

		cu_test_suite * ts = cu_test_suite_new(ts_name);

		for(uint8_t j = 0; j < ts_number; j++)
		{
			char * tc_name = remote_get_test_case_name(con, i+1, j+1);
			tc_address * address = tc_address_new(i+1, j+1, con);
			cu_test_case * tc = cu_test_case_new(tc_name, remote_test, address);

			cu_test_suite_add_test(ts, tc);

			free(tc_name);
		}

		cu_add_test_suite(this->cu_, ts);
		free(ts_name);
	}

	connection_finalize(con);

	sleep(1);
	this->updating = false;

	return NULL;
}

static gboolean salute_view_performe_testing(gpointer param)
{
	salute_view * this = (salute_view *) param;

	bool next = cu_run_next(this->cu_);

	salute_view_reload_test_tree_list(this);

	if(next == true)
		gtk_spinner_start (GTK_SPINNER(this->ui->spinner));
	else
		gtk_spinner_stop(GTK_SPINNER(this->ui->spinner));

	return next;
}

void salute_view_show(salute_view * this)
{
	gtk_widget_show_all(GTK_WIDGET(this->ui->window));
}

static void salute_view_signals(salute_view * this)
{
	g_signal_connect(G_OBJECT(this->ui->btn_run_tests), "clicked", G_CALLBACK(salute_view_btn_run_tests_click_callback), this);
	g_signal_connect(G_OBJECT(this->ui->btn_initialize_tests), "clicked", G_CALLBACK(salute_view_btn_initialize_tests_click_callback), this);
}

void salute_view_finalize(salute_view * this)
{
	free(this);
}
