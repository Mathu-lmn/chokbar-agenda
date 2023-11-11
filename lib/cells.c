/**
 * @file cells.c
 * @author Mathurin Lemoine / Thomas Fischer / Idir Nait Meddour
 * @brief Fichier contenant les fonctions de manipulation des cellules
 * @version 1.0
 * @date 24/10/2023
 */
#include <stdlib.h>
#include "cells.h"

/**
 * @brief Fonction de création d'une cellule
 * @param value La valeur de la cellule
 * @param level Le niveau de la cellule
 * @return Un pointeur vers la cellule créée
 */
p_cell create_cell(int value, int level) {
    p_cell new_cell = (p_cell) malloc(sizeof(t_cell));
    new_cell->value = value;
    new_cell->level = level;
    new_cell->next = (p_cell*) malloc((level + 1) * sizeof(p_cell));

    return new_cell;
}