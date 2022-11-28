This engine can prove a conclusion, given premises, using some core rules of logic. Almost all of the time, it is able to solve any such problem that I'm able to solve. It's far quicker than me, but sometimes its methods of proving can be inefficient. For example, it can be too eager to try proofs by contradiction. E.g., in order to prove P, assume !P. Then prove P, showing !P leads to a contradiction. Therefore, P. However, this is usually only done when the engine is a bit stuck.

To run the program, just do ./a.out in the terminal. You can also compile main.cpp yourself (with "g++ main.cpp -O2").

The following symbols represent negation, conjunction, disjunction, implication, and biconditional respectively:  !, ^, v, >, =
Note that the v is the letter v.

For your input, enter a number of statements for the premises. The last statement entered (before typing "done") is the conclusion that the engine will try to prove.  
For example:

Enter statement #1: P>Q  
Enter statement #2: !R=P  
Enter statement #3: !R  
Enter statement #4: Q  
Enter statement #5: done

Here, the engine will try to prove Q based off the first three statements as the premises.

If only one statement is entered, the engine will try to prove that as the conclusion (using no premises). For example:

Enter statement #1: P=P  
Enter statement #2: done

The engine will show P <--> P, using no prior premises.

Brackets can be used in the input for compound statements. E.g.:   !(PvQ)  is a negation of the disjunction between P and Q.

The engine can also be used to prove if something leads to a contradiction. This is done by typing the string "contradiction" as the final statement. E.g.:

Enter statement #1: P>Q  
Enter statement #2: !Q  
Enter statement #3: P  
Enter statement #4: contradiction  
Enter statement #5: done  

In this example, the engine would show Q follows, but then there is a contradiction with Q and !Q.

It's also possible to type the "contradiction" string as a premise, but then the conclusion will follow no matter what.

If you can find solvable problems in propositional logic that the engine gets stuck on, feel free to let me know!
