/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:21:37 by chon              #+#    #+#             */
/*   Updated: 2024/08/08 15:25:39 by chon             ###   ########.fr       */
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
	size_t		start_time;	
	int			i;
	int			*pid;
	sem_t		*forks_sem;
	sem_t		*print_sem;
	sem_t		*meal_sem;
	sem_t		*full_sem;
	sem_t		*sim_sem;
	t_philo		*p;
}	t_setup;

int		ft_atoi(char *str);
int		ft_usleep(size_t msec);
size_t	cur_time(void);
void	*routine(t_philo *arg);
int		valid_int(char **av);
void	free_all(t_philo *p);

#endif