#include "philo.h"

void	free_all(t_setup *s, t_philo *p)
{
	s->i = -1;
	while (++s->i < s->philo_ct)
		pthread_mutex_destroy(&p->forks_lock[s->i]);
	free(p->forks_lock);
	s->i = -1;
	while (++s->i < s->philo_ct)
		pthread_mutex_destroy(&p->times_ate_lock[s->i]);
	free(p->times_ate_lock);
	s->i = -1;
	pthread_mutex_destroy(&p->dead_lock);
	free(s->threads);
	free(s->times_ate);
	free(s->forks);
	free(s->dead);
	free(s);
	free(p);
}