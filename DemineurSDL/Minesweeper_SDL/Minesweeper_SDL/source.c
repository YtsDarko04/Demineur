#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define row 10
#define col 10
#define bomb 10
#define BOMB 9


typedef struct Cell
{
	int status; // 0 � 9 (9 == BOMB)
	int hidden; //0 si decouvert 1 si cache
	int flag; //0 si place 1 si pas de drapeau
	int secure; //0 si case securise 1 si case non securise

} Cell;


int init(SDL_Window** window, SDL_Renderer** renderer, int w, int h)
{
	if (0 != SDL_Init(SDL_INIT_VIDEO))
	{
		fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
		return -1;
	}
	if (0 != SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_SHOWN, window, renderer))
	{
		fprintf(stderr, "Erreur SDL_CreateWindowAndRenderer : %s", SDL_GetError());
		return -1;
	}
	return 0;
}
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


//void afficherJeu(int nGrid, Cell grid[row][col])
//{
//	int c, l, i, j;
//
//	printf("Grille #%d\n", nGrid);
//	printf("   |");
//	for (c = 1; c < col + 1; c++)
//	{
//		printf(" %d ", c);
//	}
//	printf("\n");
//	printf("___|");
//	for (l = 1; l < col + 1; l++)
//	{
//		printf("___");
//	}
//	printf("\n");
//
//	for (i = 0; i < row; i++)
//	{
//		if (i < 9)
//			printf(" %d |", i + 1);
//		else
//			printf("%d |", i + 1);
//		for (j = 0; j < col; j++)
//		{
//			int x = grid[i][j].status;
//			int y = grid[i][j].hidden;
//			int z = grid[i][j].flag;
//
//			if (y == 1)
//				printf(" %c ", 254);
//			else if (z == 0)
//				printf(" %c ", 159);
//			else if (x >= 1 && x <= 8)
//				printf(" %d ", x);
//			else if (x == BOMB)
//				printf(" B ");
//			else if (x == 0)
//				printf("   ");
//
//		}
//		printf("\n");
//	}
//}


void tabTexture(SDL_Renderer* renderer, SDL_Texture* texture[12])
{
	SDL_Surface* pSurface[12];
	pSurface[0] = SDL_LoadBMP("img/vide.bmp");
	pSurface[1] = SDL_LoadBMP("img/un.bmp");
	pSurface[2] = SDL_LoadBMP("img/deux.bmp");
	pSurface[3] = SDL_LoadBMP("img/trois.bmp");
	pSurface[4] = SDL_LoadBMP("img/quatre.bmp");
	pSurface[5] = SDL_LoadBMP("img/cinq.bmp");
	pSurface[6] = SDL_LoadBMP("img/six.bmp");
	pSurface[7] = SDL_LoadBMP("img/sept.bmp");
	pSurface[8] = SDL_LoadBMP("img/huit.bmp");
	pSurface[9] = SDL_LoadBMP("img/bomb.bmp");
	pSurface[10] = SDL_LoadBMP("img/nondecouvert.bmp");
	pSurface[11] = SDL_LoadBMP("img/flag.bmp");

	texture[0] = SDL_CreateTextureFromSurface(renderer, pSurface[0]);
	texture[1] = SDL_CreateTextureFromSurface(renderer, pSurface[1]);
	texture[2] = SDL_CreateTextureFromSurface(renderer, pSurface[2]);
	texture[3] = SDL_CreateTextureFromSurface(renderer, pSurface[3]);
	texture[4] = SDL_CreateTextureFromSurface(renderer, pSurface[4]);
	texture[5] = SDL_CreateTextureFromSurface(renderer, pSurface[5]);
	texture[6] = SDL_CreateTextureFromSurface(renderer, pSurface[6]);
	texture[7] = SDL_CreateTextureFromSurface(renderer, pSurface[7]);
	texture[8] = SDL_CreateTextureFromSurface(renderer, pSurface[8]);
	texture[9] = SDL_CreateTextureFromSurface(renderer, pSurface[9]);
	texture[10] = SDL_CreateTextureFromSurface(renderer, pSurface[10]);
	texture[11] = SDL_CreateTextureFromSurface(renderer, pSurface[11]);

	for (int i = 0; i < 12; ++i)
	{
		SDL_FreeSurface(pSurface[i]);
	}
}

void displayGrid(SDL_Renderer* renderer, Cell grid[row][col], SDL_Texture *texture[])
{
	SDL_RenderClear(renderer);
	SDL_Rect rectangle;
	rectangle.x = 0;
	rectangle.y = 0;
	
	int i, j;

	for (i = 0; i < 10; i++)
	{
		rectangle.y = 50 * i;
		for (j = 0; j < 10; j++)
		{
			int x = grid[i][j].status;
			int y = grid[i][j].hidden;
			int z = grid[i][j].flag;
			int target = 0;

			if (y == 1)
				target = 10;
			else if (z == 0)
				target = 11;
			else if (x >= 1 && x <= 8)
			{
				if (x == 1) target = 1;
				else if (x == 2) target = 2;
				else if (x == 3) target = 3;
				else if (x == 4) target = 4;
				else if (x == 5) target = 5;
				else if (x == 6) target = 6;
				else if (x == 7) target = 7;
				else if (x == 8) target = 8;
			}
			else if (x == BOMB)
				target = 9;
			else if (x == 0)
				target = 0;

			rectangle.x = 50 * j;
			SDL_QueryTexture(texture[target], NULL, NULL, &rectangle.h, &rectangle.w);

			SDL_RenderCopy(renderer, texture[target], NULL, &rectangle);
			SDL_DestroyTexture(texture);
		}
	}
	SDL_RenderPresent(renderer);
	
}

int main(int argc, char* argv)
{
	srand((unsigned int)time(NULL));
	Cell grid[row][col];

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Texture* texture[12];
	int statut = EXIT_FAILURE;
	if (0 != init(&window, &renderer, 640, 480)) /* ecrire cette fonction */
		goto Quit;

	tabTexture(&renderer, texture);
	displayGrid(&renderer, grid, texture);
	
	SDL_Delay(5000);

Quit:
	if (NULL != renderer)
		SDL_DestroyRenderer(renderer);
	if (NULL != window)
		SDL_DestroyWindow(window);
	SDL_Quit();
	return statut;
}
//void afficherBomb(int nGrid, Cell grid[row][col])
//{
//	int i, j;
//	for (i = 0; i < row; i++)
//	{
//		for (j = 0; j < col; j++)
//		{
//			if (grid[i][j].status == 9)
//			{
//				grid[i][j].hidden = 0;
//			}
//		}
//	}
//	afficherJeu(nGrid, grid);
//}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
//void victory(int nGrid, Cell grid[row][col], int* win)
//{
//	int count = *win;
//	int i, j;
//	for (i = 0; i < row; i++)
//	{
//		for (j = 0; j < col; j++)
//		{
//			if (grid[i][j].status != 9 && grid[i][j].hidden == 0)
//			{
//				count -= 1;
//				if (count == 0)
//				{
//					*win = 0;
//				}
//			}
//		}
//	}
//}
//
//void flagTime(int nGrid, Cell grid[row][col])
//{
//	system("CLS");
//	afficherJeu(nGrid, grid);
//	int fRow, fCol;
//	printf("Quelle ligne et colonne choisissez-vous ? \n");
//	scanf_s("%d %d", &fRow, &fCol);
//	if (fRow <= row && fRow >= 1 && fCol <= col && fCol >= 1)
//	{
//		if (grid[fRow - 1][fCol - 1].flag == 0)
//		{
//			grid[fRow - 1][fCol - 1].hidden = 1;
//			grid[fRow - 1][fCol - 1].flag = 1;
//			system("CLS");
//			afficherJeu(nGrid, grid);
//			printf("\nDrapeau retirer en %d %d \n\n", fRow, fCol);
//		}
//		else if (grid[fRow - 1][fCol - 1].secure == 0)
//		{
//			system("CLS");
//			afficherJeu(nGrid, grid);
//			printf("\nCase %d %d deja securiser\n\n", fRow, fCol);
//		}
//		else
//		{
//			grid[fRow - 1][fCol - 1].hidden = 0;
//			grid[fRow - 1][fCol - 1].flag = 0;
//			system("CLS");
//			afficherJeu(nGrid, grid);
//		}
//	}
//
//}
//
//void diggingTime(int nGrid, Cell grid[row][col], int* vie, char replay)
//{
//	system("CLS");
//	afficherJeu(nGrid, grid);
//	int dRow, dCol;
//	printf("Quelle ligne et colonne choisissez-vous ? \n");
//	scanf_s("%d %d", &dRow, &dCol);
//
//	int count = 0;
//	while (getchar() != '\n')
//	{
//		count++;
//	}
//
//	if (dRow <= row && dRow >= 1 && dCol <= col && dCol >= 1)
//	{
//		if (grid[dRow - 1][dCol - 1].status == 9)
//		{
//			*vie = 0;
//			system("CLS");
//			afficherBomb(nGrid, grid);
//			printf("Game Over !\n");
//
//		}
//		else if (grid[dRow - 1][dCol - 1].secure == 0)
//		{
//			system("CLS");
//			afficherJeu(nGrid, grid);
//			printf("\nCase %d %d deja revele\n\n", dRow, dCol);
//		}
//		else if (grid[dRow - 1][dCol - 1].flag == 0)
//		{
//			system("CLS");
//			afficherJeu(nGrid, grid);
//			printf("\nCase %d %d securiser par un drapeau\n\n", dRow, dCol);
//		}
//		else
//		{
//			grid[dRow - 1][dCol - 1].hidden = 0;
//			grid[dRow - 1][dCol - 1].secure = 0;
//			system("CLS");
//			afficherJeu(nGrid, grid);
//		}
//	}
//	else
//	{
//		system("CLS");
//		afficherJeu(nGrid, grid);
//		printf("Error OOB | Les valeurs entrees ne rentrent pas dans le tableau \n\n");
//	}
//}
//
//void playingTime(int nGrid, Cell grid[row][col], int vie, int* win, char replay)
//{
//	system("CLS");
//	afficherJeu(nGrid, grid);
//	int cMove;
//	while (vie == 1)
//	{
//		printf("Quel sera votre prochain coup ?\n\n 1: Placer un drapeau\n 2: Creuser une case\n");
//		scanf_s("%d", &cMove);
//
//		int count = 0;
//		while (getchar() != '\n')
//		{
//			count++;
//		}
//
//		if (cMove == 1)
//		{
//			flagTime(nGrid, grid);
//		}
//		else if (cMove == 2)
//		{
//			diggingTime(nGrid, grid, &vie, replay);
//			victory(nGrid, grid, &win);
//			if (win == 0)
//			{
//				vie = 0;
//				printf("Bien jouer terrain deminer !\n\n");
//			}
//		}
//		else
//		{
//			system("CLS");
//			afficherJeu(nGrid, grid);
//		}
//	}
//}
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
//int main()
//{
//	srand((unsigned int)time(NULL));
//	int vie = 1;
//	Cell grid[row][col];
//	int nGrid = 1;
//	int win = row * col - bomb; //nombre de cases sans bombe
//
//	char replay = 'y';
//	while (replay == 'y')
//	{
//		system("CLS");
//		createTable(grid);
//		playingTime(nGrid, grid, vie, win, replay);
//		printf("Voulez-vous rejouer ? (y/n)\n");
//		scanf_s("%c", &replay, 1);
//		while (replay != 'n' && replay != 'y')
//		{
//			printf("Veuillez rentrer y ou n \n");
//			scanf_s("%c", &replay, 1);
//			while (getchar() != '\n');
//		}
//		nGrid++;
//	}
//	return 0;
//}