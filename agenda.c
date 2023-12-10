/**
 * @file agenda.c
 * @author Mathurin Lemoine / Thomas Fischer / Idir Nait Meddour
 * @brief Fichier contenant les fonctions de l'agenda
 * @version 1.0
 * @date 26/11/2023
 */
#include "agenda.h"
#include "lib/agenda_utils.h"

/** @brief Le dernier ID attribué */
unsigned int gID = 0;

unsigned int get_next_id() {
    return gID++;
}

/** @brief Le tableau de noms */
char * name_list[NAME_FILE_SIZE];
/** @brief Le tableau de prénoms */
char * firstname_list[FIRST_NAME_FILE_SIZE];

/**
 * @brief Initialise les données de l'agenda, notamment les tableaux de noms et prénoms
 */
void initData() {
    FILE *prenom_file;
    if ((prenom_file = fopen("data\\prenoms.csv", "r")) == NULL) {
        printf("Erreur lors de l'ouverture du fichier prenoms.\n");
        exit(EXIT_FAILURE);
    }
    FILE *nom_file;
    if ((nom_file = fopen("data\\noms.csv", "r")) == NULL) {
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

/**
 * @brief Fonction pour scanner un string d'une taille maximale de 100 caractères
 * @return le string scanné
 */
char *scanString(void) {
    char buffer[100];

    fseek(stdin, 0, SEEK_SET);  // "Vider" le stdin (plutôt ignorer son contenu)
    fgets(buffer, 99, stdin);  // Limiter la saisie pour éviter les débordements de mémoire
    buffer[strlen(buffer) - 1] = '\0';  // Supprimer le \n à la fin de la chaîne

    char *str = malloc(strlen(buffer) + 1);
    strcpy(str, buffer);
    return str;
}

/**
 * @brief Fonction pour initialiser un agenda
 * @param levels le nombre de niveaux de l'agenda (c'est-à-dire, de la skip list qui est utilisée derrière)
 * @return un pointeur vers l'agenda créé
 */
t_agenda *create_agenda(int levels) {
    t_agenda *agenda = (t_agenda *) malloc(sizeof(t_agenda));
    agenda->nb_levels = levels;
    agenda->heads = (t_agenda_cell **) malloc(levels * sizeof(t_agenda_cell *));
    for (int i = 0; i < levels; i++) {
        agenda->heads[i] = NULL;
    }

    return agenda;
}

/**
 * @brief Fonction pour remplir un agenda avec des contacts aléatoires
 * @param number_of_contacts le nombre de contacts à ajouter
 * @param levels le nombre de niveaux de l'agenda
 * @return un pointeur vers l'agenda rempli
 */
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

/**
 * @brief Fonction pour créer une cellule de l'agenda
 * @param contact le contact à ajouter, "propriétaire" de la cellule
 * @param levels le nombre de niveaux de l'agenda
 * @return un pointeur vers la cellule créée
 */
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

/**
 * @brief Fonction pour insérer un contact au niveau 0 de l'agenda, au lieu d'effectuer une insertion optimisée.
 * Cette fonction est principalement utilisée pour comparer l'efficacité de l'insertion optimisée avec une insertion
 * classique.
 * @param agenda l'agenda dans lequel insérer
 * @param agenda_entry la cellule à insérer
 */
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

/**
 * @brief Fonction pour rechercher un contact dans l'agenda, au niveau 0.
 * Principalement utilisée pour comparer l'efficacité de la recherche optimisée avec une recherche classique.
 * @param agenda l'agenda dans lequel rechercher
 * @param nom le nom du contact à rechercher
 * @param prenom le prénom du contact à rechercher
 */
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

/**
 * @brief Fonction pour insérer un contact dans l'agenda
 * @param agenda l'aenda dans lequel insérer
 * @param agenda_entry la cellule (contenant le contact) à insérer
 */
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

    // La logique suivante cherche à insérer le contact de manière optimisée, mais en plus de cela, à lui donner le
    // nombre correct de niveaux et à mettre à jour les pointeurs des contacts précédents et suivants. On rappelle que
    // c'est parce que les skip lists ne sont bien optimisées qu'à condition que les niveaux des cellules sont
    // stratégiquement choisies (comme décrit dans les consignes du projet).
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
            // Selon le niveau, on compare plus ou moins de caractères du nom pour savoir si on doit mettre à jour
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

/**
 * @brief Fonction pour rechercher un contact de l'agenda, simplement pour savoir s'il existe.
 * @param agenda l'agenda dans lequel rechercher
 */
void startSearch(t_agenda agenda) {
    printf("\bVeuillez entrer les 3 premiers caracteres du nom du contact pour voir les propositions : ");
    char *nom = scanString();
    while (strlen(nom) < 3) {
        printf("\bLe nom doit faire 3 caracteres.\n");
        printf("\bVeuillez entrer les 3 premiers caracteres du nom du contact pour voir les propositions : ");
        free(nom);
        nom = scanString();
    }

    t_agenda_cell *curr = agenda.heads[agenda.nb_levels - 1];
    while (curr != NULL) {
        if (strcmp(curr->contact.nom, nom) >= 0) {
            break;
        }
        curr = curr->tab_next[agenda.nb_levels - 1];
    }
    if (curr == NULL) {
        printf("\bAucun contact trouve\n");
        return;
    }
    int found = 0;
    while (curr != NULL && strncmp(curr->contact.nom, nom, 3) == 0) {
        if (found == 0) {
            printf("\bContacts trouves :\n");
            found = 1;
        }
        printf("%s %s\n", curr->contact.prenom, curr->contact.nom);
        curr = curr->tab_next[agenda.nb_levels - 1];
    }
    if (found == 0) {
        printf("\bAucun contact trouve\n");
    }
}

/**
 * @brief Fonction pour rechercher et retourner un contact de l'agenda, avec une recherche optimisée.
 * @param agenda l'agenda dans lequel rechercher
 * @param nom le nom du contact à rechercher
 * @param prenom le prénom du contact à rechercher
 * @return un pointeur vers le contact recherché, ou NULL s'il n'existe pas
 */
t_agenda_cell * search_contact(t_agenda * agenda, char * nom, char * prenom) {
    if (agenda == NULL) {
        printf("Agenda non initialise.\n");
        return NULL;
    }

    t_agenda_cell * curr = agenda->heads[agenda->nb_levels - 1];
    int i = agenda->nb_levels - 1;
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

/**
 * @brief Fonction pour afficher les RDV associés à un contact de l'agenda.
 * @param agenda l'agenda dans lequel rechercher
 */
void displayContactRdv(t_agenda *agenda) {
    printf("\bNom du contact: ");
    char *nom = scanString();
    while (strlen(nom) < 3) {
        printf("\bLe nom doit faire au moins 3 caracteres.\n");
        printf("\bNom du contact: ");
        free(nom);
        nom = scanString();
    }

    printf("\bPrenom du contact: ");
    char *prenom = scanString();
    while (strlen(prenom) < 3) {
        printf("\bLe prenom doit faire au moins 3 caracteres.\n");
        printf("\bPrenom du contact: ");
        free(prenom);
        prenom = scanString();
    }

    t_agenda_cell * contact = search_contact(agenda, nom, prenom);
    if (contact == NULL) {
        printf("\bContact non trouve.\n");
        return;
    }
    if (contact->rdv == NULL) {
        printf("\bAucun rendez-vous pour ce contact.\n");
        return;
    }
    printf("\bRendez-vous de %s %s:\n", contact->contact.prenom, contact->contact.nom);
    t_rdv *rdv = contact->rdv;
    while (rdv != NULL) {
        printf("ID: %d\t", rdv->id);
        printf("Date: %02d/%02d/%04d\t", rdv->date.jour, rdv->date.mois, rdv->date.annee);
        printf("Heure: %02dh%02d\t", rdv->heure.heure, rdv->heure.minute);
        printf("Duree: %02d:%02d\t", rdv->duree.heure, rdv->duree.minute);
        printf("Objet: %s\n", rdv->objet);
        rdv = rdv->suivant;
    }
}

/**
 * @brief Fonction pour créer un nouveau contact et l'ajouter à l'agenda.
 * @param agenda l'agenda dans lequel ajouter
 */
void createNewContact(t_agenda *agenda) {
    printf("\bNom du contact: ");
    char *nom = scanString();
    while (strlen(nom) < 3) {
        printf("\bLe nom doit faire au moins 3 caracteres.\n");
        printf("\bNom du contact: ");
        free(nom);
        nom = scanString();
    }

    printf("\bPrenom du contact: ");
    char *prenom = scanString();
    while (strlen(prenom) < 3) {
        printf("\bLe prenom doit faire au moins 3 caracteres.\n");
        printf("\bPrenom du contact: ");
        free(prenom);
        prenom = scanString();
    }

    struct Contact contact = {nom, prenom};
    t_agenda_cell * agenda_entry = create_agenda_cell(contact, 4);
    add_contact_to_agenda(agenda, agenda_entry);
}

/**
 * @brief Ajouter un RDV à l'agenda.
 * @param agenda l'agenda dans lequel ajouter
 */
void addNewRdv(t_agenda *agenda) {
    printf("\bNom du contact: ");
    char *nom = scanString();
    while (strlen(nom) < 3) {
        printf("\bLe nom doit faire au moins 3 caracteres.\n");
        printf("\bNom du contact: ");
        // On libère le nom précédent pour éviter les fuites de mémoire
        free(nom);
        nom = scanString();
    }

    printf("\bPrenom du contact: ");
    char *prenom = scanString();
    while (strlen(prenom) < 3) {
        printf("\bLe prenom doit faire au moins 3 caracteres.\n");
        printf("\bPrenom du contact: ");
        free(prenom);
        prenom = scanString();
    }

    t_agenda_cell *contact = search_contact(agenda, nom, prenom);
    if (contact == NULL) {
        printf("\bContact non trouve. Ajout au carnet d'adresses.\n");
        t_agenda_cell *agenda_entry = create_agenda_cell((struct Contact) {nom, prenom}, agenda->nb_levels);
        add_contact_to_agenda(agenda, agenda_entry);
        contact = agenda_entry;
    }

    printf("\bDate du rendez-vous (jj/mm/aaaa): ");
    int jour = -1, mois = -1, annee = -1;
    while (scanf("%d/%d/%d", &jour, &mois, &annee) != 3 || jour < 1 || jour > 31 || mois < 1 || mois > 12 || annee < 2023) {
        printf("\bVeuillez saisir une date valide.\n");
        printf("\bDate du rendez-vous (jj/mm/aaaa): ");
        while ((getchar()) != '\n');
    }
    while ((getchar()) != '\n');
    printf("\bHeure du rendez-vous (hh:mm): ");
    int heure = -1, minute = -1;
    while (scanf("%02d:%02d", &heure, &minute) != 2 || heure < 0 || heure > 23 || minute < 0 || minute > 59) {
        printf("\bVeuillez saisir une heure valide.\n");
        printf("\bHeure du rendez-vous (hh:mm): ");
        while ((getchar()) != '\n');
    }
    printf("\bDuree du rendez-vous (hh:mm): ");
    int duree_heure = -1, duree_minute = -1;
    while (scanf("%02d:%02d", &duree_heure, &duree_minute) != 2 || duree_heure < 0 || duree_minute < 0) {
        printf("\bVeuillez saisir une duree valide.\n");
        printf("\bDuree du rendez-vous (hh:mm): ");
        while ((getchar()) != '\n');
    }

    printf("\bObjet du rendez-vous: ");
    char *objet = scanString();
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
        // Si le contact n'a pas de RDV, on ajoute le RDV directement
        contact->rdv = rdv;
    } else {
        // Sinon, on ajoute le RDV à sa place dans la liste chaînée de RDV
        insertRDV(&(contact->rdv), rdv);
    }

    // Vider le buffer pour éviter les \n qui traînent
    fflush(stdout);
    printf("RDV ajoute. ID : %d\n", rdv->id);
}

/**
 * @brief Fonction pour insérer un RDV dans une liste chaînée de RDV, en fonction de la date et de l'heure.
 * @param agenda l'agenda dans lequel ajouter
 */
void deleteRdv(t_agenda *agenda) {
    printf("Nom du contact: ");
    char *nom = scanString();
    while (strlen(nom) < 3) {
        printf("Le nom doit faire au moins 3 caracteres.\n");
        printf("Nom du contact: ");
        free(nom);
        nom = scanString();
    }

    printf("Prenom du contact: ");
    char *prenom = scanString();
    while (strlen(prenom) < 3) {
        printf("Le prenom doit faire au moins 3 caracteres.\n");
        printf("Prenom du contact: ");
        free(prenom);
        prenom = scanString();
    }

    t_agenda_cell *contact = search_contact(agenda, nom, prenom);
    if (contact == NULL) {
        printf("Contact non trouve.\n");
        return;
    }
    if (contact->rdv == NULL) {
        printf("Aucun rendez-vous pour ce contact.\n");
        return;
    }
    printf("Rendez-vous de %s %s:\n", contact->contact.prenom, contact->contact.nom);
    t_rdv * rdv_list = contact->rdv;
    while (rdv_list != NULL) {
        printf("ID: %d\t", rdv_list->id);
        printf("Date: %d/%d/%d\t", rdv_list->date.jour, rdv_list->date.mois, rdv_list->date.annee);
        printf("Heure: %d:%d\t", rdv_list->heure.heure, rdv_list->heure.minute);
        printf("Duree: %d:%d\t", rdv_list->duree.heure, rdv_list->duree.minute);
        printf("Objet: %s\n", rdv_list->objet);
        rdv_list = rdv_list->suivant;
    }

    printf("ID du rendez-vous a supprimer: ");
    int id = -1;
    while (scanf("%d", &id) != 1) {
        printf("Veuillez saisir un nombre.\n");
        printf("ID du rendez-vous: ");
        while ((getchar()) != '\n');
    }
    while ((getchar()) != '\n');

    t_rdv *rdv = contact->rdv;
    t_rdv *prev = NULL;
    while (rdv != NULL) {
        if (rdv->id == id) {
            if (prev == NULL) {
                contact->rdv = rdv->suivant;
            } else {
                prev->suivant = rdv->suivant;
            }
            free(rdv);
            printf("Rendez-vous supprime.\n");
            return;
        }
        prev = rdv;
        rdv = rdv->suivant;
    }
    printf("Rendez-vous non trouve.\n");
}

/* FONCTION DE DEBUG pour afficher les contacts de l'agenda à chaque niveau
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
 */

/**
 * @brief Libérer (supprime) la mémoire allouée pour une liste chaînée de RDV
 * @param firstRDV le premier RDV (la tête) de la liste
 */
void freeRDVLLC(t_rdv * firstRDV) {
    t_rdv * cur = firstRDV;

    t_rdv * next = NULL;
    while (cur != NULL) {
        next = cur->suivant;
        free(cur);
        cur = next;
    }
}

/**
 * @brief Libérer (supprime) la mémoire allouée pour une cellule de l'agenda
 * @param cell la cellule à libérer
 */
void freeAgendaCell(p_agenda_cell cell) {
    if (cell == NULL) return;
    freeRDVLLC(cell->rdv);
    free(cell->tab_next);
    free(cell);
}

/**
 * @brief Libérer (supprime) la mémoire allouée pour l'agenda
 * @param agenda l'agenda à libérer
 */
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

/**
 * @brief Teste le temps d'execution de l'insertion et de la recherche dans l'agenda.
 * On teste avec l'insertion naïve (au niveau 0) et l'insertion optimisée (avec les niveaux),
 * et de même pour la recherche.
 */
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

/**
 * @brief Sauvegarder l'agenda dans un fichier CSV : nom, prénom, date, heure, durée, objet
 * @param agenda l'agenda à sauvegarder
 */
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
                // On enlève les virgules pour éviter les problèmes avec le CSV, dans l'objet de chaque RDV
                char* objet = sanitizeObject(rdv->objet);
                // C'est mieux de le faire ici, pour pouvoir libérer la mémoire de l'ancien objet
                free(rdv->objet);
                rdv->objet = objet;

                fprintf(file, "%s,%s,%02d/%02d/%04d,%02dh%02d,%02d:%02d,%s\n",
                        curr->contact.prenom, curr->contact.nom,
                        rdv->date.jour, rdv->date.mois, rdv->date.annee,
                        rdv->heure.heure, rdv->heure.minute,
                        rdv->duree.heure, rdv->duree.minute,
                        objet);
                rdv = rdv->suivant;
            }
        }
        curr = curr->tab_next[0];
    }
    fclose(file);
    free(filename);
}

/**
 * @brief Charger un agenda depuis un fichier CSV
 * L'ancien agenda est automatiquement écrasé.
 * @param agenda l'agenda à remplir
 */
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
    fgets(line, 100, file);

    // Cette variable est initialisée à 1 car on l'incrémente juste après, au début du while, pour éviter les problèmes
    // avec "continue;".
    // Elle sert notamment à afficher le nombre de lignes lues et le nombre de lignes valides.
    int lineNum = 1;
    int saneLines = 1;

    t_agenda_cell *curr = NULL;
    while (fgets(line, 200, file) != NULL) {
        lineNum++;

        // On ignore les lignes vides
        if (line[0] == '\n') { saneLines++; continue; }

        // Si la ligne a été mal lue, on l'ignore
        if (countChar(line, ',') != 5) {
            printf("Erreur sur la ligne %d : trop ou pas assez d'informations.\n", lineNum);
            continue;
        }

        char* prenom = strtok(line, ",");
        prenom = prenom == NULL ? "Inconnu" : prenom;

        char* nom = strtok(NULL, ",");
        nom = nom == NULL ? "Inconnu" : nom;

        struct Date date = parseDate(strtok(NULL, ","));

        struct Heure* heure = parseHeureStruct(strtok(NULL, ","));

        struct Heure* duree = parseHeureStruct(strtok(NULL, ","));

        char* objet = strtok(NULL, ",");
        objet = objet == NULL ? "" : sanitizeObject(objet);

        if (curr == NULL || !((int) strcmp(curr->contact.nom, nom) == 0 && (int) strcmp(curr->contact.prenom, prenom) == 0)) {
            curr = search_contact(*agenda, nom, prenom);
            if (curr == NULL) {
                // Si le contact n'existe pas, on le crée
                curr = create_agenda_cell(*createContact(nom, prenom), levels);
                add_contact_to_agenda(*agenda, curr);
            }
        }

        t_rdv *rdv = (t_rdv *) malloc(sizeof(t_rdv));
        rdv->date = date;
        rdv->heure = *heure;
        rdv->duree = *duree;
        rdv->objet = strdup(objet);
        rdv->suivant = NULL;
        rdv->id = get_next_id();
        if (curr->rdv == NULL) {
            curr->rdv = rdv;
        } else {
            insertRDV(&(curr->rdv), rdv);
        }

//        printf("\bDEBUG : %s %s %02d/%02d/%04d %02dh%02d %02d:%02d %s\n", prenom, nom, date.jour, date.mois,
//             date.annee, heure->heure, heure->minute, duree->heure, duree->minute, objet);
        saneLines++;
    }
    fclose(file);
    free(filename);
    printf("\b\n\nFichier charge. %d lignes lues, %d lignes valides.\n", lineNum, saneLines);
}

/**
 * @brief La fonction carrefour pour exécuter le choix de l'utilisateur
 * @param choice le numéro de l'option choisie entré par l'utilisateur
 * @param agenda l'agenda sur lequel effectuer l'opération
 * @return 0 si l'utilisateur a choisi de quitter, 1 sinon
 */
int executeChoice(int choice, t_agenda ** agenda) {
    // Traitement de l'option choisie
    switch (choice) {
        case 1:
            startSearch(**agenda);
            break;
        case 2:
            displayContactRdv(*agenda);
            break;
        case 3:
            createNewContact(*agenda);
            break;
        case 4:
            addNewRdv(*agenda);
            break;
        case 5:
            deleteRdv(*agenda);
            break;
        case 6:
            saveAgendaToFile(*agenda);
            break;
        case 7:
            loadAgendaFromFile(agenda);
            break;
        case 8:
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