#ifndef __djb2_h__
#define __djb2_h__
/*****************************************************************************/
/**
@ingroup        simple-c-dict

@file

@brief          djb2 declaration

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

/*---------------------------------------------------------------------------*/
/**
@brief          Simple hash function

@param[in]      str                 String to be hashed

@returns        hash value for str
*/
/*---------------------------------------------------------------------------*/
unsigned long
djb2(const char *str);


#endif
