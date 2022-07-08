#include <pthread.h>
#include "philosophers.h"

int	error(char *str)
{
	if (str)
		printf("%s", str);
	return (1);
}

void ft_exit(t_simul *simul)
{
	int i;

	i = 0;
	while (i++ < simul->num_phis)
		pthread_join(simul->phis[i].thread, NULL);
	i = 0;
	while (i++ < simul->num_phis)
		pthread_mutex_destroy(&(simul->forks[i]));
		
}