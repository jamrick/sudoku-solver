// Jonathon Hamrick
// Implementation of sudoku solver
// 7/31/17

#include "Sudoku.h"
#include <fstream>
#include <iostream>

using namespace std;

// cell struct method definitions
SudokuSolver::Cell::Cell()
{
	nVal = 0;
	for (int i = 1; i <= 9; ++i)
	{
		nvPossible.Append(i);
	}
}
void SudokuSolver::Cell::CompleteCell(int n)
{
	nVal = n;
	if (nVal != 0)
		nvPossible.Clear();
}
void SudokuSolver::Cell::ResetCell()
{
	nVal = 0;
	nvPossible.Clear();
	for (int i = 1; i <= 9; ++i)
	{
		nvPossible.Append(i);
	}
}
bool SudokuSolver::Cell::RemovePossible(int n)
{
	for (int i = 0; i < nvPossible.Size(); ++i)
	{
		if (nvPossible[i] == n)
		{
			nvPossible.Remove(i);
			return true;
		}
	}
	return false;
}


// sudokusolver class method definitions
SudokuSolver::SudokuSolver()
{
	ResetCells();
}

void SudokuSolver::LoadSudokuLevel(const char* fileName)
{
	if (fileName != NULL)
	{
		ifstream ifs(fileName);
		if (ifs.is_open())
		{
			ResetCells();
			int r = 0, c = 0;
			for (;;)
			{
				char in = ifs.get();
				if (ifs.eof())
					break;
				if (in == '\n')
					continue;

				SudokuLevel[r][c++].CompleteCell((int)(in - '0'));

				if (c == 9)
				{
					r++;
					c = 0;
				}
			}
			ifs.close();
		}

		// level is loaded, take out exisitng values
		for (int startRow = 1; startRow < 9; startRow += 3)
		{
			for (int startCol = 1; startCol < 9; startCol += 3)
			{
				RemoveExisting(startRow, startCol);
			}
		}
	}
}
void SudokuSolver::ResetCells()
{
	for (int r = 0; r < 9; r++)
		for (int c = 0; c < 9; c++)
			SudokuLevel[r][c].ResetCell();
}
void SudokuSolver::PrintSudokuLevel() const
{
	for (int r = 0; r < 9; r++)
	{
		for (int c = 0; c < 9; c++)
		{
			int val = SudokuLevel[r][c].nVal;
			if (val == 0)
			{
				cout << '{';
				for (int i = 0; i < SudokuLevel[r][c].nvPossible.Size(); ++i)
					cout << SudokuLevel[r][c].nvPossible[i] << ',';
				cout << '}';
			}
			else
			{
				cout << val;
			}
			cout << ' ';
		}
		cout << '\n';
	}
}
bool SudokuSolver::CompleteSudoku() const
{
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
		{
			if (SudokuLevel[i][j].nVal == 0)
				return false;
		}
	return true;
}
void SudokuSolver::RemoveExisting(int row, int col)
{
	int adjRow1 = row - 1, adjRow2 = row + 1;
	int adjCol1 = col - 1, adjCol2 = col + 1;

	for (int r = adjRow1; r <= adjRow2; ++r)
	{
		for (int c = adjCol1; c <= adjCol2; ++c)
		{
			// remove the val from the other possible numbers 
			for (int i = 0; i < 9; ++i)
			{
				// row
				SudokuLevel[r][i].RemovePossible(SudokuLevel[r][c].nVal);
				// col
				SudokuLevel[i][c].RemovePossible(SudokuLevel[r][c].nVal);
			}

			for (int r2 = adjRow1; r2 <= adjRow2; ++r2)
			{
				for (int c2 = adjCol1; c2 <= adjCol2; ++c2)
				{
					if (c == c2 && r == r2)
						continue;
					SudokuLevel[r][c].RemovePossible(SudokuLevel[r2][c2].nVal);
				}
			}
		}
	}
}