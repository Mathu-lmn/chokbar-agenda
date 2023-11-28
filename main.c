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
#include "agenda.h"
/**
 * @brief Fonction principale du programme
 * @return La valeur de sortie du programme
 */
int main() {
    t_agenda * agenda = create_agenda(4);
    agenda = fillAgenda(1000, 4);
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

        // Saisie de l'option choisie
        printf("Votre choix: ");
        scanf("%d", &choix);

    } while (executeChoice(choix, agenda) != 0);

return 0;
}
