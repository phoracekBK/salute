/*
 * remote.c
 *
 *  Created on: 10. 12. 2019
 *      Author: HORACEK
 */

#include "include/remote.h"


bool remote_ping(connection * con)
{
	return false;
}

bool remote_reset(connection * con)
{
	return false;
}

uint8_t remote_count_test_suites(connection * con)
{
	const char msg[2] = {REMOTE_CMD_COUNT_TEST_SUITES, 0};

	char * resp = network_transaction(con, msg, 2);

	if(resp != NULL)
	{
		if(resp[0] == REMOTE_CMD_COUNT_TEST_SUITES)
		{
			uint8_t ts_count = resp[1];
			free(resp);

			return ts_count;
		}
		else
		{
			free(resp);
		}
	}

	return 0;
}

uint8_t remote_count_test_cases(connection * con, int ts_index)
{
	const char msg[3] = {REMOTE_CMD_COUNT_TEST_CASES, ts_index, 0};

	char * resp = network_transaction(con, msg, 2);

	if(resp != NULL)
	{
		if(resp[0] == REMOTE_CMD_COUNT_TEST_CASES)
		{
			uint8_t tc_count = resp[1];
			free(resp);

			return tc_count;
			}
		else
		{
			free(resp);
		}
	}

	return 0;
}

char* remote_get_test_suite_name(connection * con, int ts_index)
{
		const char msg[3] = {REMOTE_CMD_GET_TEST_SUITE_NAME, ts_index, 0};

		char * resp = network_transaction(con, msg, 128);

		if(resp != NULL)
		{
			if(resp[0] == REMOTE_CMD_GET_TEST_SUITE_NAME)
			{
				int name_length = strlen(resp);
				char * ts_name = malloc(sizeof(char) * name_length);
				memcpy(ts_name, resp+1, name_length);

				free(resp);

				return ts_name;
			}
			else
			{
				free(resp);

			}
		}

	return NULL;
}

char * remote_get_test_case_name(connection * con, int ts_index, int tc_index)
{
	const char msg[4] = {REMOTE_CMD_GET_TEST_CASE_NAME, ts_index, tc_index, 0};

	char * resp = network_transaction(con, msg, 128);

	if(resp != NULL)
	{
		if(resp[0] == REMOTE_CMD_GET_TEST_CASE_NAME)
		{
			int name_length = strlen(resp);
			char * tc_name = malloc(sizeof(char) * name_length);
			memcpy(tc_name, resp+1, name_length);

			free(resp);

			return tc_name;
		}
		else
		{
			free(resp);
		}
	}

	return NULL;
}

bool remote_test(void * address)
{
	const char msg[4]= {REMOTE_CMD_RUN_TEST_CASE, tc_address_get_test_suite_index(address), tc_address_get_test_case_index(address), 0};

	char * resp = network_transaction(tc_address_get_connection(address), msg, 2);

	if(resp != NULL)
	{
		if((resp[0] == REMOTE_CMD_RUN_TEST_CASE) && (resp[1] == 1))
		{
			free(resp);
			tc_address_finalize(address);

			return true;
		}
		else
		{
			free(resp);
		}
	}

	tc_address_finalize(address);

	return false;
}
