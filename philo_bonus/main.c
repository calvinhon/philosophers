/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:21:23 by chon              #+#    #+#             */
/*   Updated: 2024/08/08 15:27:27 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor_full(void *arg)
{
	t_philo	*p;
	int		i;

	p = (t_philo *)arg;
	if (p->s->p_ct > 1)
	{
		i = -1;
		while (++i < p->s->p_ct)
			sem_wait(p->s->full_sem);
		sem_post(p->s->sim_sem);
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
	s->i = -1;
	while (++s->i < s->p_ct)
	{
		s->pid[s->i] = fork();
		if (s->pid[s->i] < 0)
			return (free_all(p), 1);
		if (!s->pid[s->i])
			routine(&s->p[s->i]);
	}
	return (1);
}

int	init_sems(t_setup *s)
{
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/meal");
	sem_unlink("/full_philo");
	sem_unlink("/simulation");
	s->forks_sem = sem_open("/forks", O_CREAT, 0644, s->p_ct);
	if (s->forks_sem == SEM_FAILED)
		return (0);
	s->print_sem = sem_open("/print", O_CREAT, 0644, 1);
	if (s->print_sem == SEM_FAILED)
		return (0);
	s->meal_sem = sem_open("/meal", O_CREAT, 0644, 1);
	if (s->meal_sem == SEM_FAILED)
		return (0);
	s->full_sem = sem_open("/full_philos", O_CREAT, 0644, 0);
	if (s->full_sem == SEM_FAILED)
		return (0);
	s->sim_sem = sem_open("/simulation", O_CREAT, 0644, 0);
	if (s->sim_sem == SEM_FAILED)
		return (0);
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
	if (!init_sems(s))
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

int	main(int ac, char **av)
{
	t_setup		*s;
	t_philo		*p;
	pthread_t	full_thread;

	s = NULL;
	p = NULL;
	if (ac < 5 || ac > 6 || !valid_int(av) || (av[5] && ft_atoi(av[5]) == 0))
		return (1);
	if (!init_vars(av, &s, &p) || s->p_ct > 200 || !s->p_ct)
	{
		printf("malloc failed or invalid p_ct\n");
		return (free_all(p), 1);
	}
	pthread_create(&full_thread, NULL, monitor_full, p);
	pthread_detach(full_thread);
	sem_wait(s->sim_sem);
	s->i = -1;
	while (++s->i < s->p_ct)
		kill(s->pid[s->i], SIGKILL);
	s->i = -1;
	while (++s->i < s->p_ct)
		waitpid(s->pid[s->i], NULL, 0);
	free_all(p);
	return (0);
}
