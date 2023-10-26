/**
 * @file lists.h
 * @author Mathurin Lemoine / Thomas Fischer / Idir Nait Meddour
 * @brief Fichier contenant les structures et prototypes de fonctions de manipulation des skiplists
 * @version 1.0
 * @date 24/10/2023
 */
#ifndef CHOKBAR_AGENDA_LISTS_H
#define CHOKBAR_AGENDA_LISTS_H

#include "cells.h"

// Structures

/**
 * @brief Structure représentant une skiplist
 *
 * Une skiplist est une liste à plusieurs niveaux.
 * Elle contient un tableau de pointeurs vers les têtes de liste et le nombre maximum de niveaux.
 */
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
