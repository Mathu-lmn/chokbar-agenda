/**
 * @file agenda.h
 * @author Mathurin Lemoine / Thomas Fischer / Idir Nait Meddour
 * @brief Fichier contenant les structures et prototypes de fonctions de l'agenda
 * @version 1.0
 * @date 26/11/2023
 */
#ifndef CHOKBAR_AGENDA_AGENDA_H
#define CHOKBAR_AGENDA_AGENDA_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/lists.h"

// Structure pour stocker une date
struct Date {
    int jour;
    int mois;
    int annee;
};

// Structure pour stocker l'heure
struct Heure {
    int heure;
    int minute;
};

// Structure pour stocker un contact
struct Contact {
    char *nom;
    char *prenom;
};

// Structure pour stocker un rendez-vous
typedef struct s_rdv {
    struct Date date;
    struct Heure heure;
    struct Heure duree;
    char * objet;
    struct s_rdv * suivant;
} t_rdv, * p_rdv;

// Structure pour stocker une entrée de l'agenda
typedef struct s_agenda_cell {
    struct Contact contact;
    int level;
    t_rdv *rdv;
    struct s_agenda_cell ** tab_next;
} t_agenda_cell, * p_agenda_cell;

typedef struct s_agenda {
    p_agenda_cell * heads;
    int nb_levels;
} t_agenda;

char *scanString(void);
t_agenda * create_agenda(int);
t_agenda * fillAgenda(int, int);
void add_contact_to_agenda(t_agenda *, t_agenda_cell *);
t_agenda_cell * create_agenda_cell(struct Contact, int);
int executeChoice(int, t_agenda *);
t_agenda_cell * search_contact(t_agenda *, char *, char *);
void createNewContact(t_agenda *);
#endif //CHOKBAR_AGENDA_AGENDA_H
