/*
 * connection.c
 *
 *  Created on: 10. 12. 2019
 *      Author: HORACEK
 */

#include "include/connection.h"

struct _connection_
{
	char * ip_address;
	int tcp_port;
};


connection * connection_new(const char * ip_address, int tcp_port)
{
	connection * this = malloc(sizeof(connection));

	this->ip_address = malloc(sizeof(char)*(strlen(ip_address)+1));
	strcpy(this->ip_address, ip_address);

	this->tcp_port = tcp_port;

	return this;
}

connection * connection_clone(connection * this)
{
	return connection_new(this->ip_address, this->tcp_port);
}

const char * connection_get_ip_address(connection * this)
{
	return (const char *) this->ip_address;
}

int connection_get_tcp_port(connection * this)
{
	return this->tcp_port;
}

void connection_finalize_v2(void * this)
{
	connection_finalize((connection *) this);
}

void connection_finalize(connection * this)
{
	free(this->ip_address);
	free(this);
}
