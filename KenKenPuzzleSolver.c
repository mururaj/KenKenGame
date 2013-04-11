/* Project1: KenKen Game development using backtracking and constraint satisfaction
   Course: CSCI 561 Airtificial Intelligence
   Term: Fall 2012
   Author: Muruganantham Raju (mraju@usc.edu,muru.apr@gmail.com)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

# define PUZZLEARRAYSIZE 81 // Set based on MAXBOARDSIZE*MAXBOARDSIZE
# define MAXBOARDSIZE 9 // Set based on Sudoku game board sizes
# define TRUE 1  // Represents condition TRUE
# define FALSE 0 // Represents condition FALSE

FILE *outFile;
// Important data strucutres used in KenKen implementation
int numCages=0;
int boardSize;
int KenKenBoard[MAXBOARDSIZE][MAXBOARDSIZE];  // Array to represent KenKenBoard
int cageIndex[PUZZLEARRAYSIZE][PUZZLEARRAYSIZE]; // Array to capture the cages and
                                                 // corresponding KenKenBoard indexes
int cageValue[PUZZLEARRAYSIZE]; // Arry to capture the arithmatic outcome value of the cage
char cageOperator[PUZZLEARRAYSIZE]; // Array to capture the arithmatic operator of the cage

// Print list of operators in given KenKenBoard
void printOperators(char cageOperator[]) {
	printf("DEBUG: Printing Cage Operator \n");
	for(int i=0;i<numCages;i++){
		printf("DEBUG: %c\n",cageOperator[i]);
	}
}

// Print arithmatic or . operation values of cages in given KenKenBoard
void printCageValues(int cageValue[]) {
	printf("DEBUG: Printing Cage Values \n");
	for(int i=0;i<numCages;i++){
		printf("DEBUG: %d\n",cageValue[i]);
	}
}

// Print Cage number and corresponding indexes of cells in given KenKenBoard
// Note that cell indexes are from 0 to boardSize^2
void printCageIndexes() {
	printf("DEBUG: Printing Cage Indexes \n");
	for(int i=0;i<numCages;i++){
		for(int j=0;cageIndex[i][j] != -1;j++)
		{
			printf("DEBUG: Cage %d: %d\n",i,cageIndex[i][j]);
		}				
	}
}



// This function computes the size of KenKen boad 
int getBoardSize() {
	int largeIndex = 1; //Assuming largest index be 1
	for(int i=0;i<numCages;i++){
		for(int j=0;cageIndex[i][j] != -1;j++)
		{
			// Find the max cell index
			if (cageIndex[i][j]>largeIndex)
			{
				largeIndex = cageIndex[i][j];
			}
		}
	}
	// Square Root of max cell index is the board size
	return sqrt(largeIndex+1);	
}

// This function check row-column unique value constraint
int isRowColConstraintSatisfied(int rowIndex, int colIndex, int cellValue) {
	
	// Check if the value matches any of the cells in same row
	for(int j=0;j<boardSize;j++)
	{
		if(KenKenBoard[rowIndex][j] == cellValue) {
			// printf("DEBUG:Matching cell value\n");
			return FALSE;
		}
	}

	// Check if the value matches any of the cells in same column
	for(int i=0;i<boardSize;i++)
	{
		if(KenKenBoard[i][colIndex] == cellValue) {
			// printf("DEBUG:Matching cell value\n");
			return FALSE;
		}
	}

	return TRUE;
}

// This function checks if the given cell is empty.
// By default the KenKenBoard initialized to all 0 values
// Presence of 0 represents the cell is empty, otherwise not empty
int isCellEmpty(int rowIndex, int colIndex) {
	if(KenKenBoard[rowIndex][colIndex] == 0) {
		return TRUE;
	} 
	else {
		return FALSE;
	}
}

// Check if all cells except single cell of the cage contains tryCellValues
// Returns TRUE if the cell has two or more empty cells, 
// otherwise returns FALSE
int areEmptyCageCells(int myCageIndex,int myCellIndex) {

	// Note that -1 value in cageIndex array represents
	// end of cells  in that cage
	for(int i=0;cageIndex[myCageIndex][i] !=-1; i++) {
		if (cageIndex[myCageIndex][i] != myCellIndex)
		{
			if(isCellEmpty(floor(cageIndex[myCageIndex][i]/boardSize),cageIndex[myCageIndex][i]%boardSize))
			{
				// printf("DEBUG:Found empty cage cells\n");
				return TRUE;
			}
		}
	}

	// At this point, all the cells seem to have values
	// printf("DEBUG:Cage cells are filled\n");
	return FALSE;
}

// Given rowIndex and columnIndex of a cell, this function
// returns the cage number to which this cell belongs to.
// If given cell not part of any cage, then returns -1
int getCageIndex(int rowIndex, int colIndex) {
	int cellIndex = rowIndex * boardSize + colIndex;

	for(int i=0;i<numCages;i++){
		for(int j=0;cageIndex[i][j] != -1;j++)
		{
			if (cageIndex[i][j] == cellIndex)
			{
				// printf("DEBUG: Cage Index is %d\n",i);
				return i;
			}			
		}				
	}
	
	//Unable to find cage Index
	printf("Unable to find cage index for cell %d\n",cellIndex);
	return -1;

}

// This function prints the values of KenKenBoard
void printKenKen(int fileFlag) {
	for (int i=0;i<boardSize;i++)
	{
		for (int j=0;j<boardSize;j++)
		{
			printf("%d ",KenKenBoard[i][j]);
			if (fileFlag)
			{
				fprintf(outFile,"%d ",KenKenBoard[i][j]);
			}			
		}

		printf("\n");
		if (fileFlag)
		{
			fprintf(outFile,"\n");
		}		
	}

	fprintf(outFile,"------------------\n");

}

// Given the cage index, this function returns summation
// of cell values of the cage
int getSumCageValues(int myCageIndex){
	int cageValueSum = 0,rowIndex,colIndex;
	for(int i=0;cageIndex[myCageIndex][i] != -1;i++) {
		rowIndex = floor(cageIndex[myCageIndex][i]/boardSize);
		colIndex = cageIndex[myCageIndex][i]%boardSize;
		cageValueSum = KenKenBoard[rowIndex][colIndex] + cageValueSum;		
	}
	return cageValueSum;
}

// Given cage index, this function returns product of cell
// values of the cage
int getProductCageValues(int myCageIndex){
	int cageValueProduct = 1,rowIndex,colIndex;
	for(int i=0;cageIndex[myCageIndex][i] != -1;i++) {
		rowIndex = floor(cageIndex[myCageIndex][i]/boardSize);
		colIndex = cageIndex[myCageIndex][i]%boardSize;
		if (!isCellEmpty(rowIndex,colIndex))
		{
			cageValueProduct = KenKenBoard[rowIndex][colIndex] * cageValueProduct;
		}				
	}
	return cageValueProduct;
}

// Checks if substract constraint is satisfiable for a given cellValue
// If yes returns TRUE otherwise FALSE
int isSubstractConstraintSatisfied(int myCageIndex,int cellIndex,int cellValue) {
	int rowIndex,colIndex;
	for (int i=0;cageIndex[myCageIndex][i] != -1;i++) 
	{
		if (cageIndex[myCageIndex][i] != cellIndex)
		{
			rowIndex = floor(cageIndex[myCageIndex][i]/boardSize);
    		colIndex = cageIndex[myCageIndex][i]%boardSize;

			// Either A-B or B-A should get corresponding cageValue
			if (KenKenBoard[rowIndex][colIndex] - cellValue == cageValue[myCageIndex] ||
				cellValue - KenKenBoard[rowIndex][colIndex]  == cageValue[myCageIndex])
			{
				return TRUE;
			}
			else {
				return FALSE;
			}
		}
	}

	// At this point, condition seem to be unsatisfied
	return FALSE;
}

// Checks if multiplication constraint is satisfiable for a given cellValue
// If yes returns TRUE otherwise FALSE
int isMultiplicationConstraintSatisfied(int myCageIndex,int cellIndex,int cellValue)
{
	int rowIndex,colIndex;

	// Condition1: cellValue being tried should not be > cage value and it should divide cage value	
	// If not, the passed cellValue can't be a potential member of this product cell
	if (cellValue <= cageValue[myCageIndex] && cageValue[myCageIndex]%cellValue == 0)
	{
		// Condition2: If Condition1 is satisfied, then check for empty cells in the cage
		// If there is still cells not filled, then product of cellValue with existing
		// cell values must be <= cageValue
		if (areEmptyCageCells(myCageIndex,cellIndex))
		{
			if (cellValue * getProductCageValues(myCageIndex) <= cageValue[myCageIndex])
			{
				return TRUE;
			}
			else {
				return FALSE;
			}
		}
		else {
			// Condition3: If condition2 not satisfied, then product of cellValue being tried
			// with existing cellvalues must equal to cageValue
			if (cellValue * getProductCageValues(myCageIndex) == cageValue[myCageIndex])
			{
				return TRUE;
			}
			else {
				return FALSE;
			}
		}
	}
	else {
		return FALSE;
	}
}

// Checks if Division constraint is satisfiable for a given cellValue
// If yes returns TRUE otherwise FALSE
int isDivisionConstraintSatisfied(int myCageIndex,int cellIndex,int cellValue)
{
	int rowIndex,colIndex;

	// Condition1: Check for empty cells
	if (areEmptyCageCells(myCageIndex,cellIndex))
	{
		// Condition2: If empty cells found, then say A/B = C 
		// B * C <=boardSize or C/A == 0
		if ( (cellValue * cageValue[myCageIndex]) <= boardSize || 
			(cellValue % cageValue[myCageIndex] == 0) )
		{
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	else {
		for (int i=0;cageIndex[myCageIndex][i] != -1;i++)
			{
				if (cageIndex[myCageIndex][i] != cellIndex)
					{
						rowIndex = floor(cageIndex[myCageIndex][i]/boardSize);
			    		colIndex = cageIndex[myCageIndex][i]%boardSize;
						// Condition3: If there are no empty cells, then A / B
						// or B / A should equal to 0
						if (KenKenBoard[rowIndex][colIndex] % cellValue == 0 ||
						cellValue % KenKenBoard[rowIndex][colIndex]  == 0)
						{
							return TRUE;
						}
						else {
							return FALSE;
						}
					}
			}
	}	
	
	// If control reaches here, potentially there is an input issue. 
	// Check input.txt if the format is correct
	return FALSE;
}

// Function to check the cage constriant. Returns TRUE if constraint is 
// satisfiable otherwise FALSE
int isCageConstraintSatisfied(int rowIndex,int colIndex,int cellValue) {
	// get the cage index
	int myCageIndex = getCageIndex(rowIndex,colIndex);
	int cellIndex = rowIndex * boardSize + colIndex;

	switch(cageOperator[myCageIndex])
	{
		case '.':
			// printf("DEBUG: Cage operator matches \'.\' \n");
			if (cellValue == cageValue[myCageIndex])
			{
				return TRUE;
			}
			else {
				return FALSE;
			}
			break;
		case '+':
			// printf("DEBUG: Cage operator matches \'+\' \n");
			
			// Condition1: cellValue must be less than cage value
			if (cellValue < cageValue[myCageIndex])
			{
				// Condition2: If C1 satisified, then look for empty cells
				if (areEmptyCageCells(myCageIndex,cellIndex))
				{
					// Condition3: If C2 satisifed, then summation of cellValue being tried
					// with filled cell values <= cageValue
					if (cellValue + getSumCageValues(myCageIndex) < cageValue[myCageIndex])
					{
						// This may be a possible value
						return TRUE;
					}
					else {
						// printf("DEBUG: Cell index %d: Trying cellValue %d and sum %d \n",cellIndex,
						//	cellValue,getSumCageValues(myCageIndex));
						// printf("Empty cells found. But summation >= cageValue %d \n",
						//	cageValue[myCageIndex]);
						// printKenKen();
						return FALSE;
					}
				}
				else {
					// Condition4: If C2 not satisfied, then no more empty cells, 
					// Summation of cellValue with rest of the cell values == cageValue
					if (cellValue + getSumCageValues(myCageIndex) == cageValue[myCageIndex])
					{
						// This may be a possible value
						return TRUE;
					}
					else {
						// printf("DEBUG: Summation exceeds the cageValue %d \n",
						//	cageValue[myCageIndex]);
						return FALSE;
					}
				}
			}
			else {
				return FALSE;
			}
			break;
		case '-':
			// printf("DEBUG: Cage operator matches \'-\' \n");
			if (areEmptyCageCells(myCageIndex,cellIndex))
			{
				// Return true with a possible hope
				return TRUE;
			}
			else {
				if (isSubstractConstraintSatisfied(myCageIndex,cellIndex,cellValue))
				{
					return TRUE;
				}
				else {
					return FALSE;
				}
			}
			break;
		case '*':
			// printf("DEBUG: Cage operator matches \'*\' \n");
			if (isMultiplicationConstraintSatisfied(myCageIndex,cellIndex,cellValue))
			{
				return TRUE;
			} 
			else {
				return FALSE;
			}
			break;
		case '/':
			// printf("DEBUG: Cage operator matches \'/\' \n");
			if (isDivisionConstraintSatisfied(myCageIndex,cellIndex,cellValue))
			{
				return TRUE;
			} 
			else {
				return FALSE;
			}
			break;
		default:
			printf("DEBUG: No matching cage operator found \n");
			return FALSE;
	}

}

// Combines both the constraints: Row col uniqueness and cage operation constraints
// Returns TRUE if both are satisfiable otherwise FALSE
int isBoardConstraintSatfisfied(int rowIndex,int colIndex,int cellValue) {

	// Check if both the constraints are satisfied
	if (isRowColConstraintSatisfied(rowIndex,colIndex,cellValue) && 
		isCageConstraintSatisfied(rowIndex,colIndex,cellValue))
	{
		return TRUE;
	}
	else {
		return FALSE;
	}

}

// Flush the whole board before starting all over again
void flushKenKenBoard() {
	for (int i=0;i<boardSize;i++)
	{
		for (int j=0;j<boardSize;j++)
		{
			KenKenBoard[i][j] = 0;
		}
	}
	// printf("DEBUG: Board flushed\n");
}

// Flush the specific cell value
void flushKenKenCell(int rowIndex,int colIndex) {
	KenKenBoard[rowIndex][colIndex] = 0;
}

// Function to begin game exploration and backtracking
int exploreGame(int rowIndex,int colIndex,int cellValue) {
	int retStatus;
	// If all the rows are filled, then return true
	if (rowIndex >= boardSize)
	{
		printf("All the rows are filled. Game done\n");
		printf("Solution is found\n");
		printKenKen(TRUE);
		return TRUE;
	}

	if (isBoardConstraintSatfisfied(rowIndex,colIndex,cellValue))
	{
		int tryCellValue;


		//Possible cell value is assigned
		KenKenBoard[rowIndex][colIndex] = cellValue;
		//printKenKen();

		if (colIndex+1 < boardSize)
		{
			//Move to next column
			colIndex = colIndex + 1;
		}
		else {
			// Move to next row
			rowIndex = rowIndex + 1;
			// Start from column 0 
			colIndex = 0;
		}

		if (rowIndex >= boardSize)
			{
				printf("All the rows are filled. Game done\n");
				printf("Solution is found\n");
				printKenKen(TRUE);
				return TRUE;
			}

		for (tryCellValue=1;tryCellValue<=boardSize;tryCellValue++)
		{
			flushKenKenCell(rowIndex,colIndex);

			// printf("DEBUG: Calling exploreGame(%d,%d,%d) \n",rowIndex,colIndex,tryCellValue);
			
			retStatus = exploreGame(rowIndex,colIndex,tryCellValue);
			
			if (retStatus)
			{
				// printf("DEBUG: Explore Game returned success rowIndex =%d colIndex =%d tryCellVal = %d\n",rowIndex,colIndex,
				// tryCellValue);
			} 
			else {
				// printf("DEBUG: Explore Game returned fail rowIndex =%d colIndex =%d tryCellVal = %d\n",rowIndex,colIndex,
				// tryCellValue);
			}
					
						
		}

		if (tryCellValue > boardSize)
		{
			// None of the cell values match the constraint. Exploration failed
			flushKenKenCell(rowIndex,colIndex);
			return FALSE;
		}

		
	}
	else {
		// Constraint is not satisfied
		flushKenKenCell(rowIndex,colIndex);
		return FALSE;
	}
}

// Main begins here
int main(int argc, char *argv[])
{
	// Declare all required variables
	FILE *InFile;
	int x=0,y=0,i=0,j=0;
	int t2=0; 
	char temp1[10];
	char temp2[10];
			
	if (argc < 3)
	{
		printf("Program execution syntax: KenKenPuzzleSolver <InputFile.txt> <OutputFile.txt>\n");
		printf("You haven't given one or more required arguments. Exiting execution\n");
		exit(1);
	}

	InFile = fopen(argv[1],"r+");

	if(InFile == NULL)
	{
		printf("Error in opening input file %s\n",argv[1]);
		printf("Exiting execution\n");
		exit(1);
	}

	while (!feof(InFile))
	{
		
		// printf("DEBUG: Reading file\n");
		fscanf(InFile,"%s",temp1);
		// printf("DEBUG: %s\n",temp1);

		// Parse cage value, operator and cell indexes from input file
		if (strlen(temp2) == 0)
		{
			strcpy(temp2,temp1);			
		}
		else {
			if (temp1[0] == '+' || temp1[0] == '-' || temp1[0] == '*' || temp1[0] == '/' || temp1[0] == '.')
			{
				// printf("DEBUG:Temp2 value is %s\n",temp2);
				cageValue[i] = atoi(temp2);
				cageOperator[i] = temp1[0];	
				cageIndex[x][y] = -1;
				x=i;
				i++;	
				y=0;
			}
			else {
				if (temp2[0] != '+' && temp2[0] != '-' && temp2[0] != '*' && temp2[0] != '/' && temp2[0] != '.')
				{
					// printf("DEBUG:Temp2 value is %s\n",temp2);
					cageIndex[x][y++] = atoi(temp2);
				}					
			}
			strcpy(temp2,temp1);			
		}
	}
	cageIndex[x][y] = -1; // -1 to denote end of cells in a given cage

	numCages = i;

	fclose(InFile);
	
	/* DEBUG print functions
	printOperators(cageOperator);
	printCageValues(cageValue);
	printCageIndexes();
	*/

	boardSize = getBoardSize();

	printf("The board size is %d\n",boardSize);

	if (boardSize > MAXBOARDSIZE)
	{
		printf("The boardSize %d exceeded maximum board limit %d\n",boardSize,MAXBOARDSIZE);
		printf("Exiting execution\n");
	}
	
	outFile = fopen(argv[2],"w+");
	for (i=1;i<=boardSize;i++)
	{		
		// Flush the board duing each iteration
		flushKenKenBoard();
		exploreGame(0,0,i);				
	}

	fclose(outFile);

	// printf("Solution not found\n");
	return 0;
}
