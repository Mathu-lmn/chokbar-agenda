/**
 * @file agenda_utils.c
 * @author Mathurin Lemoine / Thomas Fischer / Idir Nait Meddour
 * @brief Fichier contenant des utilitaires pour l'agenda
 * @version 1.0
 * @date 03/12/2023
 */

#import <stdio.h>
#import <string.h>
#include "agenda_utils.h"

// Pour éviter d'importer math.h
int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

int parsePositiveInt(char *str) {
    int res = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] >= '0' && str[i] <= '9')
            res = res * 10 + (str[i] - '0');
        else
            return -1;
    }
    return res;
}

struct Date parseDate(char* string) {
    if (string == NULL) return (struct Date) {0, 0, 0};

    struct Date date = {0, 0, 0};
    char* token = strtok(string, "/");

    int i = 0;
    while (token != NULL) {
        int value = parsePositiveInt(token);
        if (value == -1) {
            printf("Erreur lors de la lecture de la date.\n");
            return date;
        }
        switch (i) {
            case 0:
                date.jour = value;
                break;
            case 1:
                date.mois = value;
                break;
            case 2:
                date.annee = value;
                break;
            default:
                printf("Erreur lors de la lecture de la date.\n");
                return date;
        }
        token = strtok(NULL, "/");
        i++;
    }
    return date;
}

struct Heure parseHeureStruct(char* string, char* delimiter) {
    if (string == NULL) return (struct Heure) {0, 0};

    struct Heure heure = {0, 0};
    char* token = strtok(string, "h");

    int i = 0;
    while (token != NULL) {
        int value = parsePositiveInt(token);
        if (value == -1) {
            printf("Erreur lors de la lecture de l'heure.\n");
            return heure;
        }
        if (i == 0) {
            heure.heure = value;
            if (heure.heure < 0 || heure.heure > 23) {
                heure.heure = value % 24;
            }
        } else if (i == 1) {
            heure.minute = value;
        } else {
            printf("Erreur lors de la lecture de l'heure.\n");
            return heure;
        }
        token = strtok(NULL, "h");
        i++;
    }
    return heure;
}

struct Heure parseHeure(char* string) {
    return parseHeureStruct(string, "h");
}

struct Heure parseDuree(char* string) {
    return parseHeureStruct(string, ":");
}

void shuffle_list(char ** list, int size) {
    for (int i = 0; i < size; i++) {
        int j = rand() % size;
        char * tmp = list[i];
        list[i] = list[j];
        list[j] = tmp;
    }
}