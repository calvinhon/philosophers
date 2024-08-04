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

bool	print_state(char *str, t_philo *p)
{
	pthread_mutex_lock(&p->s->lock);
	if (p->s->end_threads)
	{
		pthread_mutex_unlock(&p->s->lock);
		return (0);
	}
	printf("%zu %u %s\n", cur_time() - p->s->start_time, p->p_index, str);
	pthread_mutex_unlock(&p->s->lock);
	return (1);
}

bool	check_death(t_philo *p)
{
	pthread_mutex_lock(&p->s->lock);
	if (p->s->end_threads)
	{
		pthread_mutex_unlock(&p->s->lock);
		return (1);
	}
	if (cur_time() - p->last_meal >= p->s->time_to_die)
	{
		pthread_mutex_unlock(&p->s->lock);
		print_state("died", p);
		return (1);
	}
	pthread_mutex_unlock(&p->s->lock);
	return (0);
}

bool	use_forks(t_philo *p)
{
	if (check_death(p))
		return (0);
	// printf("l_idx:%p\nl_ptr:%p\n\nr_idx:%p\nr_ptr:%p\n\n", &p->s->forks_lock[p->p_index - 1], p->r_fork, &p->s->forks_lock[p->p_index % p->s->p_ct], p->l_fork);
	// pthread_mutex_lock(&p->s->forks_lock[p->p_index - 1]);
	pthread_mutex_lock(p->r_fork);
	if (!print_state("has taken a fork", p))
	{
		pthread_mutex_unlock(p->r_fork);
		// pthread_mutex_unlock(&p->s->forks_lock[p->p_index - 1]);
		return (0);
	}
	if (p->s->p_ct == 1)
	{
		pthread_mutex_unlock(p->r_fork);
		// pthread_mutex_unlock(&p->s->forks_lock[p->p_index - 1]);
		ft_usleep(p->s->time_to_die);
		return (0);
	}
	pthread_mutex_lock(&p->s->forks_lock[p->p_index % p->s->p_ct]);
	if (!print_state("has taken a fork", p))
	{
		pthread_mutex_unlock(p->l_fork);
		pthread_mutex_unlock(p->r_fork);
		// pthread_mutex_unlock(&p->s->forks_lock[p->p_index % p->s->p_ct]);
		// pthread_mutex_unlock(&p->s->forks_lock[p->p_index - 1]);
		return (0);
	}
	return (1);
}

void	*routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	// if (!(p->p_index % 2) || (p->s->p_ct > 1 && p->p_index == p->s->p_ct))
	// 	ft_usleep(1);
	while (1)
	{
		if (check_death(p))
			return (NULL);
		if (!use_forks(p))
			return (NULL);
		pthread_mutex_lock(&p->s->lock);
		p->last_meal = cur_time();
		pthread_mutex_unlock(&p->s->lock);
		print_state("is eating", p);
		ft_usleep(p->s->time_to_eat);
		pthread_mutex_unlock(p->l_fork);
		pthread_mutex_unlock(p->r_fork);
		// pthread_mutex_unlock(&p->s->forks_lock[p->p_index % p->s->p_ct]);
		// pthread_mutex_unlock(&p->s->forks_lock[p->p_index - 1]);
		if (check_death(p))
			return (NULL);
		print_state("is sleeping", p);
		ft_usleep(p->s->time_to_sleep);
		print_state("is thinking", p);
	}
	return (NULL);
}
