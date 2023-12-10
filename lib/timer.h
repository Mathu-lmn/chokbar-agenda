/**
 * @file timer.h
 * @author Nicolas Flasque
 * @brief Fichier contenant les structures et prototypes de fonctions de gestion de timer
 * @version 1.0
 * @date 11/10/2023
 */
#ifndef TIMER_H
#define TIMER_H

#include <time.h>

/** @brief Structure pour stocker un timer */
typedef struct s_timer
{
    /** @brief Début du timer */
    clock_t _start;
    /** @brief Fin du timer */
    clock_t _end;
    /** @brief Durée en millisecondes */
    double _msecs;  // duration in milliseconds
} t_timer;

/** @brief Timer */
static t_timer _timer;

/** @brief Commence le timer */
void startTimer();

/** @brief Stoppe le timer et calcule la durée en millisecondes */
void stopTimer();

/** @brief Affiche le temps */
void displayTime();

/**
 * @brief Retourne une chaîne de caractères avec le temps en secondes et millisecondes
 * @return Une chaîne de caractères avec le temps en secondes et millisecondes
 */
char *getTimeAsString();

#endif //TIMER_H
