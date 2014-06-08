## Square     Matrix
This project relies on the usage of our previous library, Square_Matrix, 
as what holds the contents of the sodoku puzzle.


## Partial solve algorithms
This uses two "filling" algorithms. The first is used to fill just a small
section of the puzzle to get us started. It should be used for only small
percentages of the puzzle because it fills the puzzle using a random brute
force method which is very inefficiant.

This does NOT guarantee a solvable puzzle. To check if it would be
solvable would require to try to actually solve it, which would
defeat the purpose of partially filling it. If the user wants partial
fill to guarantee a solvable puzzle, then it is up to the user to do it
him/her self by partially filling a puzzle, try to solve it, and if fails
then try to partially fill it again.


## The actual filling algorithm (backtrack)
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


## The actual filling algorithm (recursive)
There is also a dash of recursion solely for finding the next or previous node.
It goes forwards and back a cell as expected, in a clockwise and counter 
clockwise direction, but if it finds that the next or previous node was filled
using partial_fill then it calls itself to go even further forwards or backwards
a cell. This behavior allows any number of partial_fill cells being "in the way"
without any issues.

## Very rough and quick algorithm  "complexity" estimation.
Using my algorithm , the worst case total cell content increment count would be
size^(size^2), or if taking into account pre filled cells size^(size^2 - prefilled).
This is one amazingly inefficiant algorithm !


## Tests
There are a series of extensive tests both for Square_Matrix and Sodoku. I was
not sure if I should have included the tests for Square_Matrix but decided to
add them anyways. These tests are run automatically in the beginning of the 
program when executed.

Test Status: **PASS**
Tested on: Ubuntu raring64 (13.04) 64bit, gcc 4.8.1, 512MB ram.

##  Misc
This is on: https://bitbucket.org/hak8or/soduku/overview
While the repo is public, no one but myself worked on it for the entire duration.
It is open solely for if blackboard looses my assignment and if requested I will
make the repository private or even delete the repository.

I also threw in my notes onto the VCS where this is stored.

There were troubles with using version control for this since I originally was 
just putting everything into a develop branch but later wanted to use gitflow 
which did not work out too well so it remained as is. The branching and merging
is not miraculous.

The main filling algorithm  was rewritten a few times due to me wanting to make it more
clear, faster, or cleaner. While I did update the comments to reflect the changes,
some old comments from previous implimentations might remain.

The -std=c++0x flag was added due to a bug for converting int to string from the
standard library. 
http://stackoverflow.com/questions/12975341/to-string-is-not-a-member-of-std-says-so-g

## Now

## Todo
- [ ] Allow entering of coordinates via a vector or string to make setting up puzzles for testing quicker.
- [ ] Add in terminal GUI or use QT.
- [ ] Addd support for Travis CI
	- [ ] Test mode

## Complete
- [x] Shut down threads when we found a thread with a solution.
- [x] Update compilation instructions.
- [x] Change test.cpp to main.cpp (kept test.cpp, made main.cpp)
- [x] Say if using backtrack or recusrion in program start.
- [x] Display num_steps
- [x] Change partial fill to use the backtracking algo but somehow limited.
- [x] Change names for increment decrement cell to indicate we are changing contents.
- [x] Use matrix.Get/Set and Soduku.Get/Set correctly. In class use matrix only.
- [x] set_const_cell needs info for it not being used for anything but unit tests.
- [x] Fix upper and lower case diffirences.
- [x] Add warning if trying to fill more than 9 cells to indicate solving could take a while.
- [x] Fix description for solve_puzzle when taking away solve_Status.
- [x] Change can_set description
- [x] Prompt user for puzzle size.
- [x] User defines size of sodoku puzzle
- [x] Remove Attempted Cells
- [x] Remove solve Status.
- [x] Add test and fix for when the first cell is unset.
- [x] Have display() output an input string.
- [x] Have the tests run before the the main program.
- [x] Use an "unset" var of -1
- [x] When using display, display nothing for unset var's
- [x] Impliment row + column check taking into account unset vars
- [x] Fill in some cells by dividing size by filling in 15% with at least 1 cell.
- [x] Add padding to display depending on if 1 digit ints or 2 digit ints.
