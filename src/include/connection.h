/*
 * connection.h
 *
 *  Created on: 10. 12. 2019
 *      Author: HORACEK
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <stdlib.h>
#include <string.h>


struct _connection_;
typedef struct _connection_ connection;


connection * connection_new(const char *, int);
connection * connection_clone(connection *);
const char * connection_get_ip_address(connection *);
int connection_get_tcp_port(connection *);
void connection_finalize(connection *);
void connection_finalize_v2(void * this);


#endif /* CONNECTION_H_ */
