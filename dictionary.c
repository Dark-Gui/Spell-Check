#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"
#define LOAD_FACTOR_THRESHOLD 0.8

table_t *create_table() {
    table_t *new_table = malloc(sizeof(table_t));
    if (new_table == NULL) {
        free(new_table);
        return NULL; // Allocation failure
    }
    new_table->array = malloc(INITIAL_HASH_TABLE_SIZE * sizeof(list_node_t *));
    if (new_table->array == NULL) {
        free(new_table->array); // Clean up on failure
        return NULL;
    }
    // Initialize each element in the array to NULL
    for (unsigned int i = 0; i < INITIAL_HASH_TABLE_SIZE; ++i) {
        new_table->array[i] = NULL;
    }
    new_table->length = INITIAL_HASH_TABLE_SIZE;
    return new_table;
}



dictionary_t *create_dictionary() {
    dictionary_t *dict = malloc(sizeof(dictionary_t));
    if (dict == NULL) {
        free(dict);
        return NULL;
    }
    dict->table = create_table();
    if (dict->table == NULL) {
        free(dict);  // Free the dictionary if table creation fails
        return NULL;
    }
    dict->size = 0;
    return dict;
}


int hash_code(const char *word) {
    int hash = 0;
    while (*word) {
        hash = (hash << 5) + *word++;
    }
    return hash % INITIAL_HASH_TABLE_SIZE;
}

// table_t *resize_table(table_t *orig) {
//     // Double the size of the hash table
//     unsigned int new_length = orig->length * 2;
//     // Create a new table with the doubled size
//     //table_t *new_table = malloc(sizeof(table_t));
//     dictionary_t *dict = create_dictionary();
//     if (dict->table == NULL) {
//         return NULL; // Allocation failure
//     }
//     // Initialize each element in the new array to NULL
//     dict->table->array = malloc(new_length * sizeof(list_node_t *));
//     if (dict->table->array == NULL) {
//         free(dict->table); // Clean up on failure
//         return NULL;
//     }
//     for (unsigned int i = 0; i < new_length; ++i) {
//         dict->table->array[i] = NULL;
//     }
//     dict->table->length = new_length;
//     // Rehash and insert nodes from the original table to the new table
//     for (unsigned int i = 0; i < orig->length; ++i) {
//         list_node_t *current = orig->array[i];
//         while (current != NULL) {
//             // list_node_t *next = current->next;

//             // // Rehash the word and insert into the new table
//             // int index = hash_code(current->word) % dict->table->length;
//             // current->next = dict->table->array[index];
//             // dict->table->array[index] = current;

//             // current = next;
//             dict_insert(dict,current->word);
//             current = current->next;
//         }
//     }
//     // // Free the old table, but not the array
//     // for (unsigned int i = 0; i < orig->length; ++i) {
//     //     list_node_t *current = orig->array[i];
//     //     while (current != NULL) {
//     //         list_node_t *next = current->next;
//     //         current = next;
//     //         free(current);
//     //     }
//     // }
//     return dict->table;
// }


int dict_insert(dictionary_t *dict, const char *word) {
    if (dict == NULL || dict->table == NULL) {
        return 0; // Dictionary not properly initialized
    }
    // Check if resizing is needed
    // double load_factor = (double)dict->size / dict->table->length;
    // if (load_factor > LOAD_FACTOR_THRESHOLD) {
    //     dictionary_t *dict = create_dictionary();
    //     table_t *new_table = resize_table(dict->table);
    //     if(new_table == NULL) {
    //         fprintf(stderr, "Error: Failed to resize the table.\n");
    //         return -1; // Resize failure
    //     }
    //     dict->table = new_table;
    //     free(dict->table);
    // }
    // Check if the word already exists in the dictionary
    if (dict_find(dict, word)) {
        printf("Word '%s' already exists in the dictionary.\n", word);
        return 0; // Word already exists, no need to add
    }
    int index = hash_code(word) % dict->table->length;
    // Create a new node for the word
    list_node_t *new_node = malloc(sizeof(list_node_t));
    if (new_node == NULL){
        fprintf(stderr, "Error: Memory allocation failed for new_node.\n");
        return -1; // Allocation failure
    }
    strncpy(new_node->word, word, MAX_WORD_LEN - 1);
    new_node->word[MAX_WORD_LEN - 1] = '\0';
    new_node->next = NULL;
    // Check if the linked list is empty
    if (dict->table->array[index] == NULL) {
        dict->table->array[index] = new_node;
    } else {
        // Linked list is not empty, add the new node to the end
        list_node_t *current = dict->table->array[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    dict->size++; // Increase the size of the dictionary
    //printf("Word '%s' added to the dictionary.\n", word);
    return 0; // Word successfully added
}





int dict_find(const dictionary_t *dict, const char *query) {
    if (dict == NULL || dict->table == NULL) {
        return 0;  // Dictionary not properly initialized
    }
    int index = hash_code(query) % dict->table->length;
    list_node_t *current = dict->table->array[index];
    while (current != NULL) {
        if (strcmp(current->word, query) == 0) {
            return 1;  // Word found
        }
        current = current->next;
    }
    return 0;  // Word not found
}





void dict_print(const dictionary_t *dict) {
    if (dict == NULL || dict->table == NULL) {
        printf("Dictionary is empty.\n");
        return;
    }
    for (int i = 0; i < dict->table->length; i++) {
        list_node_t *current = dict->table->array[i];
        while (current != NULL) {
            printf("%s\n", current->word);
            current = current->next;
        }
    }
}




void dict_free(dictionary_t *dict) {
    if (dict == NULL) {
        return;
    }
    // Free the hash table and its elements
    if (dict->table != NULL) {
        for (unsigned int i = 0; i < dict->table->length; ++i) {
            list_node_t *current = dict->table->array[i];
            while (current != NULL) {
                list_node_t *next = current->next;
                free(current);
                current = next;
            }
        }
        free(dict->table->array);
        free(dict->table);
    }
    // Free the dictionary structure
    free(dict);
}



dictionary_t *read_dict_from_text_file(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Failed to read dictionary from text file\n");
        return NULL;  // File opening failed
    }
    dictionary_t *dict = create_dictionary();
    if (dict == NULL){
        fclose(file);
        return NULL;  // Dictionary creation failed
    }
    char word[MAX_WORD_LEN];
    while (fscanf(file, "%s", word) != EOF) {
        if (dict_insert(dict, word) == -1) {
            fclose(file);
            dict_free(dict);  // Free the dictionary if insertion fails
            return NULL;
        }
    }
    fclose(file);
    return dict;
}


int write_dict_to_text_file(const dictionary_t *dict, const char *file_name) {
    if (dict == NULL || file_name == NULL) {
        return -1; // Invalid input
    }
    FILE *file = fopen(file_name, "w");
    if (file == NULL) {
        return -1; // File opening failed
    }
    for (unsigned int i = 0; i < dict->table->length; ++i) {
        list_node_t *current = dict->table->array[i];
        while (current != NULL) {
            fprintf(file, "%s\n", current->word);
            current = current->next;
        }
    }
    fclose(file);
    return 0; // Successful writing
}





