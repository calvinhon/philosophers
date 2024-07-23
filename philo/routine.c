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

int	forks_state(t_philo *p, char mode)
{
	size_t	r_fork;
	size_t	l_fork;

	r_fork = p->p_index - 1;
	l_fork = p->p_index % p->philo_ct;
	if (mode == 'b')
	{
		// pthread_mutex_lock(&p->forks_lock[r_fork]);
		// pthread_mutex_lock(&p->forks_lock[l_fork]);
		if (p->p_forks[r_fork] || p->p_forks[l_fork])
		{
			// pthread_mutex_unlock(&p->forks_lock[r_fork]);
			// pthread_mutex_unlock(&p->forks_lock[l_fork]);
			return ('i');
		}
		else
		{
			// pthread_mutex_unlock(&p->forks_lock[r_fork]);
			// pthread_mutex_unlock(&p->forks_lock[l_fork]);
			return ('f');
		}
	}
	if (mode == 'r')
	{
		// pthread_mutex_lock(&p->forks_lock[r_fork]);
		p->p_forks[r_fork] = 0;
		// pthread_mutex_unlock(&p->forks_lock[r_fork]);
		// pthread_mutex_lock(&p->forks_lock[l_fork]);
		p->p_forks[l_fork] = 0;
		// pthread_mutex_unlock(&p->forks_lock[l_fork]);
		return (1);
	}
	return (0);
}

void	print_state(char state, t_philo *p)
{
	size_t	time_stamp;

	time_stamp = cur_time() - p->start_time;
	if (state == 'f')
		printf("%zu %zu has taken a fork\n", time_stamp, p->p_index);
	if (state == 'e')
	{
		printf("%zu %zu is eating\n", time_stamp, p->p_index);
		p->last_meal = cur_time();
		ft_usleep(p->args.time_to_eat);
		if (check_death(p))
			return ;
		forks_state(p, 'r');
	}
	if (state == 's')
	{
		printf("%zu %zu is sleeping\n", cur_time() - p->start_time, p->p_index);
		ft_usleep(p->args.time_to_sleep);
	}
	if (state == 't')
		printf("%zu %zu is thinking\n", time_stamp, p->p_index);
	if (state == 'd' && cur_time() - p->last_meal >= p->args.time_to_die)
	{
		printf("%zu %zu died\n", time_stamp, p->p_index);
		pthread_mutex_lock(&p->dead_lock);
		p->p_dead[0] = 1;
		pthread_mutex_unlock(&p->dead_lock);
	}
}

void	use_forks(t_philo *p)
{
	size_t	r_fork;
	size_t	l_fork;

	r_fork = p->p_index - 1;
	l_fork = p->p_index % p->philo_ct;
	// printf("rfork:%zu lfork:%zu\n", r_fork, l_fork);
	pthread_mutex_lock(&p->forks_lock[r_fork]);
	p->p_forks[r_fork] = 1;
	print_state('f', p);
	if (p->philo_ct > 1)
	{
		pthread_mutex_lock(&p->forks_lock[l_fork]);
		p->p_forks[l_fork] = 1;
		print_state('f', p);
		print_state('e', p);
		pthread_mutex_unlock(&p->forks_lock[l_fork]);
	}
	pthread_mutex_unlock(&p->forks_lock[r_fork]);
	if (p->philo_ct > 1)
	{
		if (check_death(p))
			return ;
		print_state('s', p);
	}
}

void	*routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	if (p->p_index % 2 && !(p->philo_ct > 1 && p->p_index == p->philo_ct))
	{
		use_forks(p);
		if (check_death(p))
			return (NULL);
		print_state('d', p);
	}
	else
		ft_usleep(p->args.time_to_eat);
	while (1)
	{
		// printf("%zu: %c\n", p->p_index, forks_state(p, 'b'));
		if (check_death(p))
			return (NULL);
		print_state('d', p);
		if (check_death(p))
			return (NULL);
		if (forks_state(p, 'b') == 'f')
		{
			use_forks(p);
			print_state('d', p);
			if (check_death(p))
				return (NULL);
			if (forks_state(p, 'b') == 'i')
				print_state('t', p);
		}
		// else
		// 	print_state('t', p);
	}
	// printf("time to die:%zu\n", p->args.time_to_die);
	// printf("cur:%zu start:%zu diff:%zu\n", cur_time(), p->start_time, p->last_meal);
	return (NULL);
}
