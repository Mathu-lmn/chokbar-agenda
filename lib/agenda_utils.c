/**
 * @file agenda_utils.c
 * @author Mathurin Lemoine / Thomas Fischer / Idir Nait Meddour
 * @brief Fichier contenant des utilitaires pour l'agenda
 * @version 1.0
 * @date 03/12/2023
 */

#include <stdio.h>
#include <string.h>
// Debug
// #include <assert.h>
#include "agenda_utils.h"

/**
 * @brief Crée un contact
 * @param nom le nom du contact
 * @param prenom le prénom du contact
 * @return un pointeur vers le contact créé
 */
struct Contact* createContact(char* nom, char* prenom) {
    struct Contact* contact = malloc(sizeof(struct Contact));
    contact->nom = strdup(nom);
    contact->prenom = strdup(prenom);
    return contact;
}

/**
 * @brief Remplace les caractères problématiques dans un objet de RDV
 * @param object l'objet du RDV à nettoyer
 * @return un pointeur vers l'objet nettoyé
 */
char* sanitizeObject(char* object) {
    char* sanitized = strdup(object);
    for (int i = 0; i < strlen(object); i++) {
        if (object[i] == '\n') sanitized[i] = ' ';
        else if (object[i] == ',') sanitized[i] = ';';
        else sanitized[i] = object[i];
    }
    return sanitized;
}

/**
 * @brief Parse un entier positif d'un string
 * @param str le string à parser
 * @return l'entier positif parsé, -1 si erreur
 */
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

/**
 * @brief Parse une date d'un string
 * @param string le string à parser
 * @return la date parsée
 */
struct Date parseDate(char* string) {
    // Méthode la plus simple : on exige que le string soit de la forme "jj/mm/aaaa", sinon on renvoie une date nulle
    if (string == NULL || strlen(string) != 10) return (struct Date) {0, 0, 0};

    struct Date date = {0, 0, 0};
    char jour[3] = {string[0], string[1], '\0'};
    char mois[3] = {string[3], string[4], '\0'};
    char annee[5] = {string[6], string[7], string[8], string[9], '\0'};

    // Pour pouvoir itérer sur les tokens au lieu de faire un switch dans lequelle on se répèterait
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

/**
 * @brief Parse une heure d'un string
 * @param string le string à parser
 * @return un pointeur vers l'heure parsée
 */
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

/**
 * @brief Compare deux dates
 * @param ref la date réferentielle
 * @param other la date contre laquelle comparer
 * @return -1 si ref est antérieure other, 0 si ref == other, 1 si ref est ultérieure other
 */
int compareDate(struct Date ref, struct Date other) {
    if (ref.annee < other.annee) return -1;
    if (ref.annee > other.annee) return 1;
    if (ref.mois < other.mois) return -1;
    if (ref.mois > other.mois) return 1;
    if (ref.jour < other.jour) return -1;
    if (ref.jour > other.jour) return 1;
    return 0;
}

/**
 * @brief Compare deux heures
 * @param ref l'heure réferentielle
 * @param other l'heure contre laquelle comparer
 * @return -1 si ref est antérieure other, 0 si ref == other, 1 si ref est ultérieure other
 */
int compareHeure(struct Heure ref, struct Heure other) {
    if (ref.heure < other.heure) return -1;
    if (ref.heure > other.heure) return 1;
    if (ref.minute < other.minute) return -1;
    if (ref.minute > other.minute) return 1;
    return 0;
}

/**
 * @brief Une fonction de debug pour vérifier si une liste de RDV est triée
 * @param rdvHead la tête de la liste de RDV
 * @return 1 si la liste est triée, 0 sinon
 */
int isRDVListOrdered(p_rdv rdvHead) {
    if (rdvHead == NULL) return 1;
    p_rdv curr = rdvHead;
    while (curr->suivant != NULL) {
        if (compareDate(curr->date, curr->suivant->date) == 1) return 0;
        if (compareDate(curr->date, curr->suivant->date) == 0) {
            if (compareHeure(curr->heure, curr->suivant->heure) == 1) return 0;
        }
        curr = curr->suivant;
    }
    return 1;
}

/**
 * @brief Insère un RDV à la bonne place dans une liste de RDV triée
 * @param rdvHead la tête de la liste de RDV
 * @param newRDV le RDV à insérer
 */
void insertRDV(p_rdv* rdvHead, p_rdv newRDV) {
    // Si la liste est vide, le nouveau RDV devient la tête
    if (*rdvHead == NULL) {
        *rdvHead = newRDV;
        return;
    }

    p_rdv curr, prev;
    curr = prev = *rdvHead;
    while (curr != NULL) {
        if (compareDate(curr->date, newRDV->date) == 1) {
            if (curr == *rdvHead) {
                newRDV->suivant = *rdvHead;
                *rdvHead = newRDV;
                break;
            } else {
                prev->suivant = newRDV;
                newRDV->suivant = curr;
                break;
            }
        } else if (compareDate(curr->date, newRDV->date) == -1) {
            if (curr->suivant == NULL) {
                curr->suivant = newRDV;
                break;
            }
        } else {
            if (compareHeure(curr->heure, newRDV->heure) == 1) {
                if (curr == *rdvHead) {
                    newRDV->suivant = *rdvHead;
                    *rdvHead = newRDV;
                    break;
                } else {
                    prev->suivant = newRDV;
                    newRDV->suivant = curr;
                    break;
                }
            } else if (compareHeure(curr->heure, newRDV->heure) == -1) {
                if (curr->suivant == NULL) {
                    curr->suivant = newRDV;
                    break;
                }
            } else {
                // Si les deux RDV sont au même moment, alors peu importe l'ordre
                prev->suivant = newRDV;
                newRDV->suivant = curr;
                break;
            }
        }
        prev = curr;
        curr = curr->suivant;
    }

    // Debug
    // assert(isRDVListOrdered(*rdvHead));
}

/**
 * @brief Mélange une liste de strings
 * @param list la liste à mélanger
 * @param size la taille de la liste
 */
void shuffle_list(char ** list, int size) {
    // On génére un nombre aléatoire pour swap les éléments de la liste un à un
    for (int i = 0; i < size; i++) {
        int j = rand() % size;
        char * tmp = list[i];
        list[i] = list[j];
        list[j] = tmp;
    }
}

/**
 * @brief Compte le nombre d'occurences d'un caractère dans un string
 * @param str le string sur lequel compter
 * @param c le caractère à compter
 * @return le nombre d'occurences de c dans str
 */
int countChar(char * str, char c) {
    int count = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == c) count++;
    }
    return count;
}