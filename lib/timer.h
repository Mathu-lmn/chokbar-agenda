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

// timer structure
typedef struct s_timer
{
    clock_t _start; // start time
    clock_t _end;   // end time
    double _msecs;  // duration in milliseconds
} t_timer;

static t_timer _timer;

/**
 * @brief Commence le timer
 */
void startTimer();

/**
 * @brief Stoppe le timer et calcule la durée en millisecondes
 */
void stopTimer();

/**
 * @brief Affiche le temps
 */
void displayTime();

/**
 * @brief Retourne une chaîne de caractères avec le temps en secondes et millisecondes
 * @return Une chaîne de caractères avec le temps en secondes et millisecondes
 */
char *getTimeAsString();

#endif //TIMER_H
