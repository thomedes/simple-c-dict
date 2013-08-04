#ifndef __dict_h__
#define __dict_h__
/*****************************************************************************/
/**
@defgroup       simple-c-dict

@brief          Implement a simple, hash table based, dictionnary.

@{

@file

@brief          dict declaration and documentation

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

#if __cplusplus || __STDC_VERSION__ >= 90
#    include <stdbool.h>
#else
     typedef int   bool;
#    if !__bool_true_false_are_defined
#        define  true  (1)
#        define  false (0)
#    endif
#endif

#if __cplusplus
    typedef struct _dicthdl *dicthdl;
#else
    typedef struct _dicthdl { int bar; } *dicthdl;
#endif

/*---------------------------------------------------------------------------*/
/**
@brief          Create a new dictionnary

@usage          Call at any time to create a new dictionnary

@param[in]      replace             Alter replace behaviour.

@param[in]      hash_table_size     Size of hash table
@param[in]      hashfnc             Pointer to custom hash function
                                    or NULL to use provided one

@returns        New dict handle or NULL if error
*/
/*---------------------------------------------------------------------------*/
dicthdl dict_new(
    int      replace,
    unsigned hash_table_size,
    unsigned long (*hashfnc)(const char key[]));

/*---------------------------------------------------------------------------*/
/**
@brief          Free a dictionnary and all it's (key, value) pairs

@usage          Call at any time to create a new dictionnary

@param[in]      replace             Alter replace behaviour.

@param[in]      hash_table_size     Size of hash table
@param[in]      hashfnc             Pointer to custom hash function
                                    or NULL to use provided one

@returns        New dict handle or NULL if error
*/
/*---------------------------------------------------------------------------*/
bool dict_free(dicthdl dict);

/*---------------------------------------------------------------------------*/
/**
@brief          Set a new (key, value) pair. Takes care of existing / new keys

@param[in]      dicthdl  
@param[in]      key
@param[in]      value

@returns        1 if OK or 0 if error
*/
/*---------------------------------------------------------------------------*/
bool dict_set(dicthdl, const char key[], const char value[]);

/*---------------------------------------------------------------------------*/
/**
@brief Get the value for given key

@param[in]      dicthdl  
@param[in]      key

@return         Pointer to value or NULL if not found
*/
/*---------------------------------------------------------------------------*/
const char *dict_get(dicthdl, const char key[]);

/*---------------------------------------------------------------------------*/
/**
@brief Get the value for given key

@param[in]      dicthdl  
@param[in]      key

@return         1 if OK or 0 if ERROR
*/
/*---------------------------------------------------------------------------*/
bool dict_unset(dicthdl, const char key[]);

/** @} */
#endif
