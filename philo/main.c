/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:21:23 by chon              #+#    #+#             */
/*   Updated: 2024/07/16 10:41:07 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	fill_args(t_params *p_args, char **av)
{
	p_args->time_to_die = ft_atoi(av[2]);
	p_args->time_to_eat = ft_atoi(av[3]);
	p_args->time_to_sleep = ft_atoi(av[4]);
	p_args->num_times_philo_must_eat = -1;
	if (av[5])
		p_args->num_times_philo_must_eat = ft_atoi(av[5]);
}

int	init_args(char **av, t_setup *s, t_philo *p)
{
	t_params	p_args;

	s->philo_ct = ft_atoi(av[1]);
	p = malloc(sizeof(t_philo) * s->philo_ct);
	if (!p)
		return (ft_error("philo array malloc failed\n", 0));
	s->threads = malloc(sizeof(pthread_t) * s->philo_ct);
	if (!s->threads)
		return (ft_error("threads array malloc failed\n", 0));
	s->forks = malloc(sizeof(pthread_mutex_t) * s->philo_ct);
	if (!s->forks)
		return (ft_error("fork array malloc failed\n", 0));
	s->i = -1;
	fill_args(&p_args, av);
	while (++s->i < s->philo_ct)
	{
		p[i].p_index = i + 1;
		p[i].times_ate = 0;
		p[i].args = p_args;
		p[i].eating = FALSE;
		p[i].sleeping = FALSE;
		p[i].thinking = FALSE;
	}
}

int	parse(char **av)
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

void	*routine(t_philo p)
{
	if (p.index % 2)
		printf("hello\n");
}

int main(int ac, char **av)
{
	t_setup	*s;
	t_philo	*p;

	if (ac > 4 && ac < 7 && parse(av))
	{
		init_args(av, s, p);
		if (s.philo_ct > 200 || !p || !s->forks)
			return (1);
		s.i = -1;
		while (++s.i < s.philo_ct)
			if (pthread_create(s.threads[i], NULL, &routine, p[i]))
				printf("thread creation failed\n");
		s.i = -1;
		while (++s.i < s.philo_ct)
			if (pthread_join(s.threads[i], NULL))
				printf("thread join failed\n");
		free_all(s, p);
		// printf("%zu %zu %zu %zu %zu\n", p.num_of_philos, p.time_to_die, p.time_to_eat, p.time_to_sleep, p.num_times_philo_must_eat);
	}
	return (0);
}
