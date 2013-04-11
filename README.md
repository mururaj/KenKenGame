KenKenGame
==========
You Love KenKen! Don't you - http://www.kenken.com/

KenKen puzzle implementation
----------------------------

Compiler version:
g++ (GCC) 4.2.1

Compiler location:
/usr/usc/bin/g++

Compilation Steps:
g++  KenKenPuzzleSolver.c -o KenKenPuzzleSolver

Execution Steps:
./KenKenPuzzleSolver input.txt output.txt

where,
input.txt -> KenKen cage layout is stored

Sample input files can be found:TestInputFiles

output.txt -> All possible solutions for given input is stored

Following briefs the design of the program:
-------------------------------------------
Important data structures:
-> Arrray KenKenBoard[MAXBOARDSIZE][MAXBOARDSIZE] represents the game board.
-> MAXBOARDSIZE represents the maximum size of the board supported. As of now 9*9 is the maximum board size input accepted by the program.
-> Array cageOperator[PUZZLEARRAYSIZE] captures the operators  found in cages of given KenKen board.
-> Array cageIndex[PUZZLEARRAYSIZE][PUZZLEARRAYSIZE] associates the cells with corresponding cage numbers. 
-> Array cageValue[PUZZLEARRAYSIZE] captures values found in cages of given KenKen board.

Constraints:
-> isRowColConstraintSatisfied(int rowIndex,int colIndex,int cellValue) checks uniqueness constraint for a given value across its row and column.
-> isCageConstraintSatisfied(int rowIndex,int colIndex,int cellValue) checks whether the given value satisfies the arithmetic operations such as +, -, * and % represented by the corresponding cage.
-> exploreGame(int rowIndex,int colIndex,int cellValue) explores the game state for given cellValue and check for both the RowCol and CageConstraints. If both the conditions are satisfied, then the algorithm proceeds to next cell otherwise it backtracks and try different value for the same cell.


Explicit information:
Following aspects are explicitly captured in game description.
-> Size of the board and number of elements in the board
-> Row and column uniqueness constraint
-> Cage arithmetic and . operator constraint
-> Initial state of the KenKenBoard which is all cell empty with cage information captured.
-> Goal state ÎíÎñ a possible arrangement of cell values that satisfies the given constraints 
-> Successor function is assigning a value to a given cell and check for the constriants

Implicit Information:
Following aspects are implicitly represented. 
-> A cell value can never be 0
-> Values across the diagonal needn't satisfy uniqueness constraint
-> There is no limit on number of cages. Entire board could be a cage or each cell could be a cage with . operator everywhere.
-> There is no restriction on position of values in order to satisfy the cage operator constraint. For example if the two cells (0,0) and (0,1) are part of a cage with '+' operator and value 5. The two numbers that adds up to 5 can be either in cell(0,0) or cell(0,1) as long as it satisfy the row column uniqueness property.
-> If the cage operator is *, then none of the cell values in that cage can be greater than the resultant cage value.
-> If the cage operator is  *, then resultant cage value must be divisible by each cell in that cage.
-> If the cage operator is +, then none of the cell values in that cage can be greater than the resultant cage value.
-> If empty cells found in cage with %, then say if A/B = C (where C is resultant cage value), then B * C <=boardSize or C/A == 0
-> There are no negative and decimal cell values. All the values are integers.
-> Set of next possible states in the game tree depend on the current value being assigned to the cell. Note that the possible states are not totally explicit like a fixed graph. 



