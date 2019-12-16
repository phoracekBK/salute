/*
 * salute_view.h
 *
 *  Created on: 10. 12. 2019
 *      Author: HORACEK
 */

#ifndef SALUTE_VIEW_H_
#define SALUTE_VIEW_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <aclib.h>

#include "salute_ui.h"
#include "connection.h"
#include "tc_address.h"
#include "remote.h"


struct _salute_view_;
typedef struct _salute_view_ salute_view;



struct _network_;
typedef struct _network_ network;


salute_view * salute_view_new();
void salute_view_reload_test_tree_list(salute_view *);
connection * salute_view_get_connection(salute_view *);
void salute_view_show(salute_view *);
void salute_view_finalize(salute_view *);

#endif /* SALUTE_VIEW_H_ */
