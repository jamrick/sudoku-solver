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
			if (nvPossible.Size() == 1)
			{
				CompleteCell(nvPossible[0]);
				return true;
			}
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

		//// level is loaded, take out exisitng values
		//for (int r = 0; r < 9; r++)
		//{
		//	for (int c = 0; c < 9; c++)
		//	{
		//		RemoveExistingRow(r, c);
		//		RemoveExistingCol(r, c);
		for (int startRow = 1; startRow < 9; startRow += 3)
		{
			for (int startCol = 1; startCol < 9; startCol += 3)
			{
				RemoveExisting(startRow, startCol);
			}
		}
		//	}
		//}

	}
}
void SudokuSolver::ResetCells()
{
	for (int r = 0; r < 9; r++)
		for (int c = 0; c < 9; c++)
			SudokuLevel[r][c].ResetCell();
}
void SudokuSolver::Print() const
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
			if ((c + 1) % 3 == 0)
				cout << ' ';
		}
		cout << '\n';
		if ((r + 1) % 3 == 0)
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
void SudokuSolver::RemoveExistingRow(int row, int col)
{
	for (int c = 0; c < 9; ++c)
	{
		if (c == col)
			continue;
		if (SudokuLevel[row][c].RemovePossible(SudokuLevel[row][col].nVal))
		{
			RemoveExistingRow(row, c);
			RemoveExistingCol(row, c);
		}
	}
}
void SudokuSolver::RemoveExistingCol(int row, int col)
{
	for (int r = 0; r < 9; ++r)
	{
		if (r == row)
			continue;
		if (SudokuLevel[r][col].RemovePossible(SudokuLevel[row][col].nVal))
		{
			RemoveExistingRow(r, col);
			RemoveExistingCol(r, col);
		}
	}
}
void SudokuSolver::RemoveExistingBox(int row, int col, int val)
{
	int adjRow1 = row - 1, adjRow2 = row + 1;
	int adjCol1 = col - 1, adjCol2 = col + 1;
	for (int r = adjRow1; r <= adjRow2; ++r)
	{
		for (int c = adjCol1; c <= adjCol2; ++c)
		{
			if (col == c && row == r)
				continue;
			if (SudokuLevel[r][c].RemovePossible(val/*SudokuLevel[row][col].nVal*/))
			{
				RemoveExistingRow(r, c);
				RemoveExistingCol(r, c);
			}
		}
	}
}

// solution algorithm putting everything together to solve the sudoku board.
void SudokuSolver::Solve()
{
	while (!CompleteSudoku())
	{
		Update();
	}
}

void SudokuSolver::Update()
{
	// search the grid and remove any possible values that have already been solved
	bool found1 = false;
	for (int startRow = 1; startRow < 9; startRow += 3)
	{
		for (int startCol = 1; startCol < 9; startCol += 3)
		{
			RemoveExisting(startRow, startCol);

			// handle naked pair row
			jhVector<SudokuIndex> nakedPairRow = FindNakedPairRow(startRow, startCol);
			if (nakedPairRow.Size() == 2)
			{
				RemoveNakedPairRow(nakedPairRow);//break;
			}

			// handle naked pair col
			jhVector<SudokuIndex> nakedPairCol = FindNakedPairCol(startRow, startCol);
			if (nakedPairCol.Size() == 2)
			{
				RemoveNakedPairCol(nakedPairCol);//break;
			}
			// handle naked pair box
			jhVector<SudokuIndex> nakedPairBox = FindNakedPairBox(startRow, startCol);
			if (nakedPairBox.Size() == 2)
			{
				RemoveNakedPairBox(nakedPairBox, startRow, startCol);//break;
			}

			//if (!CheckPossibleRow(startRow, startCol))
			//CheckPossibleRow(startRow, startCol);
			//	if (!CheckPossibleCol(startRow, startCol))
			//		
			//if (found1)
			//	return;
			//{
			//	//RemoveExisting(startRow, startCol);
			//	break;
			//}
			//	
			//found1 = 
			//CheckPossibleCol(startRow, startCol);//CheckPossibleBox(startRow, startCol);
			//if (found1)
			//	return;
			//	break;
			/*found1 = *///CheckPossibleBox(startRow, startCol);
			//if (found1)
			//	return;
			//	break;
		}
	}
}

bool SudokuSolver::CheckPossibleBox(int row, int col)
{
	int adjRow1 = row - 1, adjRow2 = row + 1;
	int adjCol1 = col - 1, adjCol2 = col + 1;
	for (int r = adjRow1; r <= adjRow2; ++r)
	{
		for (int c = adjCol1; c <= adjCol2; ++c)
		{

			//for (int c = 0; c < 9; ++c)
			//{
			for (int i = 0; i < SudokuLevel[r][c].nvPossible.Size(); ++i)
			{
				//int ar1 = row - 1, ar2 = row + 1;
				//int ac1 = col - 1, ac2 = col + 1;
				bool found = false;
				for (int r2 = adjRow1; r2 <= adjRow2; ++r2)
				{
					for (int c2 = adjCol1; c2 <= adjCol2; ++c2)
					{

						//for (int c2 = 0; c2 < 9; ++c2)
						//{
						if (c == c2 && r == r2)
						{
							continue;
						}
						if (SudokuLevel[r2][c2].nvPossible.Size() > 0)
						{
							for (int i2 = 0; i2 < SudokuLevel[r2][c2].nvPossible.Size() && found == false; ++i2)
							{
								found = SudokuLevel[r][c].nvPossible[i] == SudokuLevel[r2][c2].nvPossible[i2];
							}
						}
						else
						{
							found = SudokuLevel[r][c].nvPossible[i] == SudokuLevel[r2][c2].nVal;
						}
						if (found == true)
						{
							break;
						}
					}
				}
				if (found == false)
				{
					SudokuLevel[r][c].CompleteCell(SudokuLevel[r][c].nvPossible[i]);
					RemoveExistingBox(row, col, SudokuLevel[r][c].nVal);
					//c = -1;
					return true;
				}
			}
			//}

		}
	}
	return false;
}

bool SudokuSolver::CheckPossibleRow(int row, int col)
{
	int adjRow1 = row - 1, adjRow2 = row + 1;
	//int adjCol1 = col - 1, adjCol2 = col + 1;
	for (int r = adjRow1; r <= adjRow2; ++r)
	{
		//for (int c = adjCol1; c <= adjCol2; ++c)
		//{

		for (int c = 0; c < 9; ++c)
		{
			for (int i = 0; i < SudokuLevel[r][c].nvPossible.Size(); ++i)
			{
				bool found = false;
				for (int c2 = 0; c2 < 9; ++c2)
				{
					if (c == c2)
					{
						continue;
					}
					if (SudokuLevel[r][c2].nvPossible.Size() > 0)
					{
						for (int i2 = 0; i2 < SudokuLevel[r][c2].nvPossible.Size() && found == false; ++i2)
						{
							found = SudokuLevel[r][c].nvPossible[i] == SudokuLevel[r][c2].nvPossible[i2];
						}
					}
					else
					{
						found = SudokuLevel[r][c].nvPossible[i] == SudokuLevel[r][c2].nVal;
					}
					if (found == true)
					{
						break;
					}
				}
				if (found == false)
				{
					SudokuLevel[r][c].CompleteCell(SudokuLevel[r][c].nvPossible[i]);
					RemoveExistingCol(r, c);
					//c = -1;
					return true;
				}
			}
		}

		//}
	}
	return false;
}
bool SudokuSolver::CheckPossibleCol(int row, int col)
{
	//int adjRow1 = row - 1, adjRow2 = row + 1;
	int adjCol1 = col - 1, adjCol2 = col + 1;
	//for (int r = adjRow1; r <= adjRow2; ++r)
	{
		for (int c = adjCol1; c <= adjCol2; ++c)
		{

			for (int r = 0; r < 9; ++r)
			{
				for (int i = 0; i < SudokuLevel[r][c].nvPossible.Size(); ++i)
				{
					bool found = false;
					for (int r2 = 0; r2 < 9; ++r2)
					{
						if (c == r2)
						{
							continue;
						}
						if (SudokuLevel[r2][c].nvPossible.Size() > 0)
						{
							for (int i2 = 0; i2 < SudokuLevel[r2][c].nvPossible.Size() && found == false; ++i2)
							{
								found = SudokuLevel[r][c].nvPossible[i] == SudokuLevel[r2][c].nvPossible[i2];
							}
						}
						else
						{
							found = SudokuLevel[r][c].nvPossible[i] == SudokuLevel[r2][c].nVal;
						}
						if (found == true)
						{
							break;
						}
					}
					if (found == false)
					{
						SudokuLevel[r][c].CompleteCell(SudokuLevel[r][c].nvPossible[i]);
						RemoveExistingRow(r, c);
						//c = -1;
						return true;
					}
				}
			}
		}
	}
	return false;
}

jhVector<SudokuSolver::SudokuIndex> SudokuSolver::FindNakedPairRow(int row, int col)
{
	jhVector<SudokuIndex> nakedHolder;
	int adjRow1 = row - 1, adjRow2 = row + 1;
	//int adjCol1 = col - 1, adjCol2 = col + 1;

	for (int r = adjRow1; r <= adjRow2; ++r)
	{
		for (int i = 0; i < 9; ++i)
		{
			if (SudokuLevel[r][i].nvPossible.Size() == 2)
			{
				for (int j = i + 1; j < 9; ++j)
				{
					if (SudokuLevel[r][j].nvPossible.Size() == 2)
					{
						bool foundPair = true;
						for (int c = 0; c < SudokuLevel[r][i].nvPossible.Size(); ++c)
						{
							if (SudokuLevel[r][i].nvPossible[c] != SudokuLevel[r][j].nvPossible[c])
							{
								foundPair = false;
								break;
							}
						}
						if (foundPair == true)
						{
							SudokuIndex si1(r, i);
							SudokuIndex si2(r, j);
							nakedHolder.Append(si1);
							nakedHolder.Append(si2);
							return nakedHolder;
						}
					}
				}
			}
		}
	}

	return nakedHolder;
}
jhVector<SudokuSolver::SudokuIndex> SudokuSolver::FindNakedPairCol(int row, int col)
{
	jhVector<SudokuIndex> nakedHolder;
	//int adjRow1 = row - 1, adjRow2 = row + 1;
	int adjCol1 = col - 1, adjCol2 = col + 1;

	for (int c = adjCol1; c <= adjCol2; ++c)
	{
		for (int i = 0; i < 9; ++i)
		{
			if (SudokuLevel[i][c].nvPossible.Size() == 2)
			{
				for (int j = i + 1; j < 9; ++j)
				{
					if (SudokuLevel[j][c].nvPossible.Size() == 2)
					{
						bool foundPair = true;
						for (int x = 0; x < SudokuLevel[i][c].nvPossible.Size(); ++x)
						{
							if (SudokuLevel[i][c].nvPossible[x] != SudokuLevel[j][c].nvPossible[x])
							{
								foundPair = false;
								break;
							}
						}
						if (foundPair == true)
						{
							SudokuIndex si1(i, c);
							SudokuIndex si2(j, c);
							nakedHolder.Append(si1);
							nakedHolder.Append(si2);
							return nakedHolder;
						}
					}
				}
			}
		}
	}
	return nakedHolder;
}
jhVector<SudokuSolver::SudokuIndex> SudokuSolver::FindNakedPairBox(int row, int col)
{
	jhVector<SudokuIndex> nakedHolder;
	int adjRow1 = row - 1, adjRow2 = row + 1;
	int adjCol1 = col - 1, adjCol2 = col + 1;
	for (int r = adjRow1; r <= adjRow2; ++r)
	{
		for (int c = adjCol1; c <= adjCol2; ++c)
		{
			if (SudokuLevel[r][c].nvPossible.Size() == 2)
			{
				for (int r2 = adjRow1; r2 <= adjRow2; ++r2)
				{
					for (int c2 = adjCol1; c2 <= adjCol2; ++c2)
					{
						if (r2 == r && c2 == c) continue;
						if (SudokuLevel[r2][c2].nvPossible.Size() == 2)
						{
							bool foundPair = true;
							for (int x = 0; x < SudokuLevel[r][c].nvPossible.Size(); ++x)
							{
								if (SudokuLevel[r][c].nvPossible[x] != SudokuLevel[r2][c2].nvPossible[x])
								{
									foundPair = false;
									break;
								}
							}
							if (foundPair == true)
							{
								SudokuIndex si1(r, c);
								SudokuIndex si2(r2, c2);
								nakedHolder.Append(si1);
								nakedHolder.Append(si2);
								return nakedHolder;
							}
						}
					}
				}
			}
		}
	}
	return nakedHolder;
}

void SudokuSolver::RemoveNakedPairRow(jhVector<SudokuSolver::SudokuIndex> nakedHolder)
{
	int row = nakedHolder[0].row;
	int col = nakedHolder[0].col;

	for (int i = 0; i < 9; ++i)
	{
		if (i == nakedHolder[0].col || i == nakedHolder[1].col)
		{
			continue;
		}

		for (int j = 0; j < SudokuLevel[row][col].nvPossible.Size(); ++j)
		{
			SudokuLevel[row][i].RemovePossible(SudokuLevel[row][col].nvPossible[j]);
		}
	}
}

void SudokuSolver::RemoveNakedPairCol(jhVector<SudokuSolver::SudokuIndex> nakedHolder)
{
	int row = nakedHolder[0].row;
	int col = nakedHolder[0].col;

	for (int i = 0; i < 9; ++i)
	{
		if (i == nakedHolder[0].row || i == nakedHolder[1].row)
		{
			continue;
		}

		for (int j = 0; j < SudokuLevel[row][col].nvPossible.Size(); ++j)
		{
			SudokuLevel[i][col].RemovePossible(SudokuLevel[row][col].nvPossible[j]);
		}
	}
}
void SudokuSolver::RemoveNakedPairBox(jhVector<SudokuSolver::SudokuIndex> nakedHolder, int row, int col)
{
	int adjRow1 = row - 1, adjRow2 = row + 1;
	int adjCol1 = col - 1, adjCol2 = col + 1;
	for (int r = adjRow1; r <= adjRow2; ++r)
	{
		for (int c = adjCol1; c <= adjCol2; ++c)
		{
			if ((r == nakedHolder[0].row && c == nakedHolder[0].col) || 
				(r == nakedHolder[1].row && c == nakedHolder[1].col))
			{
				continue;
			}

			for (int i = 0; i < SudokuLevel[nakedHolder[0].row][nakedHolder[0].col].nvPossible.Size(); ++i)
			{
				SudokuLevel[r][c].RemovePossible(SudokuLevel[nakedHolder[0].row][nakedHolder[0].col].nvPossible[i]);
			}
		}
	}
}