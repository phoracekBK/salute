/*
 * remote.h
 *
 *  Created on: 10. 12. 2019
 *      Author: HORACEK
 */

#ifndef REMOTE_H_
#define REMOTE_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>


#include "network.h"
#include "tc_address.h"
#include "connection.h"

enum _remote_cmd_
{
	REMOTE_CMD_PING = 1,
	REMOTE_CMD_RESET,
	REMOTE_CMD_COUNT_TEST_SUITES,
	REMOTE_CMD_COUNT_TEST_CASES,
	REMOTE_CMD_GET_TEST_SUITE_NAME,
	REMOTE_CMD_GET_TEST_CASE_NAME,
	REMOTE_CMD_RUN_TEST_CASE,
};



bool remote_ping(connection *);
bool remote_reset(connection *);
uint8_t remote_count_test_suites(connection *);
uint8_t remote_count_test_cases(connection *, int);
char* remote_get_test_suite_name(connection *, int);
char* remote_get_test_case_name(connection *, int,int);
bool remote_test(void *);


#endif /* REMOTE_H_ */
