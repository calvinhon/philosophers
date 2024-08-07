/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:21:23 by chon              #+#    #+#             */
/*   Updated: 2024/08/06 15:26:38 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor(void *arg)
{
	t_setup *s;

	s = (t_setup *)arg;
	// wait_sem(s->full_philo);
	while (1)
	{
		if (check_death(s->p))
		{
			sem_wait(s->deadSem);
			break ;
		}
		// if (s->num_of_full_philos == s->p_ct)
		// {
		// 	s->all_philos_full = 1;
		// 	break ;
		// }
	}
	return (NULL);
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
	s->i = -1;
	while (++s->i < s->p_ct)
	{
		p[s->i].p_index = s->i + 1;
		p[s->i].last_meal = s->start_time;
		p[s->i].times_ate = 0;
		p[s->i].s = s;
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
	memset(s, 0, sizeof(t_setup));
	s->p_ct = ft_atoi(av[1]);
	s->threads = malloc(sizeof(pthread_t) * s->p_ct);
	if (!s->threads)
		return (0);
	s->forksSem = sem_open("/forks", O_CREAT, 0644, s->p_ct);
	if (s->forksSem == SEM_FAILED)
        return (0);
	s->fullSem = sem_open("/full_philos", O_CREAT, 0644, s->p_ct);
	if (s->fullSem == SEM_FAILED)
        return (0);
	s->deadSem = sem_open("/dead_philo", O_CREAT, 0644, 0);
	if (s->deadSem == SEM_FAILED)
        return (0);
	s->pid = malloc(sizeof(int) * s->p_ct);
	if (!s->pid)
		return (0);
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

int	main(int ac, char **av)
{
	t_setup	*s;
	t_philo	*p;

	s = NULL;
	p = NULL;
	if (ac < 5 || ac > 6 || !valid_int(av) || (av[5] && ft_atoi(av[5]) == 0))
		return (1);
	if (!init_vars(av, &s, &p) || s->p_ct > 200 || !s->p_ct)
	{
		printf("malloc failed or invalid p_ct\n");
		return (free_all(p), 1);
	}
	s->i = -1;
	while (++s->i < s->p_ct)
	{
		s->pid[s->i] = fork();
		if (s->pid[s->i] < 0)
			return (free_all(p), 1);
		if (!s->pid[s->i])
		{
			routine(&s->p[s->i]);
			if (pthread_create(&s->threads[s->i], NULL, monitor, s))
				printf("thread creation failed\n");
		}
	}
	monitor(s);
	s->i = -1;
	while (++s->i < s->p_ct)
	{
		kill(s->pid[s->i], SIGKILL);
		// if (pthread_join(s->threads[s->i], NULL))
		// 	printf("thread join failed\n");
	}
	s->i = -1;
	while (++s->i < s->p_ct)
		waitpid(s->pid[s->i], NULL, WNOHANG);
	sem_unlink("/forks");
	sem_unlink("/full_philo");
	sem_unlink("/dead_philo");
	free_all(p);
	return (0);
}
