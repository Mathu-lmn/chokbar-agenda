/**
 * @file agenda.c
 * @author Mathurin Lemoine / Thomas Fischer / Idir Nait Meddour
 * @brief Fichier contenant les fonctions de l'agenda
 * @version 1.0
 * @date 26/11/2023
 */
#include "agenda.h"

// Fonction pour saisir une chaîne de caractères dynamique
char *scanString(void) {
    char buffer[100];
    scanf("%99s", buffer);  // Limiter la saisie pour éviter les débordements de mémoire
    char *str = malloc(strlen(buffer) + 1);
    strcpy(str, buffer);
    return str;
}

t_agenda *create_agenda(int levels) {
    t_agenda *agenda = (t_agenda *) malloc(sizeof(t_agenda));
    agenda->nb_levels = levels;
    agenda->heads = (t_agenda_cell **) malloc(levels * sizeof(t_agenda_cell *));
    for (int i = 0; i < levels; i++) {
        agenda->heads[i] = NULL;
    }

    return agenda;
}

t_agenda *fillAgenda(int number_of_contacts, int levels) {
    t_agenda *agenda = create_agenda(levels);
    FILE *prenom_file;
    if ((prenom_file = fopen("..\\data\\prenoms.csv", "r")) == NULL) {
        printf("Erreur lors de l'ouverture du fichier prenoms.\n");
        exit(EXIT_FAILURE);
    }
    FILE *nom_file;
    if ((nom_file = fopen("..\\data\\noms.csv", "r")) == NULL) {
        printf("Erreur lors de l'ouverture du fichier noms.\n");
        exit(EXIT_FAILURE);
    }
    int i = 0;

    if (prenom_file == NULL || nom_file == NULL) {
        printf("Erreur lors de l'ouverture des fichiers.\n");
        exit(EXIT_FAILURE);
    }

    while (i < number_of_contacts) {
        char prenom[100];
        char nom[100];
        fgets(prenom, 100, prenom_file);
        fgets(nom, 100, nom_file);
        // Suppression du retour à la ligne
        prenom[strlen(prenom) - 1] = '\0';
        nom[strlen(nom) - 1] = '\0';
        struct Contact contact = {strdup(nom), strdup(prenom)};
        t_agenda_cell *agenda_entry = create_agenda_cell(contact, levels);
        add_contact_to_agenda(agenda, agenda_entry);
        i++;
    }

    return agenda;
}

t_agenda_cell *create_agenda_cell(struct Contact contact, int levels) {
    t_agenda_cell *new_cell = (t_agenda_cell *) malloc(sizeof(t_agenda_cell));
    new_cell->contact = contact;
    new_cell->level = levels;
    new_cell->tab_next = (t_agenda_cell **) malloc((levels + 1) * sizeof(t_agenda_cell *));
    for (int i = 0; i < levels; i++) {
        new_cell->tab_next[i] = NULL;
    }

    return new_cell;
}

void add_contact_to_agenda(t_agenda *agenda, t_agenda_cell *agenda_entry) {
    if (agenda == NULL) {
        printf("Agenda non initialisé.\n");
        return;
    }

    if (agenda->heads[0] == NULL) {
        for (int i = 0; i < agenda->nb_levels; i++) {
            agenda->heads[i] = agenda_entry;
        }
        return;
    }
    for (int i = 0; i < agenda->nb_levels; i++) {
        if (agenda->heads[i] == NULL || strcmp(agenda_entry->contact.nom, agenda->heads[i]->contact.nom) < 0) {
            agenda_entry->tab_next[i] = agenda->heads[i];
            agenda->heads[i] = agenda_entry;
        } else {
            t_agenda_cell *tmp = agenda->heads[i];
            t_agenda_cell *ptmp = NULL;

            while (tmp != NULL && strcmp(tmp->contact.nom, agenda_entry->contact.nom) < 0) {
                ptmp = tmp;
                tmp = tmp->tab_next[i];
            }
            // TODO : Si on veut insérer Mr GABIN et qu'il y a Mr GALLION sur le dernier niveau, on insère Mr GABIN avant Mr GALLION et on enlève Mr GALLION du dernier niveau
            if (tmp == NULL) {
                if (i == 0 ||
                    (i == 1 && ptmp->contact.nom[0] == agenda_entry->contact.nom[0] && ptmp->contact.nom[1] == agenda_entry->contact.nom[1] && ptmp->contact.nom[2] != agenda_entry->contact.nom[2]) ||
                    (i == 2 && ptmp->contact.nom[0] == agenda_entry->contact.nom[0] && ptmp->contact.nom[1] != agenda_entry->contact.nom[1]) ||
                    (i == 3 && ptmp->contact.nom[0] != agenda_entry->contact.nom[0])) {
                    ptmp->tab_next[i] = agenda_entry;
                    agenda_entry->tab_next[i] = tmp;
                }
            } else {
                if (i == 0 ||
                    (i == 1 && tmp->contact.nom[0] == agenda_entry->contact.nom[0] && tmp->contact.nom[1] == agenda_entry->contact.nom[1] && tmp->contact.nom[2] != agenda_entry->contact.nom[2]) ||
                    (i == 2 && tmp->contact.nom[0] == agenda_entry->contact.nom[0] && tmp->contact.nom[1] != agenda_entry->contact.nom[1]) ||
                    (i == 3 && tmp->contact.nom[0] != agenda_entry->contact.nom[0])) {
                    ptmp->tab_next[i] = agenda_entry;
                    agenda_entry->tab_next[i] = tmp;
                }
            }
        }
    }
}
