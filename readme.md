#Sodoku Solver
First things first, the spelling! Yes, sodoku is spelled wrong. I
decided to leave it as is since the original assignment was based on
rules which are "lighter" than the real Sudoku, making it no longer
actual Sudoku but instead a modified version, with the title reflecting
that discrepancy.

Anyways, this is my attempt at a fast solver of a sodoku puzzle using
only the rule that no number may repeat in a column or a row, and the
solving must primarly rely on backtracking. This rule was created by my professor for
an assignment during my CS 235 class. 

The project has continued to grow over time as a test bed for learning
multithreading, code profiling, optimizing, discovering [Valgrind](http://valgrind.org/) for profiling
and memory leaks, and Microsoft Visual Studio (I used [Sublime Text](http://www.sublimetext.com/) previously).


### Efficiency
Using my algorithm, the worst case total cell content increment count would be
very roughly, size!(size - 1). This is one amazingly inefficiant algorithm!


### Multithreading
In an effort to speed this up, multithreading was implimented. This was my first
fooray into multithreading, so it isn't ideal, but it works and is throughly
documented. Threads are created, one for each core avalible, with the first
thread getting the original sodoku with 25% of its cells filled. The other threads
recieve the same sodoku with the same cells filled but also one extra cell filled
at random while still following the rules in the form of a "hint". 

My implimentation is extremely beneficial from good hints which can cut the speed
of solving a puzzle drastically. In an effort to exploit this, if a thread is 
considered to have been trying to solve for too long, it is reset with a new hint. 


### Documentation
I strive to document my projects as well as possible. This project uses [Doxygen](http://www.stack.nl/~dimitri/doxygen/)
to generate documentation from comments throughout this codebase, which are
generated using ```make docs```.


### Tests
There are a series of extensive tests both for Square_Matrix and Sodoku. I was
not sure if I should have included the tests for Square_Matrix but decided to
add them anyways. These tests are run automatically in the beginning of the 
program when executed.

Test Status: **PASS**

Tested on: Ubuntu raring64 (13.04) 64bit, gcc 4.8.1, 512MB ram.


### Partial fill algorithm
This uses two "filling" algorithms. The first is used to fill just a small
section of the puzzle to get us started. It should be used for only small
portions of the puzzle because it fills the puzzle using a random brute
force method which is very inefficiant.

This does NOT guarantee a solvable puzzle. To check if it would be
solvable would require to try to actually solve it, which would
defeat the purpose of partially filling it. If the user wants partial
fill to guarantee a solvable puzzle, then it is up to the user to do it
him/her self by partially filling a puzzle, try to solve it, and if fails
then try to partially fill it again.


### Solving algorithm (backtrack)
While the assignment requires either recursion or backtrack to be used, my
method uses both. The general idea is it looks clockwise for a node which
was not set using partial_fill and attempts to fill it with a number that
follows the rules of no other same number being in the same column and row.
If it cannot find a number which satisfies those conditions for that node, 
then it goes back in a clockwise direction to a node not set using
partial solve and tries to find another number for the now current node.

It can go like this for the entire sodoku puzzle, once it finishes the
last element, then it is considered that the puzzle has been fully solved.
If it went all the way back to the earliest cell not filled with partial_fill
and incrememnted to the highest possible value, then it is considered that 
the puzzle in its current form is not solvable.


### Solving algorithm (recursive)
There is also a dash of recursion solely for finding the next or previous node.
It goes forwards and back a cell as expected, in a clockwise and counter 
clockwise direction, but if it finds that the next or previous node was filled
using partial_fill then it calls itself to go even further forwards or backwards
a cell. This behavior allows any number of partial_fill cells being "in the way"
without any issues.


###  Misc
This project relies on the usage of a previous class project, [Square_Matrix](https://bitbucket.org/hak8or/csci235hw1), 
to hold the contents of the sodoku puzzle.

There were troubles with using version control for this since I originally was 
just putting everything into a develop branch but later wanted to use gitflow 
which did not work out too well. The branching and merging is not miraculous.

There is a todo I keep which I find more satsfying to mark off completed 
goals than Githubs issue's section, which you can find in [todo.md](todo.md).

The original home of this project was at [bitbucket](https://bitbucket.org/hak8or/soduku).


### Example output
```
Running self tests ...
All tests passed (69 assertions in 18 test cases)

Size of Sodoku puzzle: 7
This is your empty sodoku puzzle.
| . . . . . . .  |
| . . . . . . .  |
| . . . . . . .  |
| . . . . . . .  |
| . . . . . . .  |
| . . . . . . .  |
| . . . . . . .  |

Lets fill 25% of it with some random yet valid starting cells.

         NOTE:
Partial fill does not guarantee to supply a valid puzzle. This loop will try to
 find a solvable partially filled puzzle by trying to actually solve it and if it
 can't solve it, then it partially fills it again.

Found an unsolvable puzzle, making a new one...
Found an unsolvable puzzle, making a new one...
Partially pre-filled puzzle.
| . . . . 5 . .  |
| . . . . . . .  |
| . . . . . 2 1  |
| 1 2 . . . . .  |
| . . . . 3 5 .  |
| 0 . . 5 1 . .  |
| . 1 4 . . . .  |

Solved puzzle
| 2 0 1 3 5 4 6  |
| 3 4 5 2 6 1 0  |
| 6 5 3 0 4 2 1  |
| 1 2 6 4 0 3 5  |
| 4 6 0 1 3 5 2  |
| 0 3 2 5 1 6 4  |
| 5 1 4 6 2 0 3  |

# of times a value was tried per cell.
|    3    1    2    4    0    5    7  |
|    4    5    6   19   71  114  137  |
|   95  286  668 1337 1637    0    0  |
|    0    0  807 1877 2657 2116 2238  |
| 1229 2031  777  818    0    0  779  |
|    0  260  107    0    0  111    5  |
|    6    0    0    7    3    1    4  |

Whew! That took 20234 steps!
```
