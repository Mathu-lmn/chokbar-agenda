/**
 * @file cells.h
 * @author Mathurin Lemoine / Thomas Fischer / Idir Nait Meddour
 * @brief Fichier contenant les structures et prototypes de fonctions de manipulation des cellules
 * @version 1.0
 * @date 24/10/2023
 */
#ifndef CHOKBAR_AGENDA_CELLS_H
#define CHOKBAR_AGENDA_CELLS_H

// Structures

/**
 * @brief Structure représentant une cellule
 *
 * Une cellule est un élément d'une skiplist.
 * Elle contient une valeur, un niveau et un tableau de pointeurs vers les cellules suivantes (une par niveau).
 *
 * @see t_list
 */
typedef struct s_cell {
    /**
     * @brief La valeur de la cellule
     */
    int value;
    /**
     * @brief Le niveau de la cellule
     */
    int level;
    /**
     * @brief Tableau de pointeurs vers les cellules suivantes (une par niveau)
     */
    struct s_cell ** next;
} t_cell;

// Prototypes

t_cell * create_cell(int, int);

#endif //CHOKBAR_AGENDA_CELLS_H
