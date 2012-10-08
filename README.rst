======
yodawg
======

A barebones implementation of a Directed Acyclic Word Graph (DAWG) in C.

Run ``make`` and then ``./yodawg <prefix>`` to test.


What is a DAWG?
===============

To quote Wikipedia_, a DAWG, or Directed Acyclic Word Graph, "is a data
structure that represents a set of strings, and allows for a query operation
that tests whether a given string belongs to the set in time proportional to
its length. In these respects, a DAWG is very similar to a trie_, but it is
much more space efficient."

.. _Wikipedia: http://en.wikipedia.org/wiki/Directed_acyclic_word_graph
.. _trie: http://en.wikipedia.org/wiki/Trie


Wait, what?
-----------

You can add a set of strings to a DAWG, then, for example, test to see if a
string is in the set in ``O(n)`` time, where ``n`` is the length of the tested
string.

You can also, in much slower time, but fairly compact space, get all the
members of the set that *begin* with a given prefix. This is useful, for
example, for autocomplete. And that is why I started researching DAWGs and
eventually wrote yodawg.


Why did you write yodawg?
=========================

A few reasons:

1. To practice some slightly non-trivial C.
2. To better understand DAWGs.
3. In hopes that I might build a space-efficient autocomplete tool.

My goal with #3 is to build a Python module around yodawg that can store a DAWG
very space-efficiently, and then write a simple web service that, given a
prefix, returns the possible completions.

By writing yodawg in C, I hope it might be able to form the core of modules for
other languages, like Ruby or Node.js, too.


Caveats
=======

1. I am, shall we say, a beginner at C.
2. I wouldn't use this in critical or production code.
3. I haven't done much (any) performance (memory or time) testing yet.
4. Patches very welcome, but please be nice (remember #1).


TODO
====

* Some form of unicode support. 
