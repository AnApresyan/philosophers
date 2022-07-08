#include "philosophers.h"

int	init_simul_fields(int argc, char **argv, t_simul *simul)
{
	//struct timeval c;

	simul->num_phis = ft_atoi(argv[1]);
	simul->time_to_die = ft_atoi(argv[2]);
	simul->time_to_eat = ft_atoi(argv[3]);
	simul->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6){
		simul->must_eat = ft_atoi(argv[5]);
		if (simul->must_eat <= 0)
			return (0);
	}
	else
		simul->must_eat = -1;
	if (simul->num_phis < 2 || simul->time_to_die <= 0 || simul->time_to_eat <= 0 || simul->time_to_sleep <= 0)
		return (0);
	simul->someoneDied = 0;
	simul->doneEating = 0;
	//gettimeofday(&c, NULL);
	return (1);
}

int	init_forks(t_simul *simul)
{
	int	i;

	i = 0;
	simul->forks = (pthread_mutex_t*)malloc(sizeof(*(simul->forks)) * simul->num_phis);
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

long long	get_time()
{
	struct timeval c;

	gettimeofday(&c, NULL);
	return ((c.tv_sec * 1000) + (c.tv_usec / 1000));
}

int	init_philos(t_simul *simul)
{
	int	i;

	i = 0;
	simul->phis = (t_phi*)malloc(sizeof(*(simul->phis)) * simul->num_phis);
	if (!simul->phis)
		return (0);
	while (i < simul->num_phis)
	{
		simul->phis[i].id = i;
		simul->phis[i].num_meals = 0;
		simul->phis[i].last_spght = 0;
		simul->phis[i].simul = simul;
		if (i % 2 == 0)
		{
			simul->phis[i].first_fork = i; 
			simul->phis[i].second_fork = (i + 1) % simul->num_phis;
		}
		else
		{
			simul->phis[i].first_fork = (i + 1) % simul->num_phis;
			simul->phis[i].second_fork = i;
		}
		i++;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_simul simulation;

	if (argc != 5 && argc != 6)
		return error("Wrong number of arguments!");
	if (!init_simul_fields(argc, argv, &simulation))
		return error("Wrong arguments");
	if (!init_forks(&simulation))
		return error("Mutex creation failed");
	if (!init_philos(&simulation))
		return error("Malloc failed");
	launch_game(&simulation);

	//CHECKING
	printf("Number of philosophers: %d\n", simulation.num_phis);
	printf("time to die: %d\n", simulation.time_to_die);
	printf("time to eat: %d\n", simulation.time_to_eat);
	printf("time to sleep: %d\n", simulation.time_to_sleep);
	printf("must eat: %d\n", simulation.must_eat);
	printf("TIMESTAMP: %lld\n", simulation.start_game);

	for (int i = 0; i < simulation.num_phis; i++)
	{
		printf("id: %d\n", simulation.phis[i].id);
		printf("first fork: %d\n", simulation.phis[i].first_fork);
		printf("second fork: %d\n", simulation.phis[i].second_fork);
		printf("last meal: %lld\n", simulation.phis[i].last_spght);
		printf("number of meals: %d\n", simulation.phis[i].num_meals);
		printf("\n");
	}
}