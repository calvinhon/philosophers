/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:21:37 by chon              #+#    #+#             */
/*   Updated: 2024/07/29 15:52:30 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <time.h>
# include <sys/time.h>
# include <stdbool.h>

typedef struct s_params
{
	size_t		time_to_die;
	size_t		time_to_eat;
	size_t		time_to_sleep;
	int			num_times_philo_must_eat;
}	t_params;

typedef struct s_philo
{
	size_t			p_index;
	size_t			last_meal;
	t_params		args;
	size_t			start_time;
	size_t			philo_ct;
	size_t			*p_times_ate;
	bool			*p_end_threads;
	pthread_mutex_t	*forks_lock;
	pthread_mutex_t	*times_ate_lock;
	pthread_mutex_t	*p_dead_lock;
	pthread_mutex_t	*p_meal_lock;
	pthread_mutex_t	*p_print_lock;
}	t_philo;

typedef struct s_setup
{
	pthread_t		*threads;
	pthread_t		monitor_th;
	size_t			*times_ate;
	int				philo_ct;
	int				i;
	bool			end_threads;
	pthread_mutex_t	*forks_lock;
	pthread_mutex_t	times_ate_lock;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	print_lock;
	t_philo			*p;
}	t_setup;

size_t	ft_atoi(char *str);
int		ft_error(char *str, t_setup *s, t_philo *p, int num);
int		ft_usleep(size_t msec);
size_t	cur_time(void);
void	free_all(t_setup *s, t_philo *p);
void	*routine(void *arg);
size_t	sum_num_arr(size_t *arr);
void	philo_state(t_philo *p, int state);
void	*monitor(void *arg);
int		check_death(t_philo *p);

#endif