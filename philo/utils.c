/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 10:14:21 by chon              #+#    #+#             */
/*   Updated: 2024/08/07 15:01:00 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_all(t_setup *s, t_philo *p)
{
	while (++s->i < s->p_ct)
		pthread_mutex_destroy(&s->forks_lock[s->i]);
	free(s->forks_lock);
	s->i = -1;
	pthread_mutex_destroy(&s->meal_lock);
	pthread_mutex_destroy(&s->print_lock);
	pthread_mutex_destroy(&s->end_thread_lock);
	free(s->threads);
	free(s);
	free(p);
}

size_t	cur_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) < 0)
	{
		printf("gettimeofday error\n");
		return (0);
	}
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int	ft_usleep(size_t msec)
{
	size_t	start;

	start = cur_time();
	while ((cur_time() - start) < msec)
		usleep(1000);
	return (0);
}

int	ft_atoi(char *str)
{
	int	num;
	int	i;

	num = 0;
	i = -1;
	while (str[++i])
		num = num * 10 + str[i] - '0';
	return (num);
}
