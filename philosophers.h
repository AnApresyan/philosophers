#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_simul t_simul;

typedef struct s_phi
{
	int	id;
	int first_fork;
	int second_fork;
	long long last_spght;
	int num_meals;
	t_simul *simul;
	pthread_t thread;
}	t_phi;

typedef struct s_simul
{
	int num_phis;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int must_eat;
	long long start_game;
	t_phi *phis;
	int someoneDied;
	int doneEating;
	pthread_t death_checker;
	pthread_mutex_t *forks;
}	t_simul;

long	ft_atoi(const char *str);
int		ft_strlen(const char *str);
int		error(char *str);
int	launch_game(t_simul *simul);
void eat(t_phi *phi);
void	*cycle(void *void_phi);
void	*check_death(void *void_simul);
void state_change_print(t_phi *phi, char *message);
void ft_sleep(t_simul *simul, long long time);
void ft_exit(t_simul *simul);
int	init_philos(t_simul *simul);
long long	get_time();
int	init_forks(t_simul *simul);
int	init_simul_fields(int argc, char **argv, t_simul *simul);

#endif