#ifndef STD_H
#define STD_H
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include <math.h>
	#include "macros.h"
#endif

#ifndef RNG_H
#define RNG_H
	#include "rng.h"
#endif

#ifndef NODE_H
#define NODE_H
	#include "node.h"
#endif

#ifndef NEWMAN_ZIFF
#define NEWMAN_ZIFF
	#include "newman_ziff.h"
#endif

Node *make_netwrk_ER();   
Node *make_netwrk_BA();
int validate(Node *graph);
int print_network(Node *graph);
Node *read_net(char *name);
int read_data();
bool filter(char *s);
Node *make_netwrk_SF(double degree);
int *degree_SF(double degree, Node *graph, int *mm);
int distribution(Node *graph);
Node *make_netwrk_ER_expercol(char *name);
Node *make_netwrk_ER_percol(char *name);
