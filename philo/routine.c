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

int	forks_state(t_philo *p, char mode)
{
	if (mode == 'b')
	{
		// printf("rfork:%i lfork:%i\n", p->p_forks[p->p_index - 1], p->p_forks[p->p_index]);
		// printf("rfork:%i lfork:%i\n", p->p_forks[p->p_index - 1], p->p_forks[0]);
		if (p->p_forks[p->p_index - 1]
			&& ((p->p_index == p->philo_ct && p->p_forks[0])
			|| p->p_forks[p->p_index]))
			return ('i');
		else
			return ('f');
	}
	if (mode == 'r')
	{
		p->p_forks[p->p_index - 1] = 0;
		if (p->p_index == p->philo_ct)
			p->p_forks[0] = 0;
		else
			p->p_forks[p->p_index] = 0;
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
		forks_state(p, 'r');
		printf("%zu %zu is sleeping\n", cur_time() - p->start_time, p->p_index);
		ft_usleep(p->args.time_to_sleep);
	}
	if (state == 't')
		printf("%zu %zu is thinking\n", time_stamp, p->p_index);
	if (state == 'd' && cur_time() - p->last_meal >= p->args.time_to_die)
	{
		printf("%zu %zu is dead\n", time_stamp, p->p_index);
		p->p_dead[p->p_index - 1] = 1;
	}
}

void	use_forks(t_philo *p)
{
	pthread_mutex_lock(&p->forks_lock[p->p_index - 1]);
	p->p_forks[p->p_index - 1] = 1;
	print_state('f', p);
	if (p->philo_ct > 1)
	{
		if (p->p_index == p->philo_ct)
			p->p_forks[0] = 1;
		else
			p->p_forks[p->p_index] = 1;
		print_state('f', p);
		// print_state('d', p);
		// if (!p->p_dead[p->p_index - 1])
		print_state('e', p);
	}
	pthread_mutex_unlock(&p->forks_lock[p->p_index - 1]);
}

void	*routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	if (p->p_index % 2 && !(p->philo_ct > 1 && p->p_index == p->philo_ct))
		use_forks(p);
	if (p->philo_ct == 1)
	{
		ft_usleep(p->args.time_to_die);
		print_state('d', p);
		return (NULL);
	}
	while (!sum_num_arr(p->p_dead))
	{
		// printf("%zu: %c\n", p->p_index, forks_state(p, 'b'));
		if (forks_state(p, 'b') == 'f')
		{
			use_forks(p);
			print_state('d', p);
			if (p->p_dead[p->p_index - 1])
				return (NULL);
		}
		else
			ft_usleep(p->args.time_to_eat);
		// print_state('d', p);
	}
	// printf("time to die:%zu\n", p->args.time_to_die);
	// printf("cur:%zu start:%zu diff:%zu\n", cur_time(), p->start_time, p->last_meal);
	return (NULL);
}
