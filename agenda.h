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

/** @brief La taille du tableau de noms */
#define NAME_FILE_SIZE 20000

/** @brief La taille du tableau de prénoms */
#define FIRST_NAME_FILE_SIZE 20000

/**
 * @brief Structure représentant une date
 *
 * Ses champs sont le jour, le mois et l'année
*/
struct Date {
    int jour;
    int mois;
    int annee;
};

/**
 * @brief Structure représentant une heure
 *
 * Ses champs sont l'heure et la minute
*/
struct Heure {
    int heure;
    int minute;
};

/**
 * @brief Structure représentant un contact
 *
 * Ses champs sont des chaines de caractères représentant le nom et le prénom
*/
typedef struct s_contact {
    /** @brief Le nom du contact */
    char *nom;
    /** @brief Le prénom du contact */
    char *prenom;
} t_contact, * p_contact;

/** @brief Structure représentant un rendez-vous */
typedef struct s_rdv {
    /** @brief La date du rendez-vous */
    struct Date date;

    /** @brief L'heure du rendez-vous */
    struct Heure heure;

    /** @brief La durée du rendez-vous */
    struct Heure duree;

    /** @brief L'objet du rendez-vous */
    char * objet;

    /** @brief Le pointeur vers le rendez-vous suivant */
    struct s_rdv * suivant;

    /** @brief L'identifiant du rendez-vous */
    unsigned int id;
} t_rdv, * p_rdv;

/** @brief Structure représentant une cellule de l'agenda */
typedef struct s_agenda_cell {
    /** @brief Le contact de la cellule */
    t_contact contact;

    /** @brief Le niveau de la cellule dans la skiplist */
    int level;

    /** @brief Le pointeur vers le rendez-vous */
    t_rdv *rdv;

    /** @brief Un tableau de pointeurs vers les cellules suivantes (une par niveau) */
    struct s_agenda_cell** tabNext;
} t_agenda_cell, * p_agenda_cell;

/** @brief Structure représentant l'agenda */
typedef struct s_agenda {
    /** @brief Un tableau de pointeurs vers les têtes de liste (une par niveau) */
    p_agenda_cell * heads;

    /** @brief Le nombre maximum de niveaux */
    int nbLevels;
} t_agenda;

/**
 * @brief Fonction d'entrée de chaînes de caractères
 * @return La chaîne de caractères entrée
 */
char *scanString();

/**
 * @brief Crée un nouvel agenda vide
 * @param levels Le nombre de niveaux de la skiplist
 * @return Un pointeur vers le nouvel agenda créé
 */
t_agenda *createAgenda(int levels);

/**
 * @brief Remplit l'agenda avec des données initiales
 * @see initData()
 * @param numberOfContacts Le nombre initial de contacts
 * @param levels Le nombre de niveaux de la skiplist
 * @return Un pointeur vers l'agenda rempli
 */
t_agenda *fillAgenda(int numberOfContacts, int levels);

/**
 * @brief Ajoute un contact à l'agenda
 * @param agenda L'agenda auquel ajouter le contact
 * @param agendaEntry La cellule contenant les informations du contact
 */
void addContactToAgenda(t_agenda *agenda, t_agenda_cell *agendaEntry);

/**
 * @brief Crée une nouvelle cellule d'agenda
 * @param contact Les informations du contact
 * @param levels Le nombre de niveaux de la skiplist (pour la création du tableau de pointeurs)
 * @return Un pointeur vers la nouvelle cellule créée
 */
t_agenda_cell *createAgendaCell(t_contact contact, int levels);

/**
 * @brief Exécute une option du menu principal
 * @param choice L'option choisie
 * @param agenda Un pointeur vers l'agenda
 * @return 1 si l'option est exécutée avec succès, 0 sinon
 */
int executeChoice(int choice, t_agenda ** agenda);

/**
 * @brief Recherche un contact dans l'agenda
 * @param agenda L'agenda dans lequel effectuer la recherche
 * @param nom Le nom du contact à rechercher
 * @param prenom Le prénom du contact à rechercher
 * @return Un pointeur vers la cellule du contact trouvé, NULL sinon
 */
t_agenda_cell *searchContact(t_agenda * agenda, char * nom, char * prenom);

/**
 * @brief Fonction de recherche de tous les contacts dont le nom commence par une chaîne de caractères
 * @param agenda L'agenda dans lequel effectuer la recherche
 */
void startSearch(t_agenda agenda);

/**
 * @brief Fonction d'ajout d'un nouveau rendez-vous pour un contact
 * @param agenda
 */
void addNewRdv(t_agenda *agenda);

/**
 * @brief Fonction de suppression d'un rendez-vous pour un contact
 * @param agenda
 */
void deleteRdv(t_agenda *agenda);

/**
 * @brief Fonction affichant les rendez-vous d'un contact
 * @param agenda
 */
void displayContactRdv(t_agenda *agenda);

/**
 * @brief Crée un nouveau contact et l'ajoute à l'agenda
 * @param agenda L'agenda auquel ajouter le nouveau contact
 */
void createNewContact(t_agenda *agenda);

/** @brief Fonction de calcul de l'efficacité des fonctions sur une skiplist ou liste chainées
 * @see timer.h
*/
void contactInsertionTimer();

/** @brief Initialise les données des fichiers csv contenant les noms et prénoms */
void initData();

/** @brief Fonction pour obtenir un identifiant unique de rendez-vous */
unsigned int getNextId();

/** @brief Fonction pour ajouter un contact à un agenda sans utiliser les avantages d'une skiplist */
void addContactToAgendaLevel0(t_agenda *agenda, t_agenda_cell *agendaEntry);

/** @brief Fonction pour rechercher un contact dans un agenda sans utiliser les avantages d'une skiplist */
void searchContactLevel0(t_agenda * agenda, char * nom, char * prenom);

/**
 * @brief Fonction qui libère la mémoire allouée pour une liste chainée de rendez-vous
 * @param firstRDV
 */
void freeRDVLLC(t_rdv * firstRDV);

/**
 * @brief Fonction qui libère la mémoire allouée pour une cellule d'agenda
 * @see freeRDVLLC()
 * @param cell
 */
void freeAgendaCell(p_agenda_cell cell);

/**
 * @brief Fonction qui libère la mémoire allouée pour un agenda
 * @see freeAgendaCell()
 * @param agenda
 */
void freeAgenda(t_agenda* agenda);

/**
 * @brief Fonction qui sauvegarde un agenda dans un fichier
 * @see loadAgendaFromFile()
 * @param agenda
 */
void saveAgendaToFile(t_agenda* agenda);

/**
 * @brief Fonction qui charge un agenda depuis un fichier
 * @param agenda
 */
void loadAgendaFromFile(t_agenda **agenda);
#endif //CHOKBAR_AGENDA_AGENDA_H
