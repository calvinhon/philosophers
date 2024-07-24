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

void	philo_state(t_philo *p, int state)
{
	p->eating = (state == 1);
	p->sleeping = (state == 2);
	p->thinking = (state == 3);
}

int	check_death(t_philo *p)
{
	pthread_mutex_lock(&p->dead_lock);
	if (p->p_dead[0])
	{
		pthread_mutex_unlock(&p->dead_lock);
		return (1);
	}
	pthread_mutex_unlock(&p->dead_lock);
	return (0);
}
