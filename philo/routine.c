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

int	forks_state(char mode, t_philo *p)
{
	size_t	r_fork;
	size_t	l_fork;

	r_fork = p->p_index - 1;
	l_fork = p->p_index % p->philo_ct;
		// pthread_mutex_lock(&p->forks_lock[r_fork]);
		// pthread_mutex_lock(&p->forks_lock[l_fork]);
	if (mode == 'u')
	{
		if (p->p_forks[r_fork] && p->p_forks[l_fork])
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
		// pthread_mutex_lock(&p->forks_lock[l_fork]);
		p->p_forks[r_fork] = 0;
		p->p_forks[l_fork] = 0;
		// pthread_mutex_unlock(&p->forks_lock[r_fork]);
		// pthread_mutex_unlock(&p->forks_lock[l_fork]);
		return (1);
	}
		// pthread_mutex_unlock(&p->forks_lock[r_fork]);
		// pthread_mutex_unlock(&p->forks_lock[l_fork]);
	return (0);
}

void	print_state(char state, t_philo *p)
{
	size_t	time_stamp;

	if (!(*p->p_end_threads))
	{
		pthread_mutex_lock(&p->print_lock);
		time_stamp = cur_time() - p->start_time;
		if (state == 'f')
		{
			printf("%zu %zu has taken a fork\n", time_stamp, p->p_index);
			// pthread_mutex_unlock(&p->print_lock);
		}
		else if (state == 'e')
		{
			printf("%zu %zu is eating\n", time_stamp, p->p_index);
			pthread_mutex_unlock(&p->print_lock);
			// philo_state(p, 1);
			p->last_meal = cur_time();
			ft_usleep(p->args.time_to_eat);
			if (check_death(p))
				return ;
			forks_state('r', p);
		}
		else if (state == 's')
		{
			printf("%zu %zu is sleeping\n", cur_time() - p->start_time, p->p_index);
			// pthread_mutex_unlock(&p->print_lock);
			// philo_state(p, 2);
			ft_usleep(p->args.time_to_sleep);
		}
		else if (state == 't')
		{
			// printf("hits\n");
			printf("%zu %zu is thinking\n", time_stamp, p->p_index);
			// pthread_mutex_unlock(&p->print_lock);
			// philo_state(p, 3);
		}
		else if (state == 'd' && cur_time() - p->last_meal >= p->args.time_to_die)
		{
			pthread_mutex_lock(&p->dead_lock);
			if (!p->p_dead[0])
			{
				printf("%zu %zu died\n", time_stamp, p->p_index);
				// pthread_mutex_unlock(&p->print_lock);
				p->p_dead[0] = 1;
			}
			pthread_mutex_unlock(&p->dead_lock);
		}
		pthread_mutex_unlock(&p->print_lock);
	}
}

void	use_forks(t_philo *p)
{
	size_t	r_fork;
	size_t	l_fork;

	r_fork = p->p_index - 1;
	l_fork = p->p_index % p->philo_ct;
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

void	*monitor(void *arg)
{
	t_setup *s;

	s = (t_setup *)arg;
	while (1)
	{
		pthread_mutex_lock(&s->dead_lock);
		if (*(s->dead))
		{
			pthread_mutex_unlock(&s->dead_lock);
			s->end_threads = 1;
			break ;
		}
		ft_usleep(1);
		pthread_mutex_unlock(&s->dead_lock);
	}
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	if (p->p_index % 2 && !(p->philo_ct > 1 && p->p_index == p->philo_ct))
	{
		use_forks(p);
		if (p->philo_ct == 1)
			ft_usleep(p->args.time_to_die);
		print_state('d', p);
		if (check_death(p))
			return (NULL);
		print_state('t', p);
	}
	else
		ft_usleep(p->args.time_to_eat);
	while (!(*p->p_end_threads))
	{
		while (!(*p->p_end_threads) && forks_state('u', p) == 'i')
		{
			print_state('d', p);
			if (check_death(p))
				return (NULL);
		}
		use_forks(p);
		print_state('d', p);
		if (check_death(p))
			return (NULL);
		print_state('t', p);
	}
	// printf("time to die:%zu\n", p->args.time_to_die);
	// printf("cur:%zu start:%zu diff:%zu\n", cur_time(), p->start_time, p->last_meal);
	return (NULL);
}
