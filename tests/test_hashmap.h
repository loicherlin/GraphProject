#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/args_parser.h"
#include "../include/array_list.h"
#include "../include/bin_builder.h"
#include "../include/cprintf.h"
#include "../include/csv_parser.h"
#include "../include/data_t.h"
#include "../include/delaunay.h"
#include "../include/graph.h"
#include "../include/handler.h"
#include "../include/min_heap.h"
#include "../include/triangle.h"
#include "../include/visualise.h"
#include "../include/hashmap.h"

#include "tps_unit_test.h"

void test_hashmap_init(void){
    hashmap_t* map = hashmap_init(10);
    float value1 = 123;
    float value2 = 456.0f;
    float value3= 121.0f;

    hashmap_insert(map, "key1", &value1, sizeof(int));
    hashmap_insert(map, "key2", &value2, sizeof(float));
    hashmap_insert(map, "key3", &value3, sizeof(sizeof(float)));

    float*retrieved_value1 = (float*)hashmap_get(map, "key1");
    float *retrieved_value2 = (float*)hashmap_get(map, "key2");
    float *retrieved_value3 = (float*)hashmap_get(map, "key3");

    assert(*retrieved_value1 == value1);
    assert(*retrieved_value2 == value2);
    assert(*retrieved_value3 == value3);

    hashmap_remove(map, "key1");
    assert(map->count == 2);
    assert(hashmap_get(map, "key1") == NULL);

    // iterate over the hashmap
    hashmap_iter_t iter = {0};
    hashmap_iter_init(&iter, map);
    while (hashmap_iter_has_next(&iter)) {
      if(!iter.node->deleted){
        char *key = hashmap_iter_get_key(&iter);
        void *value = hashmap_iter_get_value(&iter);
        printf("key: %s, value: %f\n", key, *(float*)value);
      }
      hashmap_iter_next(&iter);
    }

    hashmap_free(map);
    free(map);
}


void test_hashmap_insert_a_lot(void){
    size_t size = 220000;
    size_t values[size];
    for(size_t i = 0; i < size; i++){
        values[i] = i;
    }

    hashmap_t* map = hashmap_init(10);
    for(size_t i = 0; i < size; i++){
        size_t value = i;
        char key[10];
        sprintf(key, "%ld", i);
        hashmap_insert(map, key, &value, sizeof(size_t));
    }
    
    // iterate over the hashmap
    hashmap_iter_t iter;
    hashmap_iter_init(&iter, map);
    while (hashmap_iter_has_next(&iter)) {
      if(!iter.node->deleted){
        char *key = hashmap_iter_get_key(&iter);
        void *value = hashmap_iter_get_value(&iter);
        assert(*(size_t*)value == values[strtol(key, NULL, 10)]);
      }
      hashmap_iter_next(&iter);
    }
    size_t count = map->count;
    assert(count == size);

    size_t* tmp = (size_t*)hashmap_get(map, "219999");
    printf("tmp: %ld\n", *tmp);
    assert(*tmp == 220000-1);

    size_t* tmp2 = (size_t*)hashmap_get(map, "220000");
    assert(tmp2 == NULL);


    hashmap_free(map);
    free(map);
}

void test_hashmap_sort(void){
  hashmap_t* map = hashmap_init(10);
  size_t size = 1000000;
  for(size_t i = 0; i < size; i++){
    size_t value = i;
    char key[10];
    sprintf(key, "%ld", i);
    hashmap_insert(map, key, &value, sizeof(size_t));
  }

  hashmap_sort_keys(map);

  hashmap_iter_t iter;
  hashmap_iter_init(&iter, map);
  size_t i = 0;
  while (hashmap_iter_has_next(&iter)) {
    if(!iter.node->deleted){
      char *key = hashmap_iter_get_key(&iter);
      void *value = hashmap_iter_get_value(&iter);
      assert(i == strtol(key, NULL, 10));
      //printf("key: %s, value: %ld\n", key, *(size_t*)value);
      i++;
    }
    hashmap_iter_next(&iter);
  }

  hashmap_free(map);
  free(map);
}

void unit_test_hashmap(void){
  TEST(test_hashmap_init);
  TEST(test_hashmap_insert_a_lot);
  TEST(test_hashmap_sort);
}