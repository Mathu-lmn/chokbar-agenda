/**
 * @file main.c
 * @author Mathurin Lemoine / Thomas Fischer / Idir Nait Meddour
 * @brief Fichier contenant la fonction main
 * @version 1.0
 * @date 24/10/2023
 */

#include <stdio.h>
#include <stdlib.h>
#include "lib/lists.h"
#include "lib/cells.h"

/**
 * @fn int main()
 * @brief Fonction principale du programme
 * @return La valeur de sortie du programme
 */

int main() {
    t_list* my_list = create_list(3);

    insert_cell(my_list, 1, 5);
    print_all_levels(*my_list);
    printf("\n");
    insert_cell(my_list, 3, 10);
    print_all_levels(*my_list);
    printf("\n");
    insert_cell(my_list, 1, 3);
    print_all_levels(*my_list);
    printf("\n");
    insert_cell(my_list, 1, 12);
    print_all_levels(*my_list);
    printf("\n");
    insert_cell(my_list, 2, 15);
    print_all_levels(*my_list);
    printf("\n");

    return 0;
}


