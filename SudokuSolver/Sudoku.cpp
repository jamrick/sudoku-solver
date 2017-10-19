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
				if (SudokuLevel[r][c - 1].nVal != 0)
				{
					--nRemaining;
				}

				if (c == 9)
				{
					r++;
					c = 0;
				}
			}
			ifs.close();
		}

		// level is loaded, take out exisitng values
		//for (int startRow = 1; startRow < 9; startRow += 3)
		//{
		//	for (int startCol = 1; startCol < 9; startCol += 3)
		//	{
		//		RemoveExisting(startRow, startCol);
		//	}
		//}
	}
}
void SudokuSolver::ResetCells()
{
	for (int r = 0; r < 9; r++)
		for (int c = 0; c < 9; c++)
			SudokuLevel[r][c].ResetCell();

	nRemaining = 81;
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
	return nRemaining == 0;
}