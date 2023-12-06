//
// Created by Idir on 05/12/2023.
//

#ifndef CHOKBAR_AGENDA_AGENDA_UTILS_H
#define CHOKBAR_AGENDA_AGENDA_UTILS_H

#include "../agenda.h"

int mod(int a, int b);

int parsePositiveInt(char *str);
struct Date parseDate(char* string);
struct Heure parseHeure(char* string);
struct Heure parseDuree(char* string);

void shuffle_list(char ** list, int size);

#endif //CHOKBAR_AGENDA_AGENDA_UTILS_H
