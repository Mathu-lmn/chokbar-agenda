#include <stdio.h>
#include <stdlib.h>
#include "cells.h"

t_cell * create_cell(int value) {
    t_cell * new_cell = (t_cell *)malloc(sizeof(t_cell));
    new_cell->value = value;
    new_cell->next = NULL;
    return new_cell;
}