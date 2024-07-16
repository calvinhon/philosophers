/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:21:37 by chon              #+#    #+#             */
/*   Updated: 2024/07/16 13:47:41 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

typedef struct s_philo
{
	pthread_t	thread;
	size_t		num_of_philos;
	size_t		time_to_die;
	size_t		time_to_eat;
	size_t		time_to_sleep;
	size_t		num_times_philo_must_eat;
	size_t		eating;
	size_t		sleeping;
	size_t		thinking;
	size_t		l_fork;
	size_t		r_fork;
}	t_philo;

size_t	ft_atoi(char *str);
int		ft_error(char *str, int num);

#endif