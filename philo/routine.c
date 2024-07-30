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

void	print_state(char state, t_philo *p)
{
	size_t	time_now;
	size_t	time_stamp;

	time_now = cur_time();
	time_stamp = time_now - p->s->start_time;
	if (check_death(p))
		return ;
	// pthread_mutex_lock(&p->s->lock);
	if (state == 'f')
		printf("%zu %u has taken a fork\n", time_stamp, p->p_index);
	else if (state == 'e')
	{
		printf("%zu %u is eating\n", time_stamp, p->p_index);
		pthread_mutex_lock(&p->s->lock);
		p->last_meal = time_now;
		pthread_mutex_unlock(&p->s->lock);
		ft_usleep(p->s->time_to_eat);
	}
	else if (state == 's')
	{	
		printf("%zu %u is sleeping\n", time_now - p->s->start_time, p->p_index);
		ft_usleep(p->s->time_to_sleep);
	}
	else if (state == 't')
		printf("%zu %u is thinking\n", time_stamp, p->p_index);
	// pthread_mutex_unlock(&p->s->lock);
}

void	use_forks(t_philo *p)
{
	size_t	r_fork;
	size_t	l_fork;
	size_t	tmp_fork;

	r_fork = p->p_index - 1;
	l_fork = p->p_index % p->s->p_ct;
	if (r_fork > l_fork)
	{
		tmp_fork = r_fork;
		r_fork = l_fork;
		l_fork = tmp_fork;
	}
	pthread_mutex_lock(&p->forks_lock[r_fork]);
	print_state('f', p);
	if (p->s->p_ct == 1)
	{
		pthread_mutex_unlock(&p->forks_lock[r_fork]);
		ft_usleep(p->s->time_to_die);
		return ;
	}
	pthread_mutex_lock(&p->forks_lock[l_fork]);
	print_state('f', p);
	print_state('e', p);
	pthread_mutex_unlock(&p->forks_lock[l_fork]);
	pthread_mutex_unlock(&p->forks_lock[r_fork]);
}

bool	monitor(t_setup *s)
{
	int		i;

	i = -1;
	while (1)
	{
		if (i + 1 == (int)s->p_ct)
			i = -1;
		if (check_death(&s->p[++i]))
		{
			pthread_mutex_lock(&s->lock);
			s->end_threads = 1;
			pthread_mutex_unlock(&s->lock);
			return (0);
		}
	}
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	if (!(p->p_index % 2) || (p->s->p_ct > 1 && p->p_index == p->s->p_ct))
		ft_usleep(p->s->time_to_eat);
	while (1)
	{
		if (check_death(p))
			return (NULL);
		use_forks(p);
		print_state('s', p);
		print_state('t', p);
	}
	return (NULL);
}
