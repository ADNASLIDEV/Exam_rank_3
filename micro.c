#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FAIL	0
#define SUCCESS 1
#define INNER	2
#define BORDER  3

typedef struct s_data
{
	int		w;
	int		h;
	char	b;
	char	*map;
} t_data;

typedef struct s_rectangle
{
	char	r;
	float	x;
	float	y;
	float	width;
	float	height;
	char	c;
} t_rectangle;

int	ft_strlen(char *str)
{
	int i = 0;
	while(str[i])
		i++;
	return (i);
}

void	print_map(t_data *data)
{
	int i = 0;
	while (i < data->h)
	{
		printf("%.*s\n", data->w, data->map + i * data->w);
		i++;
	}
}

int	get_data(FILE *file, t_data *data)
{
	int ret = fscanf(file, "%i %i %c\n", &data->w, &data->h, &data->b);
	if (ret == 3)
		return (SUCCESS);
	return (FAIL);
}

float	get_power(float a, float b, float c, float d)
{
	float x = (a - c) * (a - c);
	float y = (b - d) * (b - d);
	return (x + y);
}

int	get_distance(t_rectangle *rect, float x, float y)
{
	float distance;

	if (x < rect->x || x > rect->x + rect->width)
		return (FAIL);
	if (y < rect->y || y > rect->y + rect->height)
		return (FAIL);
	
	if ((x - rect->x < 1.00000) || (rect->x + rect->width - x < 1.0000)
		|| (y - rect->y < 1.00000) ||( rect->y + rect->height - y < 1.0000))
	{
			return (BORDER);
	}
	return (INNER);
}

void	replace(t_data *data, t_rectangle *rect)
{
	int i = 0;
	int j;
	int ret;

	while (i < data->h)
	{
		j = 0;
		while (j < data->w)
		{
			ret = get_distance(rect, (float)i, (float)j);
			if (ret == BORDER || (ret == INNER && rect->r == 'R'))
				data->map[i + j * data->w] = rect->c;
			j++;
		}
		i++;
	}
}

int	do_op(FILE *file, t_data *data, t_rectangle *rect)
{
	int ret = fscanf(file, "%c %f %f %f %f %c\n", &rect->r, &rect->x, &rect->y, &rect->width, &rect->height, &rect->c);
	while (ret == 6)
	{
		if (rect->width <= 0.00000 || rect->height <= 0.00000 || (rect->r != 'r' && rect->r != 'R'))
			return (FAIL);
		replace(data, rect);
		ret = fscanf(file, "%c %f %f %f %f %c\n", &rect->r, &rect->x, &rect->y, &rect->width, &rect->height, &rect->c);
	}
	if (ret != -1)
		return (FAIL);
	return (SUCCESS);
}

int	main(int ac, char **av)
{
	FILE *file;
	t_data	data;
	t_rectangle rect;

	if (ac != 2)
	{
		write(1, "Error: argument\n", ft_strlen("Error: argument\n"));
		return (1);
	}
	else
	{
		if ((file = fopen(av[1], "r")) == NULL)
		{
			write(1, "Error: Operation file corrupted\n", ft_strlen("Error: Operation file corrupted\n"));
			return (1);
		}
		if (get_data(file, &data) == FAIL)
		{
			write(1, "Error: Operation file corrupted\n", ft_strlen("Error: Operation file corrupted\n"));
			fclose(file);
			return (1);
		}
		if ((data.map = calloc(sizeof(char), data.h * data.w)) == NULL)
			return (1);
		memset(data.map, (int)data.b, data.w * data.h);
		if (do_op(file, &data, &rect) == FAIL)
		{
			free(data.map);
			fclose(file);
			printf("do op failed\n");
			write(1, "Error: Operation file corrupted\n", ft_strlen("Error: Operation file corrupted\n"));
			return (1);
		}
		print_map(&data);
		fclose(file);
		free(data.map);
	}
	return (0);
}
