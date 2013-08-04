/*****************************************************************************/
/**
@ingroup        simple-c-dict

@file

@brief          dict implementation

@author         Toni Homedes i Saun

@copyright      2013 Toni Homedes i Saun

This file is part of simple-c-dict.

simple-c-dict is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

simple-c-dict is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with simple-c-dict.  If not, see <http://www.gnu.org/licenses/>.

*/
/*****************************************************************************/
#include "dict.h"
#include "djb2.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#ifndef NDEBUG
#    define FREE(a) do { free((void *)a); (a) = NULL; } while (0)
#else
#    define FREE(a) free((void *)a)
#endif

struct dict_node {
    const char *        key;
    unsigned long       hash;
    const char *        value;
    struct dict_node *  next;
};

struct dict {
    int replace;
    unsigned hash_table_size;
    struct dict_node **hash_table;
    long unsigned (*hashfnc)(const char key[]);
};

/* check dict */
#define check_dict(d)				\
    ((d) != NULL				\
     && ((d)->replace & ~1) == 0		\
     && (d)->hash_table_size > 0		\
     && (d)->hash_table != NULL			\
     && (d)->hashfnc != NULL)

/* check node invariant */
#define check_node(dnp)				\
    ((dnp) != NULL				\
     && (dnp)->key != NULL			\
     && (dnp)->value != NULL)

#define check_node_k(dnp, _hash, _key)			\
    (check_node(dnp)					\
     && (dnp)->hash == (_hash)				\
     && !strcmp((dnp)->key, (_key)))

#define check_node_k_v(dnp, _hash, _key, _value)	\
    (check_node_k(dnp, _hash, _key)			\
     && !strcmp((dnp)->value, (_value)))

/* check indirect node invariant */
#define check_ind_node(dnpp)			\
    ((dnpp) != NULL				\
     && check_node(*dnpp))

#define check_ind_node_k(dnpp, hash, key)	\
    ((dnpp) != NULL				\
     && check_node_k(*dnpp, hash, key))

/*---------------------------------------------------------------------------*/
/**
@brief          compute the correct hash_table entry for given key

@returns        A pointer to the hash table entry

*/
/*---------------------------------------------------------------------------*/
static struct dict_node **
hash_table_entry(const struct dict * const d, unsigned long hash) {
    assert(check_dict(d));

    return d->hash_table + (hash % d->hash_table_size);
}

/*---------------------------------------------------------------------------*/
/**
@brief          search the node for given key

@returns        A pointer to the pointer referencing the node or NULL if not
                found.
*/
/*---------------------------------------------------------------------------*/
static struct dict_node **
dict_node_owner(
    const struct dict * const d, 
    unsigned long hash, 
    const char key[]) 
{
    struct dict_node **p;

    /* Verify pre-condition */
    assert(check_dict(d));

    for (p = hash_table_entry(d, hash);
	 *p != NULL && ((*p)->hash != hash || strcmp((*p)->key, key));
	 p = &(*p)->next)
	;

    if (*p == NULL)  p = NULL;

    /* Verify post-condition */
    assert(p == NULL
	   || check_ind_node_k(p, hash, key));

    return p;
}

/*---------------------------------------------------------------------------*/
static bool
dict_node_insert(
    struct dict_node **p,
    unsigned long hash,
    const char key[], 
    const char value[]
) {
    struct dict_node *n;
    bool result = 1;

    /* Verify pre-condition */
    assert(p != NULL);

    n = (struct dict_node *)malloc(sizeof (struct dict_node));
    if (n != NULL) {
	n->key      = strdup(key);
	n->hash     = hash;
	n->value    = strdup(value);
	n->next     = *p;

	if (n->key == NULL || n->value == NULL) {
	    FREE(n->key);
	    FREE(n->value);
	    FREE(n);
	    result = 0;
	} else {
	    *p = n;
	}
    } else {
	result = 0;
    }

    /* Verify post-condition */
    assert((result 
	    && check_node_k_v(*p, hash, key, value))
	   || n == NULL);

    return 1;
}

/*---------------------------------------------------------------------------*/
static void
dict_node_free(struct dict_node **p) {
    struct dict_node *tmp;

    assert(check_ind_node(p));

    tmp = (*p)->next;

    FREE((*p)->key);
    FREE((*p)->value);
    FREE(*p);

    *p = tmp;
}

/*****************************************************************************\

    P U B L I C   I N T E R F A C E

\*****************************************************************************/

/*---------------------------------------------------------------------------*/
dicthdl dict_new(
    bool     replace,
    unsigned hash_table_size,
    unsigned long (*hashfnc)(const char key[]))
{
    unsigned i;
    struct dict *d;
    struct dict_node **h;

    assert(hash_table_size > 0);

    if (hash_table_size == 0)  return (dicthdl)NULL;

    d = (struct dict *)malloc(sizeof (struct dict));
    if (d == NULL)  return (dicthdl)NULL;

    h = ((struct dict_node **) malloc(hash_table_size 
				      * sizeof (struct dict_node *)));

    if (h == NULL) {
	free((void *)d);
	return (dicthdl)NULL;
    }

    for (i = 0; i < hash_table_size; ++i)  h[i] = NULL;

    d->replace          = replace != false;
    d->hash_table_size  = hash_table_size;
    d->hash_table       = h;
    d->hashfnc          = hashfnc != NULL ? hashfnc : djb2;

    /* check post condition */
    assert(check_dict(d));
   
    return (dicthdl)(void *)d;
}

/*---------------------------------------------------------------------------*/
bool
dict_free(dicthdl dict) {
    struct dict *d = (struct dict *)dict;
    unsigned int i;

    if (dict == NULL)  return true;

    assert(check_dict(d));
    
    /* Empty all lists in all hash table entries */
    for (i = 0; i < d->hash_table_size; ++i) {
	while (d->hash_table[i] != NULL) {
	    dict_node_free(d->hash_table + i);
	}
    }
    free(d->hash_table);
    free(d);
    return true;
}

/*---------------------------------------------------------------------------*/
bool 
dict_set(dicthdl dict, const char key[], const char value[]) {
    struct dict *d = (struct dict *)dict;
    struct dict_node **p;
    unsigned long hash;

    assert(check_dict(d));

    hash = d->hashfnc(key);

    if (d->replace) {
	p = dict_node_owner(d, hash, key);
	if (*p == NULL) {
	    return dict_node_insert(hash_table_entry(d, hash), hash, key, value);
	} else {
	    const char *nv = strdup(value);

	    if (nv == NULL)  return false;

	    free((void *)(*p)->value);
	    (*p)->value = nv;
	    return true;
	}
    } else {
	return dict_node_insert(hash_table_entry(d, hash), hash, key, value);
    }
}

/*---------------------------------------------------------------------------*/
const char *dict_get(dicthdl dict, const char key[]) {
    struct dict *d = (struct dict *)dict;
    struct dict_node **p;
    unsigned long hash;

    assert(check_dict(d));

    hash = d->hashfnc(key);
    p = dict_node_owner(d, hash, key);

    return p == NULL ? NULL : (*p)->value;
}

/*---------------------------------------------------------------------------*/
bool dict_unset(dicthdl dict, const char key[]) {
    struct dict *d = (struct dict *)dict;
    struct dict_node **p;
    unsigned long hash;

    assert(check_dict(d));

    hash = d->hashfnc(key);
    p = dict_node_owner(d, hash, key);

    if (p == NULL)  return false;

    dict_node_free(p);
    return true;
}
