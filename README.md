simple-c-dict
=============

A simple, hash table based, C dictionnary. What other languages call a hash array.

Usage
-----

~~~c
dicthdl d = dict_new(false, 256, NULL);

dict_set(d, "hola", "lola");
dict_set(d, "foo",  "bar");

puts(dict_get(d, "hola"));
puts(dict_get(d, "foo"));

dict_unset(d, "hola");

dict_free(d);
~~~
