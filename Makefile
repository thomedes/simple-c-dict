###############################################################################
#
#   Author: Toni Homedes i Saun
#
#   Copyright: 2013 Toni Homedes i Saun
#
#   This file is part of simple-c-dict.
#
#   simple-c-dict is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   simple-c-dict is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with simple-c-dict.  If not, see <http://www.gnu.org/licenses/>.
#
###############################################################################

lib=simplecdict
srcs=$(wildcard *.c)
objs=$(srcs:.c=.o)
deps=$(srcs:.c=.d)

all: $(lib)

#CC = g++
CFLAGS = -Wall -pedantic
LDFLAGS = 

LINT = splint
LINTFLAGS = +unix-lib

%.d: %.c
	$(CC) -M $(CFLAGS) $< -o $@

lib$(lib).a: dict.o djb2.o
	$(AR) rcs $@ $^

test: dict_test
	./dict_test > test

dict_test: dict_test.c lib$(lib).a
	$(CC) $(CFLAGS) $< -L. -l $(lib) -o $@

clean:
	find . -name "*.[ado]" -exec rm {} \+
	rm -rf doxygen/
	rm dict_test test

doc:
	doxygen

-include $(deps)
