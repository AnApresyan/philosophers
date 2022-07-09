/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapresya <aapresya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 13:10:11 by aapresya          #+#    #+#             */
/*   Updated: 2022/07/09 13:22:09 by aapresya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_simul_fields(int argc, char **argv, t_simul *simul)
{
	simul->num_phis = ft_atoi(argv[1]);
	simul->time_to_die = ft_atoi(argv[2]);
	simul->time_to_eat = ft_atoi(argv[3]);
	simul->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
	{
		simul->must_eat = ft_atoi(argv[5]);
		if (simul->must_eat <= 0)
			return (0);
	}
	else
		simul->must_eat = -1;
	if (simul->num_phis < 1 || simul->time_to_die <= 0
		|| simul->time_to_eat <= 0 || simul->time_to_sleep <= 0)
		return (0);
	simul->someone_died = 0;
	simul->done_eating = 0;
	return (1);
}

int	init_forks(t_simul *simul)
{
	int	i;

	i = 0;
	simul->forks = malloc(sizeof(*(simul->forks)) * simul->num_phis);
	if (pthread_mutex_init(&(simul->mutex_write), NULL))
		return (0);
	if (!simul->forks)
		return (0);
	while (i < simul->num_phis)
	{
		if (pthread_mutex_init(&(simul->forks[i]), NULL))
			return (0);
		i++;
	}
	return (1);
}

long long	get_time(void)
{
	struct timeval	c;

	gettimeofday(&c, NULL);
	return ((c.tv_sec * 1000) + (c.tv_usec / 1000));
}

int	init_philos(t_simul *simul)
{
	int	i;

	i = 0;
	simul->phis = (t_phi *)malloc(sizeof(*(simul->phis)) * simul->num_phis);
	if (!simul->phis)
		return (0);
	while (i < simul->num_phis)
	{
		simul->phis[i].id = i;
		simul->phis[i].num_meals = 0;
		simul->phis[i].simul = simul;
		simul->phis[i].first_fork = i;
		simul->phis[i].second_fork = (i + 1) % simul->num_phis;
		i++;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_simul	simulation;

	if (argc != 5 && argc != 6)
		return (error("Wrong number of arguments!"));
	if (!init_simul_fields(argc, argv, &simulation))
		return (error("Wrong arguments"));
	if (!init_forks(&simulation))
		return (error("Mutex creation failed"));
	if (!init_philos(&simulation))
		return (error("Malloc failed"));
	if (!launch_game(&simulation))
		return (error("Thread creation failed"));
}
