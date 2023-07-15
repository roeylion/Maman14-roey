
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

unsigned int hash(const char* key) {
    unsigned int hashValue = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hashValue = 31 * hashValue + key[i];
    }
    return hashValue % TABLE_SIZE;
}

struct hashtable* createHashtable() {
    struct hashtable* ht = (struct hashtable*)malloc(sizeof(struct hashtable));
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->buckets[i] = NULL;
    }
    return ht;
}


void insert(struct hashtable* ht, struct label data) {
    unsigned int index = hash(data.name);

    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    newNode->data = data;
    newNode->next = NULL;

    if (ht->buckets[index] == NULL) {
        ht->buckets[index] = newNode;
    } else {
        struct node* current = ht->buckets[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}


struct label* search(struct hashtable* ht, const char* key) {
    unsigned int index = hash(key);

    struct node* current = ht->buckets[index];
    while (current != NULL) {
        if (strcmp(current->data.name, key) == 0) {
            return &(current->data);
        }
        current = current->next;
    }

    return NULL;
}


void freeHashtable(struct hashtable* ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        struct node* current = ht->buckets[i];
        while (current != NULL) {
            struct node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(ht);
}

