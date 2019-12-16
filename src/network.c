/*
 * net_connection.c
 *
 *  Created on: 10. 12. 2019
 *      Author: HORACEK
 */


#include "include/network.h"


struct _network_
{
	GSocketConnection 	* connection;
	GSocketClient		* client;
	GInputStream		* input;
	GOutputStream		* output;
};



static network * net_singleton = NULL;
int fail_counter = 0;


bool network_open(connection * con)
{
	if(net_singleton == NULL)
	{
		net_singleton = malloc(sizeof(network));

		net_singleton->client = g_socket_client_new();
		net_singleton->connection = NULL;

		net_singleton->input = NULL;
		net_singleton->output = NULL;

		GError * error = NULL;
		net_singleton->connection = g_socket_client_connect_to_host (net_singleton->client,
																	connection_get_ip_address(con),
																	connection_get_tcp_port(con),
																	NULL,
																	&error);

		if(net_singleton->connection != NULL && error == NULL)
		{
			GSocket * socket = g_socket_connection_get_socket(net_singleton->connection);
			g_socket_set_timeout (socket, 1);

			return true;
		}
		else
		{
			g_object_unref(net_singleton->client);
			free(net_singleton);
			net_singleton = NULL;

			return false;
		}
	}

	return true;
}

void network_close(int id)
{
	if(net_singleton != NULL)
	{
		GError * error = NULL;
		GSocket * socket = g_socket_connection_get_socket (net_singleton->connection);

		if(socket != NULL)
			g_socket_close(socket, &error);

		if(net_singleton->connection != NULL)
			g_object_unref(net_singleton->connection);

		if(net_singleton->client != NULL)
			g_object_unref(net_singleton->client);

		free(net_singleton);

		net_singleton = NULL;
	}
}

int network_write(const char* message)
{
	int size_out = -1;

	if(net_singleton != NULL)
	{
		GError * error1 = NULL;
		GError * error2 = NULL;

		if(net_singleton->output == NULL)
			net_singleton->output = g_io_stream_get_output_stream (G_IO_STREAM (net_singleton->connection));

		size_out = g_output_stream_write(G_OUTPUT_STREAM(net_singleton->output), message, strlen(message), NULL, &error1);
		g_output_stream_flush (G_OUTPUT_STREAM(net_singleton->output), NULL, &error2);

		if(error1 != NULL)
			network_close(1);
	}

	return size_out;
}

char * network_read(int max_in)
{
	if(net_singleton != NULL)
	{
		GError * error1 = NULL;

		if(net_singleton->input == NULL)
			net_singleton->input = g_io_stream_get_input_stream (G_IO_STREAM (net_singleton->connection));

		char * buffer = malloc(sizeof(char)*(max_in+1));
		memset(buffer, 0, max_in+1);

		g_input_stream_read (G_INPUT_STREAM(net_singleton->input), buffer, max_in, NULL, &error1);

		if(error1 == NULL)
			return buffer;

		free(buffer);

		network_close(2);
	}

	return NULL;
}

char * network_transaction(connection * con, const char * message, int max_in)
{
	if(network_open(con) != false)
	{
		if(network_write(message) >= 0)
		{
			usleep(50000);
			char * msg_out = network_read(max_in);

			if(msg_out != NULL)
			{
				fail_counter = 0;
				return msg_out;
			}

			if(fail_counter < 3)
			{
				fail_counter++;
				return network_transaction(con, message, max_in);
			}
		}
		else
		{
			if(fail_counter < 3)
			{
				fail_counter++;
				return network_transaction(con, message, max_in);
			}
		}
	}

	return NULL;
}




















