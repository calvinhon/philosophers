#include "philo.h"

void	use_forks(t_philo *p, char fork)
{
	bool	shift;

	shift = 0;
	if (fork == 'l')
		shift = 1;
	printf("%zu %zu has taken a fork\n", cur_time() - p->start_time, p->p_index);
	pthread_mutex_lock(&p->forks_lock[p->p_index + shift]);
	if (p->p_index == p->philo_ct && fork == 'l')
		p->p_forks[0] = 1;
	p->p_forks[p->p_index - 1 + shift] = 1;
	pthread_mutex_unlock(&p->forks_lock[p->p_index + shift]);
}

void	*routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	if (p->p_index % 2 && !(p->philo_ct > 1 && p->p_index == p->philo_ct))
	{
		use_forks(p, 'r');
		if (p->philo_ct > 1)
			use_forks(p, 'l');
	}
	if (p->philo_ct > 1 && p->p_forks[p->p_index - 1]
		&& ((p->p_index == p->philo_ct && p->p_forks[0]) || p->p_forks[p->p_index]))
	{
		printf("%zu %zu is eating\n", cur_time() - p->start_time, p->p_index);
		p->last_meal = cur_time() - p->start_time;
		ft_usleep(p->args.time_to_eat);
	}
	printf("cur:%zu start:%zu diff:%zu\n", cur_time(), p->start_time, p->last_meal);
	if (cur_time() - p->last_meal >= p->args.time_to_die)
		printf("%zu %zu is dead\n", cur_time() - p->start_time, p->p_index);
	return (NULL);
}
