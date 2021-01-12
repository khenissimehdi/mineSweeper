#include "grid.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h> 


/*
 * Allocate memory for a grid and initialize each cell.
 */
grid *create_grid(int x_size, int y_size) {
	int i, j;
	grid *g = (grid *)malloc(sizeof(grid));
	g->x_size = x_size;
	g->y_size = y_size;
	g->cells = (cell **)malloc(x_size*sizeof(cell *));
	for (i = 0; i < x_size; i++)
		g->cells[i] = (cell *)malloc(y_size*sizeof(cell));

	for (j = 0; j < y_size; j++)
		for (i = 0; i < x_size; i++) {
			g->cells[i][j].x_pos = i;
			g->cells[i][j].y_pos = j;
			g->cells[i][j].visible = 0;
			g->cells[i][j].marked = 0;
			g->cells[i][j].mine = 0;
			g->cells[i][j].mine_count = 0;
		}

	return g;
}

/*
 * Free memory for a grid.
 */
void free_grid(grid *g) {
	int i;
	for (i = 0; i < g->x_size; i++)
		free(g->cells[i]);
	free(g->cells);
	free(g);
}

/*
 * Set all cells to visible (for debugging).
 */
void set_all_visible(grid *g) {
	int x, y;
	for (x = 0; x < g->x_size; x++)
		for (y = 0; y < g->y_size; y++)
			g->cells[x][y].visible = 1;
}

/*
 * Add exactly n mines to grid g in random positions.
 */
void add_mines(grid *g, int n) {
	srand(time(NULL));
	int i ;
	int x ;
	int y ;
	int dx,dy;

	for ( i = 0; i < n; i++)
	{
		
		x = rand() % (g->y_size-1);
		y = rand() % (g->y_size-1);
		if (g->cells[x][y].mine != 1)
		{
			g->cells[x][y].mine = 1;
			for (dx = -1; dx <= 1; dx++){
				for (dy = -1; dy <= 1; dy++){
					if ((x+dx) >= 0 && (x+dx) <= g->x_size && (y+dy) >= 0 && (y+dy) <= g->y_size )
					{
						if (dx != 0 || dy != 0)
						{
							g->cells[x+dx][y+dy].mine_count += 1;
							
							
						}
						
					}
		}	
	}
			
		}
		else
		{
			i--;
		}
		

	}

		 
}

/*
 * Uncover cell c in grid g.
 * Return the total number of cells uncovered.
 */
int uncover(grid *g, cell *c) {
    int dx, dy;
    if (c->visible == 1 || c->marked == 1) {
        return 0;
    } else {
        c->visible = 1;
        draw_cell_actualise_window(c);

        if (c->mine == 1 || c->mine_count > 0) {
            return 1;

        } else {
            for (dx = -1; dx <= 1; dx++) {
                for (dy = -1; dy <= 1; dy++) {
                    if ((c->x_pos + dx) >= 0 && (c->x_pos + dx) <= g->x_size && (c->y_pos + dy) >= 0 &&
                        (c->y_pos + dy) <= g->y_size) {
                        if (dx != 0 || dy != 0) {
                            cell *v = &g->cells[c->x_pos + dx][c->y_pos + dy];
                            return uncover(g, v) + 1;

                        }

                    }
                }
            }
        }
    }
    return 0;
}

