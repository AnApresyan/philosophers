/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapresya <aapresya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 13:11:07 by aapresya          #+#    #+#             */
/*   Updated: 2022/07/09 13:22:39 by aapresya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	error(char *str)
{
	if (str)
		printf("%s", str);
	return (1);
}

void	ft_exit(t_simul *simul)
{
	int	i;

	i = 0;
	pthread_join(simul->death_checker, NULL);
	while (i < simul->num_phis)
	{
		pthread_join(simul->phis[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i < simul->num_phis)
		pthread_mutex_destroy(&(simul->forks[i++]));
	pthread_mutex_destroy(&(simul->mutex_write));
}
