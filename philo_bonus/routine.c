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

void	*monitor_death(void *arg)
{
	t_philo	*p;
	int		i;

	p = (t_philo *)arg;
	while (1)
	{
		sem_wait(p->s->meal_sem);
		if (cur_time() - p->last_meal >= p->s->time_to_die)
		{
			sem_post(p->s->meal_sem);
			sem_wait(p->s->print_sem);
			printf("%zu %u died\n", cur_time() - p->s->start_time, p->p_index);
			i = -1;
			while (++i < p->s->p_ct)
				sem_post(p->s->full_sem);
			sem_post(p->s->sim_sem);
			break ;
		}
		sem_post(p->s->meal_sem);
	}
	return (NULL);
}

bool	print_state(char *str, t_philo *p)
{
	sem_wait(p->s->print_sem);
	printf("%zu %u %s\n", cur_time() - p->s->start_time, p->p_index, str);
	sem_post(p->s->print_sem);
	return (1);
}

bool	use_forks(t_philo *p)
{
	sem_wait(p->s->forks_sem);
	if (!print_state("has taken a fork", p))
		return (0);
	if (p->s->p_ct == 1)
	{
		ft_usleep(p->s->time_to_die);
		return (0);
	}
	sem_wait(p->s->forks_sem);
	if (!print_state("has taken a fork", p))
		return (0);
	sem_wait(p->s->meal_sem);
	p->last_meal = cur_time();
	sem_post(p->s->meal_sem);
	if (p->s->num_times_philo_must_eat > 0)
	{
		p->times_ate++;
		if (p->times_ate == p->s->num_times_philo_must_eat)
			sem_post(p->s->full_sem);
	}
	return (1);
}

void	*routine(t_philo *p)
{
	pthread_t	death_thread;

	if (!(p->p_index % 2) || (p->s->p_ct > 1 && p->p_index == p->s->p_ct))
		ft_usleep(p->s->time_to_eat);
	pthread_create(&death_thread, NULL, monitor_death, p);
	pthread_detach(death_thread);
	while (1)
	{
		if (!use_forks(p))
			break ;
		if (!print_state("is eating", p))
			return (NULL);
		ft_usleep(p->s->time_to_eat);
		sem_post(p->s->forks_sem);
		sem_post(p->s->forks_sem);
		if (!print_state("is sleeping", p))
			break ;
		ft_usleep(p->s->time_to_sleep);
		if (!print_state("is thinking", p))
			break ;
	}
	return (NULL);
}
