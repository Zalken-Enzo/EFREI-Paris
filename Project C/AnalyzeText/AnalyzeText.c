#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

void afficher_aide() {
    printf("\nAffichage de l'aide:\n");
    printf("Usage :\n");
    printf("  --help, -h           Affiche l'aide\n");
    printf("  --input <chemin>, -i Fournit le chemin du fichier à analyser (peut être utilisé plusieurs fois)\n");
    printf("  --report <chemin>, -r Fournit le chemin du rapport à générer (par défaut sortie standard)\n");
    printf("  --char <caractère>, -c Fournit le caractère ou mot dont on doit calculer les statistiques\n");
    printf("  --prob, -p           Calcule les probabilités d'apparition des caractères ou mots dans les fichiers\n");
}

int compter_caracteres(FILE *fichier, const char *caractere, int *total_count, int *target_count) {
    int c;
    *total_count = 0;
    *target_count = 0;

    while ((c = fgetc(fichier)) != EOF) {
        (*total_count)++;
        if (caractere != NULL && c == caractere[0]) {
            (*target_count)++;
        }
    }
    return 0;
}

void analyser_fichier(FILE *fichier, const char *chemin, const char *caractere, const char *mot, FILE *rapport, int prob_flag) {
    int total_count = 0, target_count = 0;
    int mot_count = 0;
    char buffer[1024];

    // Compter les caractères ou les mots
    if (caractere != NULL) {
        compter_caracteres(fichier, caractere, &total_count, &target_count);
    } else if (mot != NULL) {
        fseek(fichier, 0, SEEK_SET); // Rewind file pointer
        while (fscanf(fichier, "%1023s", buffer) != EOF) {
            if (strcmp(buffer, mot) == 0) {
                mot_count++;
            }
        }
    }

    fprintf(rapport, "Analyse du fichier: %s\n", chemin);

    if (caractere != NULL) {
        fprintf(rapport, "Nombre total de caractères: %d\n", total_count);
        fprintf(rapport, "Occurrences du caractère '%c': %d\n", caractere[0], target_count);
        if (prob_flag) {
            fprintf(rapport, "Probabilité du caractère '%c': %.2f%%\n", caractere[0], (float)target_count / total_count * 100);
        }
    } else if (mot != NULL) {
        fprintf(rapport, "Occurrences du mot '%s': %d\n", mot, mot_count);
    }
}

int main(int argc, char *argv[]) {
    int c;
    int help_flag = 0;
    int input_flag = 0;
    int report_flag = 0;
    int char_flag = 0;
    int prob_flag = 0;

    char *chemin_fichier[10]; // Tableau pour stocker plusieurs chemins de fichiers
    int file_count = 0;
    char *caractere = NULL;
    char *mot = NULL;
    FILE *rapport = stdout; // Par défaut, la sortie standard

    struct option long_options[] = {
        {"help", no_argument, &help_flag, 'h'},
        {"input", required_argument, NULL, 'i'},
        {"report", required_argument, NULL, 'r'},
        {"char", required_argument, NULL, 'c'},
        {"prob", no_argument, &prob_flag, 'p'},
        {0, 0, 0, 0}
    };

    // Traitement des options avec getopt_long
    while ((c = getopt_long(argc, argv, "hi:r:c:p", long_options, NULL)) != -1) {
        switch (c) {
            case 'h':
                afficher_aide();
                return 0;
            case 'i':
                if (file_count < 10) {
                    chemin_fichier[file_count++] = optarg;
                } else {
                    fprintf(stderr, "Erreur: Trop de fichiers spécifiés.\n");
                }
                input_flag = 1;
                break;
            case 'r':
                rapport = fopen(optarg, "w");
                if (rapport == NULL) {
                    fprintf(stderr, "Erreur: Impossible d'ouvrir le fichier de rapport '%s'.\n", optarg);
                    return 1;
                }
                report_flag = 1;
                break;
            case 'c':
                caractere = optarg;
                char_flag = 1;
                break;
            case 'p':
                prob_flag = 1;
                break;
            default:
                fprintf(stderr, "Option invalide. Utilisez --help pour l'aide.\n");
                return 1;
        }
    }

    // Si aucun fichier n'est fourni, utiliser l'entrée standard
    if (file_count == 0) {
        chemin_fichier[file_count++] = "stdin";
    }

    // Traiter chaque fichier spécifié
    for (int i = 0; i < file_count; i++) {
        FILE *fichier = NULL;
        if (strcmp(chemin_fichier[i], "stdin") == 0) {
            fichier = stdin;
            fprintf(rapport, "Analyse de l'entrée standard:\n");
        } else {
            fichier = fopen(chemin_fichier[i], "r");
            if (fichier == NULL) {
                fprintf(stderr, "Erreur: Impossible d'ouvrir le fichier '%s'.\n", chemin_fichier[i]);
                continue;
            }
            fprintf(rapport, "Analyse du fichier: %s\n", chemin_fichier[i]);
        }

        // Effectuer l'analyse en fonction des options spécifiées
        if (char_flag) {
            analyser_fichier(fichier, chemin_fichier[i], caractere, NULL, rapport, prob_flag);
        } else if (mot != NULL) {
            analyser_fichier(fichier, chemin_fichier[i], NULL, mot, rapport, prob_flag);
        } else {
            analyser_fichier(fichier, chemin_fichier[i], NULL, NULL, rapport, prob_flag);
        }

        // Fermer le fichier si ce n'est pas l'entrée standard
        if (fichier != stdin) {
            fclose(fichier);
        }
    }

    // Si le fichier de rapport a été ouvert, le fermer
    if (report_flag) {
        fclose(rapport);
    }

    return 0;
}

/*
Projet : Analyse de fichiers en C
Auteur : Zalken - Enzo
Contexte : Projet académique
*/
