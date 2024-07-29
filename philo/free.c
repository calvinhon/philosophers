#include "philo.h"

void	free_all(t_setup *s, t_philo *p)
{
	s->i = -1;
	while (++s->i < s->philo_ct)
		pthread_mutex_destroy(&p->forks_lock[s->i]);
	free(p->forks_lock);
	pthread_mutex_destroy(&s->times_ate_lock);
	pthread_mutex_destroy(&s->dead_lock);
	pthread_mutex_destroy(&s->meal_lock);
	pthread_mutex_destroy(&s->print_lock);
	free(s->threads);
	free(s->times_ate);
	free(s);
	free(p);
}