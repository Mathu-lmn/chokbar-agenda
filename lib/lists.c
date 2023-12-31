/**
 * @file lists.c
 * @author Mathurin Lemoine / Thomas Fischer / Idir Nait Meddour
 * @brief Fichier contenant les fonctions de manipulation des skiplists
 * @version 1.0
 * @date 24/10/2023
 */
#include <stdio.h>
#include <stdlib.h>
#include "lists.h"

t_list * create_list(int max_levels) {
    t_list * new_list = malloc(sizeof(t_list));
    new_list->max_levels = max_levels;
    new_list->heads = malloc(max_levels * sizeof(p_cell));
    // Init heads = NULL
    for (int i = 0; i < max_levels; i++) {
        new_list->heads[i] = NULL;
    }

    return new_list;
}

void insert_cell(t_list* list, int levels, int value) {
    if (levels < 1 || levels > list->max_levels) {
        printf("Invalid level.\n");
        return;
    }

    p_cell new_cell = create_cell(value, levels - 1);
    for (int i = 0; i <= levels - 1; i++) {
        p_cell curr = list->heads[i];
        p_cell prev = NULL;

        // Trouver la position de la cellule à insérer
        while (curr != NULL && curr->value < value) {
            prev = curr;
            curr = curr->next[i];
        }

        if (prev == NULL) {
            // Si prev == NULL, début de la liste
            new_cell->next[i] = curr;
            list->heads[i] = new_cell;
        } else {
            // Sinon, insérer la cellule entre prev et curr
            prev->next[i] = new_cell;
            new_cell->next[i] = curr;
        }
    }
}

void print_level(t_list list, int level) {
    if (level < 0 || level >= list.max_levels) {
        printf("Valeur de level invalide\n");
        return;
    }

    p_cell current = list.heads[level];
    printf("[list head_%d @-] --> ", level);
    while (current != NULL) {
        printf("[%3d|@-] --> ", current->value);
        current = current->next[level];
    }
    printf("NULL\n");
}

void print_all_levels(t_list list) {
    print_level(list, 0);

    for (int i = 1; i < list.max_levels; i++) {
        p_cell current = list.heads[0];
        p_cell current_level = list.heads[i];
        printf("[list head_%d @-] ", i);
        while (current != NULL) {
            if (current_level != NULL && current->value == current_level->value) {
                printf("--> [%3d|@-] ", current_level->value);
                current_level = current_level->next[i];
            } else {
                printf("-------------");
            }
            current = current->next[0];
        }
        printf("--> NULL\n");
    }
}

t_list * create_better_list(int max_level_power){
    t_list* my_list = create_list(max_level_power);

    int nb_cell = (int) (pow(2, max_level_power) + 1e-9)-1;
    int * levels = malloc(nb_cell*sizeof(int));
    if (levels == NULL){
        printf("Erreur d'allocation mémoire");
        return NULL;
    }
    for (int i = 0; i < nb_cell; i++){
        levels[i] = 1;
    }
    int split_nb = 2;
    while (split_nb <= nb_cell){
        for (int i = split_nb - 1; i < nb_cell; i = i + split_nb){
            levels[i]++;
        }
        split_nb = split_nb*2;
    }

    for (int i = 0; i < nb_cell; i++){
        insert_cell(my_list, levels[i], i+1);
    }
    return my_list;
}


void classic_search (t_list list, int val){
    t_cell * current = list.heads[0];
    while (current != NULL){
        if (current->value == val){
            //printf ("%d found", val);
            return;
        }
        current = current->next[0];
    }
    //printf("%d not found", val);
}

void better_search (t_list list, int val){
    int current_level = list.max_levels - 1;
    t_cell * current = list.heads[current_level];

    while ((current != NULL) && (current_level >= 0)){
        if (current->value == val){
            return;
        }
        if (current->value < val){
            if ((current->next[current_level] == NULL || current->next[current_level]->value > val)) {
                current_level--;
            } else {
                current = current->next[current_level];
            }
        } else {
            current_level--;
            if (current_level >= 0) current = list.heads[current_level];
        }
    }
}