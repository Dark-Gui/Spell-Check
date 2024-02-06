#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "dictionary.h"

#define MAX_CMD_LEN 128

// A helper function to spell check a specific file
// 'file_name': Name of the file to spell check
// 'dict': A dictionary containing correct words
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int spell_check_file(const char *file_name, const dictionary_t *dict) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        return -1; // Unable to open file
    }
    char line[MAX_CMD_LEN];
    while (fgets(line, sizeof(line), file) != NULL) {
        char *word = strtok(line, " \n");  // Split line into words
        while (word != NULL) {
            if (!dict_find(dict, word)) {
                printf("%s[X] ", word);
            } else {
                printf("%s ", word);
            }
            word = strtok(NULL, " \n");
        }
        printf("\n");
    }
    fclose(file);
    return 0; // Spell check completed
}






/*
 * This is in general *very* similar to the list_main file seen in lab
 */
int main(int argc, char **argv) {
//    dictionary_t *dict = create_dictionary();
//     if (dict == NULL) {
//         fprintf(stderr, "Error: Failed to initialize the dictionary.\n");
//         return 1;
//     }
dictionary_t *dict = create_dictionary();
    if (dict == NULL) {
        free(dict);
        fprintf(stderr, "Error: Failed to initialize the dictionary.\n");
        return 1;
    }

    if (argc == 2) {
        // Case: ./spell_check <dictionary_file>
        char *dictionary_file = argv[1];
        dictionary_t *new_dict = read_dict_from_text_file(dictionary_file);
        if (new_dict != NULL) {
            // Free the old dictionary and replace it with the new one
            printf("Dictionary successfully read from text file\n");
            dict_free(dict);
            dict = new_dict;
        } else {
            //fprintf(stderr, "Error: Failed to read dictionary from file '%s'.\n", dictionary_file);
            dict_free(dict);
            return 1;
        }
    } else if (argc == 3) {
        //printf("Dictionary successfully read from text file\n");
        // Case: ./spell_check <dictionary_file> <spell_check_file>
        char *dictionary_file = argv[1];
        //char *spell_check_fil = argv[2];
        dictionary_t *new_dict = read_dict_from_text_file(dictionary_file);
        if (new_dict == NULL) {
            dict_free(dict);
            return 1;
        }
        //int spell_check_result = spell_check_file(argv[2], new_dict);
        if(dictionary_file != NULL){
            printf("Dictionary successfully read from text file\n");
        }
        int spell_check_result = spell_check_file(argv[2], new_dict);
        if (spell_check_result != 0){
            printf("Spell check failed");
        }else if (spell_check_result == 1){
            printf("Failed to read dictionary from text file\n");
        }
        // Free the dictionary
        dict_free(new_dict);
        dict_free(dict);
        return 0;
    }
    char cmd[MAX_CMD_LEN];
    printf("CSCI 2021 Spell Check System\n");
    printf("Commands:\n");
    printf("  add <word>:              adds a new word to dictionary\n");
    printf("  lookup <word>:           searches for a word\n");
    printf("  print:                   shows all words currently in the dictionary\n");
    printf("  load <file_name>:        reads in dictionary from a file\n");
    printf("  save <file_name>:        writes dictionary to a file\n");
    printf("  check <file_name>: spell checks the specified file\n");
    printf("  exit:                    exits the program\n");

    while (1) {
        printf("spell_check> ");
        if (scanf("%s", cmd) == EOF) {
            printf("\n");
            break;
        }

        if (strcmp("exit", cmd) == 0) {
            break;
        } else if (strcmp("add", cmd) == 0) {
            // Add a new word to the dictionary
            char word[MAX_WORD_LEN];
            if (scanf("%s", word) == EOF) {
                printf("Error: Missing argument for 'add' command.\n");
                break;
            }
            if (dict_insert(dict, word) == 0) {
            }
        } else if (strcmp("lookup", cmd) == 0) {
            // Search for a word in the dictionary
            char word[MAX_WORD_LEN];
            if (scanf("%s", word) == EOF) {
                printf("Error: Missing argument for 'lookup' command.\n");
                break;
            }
            // printf("%i",dict_find(dict, word));
            
            if (dict_find(dict, word)) {
                printf("'%s' present in dictionary\n", word);
            }else{
                printf("'%s' not found\n", word);
            }
        } else if (strcmp("print", cmd) == 0) {
            // Print all words in the dictionary
            dict_print(dict);
        } else if (strcmp("load", cmd) == 0) {
            // Load dictionary from a file
            char file_name[MAX_CMD_LEN];
            if (scanf("%s", file_name) == EOF) {
                printf("Error: Missing argument for 'load' command.\n");
                break;
            }
            dictionary_t *new_dict = read_dict_from_text_file(file_name);
            if (new_dict != NULL) {
                // Free the old dictionary and replace it with the new one
                dict_free(dict);
                dict = new_dict;
                printf("Dictionary successfully read from text file\n");
            } else {
                //printf("Error: Failed to load dictionary from file '%s'.\n", file_name);
            }
        } else if (strcmp("save", cmd) == 0) {
            // Save dictionary to a file
            char file_name[MAX_CMD_LEN];
            if (scanf("%s", file_name) == EOF) {
                printf("Error: Missing argument for 'save' command.\n");
                break;
            }
            if (write_dict_to_text_file(dict, file_name) == 0) {
                printf("Dictionary successfully written to text file\n");
            } else {
                printf("Error: Failed to save dictionary to file '%s'.\n", file_name);
            }
        } else if (strcmp("check", cmd) == 0) {
            // Spell check a specified file
            char file_name[MAX_CMD_LEN];
            if (scanf("%s", file_name) == EOF) {
                printf("Error: Missing argument for 'check' command.\n");
                break;
            }
            if (spell_check_file(file_name, dict) == 0) {
                //printf("Spell check completed for file '%s'.\n", file_name);
            } else {
                printf("Spell check failed\n");
            }
        } else {
            printf("Unknown command %s\n", cmd);
        }
    }
    dict_free(dict);
    return 0;
}

