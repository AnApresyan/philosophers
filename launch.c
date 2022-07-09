/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapresya <aapresya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 13:10:17 by aapresya          #+#    #+#             */
/*   Updated: 2022/07/09 13:26:21 by aapresya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	eat(t_phi *phi)
{
	t_simul	*simul;

	simul = phi->simul;
	pthread_mutex_lock(&simul->forks[phi->first_fork]);
	state_change_print(phi, "has taken the first fork");
	pthread_mutex_lock(&simul->forks[phi->second_fork]);
	state_change_print(phi, "has taken the second fork");
	state_change_print(phi, "is eating");
	phi->last_spght = get_time();
	ft_sleep(simul, simul->time_to_eat);
	phi->num_meals++;
	pthread_mutex_unlock(&simul->forks[phi->first_fork]);
	pthread_mutex_unlock(&simul->forks[phi->second_fork]);
}

void	*cycle(void *void_phi)
{
	t_phi	*phi;
	t_simul	*simul;

	phi = (t_phi *)void_phi;
	simul = phi->simul;
	if (simul->num_phis == 1)
		return (NULL);
	if (phi->id % 2 == 0)
		usleep(50);
	while (!simul->someone_died)
	{
		eat(phi);
		if (phi->num_meals == simul->must_eat)
		{
			simul->done_eating = 1;
			break ;
		}
		state_change_print(phi, "is sleeping");
		ft_sleep(simul, simul->time_to_sleep);
		state_change_print(phi, "is thinking");
	}
	return (NULL);
}

void	*check_death(void *void_simul)
{
	int		i;
	t_simul	*simul;

	simul = (t_simul *)void_simul;
	while (!simul->done_eating && !simul->someone_died)
	{
		i = 0;
		while (i < simul->num_phis)
		{
			if (get_time() - simul->phis[i].last_spght >= simul->time_to_die)
			{
				state_change_print(&simul->phis[i], "died");
				simul->someone_died = 1;
				break ;
			}
			i++;
		}
	}
	return (NULL);
}

void	state_change_print(t_phi *phi, char *message)
{
	pthread_mutex_lock(&(phi->simul->mutex_write));
	if (!phi->simul->someone_died)
		printf("%lli %d %s\n", get_time()
			- phi->simul->start_game, phi->id, message);
	pthread_mutex_unlock(&(phi->simul->mutex_write));
}

int	launch_game(t_simul *simul)
{
	int	i;

	i = 0;
	simul->start_game = get_time();
	while (i < simul->num_phis)
		simul->phis[i++].last_spght = simul->start_game;
	i = 0;
	if (pthread_create(&(simul->death_checker), NULL, &check_death, simul) != 0)
		return (0);
	while (i < simul->num_phis)
	{
		if (pthread_create(&(simul->phis[i].thread),
				NULL, &cycle, &simul->phis[i]) != 0)
			return (0);
		i++;
	}
	ft_exit(simul);
	return (1);
}
