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
		bool RemovePossible(int n);
	};

	struct SudokuIndex
	{
		int row, col;
		SudokuIndex(int r = 0, int c = 0)
		{
			row = r; col = c;
		}
	};

public:
	SudokuSolver();
	
	void LoadSudokuLevel(const char* fileName);
	void ResetCells();
	void Print() const;
	bool CompleteSudoku() const;
	void RemoveExisting(int row, int col);
	void RemoveExistingRow(int row, int col);
	void RemoveExistingCol(int row, int col);
	void RemoveExistingBox(int row, int col, int val);
	void Solve();
	void Update();
	//void CheckPossible(int row, int col);
	bool CheckPossibleRow(int row, int col);
	bool CheckPossibleCol(int row, int col);
	bool CheckPossibleBox(int row, int col);

	jhVector<SudokuIndex> FindNakedPairRow(int row, int col);
	jhVector<SudokuIndex> FindNakedPairCol(int row, int col);
	jhVector<SudokuIndex> FindNakedPairBox(int row, int col);

	void RemoveNakedPairRow(jhVector<SudokuIndex>);
	void RemoveNakedPairCol(jhVector<SudokuIndex>);
	void RemoveNakedPairBox(jhVector<SudokuIndex>, int, int);

private:
	Cell SudokuLevel[9][9];
};

#endif