/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbespalk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/23 16:33:57 by vbespalk          #+#    #+#             */
/*   Updated: 2018/08/23 16:34:28 by vbespalk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int			trace(t_ray *ray, t_mlx *mlx, t_obj *objs, unsigned int pix)
{
	double		t;
	int			intersection;

	intersection = 0;
	while ((objs))
	{
		if (objs->intersect(ray->dir, ray->ori, *objs, &t))
		{
			intersection = 1;
			if (t < ray->t)
			{
				ray->t = t;
				mlx->pix_obj[pix] = objs;
			}
		}
		objs = objs->next;
	}
	return (intersection);
}

int			cast_ray(t_ray *ray, t_mlx *mlx, unsigned int pix)
{
	t_vector	li;
	t_light		*light;
	t_obj		*objs;

	ray->t = INFINITY;
	objs = mlx->obj;
	if (!trace(ray, mlx, objs, pix))
		return (0);
	ray->hit_p = ray->ori + vec_scalar_mult(ray->dir, ray->t);
	light = mlx->light;
	li = get_light(mlx, ray, mlx->pix_obj[pix], light);
	ray->hit_c[0] = L_X(mlx->pix_obj[pix]->col[0] * li[0], 255);
	ray->hit_c[1] = L_X(mlx->pix_obj[pix]->col[1] * li[1], 255);
	ray->hit_c[2] = L_X(mlx->pix_obj[pix]->col[2] * li[2], 255);
	return (1);
}

t_vector	ray_generate(const t_mlx *mlx, int i, int j)
{
	t_scene		*scene;
	t_vector	dir;

	scene = mlx->scene;
	dir[0] = ((2.0f * (i + 0.5f) / SCR_WID - 1.0f) * mlx->asp_rat * \
		tan(scene->fov / 2.0f));
	dir[1] = ((1.0f - (2 * (j + 0.5f) / SCR_HEI)) * tan(scene->fov / 2.0f));
	dir[2] = -1;
	vec_multipl(&(scene->wto_cam), &dir);
	dir -= scene->r_ori;
	normilize_vec(&dir);
	return (dir);
}

void		render(t_mlx *mlx, t_scene *sc)
{
	t_obj		*ob;
	double		c_dis;

	mlx_clear_window(mlx->mlx_ptr, mlx->win_ptr);
	ob = mlx->obj;
	while (ob)
	{
		if (ob->type == 3)
		{
			c_dis = fabs(ob->tr_pos[2] - (sc->cam_transl[2]));
			if (c_dis < 0.0f)
				c_dis = 1.0f;
			ob->tg2 = (ob->tr_siz / c_dis) * (ob->tr_siz / c_dis);
		}
		ob = ob->next;
	}
	ft_bzero(mlx->pix_obj, sizeof(t_obj *) * SCR_HEI * SCR_WID);
	transform_mat(&(sc->wto_cam), sc->cam_transl, sc->cam_angles, 1.0f);
	ft_bzero(&(sc->r_ori), sizeof(t_vector));
	vec_multipl(&(sc->wto_cam), &(sc->r_ori));
	mult_threads(mlx);
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, mlx->img_ptr, 0, 0);
}
