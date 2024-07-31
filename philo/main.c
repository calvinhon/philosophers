/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:21:23 by chon              #+#    #+#             */
/*   Updated: 2024/07/30 12:51:03 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	monitor(t_setup *s)
{
	s->i = -1;
	while (1)
	{
		if (s->i + 1 == s->p_ct)
			s->i = -1;
		if (check_death(&s->p[++s->i]))
		{
			pthread_mutex_lock(&s->lock);
			s->end_threads = 1;
			pthread_mutex_unlock(&s->lock);
			return (1);
		}
	}
	return (0);
}

int	init_vars_2(char **av, t_setup *s, t_philo *p)
{
	s->time_to_die = ft_atoi(av[2]);
	s->time_to_eat = ft_atoi(av[3]);
	s->time_to_sleep = ft_atoi(av[4]);
	s->num_times_philo_must_eat = -1;
	if (av[5])
		s->num_times_philo_must_eat = ft_atoi(av[5]);
	s->start_time = cur_time();
	s->end_threads = 0;
	s->i = -1;
	while (++s->i < s->p_ct)
	{
		p[s->i].p_index = s->i + 1;
		p[s->i].last_meal = s->start_time;
		p[s->i].times_ate = 0;
		p[s->i].s = s;
		p[s->i].r_fork = s->forks_lock[s->i];
		if (s->p_ct > 1)
			p[s->i].l_fork = s->forks_lock[(s->i + 1) % s->p_ct];
		// printf("%i: rfork:%i lfork:%i\n", p[s->i].p_index, s->i, (s->i + 1) % s->p_ct);
	}
	return (1);
}

int	init_vars(char **av, t_setup **s_ptr, t_philo **p_ptr)
{
	t_setup	*s; 
	t_philo	*p; 

	s = malloc(sizeof(t_setup));
	if (!s)
		return (0);
	*s_ptr = s;
	s->p_ct = ft_atoi(av[1]);
	s->threads = malloc(sizeof(pthread_t) * s->p_ct);
	if (!s->threads)
		return (0);
	s->forks_lock = malloc(sizeof(pthread_mutex_t) * s->p_ct);
	if (!s->forks_lock)
		return (0);
	s->i = -1;
	while (++s->i < s->p_ct)
		pthread_mutex_init(&s->forks_lock[s->i], NULL);
	pthread_mutex_init(&s->lock, NULL);
	p = malloc(sizeof(t_philo) * s->p_ct);
	if (!p)
		return (0);
	*p_ptr = p;
	s->p = p;
	return (init_vars_2(av, s, p));
}

int	valid_int(char **av)
{
	int	i;
	int	j;

	i = 0;
	j = -1;
	while (av[++i])
	{
		while (av[i][++j])
		{
			if (av[i][j] < '0' || av[i][j] > '9')
				return (0);
		}
		j = -1;
	}
	return (1);
}

int main(int ac, char **av)
{
	t_setup	*s;
	t_philo	*p;

	s = NULL;
	p = NULL;
	if (ac < 5 || ac > 6 || !valid_int(av))
		return (1);
	if (!init_vars(av, &s, &p) || s->p_ct > 200 || !s->p_ct)
	{
		printf("malloc failed or invalid p_ct\n");
		return (1);
	}
	s->i = -1;
	while (++s->i < s->p_ct)
		if (pthread_create(&s->threads[s->i], NULL, routine, &p[s->i]))
			printf("thread creation failed\n");
	monitor(s);
	// s->i = -1;
	// while (++s->i < s->p_ct)
	// 	if (pthread_join(s->threads[s->i], NULL))
	// 		printf("thread join failed\n");
	s->i = -1;
	while (++s->i < s->p_ct)
		if (pthread_detach(s->threads[s->i]))
			printf("thread detach failed\n");
	return (0);
}
