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

/**
 * @brief Fonction de création d'une skiplist
 * @param max_levels Le nombre maximum de niveaux de la skiplist
 * @return Un pointeur vers la skiplist créée
 */
t_list * create_list(int max_levels) {
    t_list * new_list = (t_list *)malloc(sizeof(t_list));
    new_list->max_levels = max_levels;
    new_list->heads = (t_cell **)malloc(max_levels * sizeof(t_cell *));
    // Init heads = NULL
    for (int i = 0; i < max_levels; i++) {
        new_list->heads[i] = NULL;
    }

    return new_list;
}

/**
 * @brief Fonction d'insertion d'une cellule dans une skiplist
 * @param list Un pointeur vers la skiplist
 * @param levels Le niveau de la cellule à insérer
 * @param value La valeur de la cellule à insérer
 */
void insert_cell(t_list* list, int levels, int value) {
    if (levels < 1 || levels > list->max_levels) {
        printf("Invalid level.\n");
        return;
    }

    t_cell* new_cell = create_cell(value, levels - 1);
    for (int i = 0; i <= levels - 1; i++) {
        t_cell* curr = list->heads[i];
        t_cell* prev = NULL;

        // Trouver la position de la cellule à insérer
        // TODO : optimiser l'insertion en partant du niveau le plus haut et en descendant jusqu'au niveau 0 si nécessaire
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

/**
 * @brief Fonction d'affichage d'un niveau d'une skiplist
 * @param list La skiplist
 * @param level Le niveau à afficher
 */
void print_level(t_list list, int level) {
    if (level < 0 || level >= list.max_levels) {
        printf("Valeur de level invalide\n");
        return;
    }

    t_cell * current = list.heads[level];
    printf("[list head_%d @-] --> ", level);
    while (current != NULL) {
        printf("[%3d|@-] --> ", current->value);
        current = current->next[level];
    }
    printf("NULL\n");
}

/**
 * @brief Fonction d'affichage de tous les niveaux d'une skiplist
 * @param list La skiplist
 */
void print_all_levels(t_list list) {
    print_level(list, 0);

    for (int i = 1; i < list.max_levels; i++) {
        t_cell * current = list.heads[0];
        t_cell * current_level = list.heads[i];
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
