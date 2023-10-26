#ifndef CHOKBAR_AGENDA_LISTS_H
#define CHOKBAR_AGENDA_LISTS_H

#include "cells.h"

typedef struct s_list {
    t_cell ** heads;  // Tableau de pointeurs vers les têtes de liste
    int max_levels;
} t_list;

// Prototypes

t_list * create_list(int);
void insert_cell(t_list *, int, int);
void print_level(t_list, int);
void print_all_levels(t_list);

#endif //CHOKBAR_AGENDA_LISTS_H
