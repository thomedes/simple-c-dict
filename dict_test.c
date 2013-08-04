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

#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[]) {
    dicthdl d = dict_new(false, 1, NULL);

    dict_set(d, "hola", "lola");
    dict_set(d, "pita", "chula");

    puts(dict_get(d, "hola"));
    puts(dict_get(d, "pita"));

    dict_unset(d, "hola");
    
    dict_get(d, "hola");
    puts(dict_get(d, "pita"));

    dict_free(d);

    return EXIT_SUCCESS;
}
