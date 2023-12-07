/**
 * @file agenda.c
 * @author Mathurin Lemoine / Thomas Fischer / Idir Nait Meddour
 * @brief Fichier contenant les fonctions de l'agenda
 * @version 1.0
 * @date 26/11/2023
 */

#include "agenda.h"
#include "lib/agenda_utils.h"

// À ne pas toucher directement, utilisez plutôt la fonction get_next_id() en-dessous
unsigned int gID = 0;

unsigned int get_next_id() {
    return gID++;
}

char * name_list[NAME_FILE_SIZE];
char * firstname_list[FIRST_NAME_FILE_SIZE];

void initData() {
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

    char line[100];
    int i = 0;
    while (i < FIRST_NAME_FILE_SIZE && (fgets(line, 100, prenom_file)) != NULL) {
        firstname_list[i] = strdup(line);
        firstname_list[i][strlen(firstname_list[i]) - 1] = '\0';
        i++;
    }
    i = 0;
    while (i < NAME_FILE_SIZE && (fgets(line, 100, nom_file)) != NULL) {
        name_list[i] = strdup(line);
        name_list[i][strlen(name_list[i]) - 1] = '\0';
        i++;
    }


    fclose(prenom_file);
    fclose(nom_file);

    shuffle_list(firstname_list, FIRST_NAME_FILE_SIZE);
    shuffle_list(name_list, NAME_FILE_SIZE);
}

// Fonction pour saisir une chaîne de caractères dynamique
// TODO: à chaque usage, free() si la chaîne est erronée
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
    
    for (int i = 0; i < number_of_contacts; i++) {
        struct Contact contact = {name_list[i], firstname_list[i]};
//        printf("DEBUG : Contact %d : %s %s\n", i, contact.prenom, contact.nom);
        t_agenda_cell *agenda_entry = create_agenda_cell(contact, levels);
        add_contact_to_agenda(agenda, agenda_entry);
    }
    return agenda;
}

t_agenda_cell *create_agenda_cell(struct Contact contact, int levels) {
    t_agenda_cell *new_cell = (t_agenda_cell *) malloc(sizeof(t_agenda_cell));
    if (new_cell == NULL) {
        printf("Erreur lors de l'allocation de memoire.\n");
        exit(EXIT_FAILURE);
    }
    new_cell->contact = contact;
    new_cell->level = 0;
    new_cell->rdv = NULL;
    new_cell->tab_next = (t_agenda_cell **) malloc((levels + 1) * sizeof(t_agenda_cell *));
    for (int i = 0; i < levels; i++) {
        new_cell->tab_next[i] = NULL;
    }

    return new_cell;
}

// Not optimized way meant for comparing efficiency
void add_contact_to_agenda_level_0(t_agenda *agenda, t_agenda_cell *agenda_entry) {
    if (agenda == NULL) {
        printf("Agenda non initialise.\n");
        return;
    }
    if (agenda_entry == NULL) {
        printf("Contact non initialise.\n");
        return;
    }
    if (agenda->heads[0] == NULL) {
        agenda->heads[0] = agenda_entry;
        return;
    }
    t_agenda_cell *tmp = agenda->heads[0];
    t_agenda_cell *ptmp = NULL;

    while (tmp != NULL && strcmp(tmp->contact.nom, agenda_entry->contact.nom) < 0) {
        ptmp = tmp;
        tmp = tmp->tab_next[0];
    }
    if (ptmp == NULL) {
        t_agenda_cell * temp_head = agenda->heads[0];
        agenda->heads[0] = agenda_entry;
        agenda_entry->tab_next[0] = temp_head;
    } else {
        ptmp->tab_next[0] = agenda_entry;
        agenda_entry->tab_next[0] = tmp;
    }
}

void search_contact_level_0(t_agenda * agenda, char * nom, char * prenom) {
    if (agenda == NULL) {
        printf("Agenda non initialise.\n");
        return;
    }

    t_agenda_cell * curr = agenda->heads[0];
    while (curr != NULL) {
        if (strcmp(curr->contact.nom, nom) == 0 && strcmp(curr->contact.prenom, prenom) == 0) {
            return;
        }
        curr = curr->tab_next[0];
    }
}

void add_contact_to_agenda(t_agenda *agenda, t_agenda_cell *agenda_entry) {
    if (agenda == NULL) {
        printf("Agenda non initialise.\n");
        return;
    }
    if (agenda_entry == NULL) {
        printf("Contact non initialise.\n");
        return;
    }
    if (agenda->heads[0] == NULL) {
        for (int i = 0; i < agenda->nb_levels; i++) {
            agenda->heads[i] = agenda_entry;
            agenda_entry->level = i;
        }
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
            if (ptmp == NULL) {
                t_agenda_cell * temp_head = agenda->heads[i];
                agenda->heads[i] = agenda_entry;
                agenda_entry->tab_next[i] = temp_head;
            } else if (i == 0 ||
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
    printf("Non implémenté pour le moment (il faut avoir le nom prénom complet afin de trouver la liste des rendez-vous).\n");
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
    t_agenda_cell * agenda_entry = create_agenda_cell(contact, 4);
    add_contact_to_agenda(agenda, agenda_entry);
}

// TODO : voir pour fix le décalage du print à gauche
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
        t_agenda_cell *agenda_entry = create_agenda_cell((struct Contact) {nom, prenom}, agenda->nb_levels);
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
    while ((getchar()) != '\n');
    printf("Heure du rendez-vous (hh:mm): ");
    int heure = -1, minute = -1;
    while (scanf("%d:%d", &heure, &minute) != 2 || heure < 0 || heure > 23 || minute < 0 || minute > 59) {
        printf("Veuillez saisir une heure valide.\n");
        printf("Heure du rendez-vous (hh:mm): ");
        while ((getchar()) != '\n');
    }
    while ((getchar()) != '\n');
    printf("Duree du rendez-vous (hh:mm): ");
    int duree_heure, duree_minute;
    while (scanf("%d:%d", &duree_heure, &duree_minute) != 2) {
        printf("Veuillez saisir une duree valide.\n");
        printf("Duree du rendez-vous (hh:mm): ");
        while ((getchar()) != '\n');
    }
    while ((getchar()) != '\n');
    printf("Objet du rendez-vous: ");
    char *objet = scanString();
    while ((getchar()) != '\n');
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

// TODO : FIX la suppression de rendez-vous qui supprime le premier et deuxième rendez-vous d'un contact si on supprime le deuxième
// TODO : Fix l'id qui commence à 0
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
            p_rdv prevRdv = NULL;
            p_rdv rdv = cur->rdv;

            while (rdv != NULL) {
                if (rdv->id == id) {
                    if (prevRdv == NULL) {
                        // Le rendez-vous à supprimer est le premier dans la liste
                        cur->rdv = rdv->suivant;
                    } else {
                        // Le rendez-vous à supprimer n'est pas le premier dans la liste
                        prevRdv->suivant = rdv->suivant;
                    }

                    free(rdv->objet);
                    free(rdv);
                    printf("Rendez-vous numero %d supprime.\n", id);
                    return;
                }

                prevRdv = rdv;
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

void freeRDVLLC(t_rdv * firstRDV) {
    t_rdv * cur = firstRDV;

    t_rdv * next = NULL;
    while (cur != NULL) {
        next = cur->suivant;
        free(cur);
        cur = next;
    }
}

void freeAgendaCell(p_agenda_cell cell) {
    if (cell == NULL) return;
    freeRDVLLC(cell->rdv);
    free(cell->tab_next);
    free(cell);
}

void freeAgenda(t_agenda* agenda) {
     p_agenda_cell cur = agenda->heads[0];
     p_agenda_cell next = NULL;
     while (cur != NULL) {
         next = cur->tab_next[0];
//         printf("DEBUG : Attempting to free %s %s\n", cur->contact.prenom, cur->contact.nom);
         freeAgendaCell(cur);
         cur = next;
     }
}

void contactInsertionTimer(){
    //Création d'un contact de test
/*
    FILE *temps_search;
    if ((temps_search = fopen("tempssearch.txt", "w")) == NULL) {
        printf("Erreur lors de l'ouverture du fichier tempssearch.\n");
        exit(EXIT_FAILURE);
    }
    FILE *temps_insert;
    if ((temps_insert = fopen("tempsinsert.txt", "w")) == NULL) {
        printf("Erreur lors de l'ouverture du fichier tempsinsert.\n");
        exit(EXIT_FAILURE);
    }
    char format[] = "%d\t%s\t%s\n" ;
*/
    printf("Test du temps d'execution.\n");
    printf("Veuillez choisir le nombre de test (< 50) :");
    int nbtest = 1;
    scanf("%d", &nbtest);
    if (nbtest > 50 || nbtest < 1){
        printf("Nombre de test incorrect.\n");
        return;
    }
    for (int i = 1; i <= nbtest; i++){
        /*
        char *time_lvl0_search;
        char *time_lvl0_insert;
        char *time_all_levels_search;
        char *time_all_levels_insert;
         */

        int n = 500*i;

        printf("\nTest number %d :\n", i);
        t_agenda * first_agenda = fillAgenda(n, 1);
        startTimer();
        for (int l = n; l < n+500; l++){
            struct Contact contact = {name_list[l], firstname_list[l]};
            t_agenda_cell *agenda_entry = create_agenda_cell(contact, 0);
            add_contact_to_agenda_level_0(first_agenda, agenda_entry);
        }
        stopTimer();
        printf("Insertion level 0 : ");
        //time_lvl0_insert = getTimeAsString();
        displayTime();

        // Recherche
        startTimer();
        for (int j = 0; j < 500; j++) {
            int random = rand() % (n + 500);
            struct Contact contact = {name_list[random], firstname_list[random]};
            search_contact_level_0(first_agenda, contact.nom, contact.prenom);
        }
        stopTimer();
        printf("Recherche level 0 : ");
        //time_lvl0_search = getTimeAsString();
        displayTime();

        freeAgenda(first_agenda);
        first_agenda = NULL;

        t_agenda * second_agenda = fillAgenda(n, 4);
        startTimer();
        for (int l = n; l < n+500; l++){
            struct Contact contact = {name_list[l], firstname_list[l]};
            t_agenda_cell *agenda_entry = create_agenda_cell(contact, 4);
            add_contact_to_agenda(second_agenda, agenda_entry);
        }
        stopTimer();
        printf("Insertion level 4 : ");
        displayTime();
        //time_all_levels_insert = getTimeAsString();

        // Recherche
        startTimer();
        for (int j = 0; j < 500; j++) {
            int random = rand() % (n + 500);
            struct Contact contact = {name_list[random], firstname_list[random]};
            search_contact(second_agenda, contact.nom, contact.prenom);
        }
        stopTimer();
        printf("Recherche level 4 : ");
        //time_all_levels_search = getTimeAsString();
        displayTime();

        /*
        fprintf(temps_insert,format,n,time_lvl0_insert, time_all_levels_insert);
        fprintf(temps_search,format,n,time_lvl0_search, time_all_levels_search);
         */

        freeAgenda(second_agenda);
        second_agenda = NULL;
    }
    /*
    fclose(temps_insert);
    fclose(temps_search);
     */
}

void saveAgendaToFile(t_agenda* agenda) {
    printf("Entrez un nom de fichier : ");
    char* filename = scanString();
    while (strlen(filename) < 3) {
        printf("Le nom doit faire au moins 3 caracteres.\n");
        printf("Entrez un nom de fichier : ");
        free(filename);
        filename = scanString();
    }

    FILE* file = NULL;
    if ((file = fopen(filename, "w")) == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s.\n", filename);
        free(filename);
        fclose(file);
        return;
    }

    fprintf(file, "prenom,nom,date,heure,duree,objet\n");
    t_agenda_cell *curr = agenda->heads[0];
    while (curr != NULL) {
        if (curr->rdv != NULL) {
            p_rdv rdv = curr->rdv;
            while (rdv != NULL) {
                fprintf(file, "%s,%s,%02d/%02d/%04d,%02dh%02d,%02d:%02d,%s\n", curr->contact.prenom, curr->contact.nom, rdv->date.jour, rdv->date.mois, rdv->date.annee, rdv->heure.heure, rdv->heure.minute, rdv->duree.heure, rdv->duree.minute, rdv->objet);
                rdv = rdv->suivant;
            }
        }
        curr = curr->tab_next[0];
    }
    fclose(file);
    free(filename);
}

void loadAgendaFromFile(t_agenda **agenda) {
    printf("Entrez un nom de fichier : ");
    char* filename = scanString();
    while (strlen(filename) < 3) {
        printf("Le nom doit faire au moins 3 caracteres.\n");
        printf("Entrez un nom de fichier : ");
        free(filename);
        filename = scanString();
    }

    FILE* file = NULL;
    if ((file = fopen(filename, "r")) == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s.\n", filename);
        free(filename);
        return;
    }

    // Écraser l'ancien agenda
    int levels = (*agenda)->nb_levels;
    freeAgenda(*agenda);
    *agenda = create_agenda(levels);

    char line[200];

    // On ignore la première ligne d'un CSV
    fgets(line, 1, file);


    t_agenda_cell *curr = NULL;
    while (fgets(line, 200, file) != NULL) {
        char* prenom = strtok(line, ",");
        prenom = prenom == NULL ? "Inconnu" : prenom;

        char* nom = strtok(NULL, ",");
        nom = nom == NULL ? "Inconnu" : nom;

        struct Date date = parseDate(strtok(NULL, ","));

        struct Heure heure = parseHeure(strtok(NULL, ","));

        struct Heure duree = parseDuree(strtok(NULL, ","));

        char* objet = strtok(NULL, ",");
        objet = objet == NULL ? "" : objet;

        if (curr->contact.nom != nom || curr->contact.prenom != prenom) {
            // Si le contact n'existe pas, on le crée
            curr = create_agenda_cell((struct Contact) {nom, prenom}, levels);
            add_contact_to_agenda(*agenda, curr);
        }

        t_rdv *rdv = (t_rdv *) malloc(sizeof(t_rdv));
        rdv->date = date;
        rdv->heure = heure;
        rdv->duree = duree;
        rdv->objet = objet;
        rdv->suivant = NULL;
        rdv->id = get_next_id();
        if (curr->rdv == NULL) {
            curr->rdv = rdv;
        } else {
            t_rdv *tmp = curr->rdv;

            // on se place à la fin de la liste
            // TODO : insérer le rendez-vous à la bonne place (par date et heure), créer une fonction pour comparer deux rendez-vous (lisibilité)
            while (tmp->suivant != NULL) {
                tmp = tmp->suivant;
            }
            tmp->suivant = rdv;
        }

        printf("%s %s %s %s %s %s\n", prenom, nom, date, heure, duree, objet);
    }
    fclose(file);
    free(filename);
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
            deleteRdv(agenda);
            break;
        case 6:
            saveAgendaToFile(agenda);
            break;

        case 7:
            loadAgendaFromFile(&agenda);
            break;

        case 8:
            // Fournir les temps de calcul pour une insertion de nouveau contact
            // TODO : reprendre partie 2 pour calculer le temps d'insertion d'un contact en demandant les informations à l'utilisateur avant le début du timer
            contactInsertionTimer();
            break;

        case 0:
            return 0;

        default:
            printf("Option invalide. Veuillez reessayer.\n");
            break;
    }
    return 1;
}