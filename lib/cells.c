#include <stdio.h>
#include <stdlib.h>
#include "cells.h"

t_cell * create_cell(int value, int level) {
    t_cell * new_cell = (t_cell *)malloc(sizeof(t_cell));
    new_cell->value = value;
    new_cell->level = level;
    new_cell->next = (t_cell **)malloc((level + 1) * sizeof(t_cell *));

    return new_cell;
}