Sparse Distributed Memory
=========================

This is an implementation of SDM created by Pentti Kanerva in 1988. SDM is a mathematical model that has some cognitive properties.

It was used to make simulations in my master thesis oriented by Ph.D. Alexandre Linhares. I would like to thanks him and M.Sc. Daniel Chada for their friendship and all their contributions to my thesis.

What is it for?
---------------

To do.

How to install?
---------------

You need GNU autotools installed in your computer. Then run the following commands:

    ./autogen.sh
	./configure
	make

The Python wrapper is in `python/` directory.



Where to start?
--------------------

The supplemental_data.pdf holds:


i) An introduction to the computational methods available in https://github.com/msbrogli/sdm; 
ii) A large set of additional heatmaps, documenting the critical distance behavior of the model in a 1000-dimensional memory; 
iii) The same tests on 256-dimensional memory, 
iv) all rehearsal results for 256-dimensional memory, and 
v) the same tests for 1000-dimensional memory.


How do I contribute?
--------------------

Just fork it and do the usual pull request dance. :)

Authors
-------

* 2011 Marcelo Salhab Brogliato
* 2011 Alexandre Linhares
* 2011 Daniel Chada
