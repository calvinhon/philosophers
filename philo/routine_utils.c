/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:49:35 by chon              #+#    #+#             */
/*   Updated: 2024/07/29 15:50:19 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(t_philo *p)
{
	size_t	time_now;

	time_now = cur_time();
	pthread_mutex_lock(&p->s->lock);
	if (p->s->end_threads)
	{
		pthread_mutex_unlock(&p->s->lock);
		return (1);
	}
	if (time_now - p->last_meal >= p->s->time_to_die)
	{
		printf("%zu %u died\n", time_now - p->s->start_time, p->p_index);
		pthread_mutex_unlock(&p->s->lock);
		return (1);
	}
	pthread_mutex_unlock(&p->s->lock);
	return (0);
}
