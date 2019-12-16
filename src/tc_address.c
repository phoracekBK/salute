/*
 * tc_address.c
 *
 *  Created on: 10. 12. 2019
 *      Author: HORACEK
 */


#include "include/tc_address.h"


struct _tc_address_
{
	int test_suite_index;
	int test_case_index;

	connection * con;
};



tc_address* tc_address_new(int ts_index, int tc_index, connection * con)
{
	tc_address * this = malloc(sizeof(tc_address));

	this->test_case_index = tc_index;
	this->test_suite_index = ts_index;

	this->con = connection_clone(con);

	return this;
}

connection * tc_address_get_connection(tc_address * this)
{
	return this->con;
}

int tc_address_get_test_case_index(tc_address * this)
{
	return this->test_case_index;
}

int tc_address_get_test_suite_index(tc_address * this)
{
	return this->test_suite_index;
}

void tc_address_finalize_v2(void * this)
{
	tc_address_finalize((tc_address *) this);
}


void tc_address_finalize(tc_address * this)
{
	connection_finalize(this->con);
	free(this);
}

