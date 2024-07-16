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

void	init_args(char **av, t_philo *p)
{
	p->num_times_philo_must_eat = 0;
	p->num_of_philos = ft_atoi(av[1]);
	p->time_to_die = ft_atoi(av[2]);
	p->time_to_eat = ft_atoi(av[3]);
	p->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
		p->num_times_philo_must_eat = ft_atoi(av[5]);
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
	t_philo	p;

	if (ac > 4 && ac < 7 && parse(av))
	{
		init_args(av, &p);
		if (p.num_of_philos > 200)
			return (1);
		// printf("%zu %zu %zu %zu %zu\n", p.num_of_philos, p.time_to_die, p.time_to_eat, p.time_to_sleep, p.num_times_philo_must_eat);
	}
	return (0);
}
