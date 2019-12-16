/*
 * net_connection.h
 *
 *  Created on: 10. 12. 2019
 *      Author: HORACEK
 */

#ifndef NET_CONNECTION_H_
#define NET_CONNECTION_H_

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "connection.h"

struct _network_;
typedef struct _network_ network;

bool network_open(connection *);
void network_close(int);
int network_write(const char*);
char * network_read(int);
char * network_transaction(connection *, const char *, int);

#endif /* NET_CONNECTION_H_ */
