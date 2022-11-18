/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c3d_check_map.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schuah <schuah@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 21:36:13 by schuah            #+#    #+#             */
/*   Updated: 2022/11/15 10:16:51 by schuah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "c3d.h"

static int	check_square(t_gm *gm, int x, int y)
{
	if (y < 0 || y >= gm->map.size.y || x < 0 || x >= gm->map.size.x)
		return (1);
	return (gm->map.map[y][x] != '1' && gm->map.map[y][x] != '0'
		&& gm->map.map[y][x] != 'N' && gm->map.map[y][x] != 'E'
		&& gm->map.map[y][x] != 'S' && gm->map.map[y][x] != 'W'
		&& gm->map.map[y][x] != 'D');
}	

static void	get_ply_dir(t_gm *gm, int x, int y)
{
	if (gm->map.map[y][x] == 'N' || gm->map.map[y][x] == 'E'
		|| gm->map.map[y][x] == 'S' || gm->map.map[y][x] == 'W')
	{
		if (gm->ply.e_dir != NOTSET)
			c3d_fail_exit("Duplicated player position in map", NULL);
		if (gm->map.map[y][x] == 'N')
			gm->ply.e_dir = NORTH;
		else if (gm->map.map[y][x] == 'E')
			gm->ply.e_dir = EAST;
		else if (gm->map.map[y][x] == 'S')
			gm->ply.e_dir = SOUTH;
		else if (gm->map.map[y][x] == 'W')
			gm->ply.e_dir = WEST;
		gm->ply.pos.x = (double)x;
		gm->ply.pos.y = (double)y;
	}
}

static void	get_door(t_gm *gm, int x, int y)
{
	t_ivct	vct;
	t_list	*new;

	if (gm->map.map[y][x] != 'D')
		return ;
	vct.x = x;
	vct.y = y;
	new = ft_lstnew(ft_calloc(1, sizeof(t_ivct)));
	ft_memcpy(new->content, &vct, sizeof(t_ivct));
	ft_lstadd_back(&gm->map.door, new);
}

void	c3d_check_map(t_gm *gm, int x, int y)
{
	int	error;

	error = 0;
	if (y < 0 || y >= gm->map.size.y || x < 0 || x >= gm->map.size.x)
		return ;
	if (gm->map.map[y][x] == '0' || gm->map.map[y][x] == 'N'
		|| gm->map.map[y][x] == 'E' || gm->map.map[y][x] == 'S'
		|| gm->map.map[y][x] == 'W' || gm->map.map[y][x] == 'D')
	{
		get_ply_dir(gm, x, y);
		get_door(gm, x, y);
		error += check_square(gm, x + 1, y);
		error += check_square(gm, x - 1, y);
		error += check_square(gm, x, y + 1);
		error += check_square(gm, x, y - 1);
		if (error)
			c3d_fail_exit("Map not surrounded by walls", NULL);
	}
	c3d_check_map(gm, x + 1, y);
	if (x == 0)
		c3d_check_map(gm, x, y + 1);
}
