# AnalyzeText - C

Welcome to this C project ! The goal of this program is to analyze text files to count occurrences of specific characters or words, calculate their probabilities, and generate a detailed report. This project was designed as an academic exercise, with a desire to explore the fundamental concepts of the C language: file management, string manipulation, and the use of command-line arguments.

## How this program works

This program allows :

- Analyze one or more user-specified text files.
- Count the occurrences of a specific character or word.
- Calculate the probabilities of characters appearing (optional).
- Generate a report of the results either to the console or to a specified file.

## What is a function?

A function is a set of instructions grouped together to accomplish a specific task. For example, here is a function used in this project to count characters in a file :

```c
int compter_caracteres(FILE *fichier, const char *caractere, int *total_count, int *target_count) {
    int c;
    *total_count = 0;
    *target_count = 0;

    while ((c = fgetc(fichier)) != EOF) { // Lire chaque caractère du fichier
        (*total_count)++;
        if (caractere != NULL && c == caractere[0]) {
            (*target_count)++; // Incrémenter si le caractère correspond
        }
    }
    return 0; // Retourner 0 pour indiquer le succès
}
```
### Quick explanation :

- ```FILE *fichier``` : Pointer to the file to be analyzed.
- ``` const char *caractere ``` : Target character to search for.
- ``` int *total_count ``` : Counter for all characters.
- ``` int *target_count ``` : Counter for occurrences of the target character.

## Command Line Arguments
This program uses command line arguments to customize the analysis. Here are the available options :
| Option | Description |
| :----- |:-----------:|
| --help or -h  |   Displays help and usage instructions.        |
| --input <chemin> or -i  |  Path to one or more files to scan. Can be used multiple times.       |
| --report <chemin> or -r  |   Path to the file to save the report to (otherwise, standard output).        |
| --char <caractère> or -c  |   Character to be analyzed (e.g., a).       |
| --prob or -p |  Calculates the probabilities of characters or words appearing.        |

### Exemple d’utilisation :
```bash 
./AnalyzeText --input texte.txt --char e --report rapport.txt
```
## Conclusion
This project is a great example of applying basic C language concepts. It gives you practice with fundamentals like file handling, conditional structures, and program arguments.
