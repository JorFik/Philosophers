/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 11:58:32 by JFikents          #+#    #+#             */
/*   Updated: 2024/03/09 07:58:39 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	simulation(t_phil_schedule *phil, int *i)
{
	t_phil_args	args[1];
	extern int	errno;

	(*i) = -1;
	while (++(*i) < phil->count)
		pthread_mutex_init(&phil->forks[(*i)], NULL);
	pthread_mutex_init(phil->print, NULL);
	(*i) = -1;
	args->phil = phil;
	printf("\t\t\t\t\tStart simulation\n");
	phil->start_time = get_time(NULL);
	if (error((int [3]){phil->start_time, GET_TIME, START_SIM}, NULL))
		return ;
	while (++(*i) < phil->count)
	{
		args->i = (*i);
		ft_stop_watch(phil, (*i), START);
		pthread_create(&phil->philosopher[(*i)], NULL, &phil_live, args);
		usleep(50);
	}
	death_timer(phil);
}

static int	initilize_philos(t_phil_schedule *phil, int argc, char **argv)
{
	memset(phil, 0, sizeof(t_phil_schedule));
	phil->count = ft_atoi(argv[1]);
	phil->die_time = ft_atoi(argv[2]);
	phil->eat_time = ft_atoi(argv[3]);
	phil->sleep_time = ft_atoi(argv[4]);
	phil->meal_count = -1;
	if (argc == 6)
		phil->meal_count = ft_atoi(argv[5]);
	phil->philosopher = ft_calloc(phil->count, sizeof(pthread_t));
	phil->ate = ft_calloc(phil->count, sizeof(int));
	phil->forks = ft_calloc(phil->count, sizeof(pthread_mutex_t));
	phil->phil_has_fork = ft_calloc(phil->count + 1, sizeof(int));
	phil->stop_watch = ft_calloc(phil->count + 1, sizeof(long long));
	if (error((int [3]){IF_NULL, ALLOC_PHIL, INIT_PHIL}, phil->philosopher)
		|| error((int [3]){IF_NULL, FLAG_ATE, INIT_PHIL}, phil->ate)
		|| error((int [3]){IF_NULL, ALLOC_FORKS, INIT_PHIL}, phil->forks)
		|| error((int [3]){IF_NULL, FLAG_FORK, INIT_PHIL}, phil->phil_has_fork)
		|| error((int [3]){IF_NULL, CAN_EAT, INIT_PHIL}, phil->stop_watch))
		return (1);
	return (0);
}

static int	finish_simulation(t_phil_schedule *phil)
{
	extern int	errno;
	int			i;

	i = -1;
	while (++i < phil->count)
		pthread_mutex_destroy(&phil->forks[i]);
	pthread_mutex_destroy(phil->print);
	if (phil->philosopher)
		free(phil->philosopher);
	if (phil->ate)
		free(phil->ate);
	if (phil->forks)
		free(phil->forks);
	if (phil->phil_has_fork)
		free(phil->phil_has_fork);
	if (phil->stop_watch)
		free(phil->stop_watch);
	memset(phil, 0, sizeof(t_phil_schedule));
	printf("\t\t\t\tSimulation finished\n");
	if (errno && errno != 60)
		return (errno);
	return (0);
}

int	main(int argc, char **argv)
{
	t_phil_schedule	phil[1];
	int				i;

	if (argc < 5)
		return (error((int [3]){-1, LESS_ARGS, ARGS}, NULL));
	if (argc > 6)
		return (error((int [3]){-1, MORE_ARGS, ARGS}, NULL));
	if (ft_atoi(argv[1]) < 1)
		return (error((int [3]){-1, NO_PHIL, SIMULATION}, NULL));
	if (initilize_philos(phil, argc, argv))
		return (finish_simulation((t_phil_schedule *)phil));
	simulation(phil, &i);
	i = 0;
	while (i < phil->count)
	{
		pthread_join(phil->philosopher[i], NULL);
		i++;
	}
	return (finish_simulation((t_phil_schedule *)phil));
}
