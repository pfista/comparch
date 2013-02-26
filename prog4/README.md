


Due Date: 8 Mar 2013
Let's take a break from the previous line of programs. The purpose of this assignment is mainly to (a) be a self-contained, well-defined problem, and (b) get you experience with malloc/free.

Write a C program to implement Topological Sort. Read an input file. Each line of the input file should be a pair of symbols. Suppose the input is "a b". This defines a precedence relationship between a and b, saying that "a must occur before b". Ignore any blank lines.

The output of your program is a list of all the symbols, one per line, in an order which satisfies all the precedence requirements. So if the input is 
a b
d c
b d
The output would be
a
b
d
c
The possible error conditions would be:

Bad input data: an input line does not have exactly two symbols. It could have one or three or more.
There is no possible solution. Assume you have input "a b" and "b a", then we have a loop between a and b -- a must be before b, b must be before a -- and there is no solution.
In an error case, print an appropriate message, and quit. You can have additional output too. If you have bad input data, you may want to show the bad line. You can begin printing your output, and then, after some output, realize that you can not continue because there is no solution.
The expected program would first read in all it's data. As each symbol is read, enter it in a symbol table, so you can recognize a multiple occurrences of the same symbol. For each symbol, you can keep either (a) all of the things that come before it, or (b) all of the things that come after it, or (c) both.

Once all your input is read in, look thru your symbol table for any symbol which has nothing that must come before it. These symbols can be output; nothing has to come before them. Each time a symbol is output, you can go to all the symbols that come after it and remove the requirement that the output symbol must come first since it has now been output. Now look for another symbol that has no symbol which has not been output that must come before it.

So if your input is "a b" and "b c", we have three symbols in the symbol table:

a (b must follow)
b (a must precede) (c must follow)
c (b must precede)
If we look, we see that a has nothing that must precede it. So "a" is our first output. Once "a" is output, "b" has nothing that precedes it that has not already been output. You can recognize this by (a) when "a" is output, remove the requirement for b that "(a must precede)", or (b) just mark the requirement for b that "(a must precede)" to say this is satisfied. So then "b" can be output. Once that happens, the requirement for c is either removed or satisfied, and "c" can be output.
One interesting part of this is how to make it efficient, in time and/or space. You may not need to keep both what must precede and what must follow -- only one of these is probably all that is needed, but maybe keeping both makes things faster. You also may be able to use counters to summarize information. For example, if you keep track of the number of symbols that must precede a symbol, and decrement that as things are output, you may be able to avoid keeping the actual list of such things.

You do not know the number of symbols, or how many relationships might be between them, so the expectation would be that you will need to malloc new symbol table entries, and the representation of the relationships.

You probably want to define a structure to represent each symbol. The structure would have (a pointer to) the external representation of the symbol, plus other information, like what symbols precede and what symbols follow. These would just be lists of pointers to the structs for the other symbols.

A symbol is basically a sequence of (non-blank) printable ASCII characters separated by blank space (space, tab, newline). You certainly want it to be alphabetic, numeric and some other characters (think about your symbols as being file names, for example). You might allow some "decoration" in your input. For example, if you treat ">" as "blank space", then your input could be "a > b". But you have to accept the basic "a b" without the decoration (the grading scripts will use this).

From a functional point of view, you probably want to first write the part that does the input and builds the symbol table. Then print out what you built, so that you can see that you are building the structures that you expect. Then write the second part that generates the output. Create sample input files that you understand, and see if the processing and output are what you expect, including error cases. Your finished program should only provide the standard output. You should either remove your development and debugging output, or you could conditionalize it on a debug flag. I use "-d" to indicate "debug output", use it to set a global "debug_flag" and then add "if (debug_flag)" before each print statement that is not part of the official program output.

Notice that some input files can produce multiple different output files. If the input is

d b
b a
d c
c a
The output can be "d c b a" or "d b c a". To ease grading, if there are multiple possible outputs at any time, choose the next output alphabetically. If strcmp(p,q) < 0 then print p before q.
Due Date: 8 Mar 2013a
