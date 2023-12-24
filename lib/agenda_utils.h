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

/**
 * @brief Crée un contact
 * @param nom le nom du contact
 * @param prenom le prénom du contact
 * @return un pointeur vers le contact créé
 */
p_contact createContact(char* nom, char* prenom);

/**
 * @brief Remplace les caractères problématiques dans un objet de RDV
 * @param object l'objet du RDV à nettoyer
 * @return un pointeur vers l'objet nettoyé
 */
char* sanitizeObject(char* object);

/**
 * @brief Parse un entier positif d'un string
 * @param str le string à parser
 * @return l'entier positif parsé, -1 si erreur
 */
int parsePositiveInt(char* str);

/**
 * @brief Parse une date d'un string
 * @param string le string à parser
 * @return la date parsée
 */
struct Date parseDate(char* string);

/**
 * @brief Parse une heure d'un string
 * @param string le string à parser
 * @return un pointeur vers l'heure parsée
 */
struct Heure* parseHeureStruct(char* string);

/**
 * @brief Insère un RDV à la bonne place dans une liste de RDV triée
 * @param rdvHead la tête de la liste de RDV
 * @param newRDV le RDV à insérer
 */
void insertRDV(p_rdv*, p_rdv);

/*
 * @brief Mélange une liste de strings
 * @param list la liste à mélanger
 * @param size la taille de la liste
void shuffle_list(char** list, int size);
 */ // Commentée car les fichier csv sont maintenant d'ordre aléatoire

/**
 * @brief Compte le nombre d'occurences d'un caractère dans un string
 * @param str le string sur lequel compter
 * @param c le caractère à compter
 * @return le nombre d'occurences de c dans str
 */
int countChar(char*, char);

#endif //CHOKBAR_AGENDA_AGENDA_UTILS_H
