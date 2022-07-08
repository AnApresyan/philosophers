#include "philosophers.h"

void eat(t_phi *phi)
{
	t_simul *simul;

	simul = phi->simul;
	pthread_mutex_lock(&simul->forks[phi->first_fork]);
	//print has taken the first fork
	state_change_print(phi, "has taken the first fork");
	pthread_mutex_lock(&simul->forks[phi->second_fork]);
	//print has taken the second fork
	state_change_print(phi, "has taken the second fork");
	state_change_print(phi, "is eating");
	//print the philosopher is eating
	ft_sleep(simul, simul->time_to_eat);
	phi->last_spght = get_time();
	phi->num_meals++;
	pthread_mutex_unlock(&simul->forks[phi->first_fork]);
	pthread_mutex_unlock(&simul->forks[phi->second_fork]);
}

void	*cycle(void *void_phi)
{
	t_phi *phi;
	t_simul *simul;

	phi = (t_phi*)void_phi;
	simul = phi->simul;
	while (!simul->someoneDied)
	{
		eat(phi);
		if (phi->num_meals == simul->must_eat)
		{
			simul->doneEating = 1;
			break;
		}
		//print that the philosopher is sleeping
		state_change_print(phi, "is sleeping");
		ft_sleep(simul, simul->time_to_sleep);
		//print that the philosopher is thinking again
		state_change_print(phi, "is thinking");

	}
	return (NULL);
}

void	*check_death(void *void_simul)
{
	int i;
	t_simul *simul;

	simul = (t_simul *)void_simul;
	i = 0;
	while (!simul->doneEating && !simul->someoneDied)
	{
		while (i < simul->num_phis)
		{
			if (get_time() - simul->phis[i].last_spght >= simul->time_to_die && !simul->someoneDied)
			{
				state_change_print(&simul->phis[i], "died");
				simul->someoneDied = 1;
				break;
			}
			i++;
		}
	}
	return (NULL);
}

void state_change_print(t_phi *phi, char *message)
{
	if (!phi->simul->someoneDied)
	{
		printf("%lli ", get_time() - phi->simul->start_game);
		printf("%d " , phi->id);
		printf("%s\n", message);
	}
}

void ft_sleep(t_simul *simul, long long time)
{
	long long i;

	i = get_time();
	while (!simul->someoneDied)
	{
		if (get_time() - i >= time)
			break;
		usleep(50);
	}
}


int	launch_game(t_simul *simul)
{
	int	i = 0;

	simul->start_game = get_time();
	if (pthread_create(&(simul->death_checker), NULL, &check_death, &simul) != 0)
		return (0);
	while (i < simul->num_phis)
	{
		if (pthread_create(&(simul->phis[i].thread), NULL, &cycle, &simul->phis[i]) != 0)
			return (0);
		i++;
	}
	ft_exit(simul);
	return (1);
}