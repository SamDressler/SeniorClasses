Delivery 2 - CSCI 465 - Sam Dressler
Submit 11.19.2020
Program loc : 1917

This program combines the lex, syntax analyzer, and intermediate code generator to produce
3-address code for expressions, IO statements, assignment statements, variable declarations,
and procedure and program definitions. 

The amount of effort spent on the parts for delivery two approach 25 hours.

The program can successfully handle expressions of various complexities. 
The program will output errors in the consol indicating missing semicolons, 
undefined references to variables, missing parenthesis, etc.

The program is compiled using a makefile which generates an executeable "s++" 
(Thought this would be a fitting name since the program was developed in c++ 
and compiled using g++ by a developer whose name starts with an 'S')

Design Descisions: 
Symbol Table - vector of tuples which contain : ID, 
						Scope(temp, procedure, program, etc),
						Scope ID (name of program or procedure, for temp it is the name of the procedure it is being set in)
						type: variable type
						value: string value or reference to temp var in the table
Parameter handling for procedures or programs:
- My goal with the design I went for was to be able to create a new node in alinked list for each
 	new procedure, program, or function. 
- This node contains information about the function as well as a pointer to the head of 
	a seperate linked list
- The second linked list contains the parameters (id, type)

Scoping for variable declaration:
- Each procedure, program, or function with a var section will have their variables assigned
	in that specific scope; 
- variables that are declared in one scope cannot be accessed in another scope without causing an
	undefined reference error to be thrown by the compiler
- each begin statement prints the current scope in the terminal
- each end statement will display the previous scope after popping it off the scope stack.

Challenges:
-handling different forms of expressions was tedious
- dealing with the linked lists required a lot of extra code and were difficult to debug 
(spent the first whole day of development on these for the program and procedure handling)
- fatigue - such a massive program requires an incredible amount of time spent sitting.
- started late - I'd say this is about half my fault, other courses have had non 
 stop assignments as well so finding time to sit down and develop this was scarce.



