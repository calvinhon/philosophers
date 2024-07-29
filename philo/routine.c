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

// void	print_msg(t_philo *p, )

void	print_state(char state, t_philo *p)
{
	size_t	time_now;
	size_t	time_stamp;

	time_now = cur_time();
	time_stamp = time_now - p->start_time;
	if (check_death(p))
		return ;
	pthread_mutex_lock(p->p_print_lock);
	if (state == 'f')
		printf("%zu %zu has taken a fork\n", time_stamp, p->p_index);
	else if (state == 'e')
	{
		printf("%zu %zu is eating\n", time_stamp, p->p_index);
		p->last_meal = time_now;
		ft_usleep(p->args.time_to_eat);
	}
	else if (state == 's')
	{	
		printf("%zu %zu is sleeping\n", time_now - p->start_time, p->p_index);
		ft_usleep(p->args.time_to_sleep);
	}
	else if (state == 't')
		printf("%zu %zu is thinking\n", time_stamp, p->p_index);
	pthread_mutex_unlock(p->p_print_lock);
}

void	use_forks(t_philo *p)
{
	size_t	r_fork;
	size_t	l_fork;
	size_t	tmp_fork;

	r_fork = p->p_index - 1;
	l_fork = p->p_index % p->philo_ct;
	tmp_fork = r_fork;
	if (r_fork > l_fork)
	{
		r_fork = l_fork;
		l_fork = tmp_fork;
	}
	pthread_mutex_lock(&p->forks_lock[r_fork]);
	if (check_death(p))
	{
		pthread_mutex_unlock(&p->forks_lock[r_fork]);
		return ;
	}
	print_state('f', p);
	if (p->philo_ct == 1)
	{
		pthread_mutex_unlock(&p->forks_lock[r_fork]);
		ft_usleep(p->args.time_to_die);
		return ;
	}
	pthread_mutex_lock(&p->forks_lock[l_fork]);
	print_state('f', p);
	print_state('e', p);
	pthread_mutex_unlock(&p->forks_lock[l_fork]);
	pthread_mutex_unlock(&p->forks_lock[r_fork]);
	if (!check_death(p))
		print_state('s', p);
}

void	*monitor(void *arg)
{
	t_setup *s;

	s = (t_setup *)arg;
	while (1)
	{
		pthread_mutex_lock(&s->dead_lock);
		if (s->dead_flag)
		{
			s->end_threads = 1;
			// printf("hits:m\n");
			pthread_mutex_unlock(&s->dead_lock);
			return (NULL);
		}
		// ft_usleep(1);
		pthread_mutex_unlock(&s->dead_lock);
	}
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	if (!(p->p_index % 2) || (p->philo_ct > 1 && p->p_index == p->philo_ct))
		ft_usleep(1);
	while (!check_death(p))
	{
		use_forks(p);
		if (check_death(p))
			return (NULL);
		print_state('t', p);
	}
	// printf("time to die:%zu\n", p->args.time_to_die);
	// printf("cur:%zu start:%zu diff:%zu\n", cur_time(), p->start_time, p->last_meal);
	return (NULL);
}
