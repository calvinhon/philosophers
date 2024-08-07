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
	if (p->s->dead_philo || p->s->all_philos_full)
		return (1);
	return (0);
}

bool	print_state(char *str, t_philo *p)
{
	if (end_thread(p))
		return (0);
	printf("%zu %u %s\n", cur_time() - p->s->start_time, p->p_index, str);
	return (1);
}

bool	check_death(t_philo *p)
{
	if (cur_time() - p->last_meal >= p->s->time_to_die)
	{
		print_state("died", p);
		sem_post(p->s->deadSem);
		return (1);
	}
	return (0);
}

bool	use_forks(t_philo *p)
{
	sem_wait(p->s->forksSem);
	if (!print_state("has taken a fork", p))
		return (0);
	if (p->s->p_ct == 1)
	{
		ft_usleep(p->s->time_to_die);
		return (0);
	}
	sem_wait(p->s->forksSem);
	if (!print_state("has taken a fork", p))
		return (0);
	p->last_meal = cur_time();
	if (p->s->num_times_philo_must_eat > 0)
	{
		p->times_ate++;
		if (p->times_ate == p->s->num_times_philo_must_eat)
			sem_wait(p->s->fullSem);
	}
	return (1);
}

void	*routine(t_philo *p)
{
	if (!(p->p_index % 2) || (p->s->p_ct > 1 && p->p_index == p->s->p_ct))
		ft_usleep(5);
	while (1)
	{
		if (!use_forks(p))
			break ;
		if (!print_state("is eating", p))
			return (NULL);
		ft_usleep(p->s->time_to_eat);
		sem_post(p->s->forksSem);
		sem_post(p->s->forksSem);
		if (!print_state("is sleeping", p))
			break ;
		ft_usleep(p->s->time_to_sleep);
		if (!print_state("is thinking", p))
			break ;
		if (end_thread(p))
			break ;
		// ft_usleep(1);
	}
	return (NULL);
}
