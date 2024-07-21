/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:21:37 by chon              #+#    #+#             */
/*   Updated: 2024/07/16 13:47:41 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>

typedef struct	s_params
{
	size_t		time_to_die;
	size_t		time_to_eat;
	size_t		time_to_sleep;
	int			num_times_philo_must_eat;
}	t_params;

typedef struct	s_philo
{
	size_t			p_index;
	size_t			last_meal;
	size_t			times_ate;
	bool			*p_forks;
	pthread_mutex_t	*forks_lock;
	t_params		args;
	bool			eating;
	bool			sleeping;
	bool			thinking;
	bool			dead;
	size_t			start_time;
	size_t			philo_ct;
}	t_philo;

typedef struct	s_setup
{
	pthread_t 	*threads;
	bool		*forks;
	size_t		philo_ct;
	size_t		i;
}	t_setup;

size_t	ft_atoi(char *str);
int		ft_error(char *str, t_setup *s, t_philo *p, int num);
int		ft_usleep(size_t msec);
size_t	cur_time();
void    free_all(t_setup *s, t_philo *p);
void	*routine(void *arg);

#endif