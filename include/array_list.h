#pragma once
#include <stddef.h>
#include <stdio.h>
struct list;
typedef struct list list_t;

struct list
{
    void **tab;
    size_t allocated;
    size_t size;
};

/**
 * Fonction list_create
 *
 * crée une liste vide.
 * Retourne NULL en cas de problème.
 */

list_t *list_create();

/**
 * Fonction list_append
 *
 * Ajoute un élément à la fin de la liste
 * en cas de problème, retourne 0
 * si tout se passe bien, retourne 1
 */
int list_append(list_t *list, void *element);

/**
 * Fonction list_prepend
 *
 * Ajoute un élément au début de la liste
 * en cas de problème, retourne 0
 * si tout se passe bien, retourne 1
 */

int list_prepend(list_t *list, void *element);

/**
 * Fonction list_insert
 *
 * insert un élémént à la position idx
 * retourn 0 s'il y a un problème, 1 sinon
 */

int list_insert(list_t *list, int idx, void *element);

/**
 * Fonction list_get
 *
 * retourne le (idx+1)-ieme élément.
 * En cas de dépassement, le comportement est indéfini
 * (très probablement une segfault)
 */

void *list_get(list_t *list, int idx);

/**
 * Fonction list_set
 *
 * remplace l'élément à la position donnée par l'élément donné
 */

void list_set(list_t *list, int idx, void *element);

/**
 * Fonction list_take
 *
 * retourne l'élément à la position idx, puis enlève cet élément de la liste
 */
void *list_take(list_t *list, int idx);

/**
 * Fonction list_size
 *
 * retourne la taille de la liste
 */
size_t list_size(list_t *list);

/**
 * Fonction list_free
 *
 * libère la liste (et pas les éléments)
 */
void list_free(list_t *list);

void list_remove(list_t *list, void *to_remove);

void list_sort(list_t *list, int (*compare_function)(const void *, const void *));