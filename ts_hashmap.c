#include <limits.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ts_hashmap.h"


/**
 * Creates a new thread-safe hashmap. 
 *
 * @param capacity initial capacity of the hashmap.
 * @return a pointer to a new thread-safe hashmap.
 */
ts_hashmap_t *initmap(int capacity) {
  // TODO
ts_hashmap_t *data = (ts_hashmap_t*) malloc(sizeof(struct ts_hashmap_t) * capacity);  
data->capacity = capacity;
data->size = 0;
data->table = (ts_entry_t**) malloc(sizeof(ts_entry_t*) * capacity);
for (int i = 0; i < capacity; i++){
    data->table[i] = NULL;
  }
return data;
}

/**
 * Obtains the value associated with the given key.
 * @param map a pointer to the map
 * @param key a key to search
 * @return the value associated with the given key, or INT_MAX if key not found
 */
int get(ts_hashmap_t *map, int key) {
  // TODO
  int pos = ((unsigned int)key) % map->capacity;
  ts_entry_t *entry = map->table[pos];

  while(entry != NULL && entry->next != NULL){
    if(entry->key == key){
      return entry->value;
    }
    entry = entry->next;
  }

  if(entry != NULL && entry->key == key){
    printf("The gotten value is: %d \n", entry->value);
    return entry->value;
  }
  return INT_MAX;
}


/**
 * Associates a value associated with a given key.
 * @param map a pointer to the map
 * @param key a key
 * @param value a value
 * @return old associated value, or INT_MAX if the key was new
 */
int put(ts_hashmap_t *map, int key, int value) {
  // TODO
  unsigned int pos = ((unsigned int)key) % map->capacity;
  ts_entry_t **hashMap = map->table;
  ts_entry_t *entry = hashMap[pos];

  while(entry != NULL && entry->key != key){
    entry = entry->next;
  }

  if(entry == NULL){
    entry = (ts_entry_t*) malloc(sizeof(ts_entry_t));
    entry->key = key;
    entry->value = value;
    entry->next = hashMap[pos];
    hashMap[pos] = entry;
    map->size += 1;
    return INT_MAX;
    
  }else{
    int temp = entry->value;
    entry->value = value;
    return temp;
  }
}

/**
 * Removes an entry in the map
 * @param map a pointer to the map
 * @param key a key to search
 * @return the value associated with the given key, or INT_MAX if key not found
 */
int del(ts_hashmap_t *map, int key) {
  // TODO
  unsigned int pos = ((unsigned int)key) % map->capacity;
  ts_entry_t **hashMap = map->table;
  ts_entry_t *entry =  hashMap[pos];
  ts_entry_t *entryNext;
  

  while(entry != NULL && entry->key != key){
    entryNext = entry;
    entry = entry->next;
  }

  if(entry == NULL){
    return INT_MAX;
  }else{
    int temp = entry->value;
    if(entryNext == NULL){
      map->table[pos] = entry->next;
    }else {
      entryNext->next = entry->next;
    }
    free(entry);
    map->size--;
    return temp;
  }
}


/**
 * @return the load factor of the given map
 */
double lf(ts_hashmap_t *map) {
  return (double) map->size / map->capacity;
}

/**
 * Prints the contents of the map
 */
void printmap(ts_hashmap_t *map) {
  for (int i = 0; i < map->capacity; i++) {
    printf("[%d] -> ", i);
    ts_entry_t *entry = map->table[i];
    while (entry != NULL) {
      printf("(%d,%d)", entry->key, entry->value);
      if (entry->next != NULL)
        printf(" -> ");
      entry = entry->next;
    }
    printf("\n");
  }
}

