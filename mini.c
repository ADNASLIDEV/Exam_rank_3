#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

typedef struct	s_data
{
	int		w;
	int		h;
	char	b;
	char	*map;
}	t_data;

typedef struct	s_circle
{
	char	f;
	float	x;
	float	y;
	float	r;
	char	c;
}	t_circle;

#define FAIL		0
#define SUCCESS 	1
#define INNER		2
#define BORDER		3

int	ft_strlen(char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return (i);
}

void	put_line(char *str, int len)
{
	int i = 0;
	while (i < len)
	{
		write(1, &str[i], 1);
		i++;
	}
	write(1, "\n", 1);
}

int	get_data(FILE *file, t_data *data)
{
	int ret = fscanf(file, "%i %i %c\n", &data->w, &data->h, &data->b);
	if (ret != 3)
		return (FAIL);
	if (data->w <= 0 || data->w > 300 || data->h <= 0 || data->h > 300)
		return (FAIL);
	return (SUCCESS);
}

void	print_map(t_data *data)
{
	int i = 0;
	while(i < data->h)
	{
		put_line(data->map + i * data->w, data->w);
		i++;
	}
}

float	get_power(float a, float b, float c, float d)
{
	float x = (a - c) * (a - c);
	float y = (b - d) * (b - d);
	return (x + y);
}

int	get_distance(t_circle *circle, float x, float y)
{
	float distancepowered;
	float distance;
	
	distancepowered = get_power(circle->x, circle->y, x, y);
	distance = sqrtf(distancepowered) - circle->r;
	if (distance <= 0.000000)
	{
		if (distance <= -1.000000)
			return (INNER);
		return (BORDER);
	}
	return (0);
}

void exec_one(t_data *data, t_circle *circle)
{
	int	i = 0;
	int j;
	int result;
	while (i < data->h)
	{
		j = 0;
		while (j < data->w)
		{
			result = get_distance(circle, (float)i, (float)j);
			if (result == BORDER || (result == INNER && circle->f == 'C'))
				data->map[i + j * data->w] = circle->c;
			j++;
		}
		i++;
	}
}

int	do_ops(FILE *file, t_data *data, t_circle *circle)
{
	int ret = fscanf(file, "%c %f %f %f %c\n", &circle->f, &circle->x, &circle->y, &circle->r, &circle->c);
	while (ret == 5)
	{
		if (circle->r <= 0.000000 || (circle->f != 'c' && circle->f != 'C'))
			return (FAIL);
		exec_one(data, circle);
		ret = fscanf(file, "%c %f %f %f %c\n", &circle->f, &circle->x, &circle->y, &circle->r, &circle->c);
	}
	if (ret != EOF)
		return (FAIL);
	return (SUCCESS);	
}

int	main(int ac, char **av)
{
	FILE	*file;
	t_data	data;
	t_circle circle;

	if (ac != 2)
	{
		write(1, "Error: argument\n", ft_strlen("Error: argument\n"));
		return (1);
	}
	if ((file = fopen(av[1], "r")) == NULL)
	{
		write(1, "Error: Operation file corrupted\n", ft_strlen("Error: Operation file corrupted\n"));
		return (1);
	}
	if (!get_data(file, &data))
	{
		fclose(file);
		write(1, "Error: Operation file corrupted\n", ft_strlen("Error: Operation file corrupted\n"));
		return (1);
	}
	if ((data.map = calloc(sizeof(char), data.w * data.h)) == NULL)
	{
		fclose(file);
		write(1, "Error: Operation file corrupted\n", ft_strlen("Error: Operation file corrupted\n"));
		return (1);

	}
	memset(data.map, data.b, data.h * data.w);
	if ((do_ops(file, &data, &circle)) == FAIL)
	{
		fclose(file);
		free(data.map);
		write(1, "Error: Operation file corrupted\n", ft_strlen("Error: Operation file corrupted\n"));
		return (1);
	}
	print_map(&data);
	return (0);
}
