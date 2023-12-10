/**
 * @file main.c
 * @author Mathurin Lemoine / Thomas Fischer / Idir Nait Meddour
 * @brief Fichier contenant la fonction main
 * @version 1.0
 * @date 24/10/2023
 */
#include <stdio.h>
//#include <stdlib.h>
//#include "lib/lists.h"
//#include "lib/cells.h"
#include "agenda.h"
//#include "lib/timer.h"

/**
 * @brief Fonction principale du programme
 * @return La valeur de sortie du programme
 */
int main() {
    // DEBUG : print in debug mode
    setvbuf(stdout, NULL, _IONBF, 0);
    initData();

    // Initialisation du générateur de nombres aléatoires
    srand(time(NULL));


    t_agenda * agenda = fillAgenda(1000, 4);

    int choix = 0;

    do {
        printf("\nMenu:\n");
        printf("1. Rechercher un contact\n");
        printf("2. Afficher les rendez-vous d'un contact\n");
        printf("3. Creer un contact\n");
        printf("4. Creer un rendez-vous pour un contact\n");
        printf("5. Supprimer un rendez-vous\n");
        printf("6. Sauvegarder le fichier de tous les rendez-vous\n");
        printf("7. Charger un fichier de rendez-vous\n");
        printf("8. Fournir les temps de calcul pour une insertion de nouveau contact\n");
        printf("0. Quitter\n");

        // Saisie de l'option choisie (vérfication de la validité de la saisie)
        printf("Votre choix: ");
        while (scanf("%d", &choix) != 1) {
            printf("Veuillez saisir un nombre.\n");
            printf("Votre choix: ");
            while (getchar() != '\n');
        }

    } while (executeChoice(choix, &agenda) != 0);

//    contactInsertionTimer();

    return 0;
    /* Partie 1
    t_list * list = create_list(4);
    insert_cell(list, 1, 1);
    insert_cell(list, 2, 2);
    insert_cell(list, 1, 3);
    insert_cell(list, 3, 4);
    insert_cell(list, 1, 5);
    insert_cell(list, 2, 6);
    insert_cell(list, 1, 7);
    print_all_levels(*list);
     */
    /*
    PARTIE 2 : Complexité
    FILE *temps;
    if ((temps = fopen("temps.txt", "w")) == NULL) {
        printf("Erreur lors de l'ouverture du fichier temps.\n");
        exit(EXIT_FAILURE);
    }
    char format[] = "%d\t%s\t%s\n" ;

    int level = 7;
    printf("Veuillez choisir un nombre de niveau a tester entre 8 et 16 : ");
    scanf("%d", &level);
    while(level < 8 || level > 16){
        printf("Nombre de niveaux invalide. Choisissez un autre nombre : ");
        scanf("%d", &level);
    }

    for (int i = 7; i < level; i++) {
        char *time_lvl0;
        char *time_all_levels;
        time_t t1;
        srand(time(&t1));
        int nb_cell = (int) (pow(2, i) + 1e-9)-1;
        int number;
        t_list *new_list = create_better_list(i);
        startTimer();
        for (int j = 0; j < 10000; j++) {
            number = rand() % (nb_cell);
            classic_search(*new_list, number);
        }
        stopTimer();
        time_lvl0 = getTimeAsString();

        startTimer();
        for (int j = 0; j < 10000; j++) {
            number = rand() %(nb_cell);
            better_search(*new_list, number);
        }
        stopTimer();
        time_all_levels = getTimeAsString();
        fprintf(temps,format,i,time_lvl0, time_all_levels);
        for(int z = 0; z<i; z++){
            free(new_list->heads[z]);
            new_list->heads[z] = NULL;
        }
        free(new_list->heads);
        new_list->heads = NULL;
        free (new_list);
        new_list = NULL;
        free(time_lvl0);
        free(time_all_levels);
        time_lvl0 = NULL;
        time_all_levels = NULL;
    }
    fclose(temps);

    free(my_list);
    my_list = NULL;

    return 0;
    */
}
