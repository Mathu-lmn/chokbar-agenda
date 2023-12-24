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

unsigned int getNextId() {
    return gID++;
}

/** @brief Le tableau de noms */
char * nameList[NAME_FILE_SIZE];
/** @brief Le tableau de prénoms */
char * firstnameList[FIRST_NAME_FILE_SIZE];

void initData() {
    FILE *prenomFile;
    if ((prenomFile = fopen("data\\prenoms.csv", "r")) == NULL) {
        printf("Erreur lors de l'ouverture du fichier prenoms.\n");
        exit(EXIT_FAILURE);
    }
    FILE *nomFile;
    if ((nomFile = fopen("data\\noms.csv", "r")) == NULL) {
        printf("Erreur lors de l'ouverture du fichier noms.\n");
        exit(EXIT_FAILURE);
    }

    if (prenomFile == NULL || nomFile == NULL) {
        printf("Erreur lors de l'ouverture des fichiers.\n");
        exit(EXIT_FAILURE);
    }

    char line[100];
    int i = 0;
    while (i < FIRST_NAME_FILE_SIZE && (fgets(line, 100, prenomFile)) != NULL) {
        firstnameList[i] = strdup(line);
        firstnameList[i][strlen(firstnameList[i]) - 1] = '\0';
        i++;
    }
    i = 0;
    while (i < NAME_FILE_SIZE && (fgets(line, 100, nomFile)) != NULL) {
        nameList[i] = strdup(line);
        nameList[i][strlen(nameList[i]) - 1] = '\0';
        i++;
    }


    fclose(prenomFile);
    fclose(nomFile);

    //shuffle_list(firstnameList, FIRST_NAME_FILE_SIZE);
    //shuffle_list(nameList, NAME_FILE_SIZE);
}

char *scanString(void) {
    char buffer[100];

    fseek(stdin, 0, SEEK_SET);  // "Vider" le stdin (plutôt ignorer son contenu)
    fgets(buffer, 99, stdin);  // Limiter la saisie pour éviter les débordements de mémoire
    buffer[strlen(buffer) - 1] = '\0';  // Supprimer le \n à la fin de la chaîne

    char *str = malloc(strlen(buffer) + 1);
    strcpy(str, buffer);
    return str;
}

t_agenda *createAgenda(int levels) {
    t_agenda *agenda = (t_agenda *) malloc(sizeof(t_agenda));
    agenda->nbLevels = levels;
    agenda->heads = (t_agenda_cell **) malloc(levels * sizeof(t_agenda_cell *));
    for (int i = 0; i < levels; i++) {
        agenda->heads[i] = NULL;
    }

    return agenda;
}

t_agenda *fillAgenda(int numberOfContacts, int levels) {
    t_agenda *agenda = createAgenda(levels);
    
    for (int i = 0; i < numberOfContacts; i++) {
        t_contact contact = {nameList[i], firstnameList[i]};
//        printf("DEBUG : Contact %d : %s %s\n", i, contact.prenom, contact.nom);
        t_agenda_cell *agendaEntry = createAgendaCell(contact, levels);
        addContactToAgenda(agenda, agendaEntry);
    }
    return agenda;
}

t_agenda_cell *createAgendaCell(t_contact contact, int levels) {
    t_agenda_cell *newCell = (t_agenda_cell *) malloc(sizeof(t_agenda_cell));
    if (newCell == NULL) {
        printf("Erreur lors de l'allocation de memoire.\n");
        exit(EXIT_FAILURE);
    }
    newCell->contact = contact;
    newCell->level = 0;
    newCell->rdv = NULL;
    newCell->tabNext = (t_agenda_cell **) malloc((levels + 1) * sizeof(t_agenda_cell *));
    for (int i = 0; i < levels; i++) {
        newCell->tabNext[i] = NULL;
    }

    return newCell;
}

// Not optimized way meant for comparing efficiency
void addContactToAgendaLevel0(t_agenda *agenda, t_agenda_cell *agendaEntry) {
    if (agenda == NULL) {
        printf("Agenda non initialise.\n");
        return;
    }
    if (agendaEntry == NULL) {
        printf("Contact non initialise.\n");
        return;
    }
    if (agenda->heads[0] == NULL) {
        agenda->heads[0] = agendaEntry;
        return;
    }
    t_agenda_cell *tmp = agenda->heads[0];
    t_agenda_cell *ptmp = NULL;

    while (tmp != NULL && strcmp(tmp->contact.nom, agendaEntry->contact.nom) < 0) {
        ptmp = tmp;
        tmp = tmp->tabNext[0];
    }
    if (ptmp == NULL) {
        t_agenda_cell * tempHead = agenda->heads[0];
        agenda->heads[0] = agendaEntry;
        agendaEntry->tabNext[0] = tempHead;
    } else {
        ptmp->tabNext[0] = agendaEntry;
        agendaEntry->tabNext[0] = tmp;
    }
}

void searchContactLevel0(t_agenda * agenda, char * nom, char * prenom) {
    if (agenda == NULL) {
        printf("Agenda non initialise.\n");
        return;
    }

    t_agenda_cell * curr = agenda->heads[0];
    while (curr != NULL) {
        if (strcmp(curr->contact.nom, nom) == 0 && strcmp(curr->contact.prenom, prenom) == 0) {
            return;
        }
        curr = curr->tabNext[0];
    }
}

void addContactToAgenda(t_agenda *agenda, t_agenda_cell *agendaEntry) {
    if (agenda == NULL) {
        printf("Agenda non initialise.\n");
        return;
    }
    if (agendaEntry == NULL) {
        printf("Contact non initialise.\n");
        return;
    }
    if (agenda->heads[0] == NULL) {
        for (int i = 0; i < agenda->nbLevels; i++) {
            agenda->heads[i] = agendaEntry;
            agendaEntry->level = i;
        }
        return;
    }

    // La logique suivante cherche à insérer le contact de manière optimisée, mais en plus de cela, à lui donner le
    // nombre correct de niveaux et à mettre à jour les pointeurs des contacts précédents et suivants. On rappelle que
    // c'est parce que les skip lists ne sont bien optimisées qu'à condition que les niveaux des cellules sont
    // stratégiquement choisies (comme décrit dans les consignes du projet).
    for (int i = 0; i < agenda->nbLevels; i++) {
        if (agenda->heads[i] == NULL || strcmp(agendaEntry->contact.nom, agenda->heads[i]->contact.nom) < 0) {
            agendaEntry->tabNext[i] = agenda->heads[i];
            agenda->heads[i] = agendaEntry;
            agendaEntry->level = i;
        } else {
            t_agenda_cell *tmp = agenda->heads[i];
            t_agenda_cell *ptmp = NULL;

            while (tmp != NULL && strcmp(tmp->contact.nom, agendaEntry->contact.nom) < 0) {
                ptmp = tmp;
                tmp = tmp->tabNext[i];
            }
            if (ptmp == NULL) {
                t_agenda_cell * tempHead = agenda->heads[i];
                agenda->heads[i] = agendaEntry;
                agendaEntry->tabNext[i] = tempHead;
            } else if (i == 0 ||
                       (i == 1 && (!(ptmp->contact.nom[0] == agendaEntry->contact.nom[0] && ptmp->contact.nom[1] == agendaEntry->contact.nom[1] && ptmp->contact.nom[2] == agendaEntry->contact.nom[2]))) ||
                       (i == 2 && (!(ptmp->contact.nom[0] == agendaEntry->contact.nom[0] && ptmp->contact.nom[1] == agendaEntry->contact.nom[1]))) ||
                       (i == 3 && ptmp->contact.nom[0] != agendaEntry->contact.nom[0])) {
                // printf("DEBUG : Found level: %d for %s %s (compared to %s %s)\n", i, agendaEntry->contact.prenom, agendaEntry->contact.nom, ptmp->contact.prenom, ptmp->contact.nom);
                ptmp->tabNext[i] = agendaEntry;
                agendaEntry->tabNext[i] = tmp;
                agendaEntry->level = i;
            }
        }
    }
    for (int i = 0; i < agenda->nbLevels; i++) {
        if (agendaEntry->tabNext[i] != NULL) {
            // Selon le niveau, on compare plus ou moins de caractères du nom pour savoir si on doit mettre à jour
            if ((i == 1 && (agendaEntry->tabNext[i]->contact.nom[0] == agendaEntry->contact.nom[0] && agendaEntry->tabNext[i]->contact.nom[1] == agendaEntry->contact.nom[1] && agendaEntry->tabNext[i]->contact.nom[2] == agendaEntry->contact.nom[2])) ||
                (i == 2 && (agendaEntry->tabNext[i]->contact.nom[0] == agendaEntry->contact.nom[0] && agendaEntry->tabNext[i]->contact.nom[1] == agendaEntry->contact.nom[1])) ||
                (i == 3 && agendaEntry->tabNext[i]->contact.nom[0] == agendaEntry->contact.nom[0])) {
                // printf("DEBUG : Removing level: %d for %s %s\n", i, agendaEntry->tabNext[i]->contact.prenom, agendaEntry->tabNext[i]->contact.nom);
                agendaEntry->tabNext[i] = agendaEntry->tabNext[i]->tabNext[i];
                if (agendaEntry->tabNext[i] != NULL) {
                    agendaEntry->tabNext[i]->level = i;
                }
            }
        }
    }
}

void startSearch(t_agenda agenda) {
    printf("\bVeuillez entrer les 3 premiers caracteres du nom du contact pour voir les propositions : ");
    char *nom = scanString();
    while (strlen(nom) < 3) {
        printf("\bLe nom doit faire 3 caracteres.\n");
        printf("\bVeuillez entrer les 3 premiers caracteres du nom du contact pour voir les propositions : ");
        free(nom);
        nom = scanString();
    }

    t_agenda_cell *curr = agenda.heads[agenda.nbLevels - 1];
    while (curr != NULL) {
        if (strcmp(curr->contact.nom, nom) >= 0) {
            break;
        }
        curr = curr->tabNext[agenda.nbLevels - 1];
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
        curr = curr->tabNext[agenda.nbLevels - 1];
    }
    if (found == 0) {
        printf("\bAucun contact trouve\n");
    }
}

t_agenda_cell * searchContact(t_agenda * agenda, char * nom, char * prenom) {
    if (agenda == NULL) {
        printf("Agenda non initialise.\n");
        return NULL;
    }

    t_agenda_cell * curr = agenda->heads[agenda->nbLevels - 1];
    int i = agenda->nbLevels - 1;
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
        if (curr->tabNext[i] == NULL || strcmp(curr->tabNext[i]->contact.nom, nom) > 0) {
            i--;
        } else {
            curr = curr->tabNext[i];
        }
    }
    return NULL;
}

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

    t_agenda_cell * contact = searchContact(agenda, nom, prenom);
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

    t_agenda_cell * agendaEntry = createAgendaCell((t_contact){nom, prenom}, 4);
    addContactToAgenda(agenda, agendaEntry);
}

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

    t_agenda_cell *contact = searchContact(agenda, nom, prenom);
    if (contact == NULL) {
        printf("\bContact non trouve. Ajout au carnet d'adresses.\n");
        t_agenda_cell *agendaEntry = createAgendaCell((t_contact) {nom, prenom}, agenda->nbLevels);
        addContactToAgenda(agenda, agendaEntry);
        contact = agendaEntry;
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
    int dureeHeure = -1, dureeMinute = -1;
    while (scanf("%02d:%02d", &dureeHeure, &dureeMinute) != 2 || dureeHeure < 0 || dureeMinute < 0) {
        printf("\bVeuillez saisir une duree valide.\n");
        printf("\bDuree du rendez-vous (hh:mm): ");
        while ((getchar()) != '\n');
    }

    printf("\bObjet du rendez-vous: ");
    char *objet = scanString();
    struct Date date = {jour, mois, annee};
    struct Heure heureStruct = {heure, minute};
    struct Heure duree = {dureeHeure, dureeMinute};
    t_rdv *rdv = (t_rdv *) malloc(sizeof(t_rdv));
    rdv->date = date;
    rdv->heure = heureStruct;
    rdv->duree = duree;
    rdv->objet = objet;
    rdv->suivant = NULL;
    rdv->id = getNextId();
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

    t_agenda_cell *contact = searchContact(agenda, nom, prenom);
    if (contact == NULL) {
        printf("Contact non trouve.\n");
        return;
    }
    if (contact->rdv == NULL) {
        printf("Aucun rendez-vous pour ce contact.\n");
        return;
    }
    printf("Rendez-vous de %s %s:\n", contact->contact.prenom, contact->contact.nom);
    t_rdv * rdvList = contact->rdv;
    while (rdvList != NULL) {
        printf("ID: %d\t", rdvList->id);
        printf("Date: %d/%d/%d\t", rdvList->date.jour, rdvList->date.mois, rdvList->date.annee);
        printf("Heure: %d:%d\t", rdvList->heure.heure, rdvList->heure.minute);
        printf("Duree: %d:%d\t", rdvList->duree.heure, rdvList->duree.minute);
        printf("Objet: %s\n", rdvList->objet);
        rdvList = rdvList->suivant;
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
    for (int i = 0; i < agenda->nbLevels; i++) {
        t_agenda_cell *curr = agenda->heads[i];
        while (curr != NULL) {
            if (curr->contact.nom[0] != 'A') {
                printf("%s %s at level %d\n", curr->contact.prenom, curr->contact.nom, i);
            }
            curr = curr->tabNext[i];
        }
        printf("\n");
    }
}
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

void freeAgendaCell(p_agenda_cell cell) {
    if (cell == NULL) return;
    freeRDVLLC(cell->rdv);
    free(cell->tabNext);
    free(cell);
}

void freeAgenda(t_agenda* agenda) {
     p_agenda_cell cur = agenda->heads[0];
     p_agenda_cell next = NULL;
     while (cur != NULL) {
         next = cur->tabNext[0];
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
        t_agenda * firstAgenda = fillAgenda(n, 1);
        startTimer();
        for (int l = n; l < n+500; l++){
            t_contact contact = {nameList[l], firstnameList[l]};
            t_agenda_cell *agendaEntry = createAgendaCell(contact, 0);
            addContactToAgendaLevel0(firstAgenda, agendaEntry);
        }
        stopTimer();
        printf("Insertion level 0 : ");
        //time_lvl0_insert = getTimeAsString();
        displayTime();

        // Recherche
        startTimer();
        for (int j = 0; j < 500; j++) {
            int random = rand() % (n + 500);
            t_contact contact = {nameList[random], firstnameList[random]};
            searchContactLevel0(firstAgenda, contact.nom, contact.prenom);
        }
        stopTimer();
        printf("Recherche level 0 : ");
        //time_lvl0_search = getTimeAsString();
        displayTime();

        freeAgenda(firstAgenda);
        firstAgenda = NULL;

        t_agenda * secondAgenda = fillAgenda(n, 4);
        startTimer();
        for (int l = n; l < n+500; l++){
            t_contact contact = {nameList[l], firstnameList[l]};
            t_agenda_cell *agendaEntry = createAgendaCell(contact, 4);
            addContactToAgenda(secondAgenda, agendaEntry);
        }
        stopTimer();
        printf("Insertion level 4 : ");
        displayTime();
        //time_all_levels_insert = getTimeAsString();

        // Recherche
        startTimer();
        for (int j = 0; j < 500; j++) {
            int random = rand() % (n + 500);
            t_contact contact = {nameList[random], firstnameList[random]};
            searchContact(secondAgenda, contact.nom, contact.prenom);
        }
        stopTimer();
        printf("Recherche level 4 : ");
        //time_all_levels_search = getTimeAsString();
        displayTime();

        /*
        fprintf(temps_insert,format,n,time_lvl0_insert, time_all_levels_insert);
        fprintf(temps_search,format,n,time_lvl0_search, time_all_levels_search);
         */

        freeAgenda(secondAgenda);
        secondAgenda = NULL;
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
        // Si le contact n'a aucun RDV, on lui attribue un faux impossible à mettre de base
        if (curr->rdv == NULL) {
            fprintf(file, "%s,%s,00/00/0000,00h00,00:00,\n", curr->contact.prenom, curr->contact.nom);
        } else {
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
        curr = curr->tabNext[0];
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
    int levels = (*agenda)->nbLevels;
    freeAgenda(*agenda);
    *agenda = createAgenda(levels);

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

        // Vérifier si le contact n'a aucun RDV
        if (!date.annee) {
            curr = searchContact(*agenda, nom, prenom);

            // L'utilisateur a trafiqué son .csv !
            if (curr != NULL) {
                printf("Erreur sur la ligne %d : le contact %s %s a des RDV alors qu'il ne devrait pas en avoir.\n", lineNum, prenom, nom);
                continue;
            }
            curr = createAgendaCell(*createContact(nom, prenom), levels);
            addContactToAgenda(*agenda, curr);

            saneLines++;
            continue;
        }

        char* objet = strtok(NULL, ",");
        objet = objet == NULL ? "" : sanitizeObject(objet);

        if (curr == NULL || !((int) strcmp(curr->contact.nom, nom) == 0 && (int) strcmp(curr->contact.prenom, prenom) == 0)) {
            curr = searchContact(*agenda, nom, prenom);
            if (curr == NULL) {
                // Si le contact n'existe pas, on le crée
                curr = createAgendaCell(*createContact(nom, prenom), levels);
                addContactToAgenda(*agenda, curr);
            }
        }

        t_rdv *rdv = (t_rdv *) malloc(sizeof(t_rdv));
        rdv->date = date;
        rdv->heure = *heure;
        rdv->duree = *duree;
        rdv->objet = strdup(objet);
        rdv->suivant = NULL;
        rdv->id = getNextId();
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