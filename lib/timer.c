/**
 * @file timer.c
 * @author Nicolas Flasque
 * @brief Fichier contenant les fonctions de gestion de timer
 * @version 1.0
 * @date 11/10/2023
 */
#include <stdio.h>
#include <malloc.h>
#include "timer.h"

void startTimer()
{
    _timer._start = clock();
}

void stopTimer()
{
    _timer._end = clock();
    // compute the duration in milliseconds wrt CLOCKS_PER_SEC
    _timer._msecs = (1000.0 * (double) (_timer._end - _timer._start)) / CLOCKS_PER_SEC;
}

void displayTime()
{
    // display the time using getTimeAsString()
    printf("%s\n", getTimeAsString());
}

char *getTimeAsString()
{
    // return a string with the time in seconds and milliseconds
    // format : integers, with 3 digits, fill with 0
    // example : 001,123

    char *buffer = (char *)malloc(10*sizeof(char));
    sprintf(buffer, "[%g] %03d,%03d", _timer._msecs, (int)(_timer._msecs)/1000, (int)(_timer._msecs)%1000);

    return buffer;
}