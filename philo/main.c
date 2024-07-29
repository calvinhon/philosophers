/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:21:23 by chon              #+#    #+#             */
/*   Updated: 2024/07/24 14:28:09 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	fill_args(char **av, t_setup *s, t_philo *p, size_t time_now)
{
	p->p_index = s->i + 1;
	p->last_meal = time_now;
	p->p_times_ate = s->times_ate;
	p->p_dead = &s->dead_flag;
	p->p_end_threads = &s->end_threads;
	p->args.time_to_die = ft_atoi(av[2]);
	p->args.time_to_eat = ft_atoi(av[3]);
	p->args.time_to_sleep = ft_atoi(av[4]);
	p->args.num_times_philo_must_eat = -1;
	if (av[5])
		p->args.num_times_philo_must_eat = ft_atoi(av[5]);
	p->start_time = time_now;
	p->philo_ct = s->philo_ct;
	p->forks_lock = s->lock_for_forks;
	p->times_ate_lock = s->lock_for_times_ate;
	p->p_dead_lock = &s->dead_lock;
	p->p_print_lock = &s->print_lock;
}

int	fill_p_data(char **av, t_setup *s, t_philo *p)
{
	size_t	time_now;

	s->lock_for_forks = malloc(sizeof(pthread_mutex_t) * s->philo_ct);
	if (!s->lock_for_forks)
		return (ft_error("fork mutex array malloc failed\n", s, p, 0));
	s->lock_for_times_ate = malloc(sizeof(pthread_mutex_t) * s->philo_ct);
	if (!s->lock_for_times_ate)
		return (ft_error("times ate mutex array malloc failed\n", s, p, 0));
	s->i = -1;
	while (++s->i < s->philo_ct)
	{
		pthread_mutex_init(&s->lock_for_forks[s->i], NULL);
		pthread_mutex_init(&s->lock_for_times_ate[s->i], NULL);
	}
	s->i = -1;
	time_now = cur_time();
	while (++s->i < s->philo_ct)
		fill_args(av, s, &p[s->i], time_now);
	return (1);
}

int	init_args(char **av, t_setup **s, t_philo **p)
{
	*s = malloc(sizeof(t_setup));
	if (!(*s))
		return (ft_error("setup array malloc failed\n", *s, *p, 0));
	(*s)->philo_ct = ft_atoi(av[1]);
	(*s)->threads = malloc(sizeof(pthread_t) * (*s)->philo_ct);
	if (!(*s)->threads)
		return (ft_error("threads array malloc failed\n", *s, *p, 0));
	(*s)->times_ate = malloc(sizeof(size_t) * (*s)->philo_ct);
	if (!(*s)->times_ate)
		return (ft_error("times ate array malloc failed\n", *s, *p, 0));
	memset((*s)->times_ate, 0, sizeof(size_t) * (*s)->philo_ct);
	(*s)->dead_flag = 0;
	pthread_mutex_init(&(*s)->dead_lock, NULL);
	pthread_mutex_init(&(*s)->print_lock, NULL);
	(*s)->end_threads = 0;
	*p = malloc(sizeof(t_philo) * (*s)->philo_ct);
	if (!(*p))
		return (ft_error("philo array malloc failed\n", *s, *p, 0));
	return (fill_p_data(av, *s, *p));
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
		while (++s->i < s->philo_ct)
			if (pthread_create(&s->threads[s->i], NULL,
				(void *)&routine, (void *)&p[s->i]))
				printf("thread creation failed\n");
		pthread_create(&s->monitor_th, NULL, (void *)&monitor, (void *)s);
		s->i = -1;
		while (++s->i < s->philo_ct)
			if (pthread_join(s->threads[s->i], NULL))
				printf("thread join failed\n");
		pthread_join(s->monitor_th, NULL);
		free_all(s, p);
	}
	return (0);
}
