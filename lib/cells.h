#ifndef CHOKBAR_AGENDA_CELLS_H
#define CHOKBAR_AGENDA_CELLS_H

typedef struct s_cell {
    int value;
    struct s_cell* next;
} t_cell;

// Prototypes

t_cell * create_cell(int);

#endif //CHOKBAR_AGENDA_CELLS_H
