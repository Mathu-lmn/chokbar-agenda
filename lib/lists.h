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
#include "timer.h"
#include <math.h>

// Structures

/**
 * @brief Structure représentant une skiplist
 *
 * Une skiplist est une liste à plusieurs niveaux.
 * Elle contient un tableau de pointeurs vers les têtes de liste et le nombre maximum de niveaux.
 */
typedef struct s_list {
    /** @brief Tableau de pointeurs vers les têtes de liste */
    p_cell * heads;
    /** @brief Le nombre maximum de niveaux */
    int max_levels;
} t_list;

// Prototypes

/**
 * @brief Fonction de création d'une skiplist
 * @param max_levels Le nombre maximum de niveaux de la skiplist
 * @return Un pointeur vers la skiplist créée
 */
t_list * create_list(int);

/**
 * @brief Fonction d'insertion d'une cellule dans une skiplist
 * @param list Un pointeur vers la skiplist
 * @param levels Le niveau de la cellule à insérer
 * @param value La valeur de la cellule à insérer
 */
void insert_cell(t_list *, int, int);

/**
 * @brief Fonction d'affichage d'un niveau d'une skiplist
 * @param list La skiplist
 * @param level Le niveau à afficher
 */
void print_level(t_list, int);

/**
 * @brief Fonction d'affichage de tous les niveaux d'une skiplist
 * @param list La skiplist
 */
void print_all_levels(t_list);

/**
 * @brief Fonction de création d'une skiplist "dichotomique"
 * @param max_level_power Nombre qui définit le nombre de cellule au niveau 0 par la formule 2^n - 1
 * @return un pointeur vers la skiplist créée
 */
t_list * create_better_list(int);

/**
 * @brief Fonction de recherche d'une valeur dans le niveau zéro d'une skiplist
 * @param list La skiplist
 * @param val La valeur à trouver
 */
void classic_search (t_list, int);

/**
 * @brief Fonction de recherche d'une valeur à partir du plus haut niveau d'une skiplist
 * @param list La skiplist
 * @param val La valeur à trouver
 */
void better_search (t_list, int);

#endif //CHOKBAR_AGENDA_LISTS_H
