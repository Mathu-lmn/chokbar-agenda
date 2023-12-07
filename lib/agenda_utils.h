//
// Created by Idir on 05/12/2023.
//

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
