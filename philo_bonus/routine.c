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

// bool	end_thread(t_philo *p)
// {
// 	if (p->s->dead_philo || p->s->all_philos_full)
// 		return (1);
// 	return (0);
// }

bool	print_state(char *str, t_philo *p)
{
	// if (end_thread(p))
	// 	return (0);
	sem_wait(p->s->printSem);
	printf("%zu %u %s\n", cur_time() - p->s->start_time, p->p_index, str);
	sem_post(p->s->printSem);
	return (1);
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
	if (pthread_create(&p->s->threads[p->p_index - 1], NULL, monitor, p))
		printf("thread creation failed\n");
	if (pthread_detach(p->s->threads[p->p_index - 1]))
		printf("thread detach failed\n");
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
		// ft_usleep(1);
	}
	return (NULL);
}
