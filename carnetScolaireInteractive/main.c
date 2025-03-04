/* Assane Thiao 28/02/2025 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_L_CARAC 80
#define MAX_L_COURS 3

typedef struct {
  char title[MAX_L_CARAC];
  double score;
} COURSE;

typedef struct {
  char name[MAX_L_CARAC];
  char surname[MAX_L_CARAC];
  int id;
  COURSE courses[MAX_L_COURS];
} FICHE;

/* Fonctions d'interface */
void setName(FICHE* fiche, const char* name);
void setSurname(FICHE* fiche, const char* surname);
void setId(FICHE* fiche, int id);
void setCourse(FICHE* fiche, int index, const char* title, double score);

char* getName(FICHE* fiche);
char* getSurname(FICHE* fiche);
int getId(FICHE* fiche);
COURSE getCourse(FICHE* fiche, int index);
char* getCourseTitle(COURSE* course);
double getCourseScore(COURSE* course);

void printFiche(FICHE fiche);
void saveFiche(FILE* file, FICHE fiche);
void loadFiche(FICHE* fiche, const char* str);
double calculerMoyenne(FICHE fiche);
void afficherTousLesEtudiants(const char* filename);

/* Fonctions interactives */
FICHE ficheFromInput(void);
COURSE courseFromInput(void);

/* Fonction principale */
void exo5(int argc, char* argv[]);

int main(int argc, char* argv[]) {
  exo5(argc, argv);
  return 0;
}

/* Conversion en majuscules */
void toUpperCase(char* str) {
  for (int i = 0; str[i] != '\0'; i++) {
    str[i] = toupper(str[i]);
  }
}

/* Setters avec conversion */
void setName(FICHE* fiche, const char* name) {
  strncpy_s(fiche->name, MAX_L_CARAC, name, _TRUNCATE);
  toUpperCase(fiche->name);
}

void setSurname(FICHE* fiche, const char* surname) {
  strncpy_s(fiche->surname, MAX_L_CARAC, surname, _TRUNCATE);
  toUpperCase(fiche->surname);
}

void setId(FICHE* fiche, int id) {
  fiche->id = id;
}

void setCourse(FICHE* fiche, int index, const char* title, double score) {
  if (index >= 0 && index < MAX_L_COURS) {
    strncpy_s(fiche->courses[index].title, MAX_L_CARAC, title, _TRUNCATE);
    toUpperCase(fiche->courses[index].title);
    fiche->courses[index].score = score;
  }
}

/* Getters */
char* getName(FICHE* fiche) {
  return fiche->name;
}

char* getSurname(FICHE* fiche) {
  return fiche->surname;
}

int getId(FICHE* fiche) {
  return fiche->id;
}

COURSE getCourse(FICHE* fiche, int index) {
  if (index >= 0 && index < MAX_L_COURS) {
    return fiche->courses[index];
  }
  COURSE emptyCourse = { "", 0.0 };  // Renvoie un cours vide si index invalide
  return emptyCourse;
}

char* getCourseTitle(COURSE* course) {
  return course->title;
}

double getCourseScore(COURSE* course) {
  return course->score;
}

/* Affichage */
void printFiche(FICHE fiche) {
  printf("Nom: %s\n", fiche.name);
  printf("Prénom: %s\n", fiche.surname);
  printf("Numéro étudiant: %d\n", fiche.id);
  for (int i = 0; i < MAX_L_COURS; i++) {
    printf("%s: %.2f\n", fiche.courses[i].title, fiche.courses[i].score);
  }
  printf("Moyenne: %.2f\n", calculerMoyenne(fiche));
}

/* Sauvegarde */
void saveFiche(FILE* file, FICHE fiche) {
  if (file == NULL) return;
  fprintf(file, "%s;%s;%d", fiche.name, fiche.surname, fiche.id);
  for (int i = 0; i < MAX_L_COURS; i++) {
    fprintf(file, ";%s;%.2f", fiche.courses[i].title, fiche.courses[i].score);
  }
  fprintf(file, "\n");
}

/* Chargement */
void loadFiche(FICHE* fiche, const char* str) {
  if (fiche == NULL || str == NULL) return;

  char buffer[256];
  strcpy_s(buffer, sizeof(buffer), str);

  char* nextToken = NULL;
  char* token = strtok_s(buffer, ";", &nextToken);
  setName(fiche, token);

  token = strtok_s(NULL, ";", &nextToken);
  setSurname(fiche, token);

  token = strtok_s(NULL, ";", &nextToken);
  setId(fiche, atoi(token));

  for (int i = 0; i < MAX_L_COURS; i++) {
    token = strtok_s(NULL, ";", &nextToken);
    char courseName[MAX_L_CARAC];
    strncpy_s(courseName, MAX_L_CARAC, token, _TRUNCATE);

    token = strtok_s(NULL, ";", &nextToken);
    double score = atof(token);

    setCourse(fiche, i, courseName, score);
  }
}

/* Calcul de la moyenne */
double calculerMoyenne(FICHE fiche) {
  double somme = 0.0;
  for (int i = 0; i < MAX_L_COURS; i++) {
    somme += fiche.courses[i].score;
  }
  return somme / MAX_L_COURS;
}

/* Saisie interactive */
FICHE ficheFromInput(void) {
  FICHE fiche;
  char buffer[100];

  printf("Nom: ");
  fgets(buffer, sizeof(buffer), stdin);
  buffer[strcspn(buffer, "\n")] = 0;
  setName(&fiche, buffer);

  printf("Prénom: ");
  fgets(buffer, sizeof(buffer), stdin);
  buffer[strcspn(buffer, "\n")] = 0;
  setSurname(&fiche, buffer);

  printf("Numéro étudiant: ");
  fgets(buffer, sizeof(buffer), stdin);
  sscanf_s(buffer, "%d", &fiche.id);

  for (int i = 0; i < MAX_L_COURS; i++) {
    printf("Cours %d:\n", i + 1);
    fiche.courses[i] = courseFromInput();
  }

  return fiche;
}

/* Saisie d'un cours */
COURSE courseFromInput(void) {
  COURSE course;
  char buffer[100];

  printf("Nom du cours: ");
  fgets(buffer, sizeof(buffer), stdin);
  buffer[strcspn(buffer, "\n")] = 0;
  strncpy_s(course.title, MAX_L_CARAC, buffer, _TRUNCATE);
  toUpperCase(course.title);

  // Validation de la note
  do {
    printf("Note (entre 0 et 20): ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf_s(buffer, "%lf", &course.score);
    if (course.score < 0.0 || course.score > 20.0) {
      printf("Erreur : La note doit être entre 0 et 20.\n");
    }
  } while (course.score < 0.0 || course.score > 20.0);

  return course;
}


/* Fonction pour afficher tous les étudiants depuis le fichier */
void afficherTousLesEtudiants(const char* filename) {
  FILE* file;
  if (fopen_s(&file, filename, "r") != 0) {
    printf("Erreur d'ouverture du fichier %s. Il pourrait ne pas exister.\n", filename);
    return;
  }

  char line[256];
  while (fgets(line, sizeof(line), file)) {
    FICHE fiche;
    loadFiche(&fiche, line);
    printFiche(fiche);
    printf("\n---\n");
  }

  fclose(file);
}

/* Gestion du fichier en ligne de commande */
void exo5(int argc, char* argv[]) {
  char filename[100] = "database.csv";  // Fichier par défaut
  if (argc > 1) {
    strncpy_s(filename, sizeof(filename), argv[1], _TRUNCATE);
  }

  FILE* file;
  FICHE fiche;

  int choix;
  do {
    // Affichage du menu
    printf("\n  |==============================================================================|\n");
    printf("  |********************************** MENU PRINCIPAL*****************************|\n");
    printf("  |==============================================================================|\n");
    printf("  |======================== 1 - Ajouter un étudiant          ====================|\n");
    printf("  |======================== 2 - Afficher tous les étudiants  ====================|\n");
    printf("  |======================== 3 - Enregistrer un étudiant      ====================|\n");
    printf("  |======================== 4 - Quitter                      ====================|\n");
    printf("  |==============================================================================|\n");

    // Demande du choix à l'utilisateur avec validation
    do {
      printf("Votre choix: ");
      scanf_s("%d", &choix);
      (void)getchar();  // Vider le buffer
      if (choix < 1 || choix > 4) {
        printf("Choix invalide, veuillez entrer un numéro entre 1 et 4.\n");
      }
    } while (choix < 1 || choix > 4);

    // Traitement du choix de l'utilisateur
    switch (choix) {
    case 1:
      fiche = ficheFromInput();
      break;
    case 2:
      afficherTousLesEtudiants(filename);
      break;
    case 3:
      fopen_s(&file, filename, "a");  // Ouvre en mode append
      if (file) {
        saveFiche(file, fiche);
        fclose(file);
        printf("Fiche enregistrée dans %s\n", filename);
      }
      else {
        printf("Erreur d'ouverture du fichier %s\n", filename);
      }
      break;
    case 4:
      printf("Au revoir !\n");
      break;
    default:
      printf("Choix invalide.\n");
    }
  } while (choix != 4);
}



