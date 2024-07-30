/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 11:33:20 by chon              #+#    #+#             */
/*   Updated: 2024/07/22 11:33:20 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_state(char state, t_philo *p)
{
	size_t	time_now;
	size_t	time_stamp;

	time_now = cur_time();
	time_stamp = time_now - p->start_time;
	if (check_death(p))
		return ;
	// pthread_mutex_lock(p->p_print_lock);
	if (state == 'f')
		printf("%zu %zu has taken a fork\n", time_stamp, p->p_index);
	else if (state == 'e')
	{
		printf("%zu %zu is eating\n", time_stamp, p->p_index);
		pthread_mutex_lock(p->p_meal_lock);
		p->last_meal = time_now;
		pthread_mutex_unlock(p->p_meal_lock);
		ft_usleep(p->args.time_to_eat);
	}
	else if (state == 's')
	{	
		printf("%zu %zu is sleeping\n", time_now - p->start_time, p->p_index);
		ft_usleep(p->args.time_to_sleep);
	}
	else if (state == 't')
		printf("%zu %zu is thinking\n", time_stamp, p->p_index);
	// pthread_mutex_unlock(p->p_print_lock);
}

void	use_forks(t_philo *p)
{
	size_t	r_fork;
	size_t	l_fork;

	r_fork = p->p_index - 1;
	l_fork = p->p_index % p->philo_ct;
	pthread_mutex_lock(&p->forks_lock[r_fork]);
	print_state('f', p);
	if (p->philo_ct == 1)
	{
		pthread_mutex_unlock(&p->forks_lock[r_fork]);
		ft_usleep(p->args.time_to_die);
		return ;
	}
	pthread_mutex_lock(&p->forks_lock[l_fork]);
	print_state('f', p);
	print_state('e', p);
	pthread_mutex_unlock(&p->forks_lock[l_fork]);
	pthread_mutex_unlock(&p->forks_lock[r_fork]);
}

void	*monitor(void *arg)
{
	t_setup *s;
	int		i;

	s = (t_setup *)arg;
	i = -1;
	while (1)
	{
		if (i + 1 == (int)s->philo_ct)
			i = -1;
		if (check_death(&s->p[++i]))
		{
			pthread_mutex_lock(s->p->p_dead_lock);
			s->end_threads = 1;
			pthread_mutex_unlock(s->p->p_dead_lock);
			return (NULL);
		}
	}
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	if (!(p->p_index % 2) || (p->philo_ct > 1 && p->p_index == p->philo_ct))
		ft_usleep(p->args.time_to_eat);
	while (1)
	{
		if (check_death(p))
			return (NULL);
		use_forks(p);
		print_state('s', p);
		print_state('t', p);
	}
	return (NULL);
}
