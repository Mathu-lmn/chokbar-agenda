#ifndef CHOKBAR_AGENDA_CELLS_H
#define CHOKBAR_AGENDA_CELLS_H

typedef struct s_cell {
    int value;
    int level;
    struct s_cell ** next;
} t_cell;

// Prototypes

t_cell * create_cell(int, int);

#endif //CHOKBAR_AGENDA_CELLS_H
