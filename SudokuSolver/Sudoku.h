// Jonathon Hamrick
// Necessary classes & functions in order to solve sudoku puzzles
// 7/31/17

#ifndef SUDOKU_H
#define SUDOKU_H

#include "jhvector.h"

class SudokuSolver
{
private:
	struct Cell
	{
		int nVal; //value of the cell
		jhVector<> nvPossible;

		Cell();
		void CompleteCell(int n);
		void ResetCell();
	};

public:
	SudokuSolver();
	
	void LoadSudokuLevel(const char* fileName);
	void ResetCells();
	void PrintSudokuLevel() const;
	bool CompleteSudoku() const;

private:
	Cell SudokuLevel[9][9];
	int nRemaining;
};

#endif