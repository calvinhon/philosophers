#include "philo.h"

void	free_all(t_setup *s, t_philo *p)
{
	free(s->threads);
	free(s->forks);
	free(p);
}