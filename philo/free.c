#include "philo.h"

void	free_all(t_setup *s, t_philo *p)
{
	s->i = -1;
	while (++s->i < p->philo_ct)
		pthread_mutex_destroy(&p->forks_lock[s->i]);
	free(p->forks_lock);
	free(s->threads);
	free(s->forks);
	free(s);
	free(p);
}