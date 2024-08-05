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
	pthread_mutex_lock(&p->s->death_lock);
	if (p->s->end_threads)
	{
		pthread_mutex_unlock(&p->s->death_lock);
		return (0);
	}
	pthread_mutex_unlock(&p->s->death_lock);
	pthread_mutex_lock(&p->s->print_lock);
	printf("%zu %u %s\n", cur_time() - p->s->start_time, p->p_index, str);
	pthread_mutex_unlock(&p->s->print_lock);
	return (1);
}

bool	check_death(t_philo *p)
{
	pthread_mutex_lock(&p->s->death_lock);
	if (p->s->end_threads)
	{
		pthread_mutex_unlock(&p->s->death_lock);
		return (1);
	}
	if (cur_time() - p->last_meal >= p->s->time_to_die)
	{
		p->s->end_threads = 1;
		pthread_mutex_unlock(&p->s->death_lock);
		pthread_mutex_lock(&p->s->print_lock);
		printf("%zu %u died\n", cur_time() - p->s->start_time, p->p_index);
		pthread_mutex_unlock(&p->s->print_lock);
		pthread_mutex_unlock(&p->s->death_lock);
		// print_state("died", p);
		return (1);
	}
	pthread_mutex_unlock(&p->s->death_lock);
	return (0);
}

bool	use_forks(t_philo *p)
{
	pthread_mutex_t *first_fork;
    pthread_mutex_t *second_fork;

	if (check_death(p))
		return (0);
	first_fork = p->r_fork;
	second_fork = p->l_fork;
	if (p->l_fork < p->r_fork)
    {
        first_fork = p->l_fork;
        second_fork = p->r_fork;
	}
	pthread_mutex_lock(first_fork);
	if (!print_state("has taken a fork", p))
	{
		pthread_mutex_unlock(first_fork);
		return (0);
	}
	if (p->s->p_ct == 1)
	{
		pthread_mutex_unlock(first_fork);
		ft_usleep(p->s->time_to_die);
		return (0);
	}
	pthread_mutex_lock(second_fork);
	if (!print_state("has taken a fork", p))
	{
		pthread_mutex_unlock(second_fork);
		pthread_mutex_unlock(first_fork);
		return (0);
	}
	return (1);
}

void	*routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	if (!(p->p_index % 2) || (p->s->p_ct > 1 && p->p_index == p->s->p_ct))
		ft_usleep(1);
	while (1)
	{
		if (check_death(p))
			break ;
		if (!use_forks(p))
			break ;
		pthread_mutex_lock(&p->s->lock);
		p->last_meal = cur_time();
		pthread_mutex_unlock(&p->s->lock);
		if (!print_state("is eating", p))
			break ;
		ft_usleep(p->s->time_to_eat);
		pthread_mutex_unlock(p->l_fork);
		pthread_mutex_unlock(p->r_fork);
		if (!print_state("is sleeping", p))
			break ;
		ft_usleep(p->s->time_to_sleep);
		if (!print_state("is thinking", p))
			break ;
	}
	return (NULL);
}
