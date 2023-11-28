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
        printf("Agenda non initialise.\n");
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

// Rechercher un contact, et proposer une complétion automatique à partir de la 3ème lettre entrée pour le nom (il faudra donc faire la saisie du nom de recherche caractère par caractère) ;
void startSearch() {

}

t_agenda_cell * search_contact(t_agenda * agenda, char * nom, char * prenom) {
    if (agenda == NULL) {
        printf("Agenda non initialise.\n");
        return NULL;
    }

    t_agenda_cell * curr = agenda->heads[3];
    int i = 3;
    while (curr != NULL) {
        if (i < 0) {
            return NULL;
        }
        if (strcmp(curr->contact.nom, nom) == 0 && strcmp(curr->contact.prenom, prenom) == 0) {
            return curr;
        }
        // si on a dépassé le nom recherché, on arrête la recherche
        if (strcmp(curr->contact.nom, nom) > 0) {
            return NULL;
        }
        // Si le prochain contact au niveau i est NULL ou plus grand que le nom recherché, on descend d'un niveau
        if (curr->tab_next[i] == NULL || strcmp(curr->tab_next[i]->contact.nom, nom) > 0) {
            i--;
        } else {
            curr = curr->tab_next[i];
        }
    }
    return NULL;
}

void displayContactRdv(t_agenda *agenda) {
    printf("Nom du contact: ");
    char *nom = scanString();
    printf("Prenom du contact: ");
    char *prenom = scanString();

    t_agenda_cell * contact = search_contact(agenda, nom, prenom);
    if (contact == NULL) {
        printf("Contact non trouve.\n");
        return;
    }
    if (contact->rdv == NULL) {
        printf("Aucun rendez-vous pour ce contact.\n");
        return;
    }
    printf("Rendez-vous de %s %s:\n", contact->contact.prenom, contact->contact.nom);
    t_rdv *rdv = contact->rdv;
    while (rdv != NULL) {
        printf("Date: %d/%d/%d\t", rdv->date.jour, rdv->date.mois, rdv->date.annee);
        printf("Heure: %d:%d\t", rdv->heure.heure, rdv->heure.minute);
        printf("Duree: %d:%d\t", rdv->duree.heure, rdv->duree.minute);
        printf("Objet: %s\n", rdv->objet);
        rdv = rdv->suivant;
    }
}

int executeChoice(int choice, t_agenda * agenda) {
    // Traitement de l'option choisie
    switch (choice) {
        case 1:
            startSearch();
            break;
        case 2:
            displayContactRdv(agenda);
            break;
        case 3:
            // Créer un contact
            // ...
            break;

        case 4:
            // Créer un rendez-vous pour un contact
            // ...
            break;

        case 5:
            // Supprimer un rendez-vous
            // ...
            break;

        case 6:
            // Sauvegarder le fichier de tous les rendez-vous
            // ...
            break;

        case 7:
            // Charger un fichier de rendez-vous
            // ...
            break;

        case 8:
            // Fournir les temps de calcul pour une insertion de nouveau contact
            // ...
            break;

        case 0:
            return 0;

        default:
            printf("Option invalide. Veuillez reessayer.\n");
            break;
    }
    return 1;
}