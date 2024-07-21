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

int	fill_p_data(t_setup *s, t_philo *p, t_params p_args)
{
	pthread_mutex_t	*lock_for_forks;
	size_t			time_now;

	lock_for_forks = malloc(sizeof(pthread_mutex_t) * s->philo_ct);
	if (!lock_for_forks)
		return (ft_error("fork mutex array malloc failed\n", s, p, 0));
	while (++s->i < s->philo_ct)
		pthread_mutex_init(&lock_for_forks[s->i], NULL);
	s->i = -1;
	time_now = cur_time();
	while (++s->i < s->philo_ct)
	{
		p[s->i].p_index = s->i + 1;
		p[s->i].last_meal = 0;
		p[s->i].times_ate = 0;
		p[s->i].p_forks = s->forks;
		p[s->i].forks_lock = lock_for_forks;
		p[s->i].args = p_args;
		p[s->i].eating = 0;
		p[s->i].sleeping = 0;
		p[s->i].thinking = 0;
		p[s->i].dead = 0;
		p[s->i].start_time = time_now;
		p[s->i].philo_ct = s->philo_ct;
	}
	return (1);
}

void	fill_args(t_params *p_args, char **av)
{
	p_args->time_to_die = ft_atoi(av[2]);
	p_args->time_to_eat = ft_atoi(av[3]);
	p_args->time_to_sleep = ft_atoi(av[4]);
	p_args->num_times_philo_must_eat = -1;
	if (av[5])
		p_args->num_times_philo_must_eat = ft_atoi(av[5]);
}

int	init_args(char **av, t_setup **s, t_philo **p)
{
	t_params	p_args;

	*s = malloc(sizeof(t_setup));
	if (!(*s))
		return (ft_error("setup array malloc failed\n", *s, *p, 0));
	(*s)->philo_ct = ft_atoi(av[1]);
	(*s)->threads = malloc(sizeof(pthread_t) * (*s)->philo_ct);
	if (!(*s)->threads)
		return (ft_error("threads array malloc failed\n", *s, *p, 0));
	(*s)->forks = malloc(sizeof(bool) * (*s)->philo_ct);
	if (!(*s)->forks)
		return (ft_error("fork array malloc failed\n", *s, *p, 0));
	memset((*s)->forks, 0, sizeof(bool) * (*s)->philo_ct);
	(*s)->i = -1;
	*p = malloc(sizeof(t_philo) * (*s)->philo_ct);
	if (!(*p))
		return (ft_error("philo array malloc failed\n", *s, *p, 0));
	fill_args(&p_args, av);
	(*s)->i = -1;
	return (fill_p_data(*s, *p, p_args));
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


int main(int ac, char **av)
{
	t_setup	*s;
	t_philo	*p;

	s = NULL;
	p = NULL;
	if (ac > 4 && ac < 7 && parse(av))
	{
		if (!init_args(av, &s, &p) || s->philo_ct > 200)
			return (ft_error("malloc failed or philo_ct > 200\n", s, p, 0));
		s->i = -1;
		// printf("%zu\n", p[0].p_index);
		while (++s->i < p->philo_ct)
			if (pthread_create(&s->threads[s->i], NULL, (void *)&routine, (void *)&p[s->i]))
				printf("thread creation failed\n");
		s->i = -1;
		while (++s->i < p->philo_ct)
			if (pthread_join(s->threads[s->i], NULL))
				printf("thread join failed\n");
		free_all(s, p);
		// printf("philo_ct:%zu\n", s->i);
	}
	return (0);
}
