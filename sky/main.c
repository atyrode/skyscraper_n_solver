#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>


#define N		6
#define TOP		0
#define BOT		1
#define LEFT	2
#define RIGHT	3


typedef struct s_meta {
	int			clues[4][N];
	int			map[N * N];
	char 		**av;
}				t_meta;

void	print_sol(t_meta *data)
{
	int i = 0;
	int j;

	int k = 0;
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("  ");
	while (k < N)
		printf ("\e[1;34m%d \e[1;0m", data->clues[TOP][k++]);
	printf("\n");
	while (i < N)
	{
		j = 0;
		while (j < N)
		{
			if (!j)
				printf ("\e[1;34m%d \e[1;0m", data->clues[LEFT][i]);
			printf("%d ", data->map[i * N + j]);
			j++;
		}
		printf("\e[1;34m%d \e[1;0m\n", data->clues[RIGHT][i]);
		i++;
	}
	k = 0;
	printf("  ");
	while (k < N)
		printf ("\e[1;34m%d \e[1;0m", data->clues[BOT][k++]);
	printf("\n");

}

void	init(t_meta *data, char **av, int ac)
{
	if (ac != 5)
	{
		printf("not enough args\n");
		exit(1);
	}
	bzero(data, sizeof(t_meta));
	data->av = av;
}

void	fill_clues(char *str, int tab[N])
{
	int i = 0;

	while (i < N)
	{
		tab[i] = str[i] - '0';
		i++;
	}
}

void	parsing(t_meta *data)
{	
	int i = 0;
	
	while (i < 4)
	{
		fill_clues(data->av[i + 1], data->clues[i]);
		i++;
	}
}

int	check_double_row(int *row)
{
	int i = 0;
	int j;

	while (i < N)
	{
		j = 0;
		while (j < N)
		{
			if (j != i && row[j] && row[i] == row[j])
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int	check_double_col(int *col)
{
	int i = 0;
	int j;

	while (i < (N * N))
	{
		j = 0;
		while (j < (N * N))
		{
			if (j != i && col[j] && col[i] == col[j])
				return (0);
			j+= N;
		}
		i+= N;
	}
	return (1);
}

int	check_left_right(t_meta *data, int idx)
{
	int seen = 0;
	int high = 0;
	int i = (idx / N) * N;

	while (i <= idx)
	{
		if (data->map[i] > high)
		{
			high = data->map[i];
			seen++;
		}
		i++;
	}
	if (idx % N == N - 1)
			return (seen == data->clues[LEFT][idx / N] ? 1 : 0);
	return (seen <= data->clues[LEFT][idx / N] ? 1 : 0);
}

int	check_right_left(t_meta *data, int idx)
{
	int seen = 0;
	int high = 0;
	int i = ((idx / N) * N) + N - 1;

	while (i >= ((idx / N) * N))
	{
		if (data->map[i] > high)
		{
			high = data->map[i];
			seen++;
		}
		i--;
	}
	return (seen == data->clues[RIGHT][idx / N] ? 1 : 0);
}

int check_top_bot(t_meta *data, int idx)
{
	int seen = 0;
	int high = 0;
	int i = idx % N;

	while (i <= idx)
	{
		if (data->map[i] > high)
		{
			high = data->map[i];
			seen++;
		}
		i+= N;
	}
	if (idx >= (N - 1) * N)
		return (seen == data->clues[TOP][idx % N] ? 1 : 0);
	return (seen <= data->clues[TOP][idx % N] ? 1 : 0);
}

int check_bot_top(t_meta *data, int idx)
{
	int seen = 0;
	int high = 0;
	int i = idx % N  + ((N - 1) * N);

	while (i >= (idx % N))
	{
		if (data->map[i] > high)
		{
			high = data->map[i];
			seen++;
		}
		i -= N;
	}
	return (seen == data->clues[BOT][idx % N] ? 1 : 0);
}

int	try_piece(t_meta *data, int idx)
{
	int spam = 0;
	if (!check_double_col(&data->map[idx % N]))
		return (0);
	else if (spam)
		printf("double col VALID\n");
	if (!check_double_row(&data->map[idx / N * N]))
		return (0);
	else if (spam)
		printf("double row VALID\n");
	if (!check_left_right(data, idx))
		return (0);
	else if (spam)
		printf("left_right VALID\n");
	if (!check_top_bot(data, idx))
		return (0);
	else if (spam)
		printf("top bot VALID\n");
	
	if (idx % N == N - 1)
	{
		if (!check_right_left(data, idx))
			return (0);
		else if (spam)
			printf("right left VALID\n");
	}
	if (idx >= (N - 1) * N)
	{
		if (!check_bot_top(data, idx))
			return (0);
		else if (spam)
			printf("bot top VALID\n");
	}

	return (1);
}


int solve(t_meta *data, int idx)
{
	int piece = 1;
	
	if (idx >= (N * N))
		return (1);
	while (piece <= N)
	{
		data->map[idx] = piece;
	//printf("================== \\/ \\/ \\/ \\/ ========\n");
//	print_sol(data);
	char buff[2];
//	usleep(2000);	
//	read(0, buff, 1);
		
		if (try_piece(data, idx))
		{
			if (solve(data, idx + 1))
				return (1);
		}
		piece++;
	}
	data->map[idx] = 0;
	return (0);
}

int main(int ac, char **av)
{
	t_meta data;

	init(&data, av, ac);	
	parsing(&data);
	if (solve(&data, 0))
		print_sol(&data);
	else
		printf("Unsolvable bolos\n");
	
	return (1);
}
