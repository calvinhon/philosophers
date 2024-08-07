/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:21:37 by chon              #+#    #+#             */
/*   Updated: 2024/08/07 17:59:37 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <time.h>
# include <sys/time.h>
# include <sys/stat.h>
# include <stdbool.h>
# include <semaphore.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>

typedef struct s_philo
{
	int				p_index;
	size_t			last_meal;
	int				times_ate;
	struct s_setup	*s;
}	t_philo;

typedef struct s_setup
{
	int			p_ct;
	size_t		time_to_die;
	size_t		time_to_eat;
	size_t		time_to_sleep;
	int			num_times_philo_must_eat;
	int			num_of_full_philos;
	size_t		start_time;	
	bool		all_philos_full;
	bool		dead_philo;
	int			i;
	int			*pid;
	sem_t		*forksSem;
	sem_t		*printSem;
	sem_t		*fullSem;
	sem_t		*deadSem;
	sem_t		*simSem;
	t_philo		*p;
}	t_setup;

int		ft_atoi(char *str);
int		ft_usleep(size_t msec);
size_t	cur_time(void);
void	*routine(t_philo *arg);
bool	check_death(t_philo *p);
void	free_all(t_philo *p);
void	*end_process(void *arg);

#endif