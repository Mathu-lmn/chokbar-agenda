#include <stdio.h>
#include <stdlib.h>
#include "lists.h"

t_list * create_list(int max_levels) {
    t_list * new_list = (t_list *)malloc(sizeof(t_list));
    new_list->max_levels = max_levels;
    new_list->heads = (t_cell **)malloc(max_levels * sizeof(t_cell *));
    // Initialize heads to NULL
    for (int i = 0; i < max_levels; i++) {
        new_list->heads[i] = NULL;
    }

    return new_list;
}

// Function to insert a cell into all levels from 0 to the specified level
// TODO : optimiser l'insertion en partant du niveau le plus haut et en descendant jusqu'au niveau 0 si nécessaire
void insert_cell(t_list* list, int levels, int value) {
    if (levels < 1 || levels > list->max_levels) {
        printf("Invalid level.\n");
        return;
    }


    for (int i = 0; i <= levels - 1; i++) {
        t_cell* curr = list->heads[i];
        t_cell* prev = NULL;
        t_cell* new_cell = create_cell(value);

        // Find the correct insertion point at this level
        while (curr != NULL && curr->value < value) {
            prev = curr;
            curr = curr->next;
        }

        if (prev == NULL) {
            // If prev is NULL, we are at the beginning of the list at this level
            new_cell->next = list->heads[i];
            list->heads[i] = new_cell;
        } else {
            // Insert in the middle or at the end of the list at this level
            prev->next = new_cell;
            new_cell->next = curr;
        }
    }
}

void print_level(t_list * list, int level) {
    if (level < 0 || level >= list->max_levels) {
        printf("Valeur de level invalide\n");
        return;
    }

    t_cell * current = list->heads[level];
    printf("[list head_%d @-] --> ", level);
    while (current != NULL) {
        printf("[%3d|@-] --> ", current->value);
        current = current->next;
    }
    printf("NULL\n");
}

void print_all_levels(t_list * list) {
    print_level(list, 0);

    for (int i = 1; i < list->max_levels; i++) {
        t_cell * current = list->heads[0];
        t_cell * current_level = list->heads[i];
        printf("[list head_%d @-] ", i);
        while (current != NULL) {
            if (current_level != NULL && current->value == current_level->value) {
                printf("--> [%3d|@-] ", current_level->value);
                current_level = current_level->next;
            } else {
                printf("-------------");
            }
            current = current->next;
        }
        printf("--> NULL\n");
    }

}
