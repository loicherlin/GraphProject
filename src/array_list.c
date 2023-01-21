#include "../include/array_list.h"
#include "../include/data_t.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Fonction list_create
 *
 * crée une liste vide.
 * Retourne 0 en cas de problème.
 */

list_t *list_create()
{
    list_t *l = malloc(sizeof(list_t));
    if (l == 0)
    {
        return 0;
    }
    l->allocated = 0;
    l->size = 0;
    l->tab = NULL;
    return l;
}

/**
 * Fonction list_append
 *
 * Ajoute un élément à la fin de la liste
 * en cas de problème, retourne 0
 * si tout se passe bien, retourne 1
 */

int list_append(list_t *list, void *element)
{
    if (list == 0)
    {
        return 0;
    }
    if (list->allocated == 0)
    {
        list->allocated = 1;
        list->tab = malloc(sizeof(void *));
        if (list->tab == 0)
        {
            return 0;
        }
    }
    else if (list->allocated == list->size)
    {
        list->allocated *= 2;
        list->tab = realloc(list->tab, list->allocated * sizeof(void *));
        if (list->tab == 0)
        {
            return 0;
        }
    }
    list->tab[list->size] = element;
    list->size++;
    return 1;
}

/**
 * Fonction list_prepend
 *
 * Ajoute un élément au début de la liste
 * en cas de problème, retourne 0
 * si tout se passe bien, retourne 1
 */
// https://koor.fr/C/cstring/memmove.wp

int list_prepend(list_t *list, void *element)
{
    if (list->size == 0)
    {
        if (list_append(list, element) == 0)
        {
            return 0;
        }
        else
        {
            return 1;
        };
    }
    else if (list->size == list->allocated)
    {
        void **tmp = realloc(list->tab, sizeof(void *) * list->allocated * 2);
        if (tmp == 0)
        {
            return 0;
        }
        list->allocated *= 2;
        list->tab = tmp;
    }
    memmove(list->tab + 1, list->tab, sizeof(void *) * list->size);
    list->tab[0] = element;
    list->size++;
    return 1;
}

/**
 * Fonction list_insert
 *
 * insert un élémént à la position idx
 * retourn 0 s'il y a un problème, 1 sinon
 */

int list_insert(list_t *list, int idx, void *element)
{
    if (0 == list->size)
    {
        if (list_append(list, element) == 0)
        {
            return 0;
        }
    }
    else if (list->size == list->allocated)
    {
        void **tmp = realloc(list->tab, sizeof(void *) * list->allocated * 2);
        if (tmp == 0)
        {
            return 0;
        }
        list->allocated *= 2;
        list->tab = tmp;
    }
    memmove(list->tab + idx + 1, list->tab + idx, sizeof(void *) * (list->size - idx));
    list->tab[idx] = element;
    list->size++;
    return 1;
}

/**
 * Fonction list_get
 *
 * retourne le (idx+1)-ieme élément.
 * En cas de dépassement, le comportement est indéfini
 * (très probablement une segfault)
 */

void *list_get(list_t *list, int idx) { return list->tab[idx]; }

/**
 * Fonction list_set
 *
 * remplace l'élément à la position donnée par l'élément donné
 */

void list_set(list_t *list, int idx, void *element) { list->tab[idx] = element; }

/**
 * Fonction list_take
 *
 * retourne l'élément à la position idx, puis enlève cet élément de la liste
 */
void *list_take(list_t *list, int idx)
{
    void *tmp = list->tab[idx];
    memmove(list->tab + idx, list->tab + idx + 1, sizeof(void *) * (list->size - idx - 1));
    list->size--;
    return tmp;
}

/**
 * Fonction list_size
 *
 * retourne la taille de la liste
 */
size_t list_size(list_t *list)
{
    return list->size; // remove +1
}

/**
 * Fonction list_free
 *
 * libère la liste (et pas les éléments)
 */
void list_free(list_t *list)
{
    if (list->tab != NULL)
    {
        free(list->tab);
    }
    free(list);
}

// list_remove
void list_remove(list_t *list, void *to_remove)
{
    for (size_t i = 0; i < list->size; i++)
    {
        if (list->tab[i] == to_remove)
        {
            list_take(list, i);
        }
    }
}

void list_sort(list_t *list, int (*compare_function)(const void *, const void *))
{
    qsort(list->tab, list->size, sizeof(void *), compare_function);
}