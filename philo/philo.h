/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:21:37 by chon              #+#    #+#             */
/*   Updated: 2024/07/30 12:49:53 by chon             ###   ########.fr       */
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

typedef struct s_philo
{
	int				p_index;
	size_t			last_meal;
	size_t			times_ate;
	pthread_mutex_t	*forks_lock;
	struct s_setup	*s;
}	t_philo;

typedef struct s_setup
{
	int				p_ct;
	pthread_t		*threads;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	int				num_times_philo_must_eat;
	size_t			start_time;
	bool			end_threads;
	int				i;
	pthread_mutex_t	*forks_lock;
	pthread_mutex_t	lock;
	t_philo			*p;
}	t_setup;

int		ft_atoi(char *str);
int		ft_error(char *str, t_setup *s, t_philo *p, int num);
int		ft_usleep(size_t msec);
size_t	cur_time(void);
void	free_all(t_setup *s, t_philo *p);
void	*routine(void *arg);
size_t	sum_num_arr(size_t *arr);
void	philo_state(t_philo *p, int state);
bool	monitor(t_setup *s);
int		check_death(t_philo *p);

#endif