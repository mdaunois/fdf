#include "minilibx_macos/mlx.h"
#include "libft/includes/libft.h"
#include <stdio.h>
# include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct	s_img
{
	void	*img_ptr;
	int	*data;
	int	size_l;
	int	bpp;
	int	endian;
}		t_img;

typedef struct s_map
{
	int x;
	int y;
	int z;	
}		t_map;

typedef struct	s_param
{	
	void	*mlx_ptr;
	void	*win_ptr;
	t_img	img;
	t_list	*head_map;
	int	img_width;
	int	img_height;
	int	x;
	int	y;
	int	color;
}		t_param;

void	ft_list_push_back(t_list **begin_list, void *data, size_t len)
{
	t_list	*list;

	list = *begin_list;
	if (list)
	{
		while (list->next)
			list = list->next;
		list->next = ft_lstnew(data, len);
	}
	else
		*begin_list = ft_lstnew(data, len);
}


void	dray_line(t_param *param, int x1, int x2, int y1, int y2)
{
	int dx;
	int dy;
	int Dx;
	int Dy;
	int ex;
	int ey;
	int i;
	int Xinc;
	int Yinc;

	i = 0;
	Xinc = 1;
	Yinc = 1;
	ex = abs(x2 - x1);
	ey = abs(y2 - y1);
	dx = 2 * ex;
	dy = 2 * ey;
	Dx = ex;
	Dy = ey;
	if (x1 > x2)
		Xinc = -1;
	if (y1 > y2)
		Yinc = -1;
	if (Dx >= Dy)
	{
		while (i <= Dx)
		{
			param->img.data[x1 * param->img_width + y1] = 0xFFFFF;
			i++;
			x1 += Xinc;
			ex -= dy;
			if (ex < 0)
			{
				y1 += Yinc;
				ex += dx;
			}
		}
	}
	if (Dx < Dy)
	{
		while (i <= Dy)
		{
			param->img.data[x1 * param->img_width + y1] = 0xFFFFF;
			i++;
			y1 += Yinc;
			ey -= dx;
			if (ey < 0)
			{
				x1 += Xinc;
				ey += dy;
			}
		}
	}
}

void	carre_img(t_param *param)
{
	int y;
	int x;
	int color;
	
	color = param->color;
	x = 0;
	while (x < param->img_height)
	{
		y = 0;
		color = param->color;
		while (y < param->img_width)
		{
	//		color = color + 0xFF;
			param->img.data[x * param->img_width + y] = color;
			y++;
		}
			color = color + 16;
		x++;
	}
}

int	deal_key(int key, t_param *param)
{
	printf("x = %d , y = %d\n",param->x, param->y);
	printf("key = %d\n", key);
	if (key == 53)
	{
		exit(0);
	}
	if (key == 2)
	{
		param->x = param->x + 10;
		mlx_clear_window(param->mlx_ptr, param->win_ptr);
		mlx_put_image_to_window(param->mlx_ptr, param->win_ptr, param->img.img_ptr, param->x, param->y);
	}
	if (key == 13)
	{
		param->y = param->y - 10;
		mlx_clear_window(param->mlx_ptr, param->win_ptr);
		mlx_put_image_to_window(param->mlx_ptr, param->win_ptr, param->img.img_ptr, param->x, param->y);
	}
	if (key == 1)
	{
		param->y = param->y + 10;
		mlx_clear_window(param->mlx_ptr, param->win_ptr);
		mlx_put_image_to_window(param->mlx_ptr, param->win_ptr, param->img.img_ptr, param->x, param->y);
	}
	if (key == 0)
	{
		param->x = param->x - 10;
		mlx_clear_window(param->mlx_ptr, param->win_ptr);
		mlx_put_image_to_window(param->mlx_ptr, param->win_ptr, param->img.img_ptr, param->x, param->y);
	}
	if (key == 31)
	{
		mlx_clear_window(param->mlx_ptr, param->win_ptr);
		mlx_destroy_image(param->mlx_ptr, param->img.img_ptr);
		param->img_height = param->img_height - 10;
		param->img_width = param->img_width - 10;
		param->img.img_ptr = mlx_new_image(param->mlx_ptr, param->img_width, param->img_height);
		param->img.data =(int *)mlx_get_data_addr(param->img.img_ptr, &param->img.bpp, &param->img.size_l, &param->img.endian);
		carre_img(param);
		mlx_put_image_to_window(param->mlx_ptr, param->win_ptr, param->img.img_ptr, param->x, param->y);
	}
	if (key == 35)
	{
		mlx_clear_window(param->mlx_ptr, param->win_ptr);
		mlx_destroy_image(param->mlx_ptr, param->img.img_ptr);
		param->img_height = param->img_height + 10;
		param->img_width = param->img_width + 10;
		param->img.img_ptr = mlx_new_image(param->mlx_ptr, param->img_width, param->img_height);
		param->img.data =(int *)mlx_get_data_addr(param->img.img_ptr, &param->img.bpp, &param->img.size_l, &param->img.endian);
		carre_img(param);
		mlx_put_image_to_window(param->mlx_ptr, param->win_ptr, param->img.img_ptr, param->x, param->y);
	}
	return (0);
}

int ft_nblen(int nb)
{
	int len;
	
	len = 0;
	if (nb == 0)
		return (1);
	if (nb < 0)
	{
		len++;
		nb *= -1;
	}
	while (nb != 0)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

int	main(int argc, char *argv[])
{
	t_param param;
	int	fd;
	int	ret;
	char	*line;
	t_map	*map;
	int	**im;
	int	i;
	int	j;
	int	j_y;
	int	space;

	i = 0;
	if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		/*sauvegard des coordonées a faire avec la structure la plus efficace*/
		while (get_next_line(fd, &line) > 0)
		{
			j = 0;
			j_y = 0;
			space = 0;
			while (line[j])
			{
				if (line[j] == ' ')
				{
					space++;
					j++;
				}
				else
				{
					map = malloc(sizeof(t_map));
					map->x = i;
					map->y = j_y;
					map->z = ft_atoi(&line[j]);
					printf("(%d %d %d)",map->x, map->y, map->z);
					//printf("len de %d = %d\n",map->z, ft_nblen(map->z));	
					j = j + ft_nblen(map->z);
					j_y++;
					ft_list_push_back(&param.head_map, map, sizeof(t_map));
				}
				//printf("%s\n", line);
				//j++;
			}
			printf("\n");
			i++;
		}
		/*********************************************************************/
	//	ft_putstr("salut\n");
	}
	im = malloc(sizeof(im) * i);
	
/*	while (param.head_map)
	{
		map = (t_map*)param.head_map->content;
		printf("%d", map->z);
		param.head_map = param.head_map->next;
	}
*/	param.x = 0;
	param.y = 0;
	param.img_height = 700;
	param.img_width = 700;
	param.color = 0xF11F;
	param.mlx_ptr = mlx_init();
	param.win_ptr = mlx_new_window(param.mlx_ptr, 1100,  800, "FdF");
	param.img.img_ptr = mlx_new_image(param.mlx_ptr, param.img_width, param.img_height);
	param.img.data =(int *)mlx_get_data_addr (param.img.img_ptr, &param.img.bpp, &param.img.size_l, &param.img.endian);
	/*mettre des info dans l'image = remplir data*/
	dray_line(&param, 0, 100, 300, 400);
	dray_line(&param, 0, 100, 300, 200);
	dray_line(&param, 100, 200, 200, 300);//
	dray_line(&param, 100, 200, 400, 300);//
	dray_line(&param, 100, 200, 200, 100);
	dray_line(&param, 100, 200, 400, 500);
	dray_line(&param, 200, 300, 100, 200);
	dray_line(&param, 200, 300, 100, 0);
	dray_line(&param, 200, 300, 500, 400);
	dray_line(&param, 200, 300, 300, 200);//
	dray_line(&param, 200, 300, 300, 400);//
	dray_line(&param, 300, 400, 200, 300);
	dray_line(&param, 300, 400, 400, 300);
	dray_line(&param, 300, 400, 0, 100);
	dray_line(&param, 400, 500, 100, 200);
	dray_line(&param, 500, 600, 200, 300);
	dray_line(&param, 600, 500, 300, 400);
	dray_line(&param, 500, 400, 400, 500);
	dray_line(&param, 400, 300, 500, 600);
	dray_line(&param, 300, 200, 600, 500);
	dray_line(&param, 400, 300, 100, 200);
	dray_line(&param, 500, 400, 200, 300);
	dray_line(&param, 400, 500, 300, 400);
	dray_line(&param, 300, 400, 400, 500);
/*	dray_line(&param, 0, 100, 200, 200);
	dray_line(&param, 100, 200, 200, 200);
	dray_line(&param, 200, 300, 200, 200);
	dray_line(&param, 300, 400, 200, 200);
	dray_line(&param, 0, 100, 300, 300);
	dray_line(&param, 100, 200, 300, 300);
	dray_line(&param, 200, 300, 300, 300);
	dray_line(&param, 300, 400, 300, 300);
	dray_line(&param, 0, 100, 400, 400);
	dray_line(&param, 100, 200, 400, 400);
	dray_line(&param, 200, 300, 400, 400);
	dray_line(&param, 300, 400, 400, 400);
	dray_line(&param, 0, 0, 0, 100);
	dray_line(&param, 0, 0, 0, 200);
	dray_line(&param, 0, 0, 0, 300);
	dray_line(&param, 0, 0, 0, 400);
	dray_line(&param, 100, 100, 0, 100);
	dray_line(&param, 100, 100, 100, 200);
	dray_line(&param, 100, 100, 200, 300);
	dray_line(&param, 100, 100, 300, 400);
	dray_line(&param, 200, 200, 0, 100);
	dray_line(&param, 200, 200, 100, 200);
	dray_line(&param, 200, 200, 200, 300);
	dray_line(&param, 200, 200, 300, 400);
	dray_line(&param, 300, 300, 0, 100);
	dray_line(&param, 300, 300, 100, 200);
	dray_line(&param, 300, 300, 200, 300);
	dray_line(&param, 300, 300, 300, 400);
	dray_line(&param, 400, 400, 0, 100);
	dray_line(&param, 400, 400, 100, 200);
	dray_line(&param, 400, 400, 200, 300);
	dray_line(&param, 400, 400, 300, 400);
*/	//carre_img(&param);
	/*---------------------------*/
	mlx_put_image_to_window(param.mlx_ptr, param.win_ptr, param.img.img_ptr, 0, 0);
//	carre(&param);
	mlx_key_hook(param.win_ptr, &deal_key, &param);
	mlx_loop(param.mlx_ptr);
	return (0);
}
