/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:49:35 by chon              #+#    #+#             */
/*   Updated: 2024/07/24 14:47:58 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(t_philo *p)
{
	size_t	time_now;

	pthread_mutex_lock(p->p_dead_lock);
	time_now = cur_time();
	if (*p->p_end_threads)
	{
		pthread_mutex_unlock(p->p_dead_lock);
		return (1);
	}
	if (time_now - p->last_meal >= p->args.time_to_die)
	{
		*p->p_dead = 1;
		pthread_mutex_unlock(p->p_dead_lock);
		pthread_mutex_lock(p->p_print_lock);
		printf("%zu %zu died\n", time_now - p->start_time, p->p_index);
		pthread_mutex_unlock(p->p_print_lock);
		return (1);
	}
	pthread_mutex_unlock(p->p_dead_lock);
	return (0);
}
