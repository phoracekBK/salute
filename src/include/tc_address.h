/*
 * tc_address.h
 *
 *  Created on: 10. 12. 2019
 *      Author: HORACEK
 */

#ifndef TC_ADDRESS_H_
#define TC_ADDRESS_H_

#include <stdlib.h>

#include "connection.h"

struct _tc_address_;
typedef struct _tc_address_ tc_address;


tc_address* tc_address_new(int, int, connection *);
int tc_address_get_test_case_index(tc_address *);
int tc_address_get_test_suite_index(tc_address *);
connection * tc_address_get_connection(tc_address *);
void tc_address_finalize(tc_address *);
void tc_address_finalize_v2(void *);

#endif /* TC_ADDRESS_H_ */
