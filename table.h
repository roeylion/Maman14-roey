
#ifndef TABLE_H
#define TABLE_H
#include "globals.h"
#define TABLE_SIZE 100

struct node {
    struct label data;
    struct node* next;
};

struct hashtable {
    struct node* buckets[TABLE_SIZE];
};

/*
 * Calculates the hash value for a given key (label name).
 *
 * @param key The label name.
 * @return The calculated hash value.
 */
unsigned int hash(const char* key);

/*
 * Creates a new hash table.
 *
 * @return A pointer to the created hash table.
 */
struct hashtable* createHashtable();

/*
 * Inserts a new label into the hash table.
 *
 * @param ht The hash table.
 * @param data The label data to be inserted.
 */
void insert(struct hashtable* ht, struct label data);

/*
 * Searches for a label in the hash table based on the label name.
 *
 * @param ht The hash table.
 * @param key The label name to search for.
 * @return A pointer to the found label, or NULL if not found.
 */
struct label* search(struct hashtable* ht, const char* key);

/*
 * Frees the memory allocated for the hash table and its nodes.
 *
 * @param ht The hash table.
 */
void freeHashtable(struct hashtable* ht);
#endif
