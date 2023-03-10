#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define row 9
#define col 9
#define bomb 10
#define BOMB 9


typedef struct Cell
{
	int status; // 0 à 9 (9 == BOMB)
	int hidden; //0 si découvert 1 si caché
	int flag; //0 si placé 1 si pas de drapeau
	int secure;

} Cell;

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////



void createTable(Cell grid[row][col])
{
	int i, j;
	int h = 0;

	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			grid[i][j].status = 0;
			grid[i][j].hidden = 1;
			grid[i][j].flag = 1;
			grid[i][j].secure = 1;
		}
	}

	while (h < bomb)
	{
		int i = rand() % row;
		int j = rand() % col;
		if (grid[i][j].status == 0)
		{
			grid[i][j].status = BOMB;
			h++;
		}
	}
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			if (grid[i][j].status == 0)
			{
				int countBomb = 0;
				if (i - 1 >= 0 && j - 1 >= 0 && grid[i - 1][j - 1].status == BOMB) { countBomb++; }

				if (i - 1 >= 0 && grid[i - 1][j].status == BOMB) { countBomb++; }

				if (i - 1 >= 0 && j + 1 < col && grid[i - 1][j + 1].status == BOMB) { countBomb++; }

				if (j - 1 >= 0 && grid[i][j - 1].status == BOMB) { countBomb++; }

				if (j + 1 < col && grid[i][j + 1].status == BOMB) { countBomb++; }

				if (i + 1 < row && j - 1 >= 0 && grid[i + 1][j - 1].status == BOMB) { countBomb++; }

				if (i + 1 < row && grid[i + 1][j].status == BOMB) { countBomb++; }

				if (i + 1 < row && j + 1 < col && grid[i + 1][j + 1].status == BOMB) { countBomb++; }
				grid[i][j].status = countBomb;
			}
		}
	}

	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{

			printf("Status = %u %u %u \n", grid[i][j].status, i, j);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


void afficherJeuDebug(int nGrid, Cell grid[row][col])
{
	int c, l, i, j;

	printf("Debug Grille #%d\n", nGrid);
	printf("   |");
	for (c = 1; c < col + 1; c++)
	{
		printf(" %d ", c);
	}
	printf("\n");
	printf("___|");
	for (l = 1; l < col + 1; l++)
	{
		printf("___");
	}
	printf("\n");

	for (i = 0; i < row; i++)
	{
		if (i < 9)
			printf(" %d |", i + 1);
		else
			printf("%d |", i + 1);
		for (j = 0; j < col; j++)
		{
			int x = grid[i][j].status;
			int y = 0;
			int z = grid[i][j].flag;

			if (y == 1)
				printf(" %c ", 254);
			else if (z == 0)
				printf(" %c ", 159);
			else if (x >= 1 && x <= 8)
				printf(" %d ", x);
			else if (x == BOMB)
				printf(" B ");
			else if (x == 0)
				printf("   ");

		}
		printf("\n");
	}
}

void afficherJeu(int nGrid, Cell grid[row][col])
{

	afficherJeuDebug(nGrid, grid);

	int c, l, i, j;

	printf("Grille #%d\n", nGrid);
	printf("   |");
	for (c = 1; c < col + 1; c++)
	{
		printf(" %d ", c);
	}
	printf("\n");
	printf("___|");
	for (l = 1; l < col + 1; l++)
	{
		printf("___");
	}
	printf("\n");

	for (i = 0; i < row; i++)
	{
		if (i < 9)
			printf(" %d |", i + 1);
		else
			printf("%d |", i + 1);
		for (j = 0; j < col; j++)
		{
			int x = grid[i][j].status;
			int y = grid[i][j].hidden;
			int z = grid[i][j].flag;

			if (y == 1)
				printf(" %c ", 254);
			else if (z == 0)
				printf(" %c ", 159);
			else if (x >= 1 && x <= 8)
				printf(" %d ", x);
			else if (x == BOMB)
				printf(" B ");
			else if (x == 0)
				printf("   ");

		}
		printf("\n");
	}
}

void afficherBomb(int nGrid, Cell grid[row][col])
{
	int i, j;
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			if (grid[i][j].status == 9)
			{
				grid[i][j].hidden = 0;
			}
		}
	}
	afficherJeu(nGrid, grid);
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


void flagTime(int nGrid, Cell grid[row][col], int* win)
{
	system("CLS");
	afficherJeu(nGrid, grid);
	int fRow, fCol;
	printf("Quelle ligne et colonne choisissez-vous ? \n");
	scanf_s("%d %d", &fRow, &fCol);
	if (fRow <= row && fRow >= 1 && fCol <= col && fCol >= 1)
	{
		if (grid[fRow - 1][fCol - 1].flag == 0)
		{
			grid[fRow - 1][fCol - 1].hidden = 1;
			grid[fRow - 1][fCol - 1].flag = 1;
			system("CLS");
			afficherJeu(nGrid, grid);
			printf("\nDrapeau retirer en %d %d \n\n", fRow, fCol);
		}
		else if (grid[fRow - 1][fCol - 1].secure == 0)
		{
			system("CLS");
			afficherJeu(nGrid, grid);
			printf("\nCase %d %d deja securiser\n\n", fRow, fCol);
		}
		else
		{
			grid[fRow - 1][fCol - 1].hidden = 0;
			grid[fRow - 1][fCol - 1].flag = 0;
			if (grid[fRow - 1][fCol - 1].status == BOMB)
			{
				*win -= 1;
			}
			system("CLS");
			afficherJeu(nGrid, grid);
		}
	}

}

void diggingAround(int dRow, int dCol, Cell grid[row][col])
{
	if (grid[dRow][dCol].hidden == 0)
	{
		return;
	}

	if (grid[dRow][dCol].status != 0 && grid[dRow][dCol].status != BOMB)
	{
		grid[dRow][dCol].hidden = 0;
		return;
	}

	grid[dRow][dCol].hidden = 0;

	if (dRow + 1 < row)
	{
		diggingAround(dRow + 1, dCol, grid);
	}

	if (dRow - 1 >= 0)
	{
		diggingAround(dRow - 1, dCol, grid);
	}

	if (dCol + 1 < row)
	{
		diggingAround(dRow, dCol + 1, grid);
	}

	if (dCol - 1 >= 0)
	{
		diggingAround(dRow, dCol - 1, grid);
	}

	if (dRow + 1 < row && dCol + 1 < row)
	{
		diggingAround(dRow + 1, dCol + 1, grid);
	}

	if (dRow - 1 >= 0 && dCol - 1 >= 0)
	{
		diggingAround(dRow - 1, dCol - 1, grid);
	}

	if (dRow - 1 >= 0 && dCol + 1 < row)
	{
		diggingAround(dRow - 1, dCol + 1, grid);
	}

	if (dRow + 1 < row && dCol - 1 >= 0)
	{
		diggingAround(dRow + 1, dCol - 1, grid);
	}
}

void diggingTime(int nGrid, Cell grid[row][col], int* vie, char replay)
{
	system("CLS");
	afficherJeu(nGrid, grid);
	int dRow, dCol;
	printf("Quelle ligne et colonne choisissez-vous ? \n");
	scanf_s("%d %d", &dRow, &dCol);

	int count = 0;
	while (getchar() != '\n')
	{
		count++;
	}

	if (dRow <= row && dRow >= 1 && dCol <= col && dCol >= 1)
	{
		if (grid[dRow - 1][dCol - 1].status == 9)
		{
			*vie = 0;
			system("CLS");
			afficherBomb(nGrid, grid);
			printf("Game Over !\n");

		}
		else if (grid[dRow - 1][dCol - 1].secure == 0)
		{
			system("CLS");
			afficherJeu(nGrid, grid);
			printf("\nCase %d %d deja revele\n\n", dRow, dCol);
		}
		else
		{
			grid[dRow - 1][dCol - 1].secure = 0;
			if (grid[dRow - 1][dCol - 1].status != BOMB) { diggingAround(dRow - 1, dCol - 1, grid); }
			system("CLS");
			afficherJeu(nGrid, grid);
		}
	}
	else
	{
		system("CLS");
		afficherJeu(nGrid, grid);
		printf("Error OOB | Les valeurs entrees ne rentrent pas dans le tableau \n\n");
	}
}

void playingTime(int nGrid, Cell grid[row][col], int vie, int win, char replay)
{

	system("CLS");
	afficherJeu(nGrid, grid);
	int cMove;
	while (vie == 1)
	{
		printf("Quel sera votre prochain coup ?\n\n 1: Placer un drapeau\n 2: Creuser une case\n");
		scanf_s("%d", &cMove);

		int count = 0;
		while (getchar() != '\n')
		{
			count++;
		}

		if (cMove == 1)
		{
			flagTime(nGrid, grid, &win);
			if (win == 0)
			{
				vie = 0;
				printf("Bien jouer terrain deminer !\n\n");
			}
		}
		else if (cMove == 2)
		{
			diggingTime(nGrid, grid, &vie, replay);
		}
		else
		{
			system("CLS");
			afficherJeu(nGrid, grid);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


int main()
{
	srand((unsigned int)time(NULL));
	int vie = 1;

	Cell grid[row][col];
	int nGrid = 1;
	int win = bomb; //nombre de cases sans bombe


	char replay = 'y';
	while (replay == 'y')
	{
		system("CLS");
		createTable(grid);
		playingTime(nGrid, grid, vie, win, replay);
		printf("Voulez-vous rejouer ? (y/n)\n");
		scanf_s("%c", &replay, 1);
		while (replay != 'n' && replay != 'y')
		{
			printf("Veuillez rentrer y ou n \n");
			scanf_s("%c", &replay, 1);
			while (getchar() != '\n');
		}
		nGrid++;
	}
	return 0;
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
