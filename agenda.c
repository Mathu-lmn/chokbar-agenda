/**
 * @file agenda.c
 * @author Mathurin Lemoine / Thomas Fischer / Idir Nait Meddour
 * @brief Fichier contenant les fonctions de l'agenda
 * @version 1.0
 * @date 26/11/2023
 */
#include "agenda.h"

// Fonction pour saisir une chaîne de caractères dynamique
char *scanString(void) {
    char buffer[100];
    scanf("%99s", buffer);  // Limiter la saisie pour éviter les débordements de mémoire
    char *str = malloc(strlen(buffer) + 1);
    strcpy(str, buffer);
    return str;
}
