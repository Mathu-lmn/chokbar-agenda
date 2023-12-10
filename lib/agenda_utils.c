/**
 * @file agenda_utils.c
 * @author Mathurin Lemoine / Thomas Fischer / Idir Nait Meddour
 * @brief Fichier contenant des utilitaires pour l'agenda
 * @version 1.0
 * @date 03/12/2023
 */

#include <stdio.h>
#include <string.h>
#include "agenda_utils.h"

struct Contact* createContact(char* nom, char* prenom) {
    struct Contact* contact = malloc(sizeof(struct Contact));
    contact->nom = strdup(nom);
    contact->prenom = strdup(prenom);
    return contact;
}

char* sanitizeObject(char* object) {
    char* sanitized = strdup(object);
    for (int i = 0; i < strlen(object); i++) {
        if (object[i] == '\n') sanitized[i] = ' ';
        else if (object[i] == ',') sanitized[i] = ';';
        else sanitized[i] = object[i];
    }
    return sanitized;
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
    if (string == NULL || strlen(string) != 10) return (struct Date) {0, 0, 0};

    struct Date date = {0, 0, 0};
    char jour[3] = {string[0], string[1], '\0'};
    char mois[3] = {string[3], string[4], '\0'};
    char annee[5] = {string[6], string[7], string[8], string[9], '\0'};

    char* tokens[] = {jour, mois, annee};

    for (int i = 0; i < 3; i++) {
        int value = parsePositiveInt(tokens[i]);
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
    }
    return date;
}

struct Heure* parseHeureStruct(char *string) {
    if (string == NULL || strlen(string) != 5) {
        struct Heure* h = malloc(sizeof(struct Heure));
        h->heure = 0;
        h->minute = 0;
        return h;
    }

    struct Heure* heure = malloc(sizeof(struct Heure));
    heure->heure = 0;
    heure->minute = 0;
    char token[] = {string[0], string[1], '\0'};

    for (int i = 0; i < 2; i++) {
        int value = parsePositiveInt(token);
        if (value == -1) {
            printf("Erreur lors de la lecture de l'heure.\n");
            return heure;
        }
        if (i == 0) {
            heure->heure = value;
            if (heure->heure < 0 || heure->heure > 23) {
                heure->heure = value % 24;
            }
        } else {
            heure->minute = value;
        }
        token[0] = string[3];
        token[1] = string[4];
    }
    return heure;
}

void shuffle_list(char ** list, int size) {
    for (int i = 0; i < size; i++) {
        int j = rand() % size;
        char * tmp = list[i];
        list[i] = list[j];
        list[j] = tmp;
    }
}

int countChar(char * str, char c) {
    int count = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == c) count++;
    }
    return count;
}