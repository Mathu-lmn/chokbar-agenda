/**
 * @file agenda.c
 * @author Mathurin Lemoine / Thomas Fischer / Idir Nait Meddour
 * @brief Fichier contenant les fonctions de l'agenda
 * @version 1.0
 * @date 26/11/2023
 */
#include "agenda.h"

// À ne pas toucher directement, utilisez plutôt la fonction get_next_id() en-dessous
unsigned int gID = 0;

unsigned int get_next_id() {
    return gID++;
}

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

void shuffle_list(char ** list, int size) {
    for (int i = 0; i < size; i++) {
        int j = rand() % size;
        char * tmp = list[i];
        list[i] = list[j];
        list[j] = tmp;
    }
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

    if (prenom_file == NULL || nom_file == NULL) {
        printf("Erreur lors de l'ouverture des fichiers.\n");
        exit(EXIT_FAILURE);
    }
    printf("Chargement de l'agenda...\n");

    char * all_prenoms[FIRST_NAME_FILE_SIZE];
    char * all_noms[NAME_FILE_SIZE];
    size_t len = 0;

    // don't use getline because it's not available on Windows
    char line[100];
    int i = 0;
    while (i < FIRST_NAME_FILE_SIZE && (fgets(line, 100, prenom_file)) != NULL) {
        all_prenoms[i] = strdup(line);
        all_prenoms[i][strlen(all_prenoms[i]) - 1] = '\0';
        i++;
    }
    i = 0;
    while (i < NAME_FILE_SIZE && (fgets(line, 100, nom_file)) != NULL) {
        all_noms[i] = strdup(line);
        all_noms[i][strlen(all_noms[i]) - 1] = '\0';
        i++;
    }


    fclose(prenom_file);
    fclose(nom_file);

    shuffle_list(all_prenoms, FIRST_NAME_FILE_SIZE);
    shuffle_list(all_noms, NAME_FILE_SIZE);


    for (i = 0; i < number_of_contacts; i++) {
        struct Contact contact = {strdup(all_noms[i]), strdup(all_prenoms[i])};
//        printf("DEBUG : Contact %d : %s %s\n", i, contact.prenom, contact.nom);
        t_agenda_cell *agenda_entry = create_agenda_cell(contact, levels);
        add_contact_to_agenda(agenda, agenda_entry);
    }
    return agenda;
}

t_agenda_cell *create_agenda_cell(struct Contact contact, int levels) {
    t_agenda_cell *new_cell = (t_agenda_cell *) malloc(sizeof(t_agenda_cell));
    new_cell->contact = contact;
    new_cell->level = 0;
    new_cell->rdv = NULL;
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
            agenda_entry->level = i;
        }
        // printf("DEBUG : Found level: %d for %s %s\n", i, agenda_entry->contact.prenom, agenda_entry->contact.nom);
        return;
    }
    for (int i = 0; i < agenda->nb_levels; i++) {
        if (agenda->heads[i] == NULL || strcmp(agenda_entry->contact.nom, agenda->heads[i]->contact.nom) < 0) {
            agenda_entry->tab_next[i] = agenda->heads[i];
            agenda->heads[i] = agenda_entry;
            agenda_entry->level = i;
        } else {
            t_agenda_cell *tmp = agenda->heads[i];
            t_agenda_cell *ptmp = NULL;

            while (tmp != NULL && strcmp(tmp->contact.nom, agenda_entry->contact.nom) < 0) {
                ptmp = tmp;
                tmp = tmp->tab_next[i];
            }
            if (i == 0 ||
                (i == 1 && (!(ptmp->contact.nom[0] == agenda_entry->contact.nom[0] && ptmp->contact.nom[1] == agenda_entry->contact.nom[1] && ptmp->contact.nom[2] == agenda_entry->contact.nom[2]))) ||
                (i == 2 && (!(ptmp->contact.nom[0] == agenda_entry->contact.nom[0] && ptmp->contact.nom[1] == agenda_entry->contact.nom[1]))) ||
                (i == 3 && ptmp->contact.nom[0] != agenda_entry->contact.nom[0])) {
                // printf("DEBUG : Found level: %d for %s %s (compared to %s %s)\n", i, agenda_entry->contact.prenom, agenda_entry->contact.nom, ptmp->contact.prenom, ptmp->contact.nom);
                ptmp->tab_next[i] = agenda_entry;
                agenda_entry->tab_next[i] = tmp;
                agenda_entry->level = i;
            }
        }
    }
    for (int i = 0; i < agenda->nb_levels; i++) {
        if (agenda_entry->tab_next[i] != NULL) {
            if ((i == 1 && (agenda_entry->tab_next[i]->contact.nom[0] == agenda_entry->contact.nom[0] && agenda_entry->tab_next[i]->contact.nom[1] == agenda_entry->contact.nom[1] && agenda_entry->tab_next[i]->contact.nom[2] == agenda_entry->contact.nom[2])) ||
                (i == 2 && (agenda_entry->tab_next[i]->contact.nom[0] == agenda_entry->contact.nom[0] && agenda_entry->tab_next[i]->contact.nom[1] == agenda_entry->contact.nom[1])) ||
                (i == 3 && agenda_entry->tab_next[i]->contact.nom[0] == agenda_entry->contact.nom[0])) {
                // printf("DEBUG : Removing level: %d for %s %s\n", i, agenda_entry->tab_next[i]->contact.prenom, agenda_entry->tab_next[i]->contact.nom);
                agenda_entry->tab_next[i] = agenda_entry->tab_next[i]->tab_next[i];
                if (agenda_entry->tab_next[i] != NULL) {
                    agenda_entry->tab_next[i]->level = i;
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
    while (strlen(nom) < 3) {
        printf("Le nom doit faire au moins 3 caracteres.\n");
        printf("Nom du contact: ");
        nom = scanString();
    }

    printf("Prenom du contact: ");
    char *prenom = scanString();
    while (strlen(prenom) < 3) {
        printf("Le prenom doit faire au moins 3 caracteres.\n");
        printf("Prenom du contact: ");
        prenom = scanString();
    }

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
        printf("ID: %d\t", rdv->id);
        printf("Date: %d/%d/%d\t", rdv->date.jour, rdv->date.mois, rdv->date.annee);
        printf("Heure: %d:%d\t", rdv->heure.heure, rdv->heure.minute);
        printf("Duree: %d:%d\t", rdv->duree.heure, rdv->duree.minute);
        printf("Objet: %s\n", rdv->objet);
        rdv = rdv->suivant;
    }
}

void createNewContact(t_agenda *agenda) {
    printf("Nom du contact: ");
    char *nom = scanString();
    while (strlen(nom) < 3) {
        printf("Le nom doit faire au moins 3 caracteres.\n");
        printf("Nom du contact: ");
        nom = scanString();
    }

    printf("Prenom du contact: ");
    char *prenom = scanString();
    while (strlen(prenom) < 3) {
        printf("Le prenom doit faire au moins 3 caracteres.\n");
        printf("Prenom du contact: ");
        prenom = scanString();
    }

    struct Contact contact = {nom, prenom};
    t_agenda_cell *agenda_entry = create_agenda_cell(contact, 4);
    add_contact_to_agenda(agenda, agenda_entry);
}

void addNewRdv(t_agenda *agenda) {
    printf("Nom du contact: ");
    char *nom = scanString();
    while (strlen(nom) < 3) {
        printf("Le nom doit faire au moins 3 caracteres.\n");
        printf("Nom du contact: ");
        nom = scanString();
    }

    printf("Prenom du contact: ");
    char *prenom = scanString();
    while (strlen(prenom) < 3) {
        printf("Le prenom doit faire au moins 3 caracteres.\n");
        printf("Prenom du contact: ");
        prenom = scanString();
    }

    t_agenda_cell *contact = search_contact(agenda, nom, prenom);
    if (contact == NULL) {
        printf("Contact non trouve. Ajout au carnet d'adresses.\n");
        t_agenda_cell *agenda_entry = create_agenda_cell((struct Contact) {nom, prenom}, 4);
        add_contact_to_agenda(agenda, agenda_entry);
        contact = search_contact(agenda, nom, prenom);
        if (contact == NULL) {
            printf("Erreur lors de l'ajout du contact.\n");
            return;
        }
    }

    printf("Date du rendez-vous (jj/mm/aaaa): ");
    int jour, mois, annee;
    while (scanf("%d/%d/%d", &jour, &mois, &annee) != 3) {
        printf("Veuillez saisir une date valide.\n");
        printf("Date du rendez-vous (jj/mm/aaaa): ");
        while ((getchar()) != '\n');
    }

    printf("Heure du rendez-vous (hh:mm): ");
    int heure, minute;
    while (scanf("%d:%d", &heure, &minute) != 2) {
        printf("Veuillez saisir une heure valide.\n");
        printf("Heure du rendez-vous (hh:mm): ");
        while ((getchar()) != '\n');
    }

    printf("Duree du rendez-vous (hh:mm): ");
    int duree_heure, duree_minute;
    while (scanf("%d:%d", &duree_heure, &duree_minute) != 2) {
        printf("Veuillez saisir une duree valide.\n");
        printf("Duree du rendez-vous (hh:mm): ");
        while ((getchar()) != '\n');
    }

    printf("Objet du rendez-vous: ");
    char *objet = scanString();
    while (strlen(objet) < 3) {
        printf("L'objet doit faire au moins 3 caracteres.\n");
        printf("Objet du rendez-vous: ");
        objet = scanString();
    }

    struct Date date = {jour, mois, annee};
    struct Heure heure_struct = {heure, minute};
    struct Heure duree = {duree_heure, duree_minute};
    t_rdv *rdv = (t_rdv *) malloc(sizeof(t_rdv));
    rdv->date = date;
    rdv->heure = heure_struct;
    rdv->duree = duree;
    rdv->objet = objet;
    rdv->suivant = NULL;
    rdv->id = get_next_id();
    if (contact->rdv == NULL) {
        contact->rdv = rdv;
    } else {
        t_rdv *tmp = contact->rdv;
        // on se place à la fin de la liste
        // TODO : insérer le rendez-vous à la bonne place (par date et heure), créer une fonction pour comparer deux rendez-vous (lisibilité)
        while (tmp->suivant != NULL) {
            tmp = tmp->suivant;
        }
        tmp->suivant = rdv;
    }
    // clear buffer
    while ((getchar()) != '\n');
    printf("RDV ajoute. ID : %d\n", rdv->id);
}

void deleteRdv(t_agenda *agenda) {
    // Approche naïve, où on avance sur le niveau 0
    // Note : Optimisable ? Mais je ne pense pas vu que la liste n'est pas triée par RDV.

    p_agenda_cell cur = agenda->heads[0];
    if (cur == NULL) {
        printf("Agenda vide.\n");
        return;
    }

    printf("ID du rendez-vous : ");
    int id = 0;
    while (scanf("%d", &id) != 1) {
        printf("Veuillez saisir un nombre.\n");
        printf("ID du rendez-vous : ");
        while (getchar() != '\n');
    }

    while (cur != NULL) {
        if (cur->rdv != NULL) {
            p_rdv rdv = cur->rdv;
            while (rdv != NULL) {
                if (rdv->id == id) {
                    cur->rdv = rdv->suivant;
                    free(rdv->objet);
                    free(rdv);
                    printf("Rendez-vous numero %d supprime.\n", id);
                    return;
                }
                rdv = rdv->suivant;
            }
        }

        cur = cur->tab_next[0];
    }

    printf("Rendez-vous numero %d non trouve.\n", id);
}

void debug_displayList(t_agenda *agenda) {
    // print all contacts
    printf("Liste des contacts:\n");
    for (int i = 0; i < agenda->nb_levels; i++) {
        t_agenda_cell *curr = agenda->heads[i];
        while (curr != NULL) {
            if (curr->contact.nom[0] != 'A') {
                printf("%s %s at level %d\n", curr->contact.prenom, curr->contact.nom, i);
            }
            curr = curr->tab_next[i];
        }
        printf("\n");
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
            createNewContact(agenda);
            break;
        case 4:
            addNewRdv(agenda);
            break;
        case 5:
            debug_displayList(agenda);
            break;
        case 6:
            // Sauvegarder le fichier de tous les rendez-vous
            // TODO : définir le format de sauvegarde (CSV ?)
            break;

        case 7:
            // Charger un fichier de rendez-vous
            // ...
            break;

        case 8:
            // Fournir les temps de calcul pour une insertion de nouveau contact
            // TODO : reprendre partie 2 pour calculer le temps d'insertion d'un contact en demandant les informations à l'utilisateur avant le début du timer
            break;

        case 0:
            return 0;

        default:
            printf("Option invalide. Veuillez reessayer.\n");
            break;
    }
    return 1;
}