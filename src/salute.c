/*
 ============================================================================
 Name        : salute.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : SALUTE	System pro vzdAlene voLani aUtomatizovanych TEstu
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <aclib.h>

#include "include/salute_view.h"




struct _salute_;
typedef struct _salute_ salute;


struct _salute_
{
	salute_view * view;
};



salute* salute_new();
void salute_finalize(salute *);










int main(int argv, char ** argc)
{
	salute * salute_instance;

	gtk_init(&argv, &argc);

	salute_instance = salute_new();

	gtk_main();

	salute_finalize(salute_instance);

	return EXIT_SUCCESS;
}



salute* salute_new()
{
	salute * this = malloc(sizeof(salute));

	this->view = salute_view_new();

	return this;
}


void salute_finalize(salute * this)
{
	salute_view_finalize(this->view);
	free(this);
}


















