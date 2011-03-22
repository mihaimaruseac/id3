ID3 classifier
==============

A. About
........

This is a homework for the Automatic Learning Course, a course I am taking
right now at my University.

We had to use the ID3 algorithm to classify some examples. The training set
consisted of examples containing discrete and integer valued attributes. The
examples might be affected by errors but on a limited scale: at most, only two
attributes will be missing.

The only thing that this assignment doesn't do is treating the missing
attributes by using ID3 to guess their exact values.

Although I wanted to use Haskell for this assignment, it wasn't possible when
I started coding. It became available as an option when I had done half of the
assignment, thus it is still done in C.

B. Usage
........

After running ``make``, you can use ``make tests`` to do all the regression
tests or use the executable (see each section below for more details):

* ``./id3 l description examples classifier`` can be used to learn a new
classifier
* ``./id3 g classifier`` can be used to output the classifier in a variety of
formats
* ``./id3 c classifier test`` can be used to classify new examples

If this seems a little confusing, try looking at ``call_overview.png`` in the
``doc`` folder.

B.1. Learning phase
```````````````````

The learning process must be started with three files:
* one describing the instance of the problem (in the ``tests`` folder it is
called ``atribute.txt``)
* one describing the learning set (called ``invatare.txt``)
* one in which to store the classifier.

Also, the learning phase can be informed about how to treat numeric attributes:
either by doing a binary discretization or a full discretization based on the
Minimum Description Length Principle. Another important flag controls how to
fill in the missing values: by using probability theory or statistics (majority
of attributes).

For example, all of the following are valid calls::

	./id3 l attribute learn dump
	./id3 l -ndiv -mprb attribute learn dump
	./id3 l -mprb attribute learn dump

B.2. The graphing phase
```````````````````````

The classifier can be outputed in a variety of formats.

One user may prefer using simple ASCII display, like this::

	$./id3 g dump
	 outlook = sunny
	   humidity < 80
	     ==> C1
	   humidity >= 80
	     ==> C2
	 outlook = overcast
	   ==> C1
	 outlook = rain
	   windy = true
	     ==> C2
	   windy = false
	     ==> C1

Or, using ``dot`` to obtain PNGs::

	$./id3 g -gdot dump | dot -Tpng > out.png

Or, some user may want to place the classifier in a Scheme function::

	$ ./id3 g -gscheme tests/1/out_div_maj 
	(cond
	  (
	    ((eqv? outlook 'sunny) 
		(cond
		  (
		    ((>= humidity 80) 'C1)
		    ((< humidity 80) 'C1)
		  )
		)
	      )
	    ((eqv? outlook 'overcast) 'C1)
	    ((eqv? outlook 'rain) 
		(cond
		  (
		    ((eqv? windy 'true) 'C2)
		    ((eqv? windy 'false) 'C1)
		  )
		)
	      )
	  )
	)

B.3. The testing (classifying) phase
````````````````````````````````````

This phase has no flags (as of now), you only have to give the classifier and the example file (optionally the output file, too).

C. The code
...........

A quick overview of the code can be obtained by running ``make doc`` with ``Doxygen`` and ``Graphviz`` installed. If not, looking at ``fct_overview.png`` from the ``doc`` folder may help.

