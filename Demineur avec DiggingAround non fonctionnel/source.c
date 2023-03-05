#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define row 9
#define col 9
#define bomb 20
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
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


void afficherJeu(int nGrid, Cell grid[row][col])
{
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

void diggingAround(int* dRow, int* dCol, Cell grid[row][col])
{
	int i, j;
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			if (i - 1 >= 0 && j - 1 >= 0 /*security OOB */ && grid[*dRow - 1][*dCol - 1].status != BOMB /*status diff de bomb (0 - 8 only)*/)
			{
				grid[*dRow - 1][*dCol - 1].hidden = 0;
			}
			if (i - 1 >= 0 /*security OOB */ && grid[*dRow - 1][*dCol].status != BOMB /*status diff de bomb (0 - 8 only)*/)
			{
				grid[*dRow - 1][*dCol].hidden = 0;
			}
			if (i - 1 >= 0 && j + 1 >= 0 /*security OOB */ && grid[*dRow - 1][*dCol + 1].status != BOMB /*status diff de bomb (0 - 8 only)*/)
			{
				grid[*dRow - 1][*dCol + 1].hidden = 0;
			}
			if (j - 1 >= 0 /*security OOB */ && grid[*dRow][*dCol - 1].status != BOMB /*status diff de bomb (0 - 8 only)*/)
			{
				grid[*dRow][*dCol - 1].hidden = 0;
			}
			if (j + 1 < col /*security OOB */ && grid[*dRow][*dCol + 1].status != BOMB /*status diff de bomb (0 - 8 only)*/)
			{
				grid[*dRow][*dCol + 1].hidden = 0;
			}
			if (i + 1 < row && j - 1 >= 0 /*security OOB */ && grid[*dRow + 1][*dCol - 1].status != BOMB /*status diff de bomb (0 - 8 only)*/)
			{
				grid[*dRow + 1][*dCol - 1].hidden = 0;
			}

			if (i + 1 < row /*security OOB */ && grid[*dRow + 1][*dCol].status != BOMB /*status diff de bomb (0 - 8 only)*/)
			{
				grid[*dRow + 1][*dCol].hidden = 0;
			}

			if (i + 1 < row && j + 1 < col /*security OOB */ && grid[*dRow + 1][*dCol + 1].status != BOMB /*status diff de bomb (0 - 8 only)*/)
			{
				grid[*dRow + 1][*dCol + 1].hidden = 0;
			}

		}
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
			grid[dRow - 1][dCol - 1].hidden = 0;
			grid[dRow - 1][dCol - 1].secure = 0;
			if (grid[dRow - 1][dCol - 1].status == 0) { diggingAround(&dRow, &dCol, grid); }
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
