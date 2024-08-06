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

bool	end_thread(t_philo *p)
{
	pthread_mutex_lock(&p->s->end_thread_lock);
	if (p->s->dead_philo || p->s->all_philos_full)
		return (pthread_mutex_unlock(&p->s->end_thread_lock), 1);
	pthread_mutex_unlock(&p->s->end_thread_lock);
	return (0);
}

bool	print_state(char *str, t_philo *p)
{
	pthread_mutex_lock(&p->s->print_lock);
	if (end_thread(p))
		return (pthread_mutex_unlock(&p->s->print_lock), 0);
	printf("%zu %u %s\n", cur_time() - p->s->start_time, p->p_index, str);
	pthread_mutex_unlock(&p->s->print_lock);
	return (1);
}

bool	check_death(t_philo *p)
{
	pthread_mutex_lock(&p->s->meal_lock);
	if (cur_time() - p->last_meal >= p->s->time_to_die)
	{
		pthread_mutex_unlock(&p->s->meal_lock);
		print_state("died", p);
		return (1);
	}
	pthread_mutex_unlock(&p->s->meal_lock);
	return (0);
}

bool	use_forks(t_philo *p)
{
	pthread_mutex_lock(p->r_fork);
	if (!print_state("has taken a fork", p))
		return (pthread_mutex_unlock(p->r_fork), 0);
	if (p->s->p_ct == 1)
	{
		ft_usleep(p->s->time_to_die);
		return (pthread_mutex_unlock(p->r_fork), 0);
	}
	pthread_mutex_lock(p->l_fork);
	if (!print_state("has taken a fork", p))
		return (pthread_mutex_unlock(p->l_fork),
			pthread_mutex_unlock(p->r_fork), 0);
	pthread_mutex_lock(&p->s->meal_lock);
	p->last_meal = cur_time();
	if (p->s->num_times_philo_must_eat > 0)
	{
		p->times_ate++;
		if (p->times_ate == p->s->num_times_philo_must_eat)
			p->s->num_of_full_philos++;
	}
	pthread_mutex_unlock(&p->s->meal_lock);
	return (1);
}

void	*routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	if (!(p->p_index % 2) || (p->s->p_ct > 1 && p->p_index == p->s->p_ct))
		ft_usleep(5);
	while (1)
	{
		if (!use_forks(p))
			break ;
		if (!print_state("is eating", p))
			return (pthread_mutex_unlock(p->l_fork),
				pthread_mutex_unlock(p->r_fork), NULL);
		ft_usleep(p->s->time_to_eat);
		pthread_mutex_unlock(p->l_fork);
		pthread_mutex_unlock(p->r_fork);
		if (!print_state("is sleeping", p))
			break ;
		ft_usleep(p->s->time_to_sleep);
		if (!print_state("is thinking", p))
			break ;
		if (end_thread(p))
			break ;
		ft_usleep(1);
	}
	return (NULL);
}
