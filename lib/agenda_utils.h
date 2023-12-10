/**
 * @file agenda_utils.h
 * @author Mathurin Lemoine / Thomas Fischer / Idir Nait Meddour
 * @brief Fichier contenant les structures et prototypes de fonctions utilitaires de l'agenda
 * @version 1.0
 * @date 26/11/2023
 */
#ifndef CHOKBAR_AGENDA_AGENDA_UTILS_H
#define CHOKBAR_AGENDA_AGENDA_UTILS_H

#include "../agenda.h"

struct Contact* createContact(char* nom, char* prenom);
char* sanitizeObject(char* object);

int parsePositiveInt(char *str);
struct Date parseDate(char* string);
struct Heure* parseHeureStruct(char* string);

void shuffle_list(char ** list, int size);

#endif //CHOKBAR_AGENDA_AGENDA_UTILS_H
