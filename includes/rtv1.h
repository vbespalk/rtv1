/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbespalk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 21:50:08 by vbespalk          #+#    #+#             */
/*   Updated: 2018/08/17 21:50:10 by vbespalk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H
# define INFINITY 18446744073709551615
# define SCR_WID 800
# define SCR_HEI 640
# define SQ_MATR 4
# define POSI "pos("
# define COLO "col("
# define RADI "rad("
# define SIZE "size("
# define DIFU "diffuse("
# define REFL "reflect("
# define DIRE "dir("
# define ROTI "rot("
# define FOV 1.570796
# define ALBEDO 0.18
# define T_COEF 0.0f
# define AMBILI 0.1f
# define NB_THREADS 8
# define L_X(a, b) ({typeof(a) _a = (a);typeof(b) _b = (b);_a >= _b ? _b : _a;})
# define L_N(a, b) ({typeof(a) _a = (a);typeof(b) _b = (b);_a <= _b ? _b : _a;})
# define DEG_TO_RAD(x) ((x) * M_PI / 180.0f)
# define RAD_TO_DEG(x) ((x) * 180.0f / M_PI)

# include <stdio.h>
# include <pthread.h>
# include <math.h>
# include "libft.h"
# include "mlx.h"

typedef	double		t_matrix[4][4];
typedef double		t_vector __attribute__((vector_size(sizeof(double)*3)));

typedef struct		s_obj
{
	int				type;
	t_vector		pos;
	t_vector		rot;
	unsigned int	size;
	unsigned int	reflect;
	t_vector		col;
	t_vector		tr_pos;
	t_vector		tr_rot;
	double			tr_siz;
	double			tg2;
	int				(*intersect)();
	struct s_obj	*next;
	t_vector		x;
}					t_obj;

typedef struct		s_light
{
	t_vector		pos;
	unsigned int	rad;
	t_vector		col;
	t_vector		dir;
	double			r2;
	struct s_light	*next;
}					t_light;

typedef struct		s_scene
{
	unsigned int	nb_obj;
	unsigned int	nb_light;
	double			fov;
	t_vector		r_ori;
	t_vector		cam_pos;
	t_vector		cam_dir;
	t_vector		cam_transl;
	t_vector		cam_angles;
	t_matrix		wto_cam;
	t_light			*light;
	t_obj			*obj;
}					t_scene;

typedef	struct		s_mlx
{
	void			*mlx_ptr;
	void			*win_ptr;
	void			*img_ptr;
	char			*data_add;
	int				bpp;
	int				s_line;
	int				endian;
	t_scene			*scene;
	t_light			*light;
	t_obj			*obj;
	double			asp_rat;
	// t_vector		*raybuffer;
	t_obj			**pix_obj;
	t_obj			*selected;
}					t_mlx;

typedef	struct		s_thread
{
	int				s_pix;
	int				e_pix;
	t_mlx			*mlx;
}					t_thread;

typedef	struct		s_ray
{
	t_vector		ori;
	t_vector		dir;
	double			t;
	t_vector		hit_p;
	t_vector		hit_n;
	t_vector		hit_c;
}					t_ray;

int					is_tag(char *s);
int					get_vec(char *str, t_vector *vec);
int					get_col(char *str, t_vector *vec);
int					get_val(char *str, unsigned int *val);
int					parser(int fd, t_scene *scene);
char				*ft_ptrtostr(const char *s1, const char *s2);
/*
** THREADS
*/
int					mult_threads(t_mlx *mlx);
/*
** SET SCENE
*/
int					set_lights(t_scene *scene, char **prop);
int					set_cam(t_scene *scene, char **prop);
int					set_obj1(t_scene *scene, char **prop, int type);
int					set_obj2(t_scene *scene, char **prop, int type);
int					to_struct(t_scene *scene, char *s, int i);
t_obj				*init_sobj(t_obj **obj, unsigned int *nb_ob);
t_light				*init_slight(t_light **light, unsigned int *nb_li);
/*
** FREESHING
*/
int					clear_mem(char ***mem, int nb);
void				struct_del(t_scene *scene);
void				slight_del(t_light **lst);
void				sobj_del(t_obj **lst);
/*
** RT RENDER
*/
void				render(t_mlx *mlx, t_scene *scene);
int					cast_ray(t_ray *ray, t_mlx *mlx, unsigned int pix);
t_vector			ray_generate(const t_mlx *mlx, int i, int j);
int					solve_qvadratic(double a, double b, double c, double *t);
int					s_intersect(t_vector dir, t_vector	orig, \
	t_obj obj, double *t);
int					p_intersect(t_vector dir, t_vector orig, \
	t_obj obj, double *t);
int					cy_intersect(t_vector dir, t_vector	orig, \
	t_obj obj, double *t);
int					co_intersect(t_vector dir, t_vector	orig, \
	t_obj obj, double *t);
/*
** LIGHTNING
*/
t_vector			get_light(t_mlx *mlx, t_ray *ray, t_obj *obj, \
	t_light *light);
t_vector			get_diff(t_light *light, t_ray *ray, t_vector l_dir);
int					trace_shad(t_vector l_dir, t_ray *ray, t_obj *objs, \
	double dis);
/*
** GRAPH_TRANSFORMATION LIBRARY
*/
t_matrix			*matrix_scale(t_matrix *m_scale, double scale);
t_matrix			*matrix_move(t_matrix *m_move, int x_m, int y_m, int z_m);
t_matrix			*matrix_rotate(t_matrix *m, double x, double y, double z);
t_matrix			*matrix_mult(t_matrix *right, t_matrix *left);
t_vector			*vec_multipl(const t_matrix *m, t_vector *p);
t_vector			*dir_multipl(const t_matrix *m, t_vector *p);
double				vec_mod(t_vector *v);
t_vector			*normilize_vec(t_vector *v);
double				dot_product(t_vector v1, t_vector v2);
t_vector			vec_scalar_mult(t_vector v, double s);
t_matrix			*inverse(t_matrix *m_inv, t_matrix *m4);
void				img_pixel_put(t_mlx *mlx, int x, int y, unsigned int color);
unsigned int		get_rgb(unsigned int red, unsigned int green, \
	unsigned int blue);
int					hook_keydown(int key, void *mlx);
int					mouse_hook(int button, int x, int y, void *mlx);
int					exit_x(void *mlx);
void				put_to_window(t_mlx *ptr);
void				translate(int key, t_vector *translation, int cam);
void				scale(int key, double *scale, int cam);
t_matrix			*transform_mat(t_matrix *lm, t_vector translation, \
	t_vector rot, double scale);
t_matrix			*transform_mat_inv(t_matrix *lm, t_vector translation, \
	t_vector rot, double scale);
void				reset(t_mlx *mlx);
void				rotate(int key, t_vector *angles);
void				cam_rotate(int key, t_vector *angles);
t_matrix			*x_rotate(t_matrix *m_xrot, double angle);
t_matrix			*y_rotate(t_matrix *m_yrot, double angle);
t_matrix			*z_rotate(t_matrix *m_zrot, double angle);
t_vector			cross_prod(t_vector u, t_vector v);
#endif
