/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 11:58:32 by JFikents          #+#    #+#             */
/*   Updated: 2024/02/29 10:11:54 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	simulation(t_phil_schedule *phil)
{
	int			i;
	t_phil_args	args[1];

	i = -1;
	while (++i < phil->count)
		pthread_mutex_init(&phil->forks[i], NULL);
	pthread_mutex_init(phil->print, NULL);
	pthread_mutex_init(phil->take_forks, NULL);
	i = -1;
	args->phil = phil;
	while (++i < phil->count)
	{
		args->i = i;
		if (i % 2 == 0)
			pthread_create(&phil->philosopher[i], NULL, &odd_phil, args);
		else
			pthread_create(&phil->philosopher[i], NULL, &even_phil, args);
	}
	pthread_create(phil->death_timer, NULL, death_timer, phil);
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
	if (error((int [3]){IF_NULL, ALLOC_PHIL, INIT_PHIL}, phil->philosopher))
		return (1);
	phil->ate = ft_calloc(phil->count, sizeof(int));
	if (error((int [3]){IF_NULL, FLAG_ATE, INIT_PHIL}, phil->ate))
		return (1);
	phil->forks = ft_calloc(phil->count, sizeof(pthread_mutex_t));
	if (error((int [3]){IF_NULL, ALLOC_FORKS, INIT_PHIL}, phil->forks))
		return (1);
	phil->phil_has_fork = ft_calloc(phil->count, sizeof(int));
	if (error((int [3]){IF_NULL, FLAG_FORK, INIT_PHIL}, phil->phil_has_fork))
		return (1);
	phil->can_eat = ft_calloc(phil->count, sizeof(int));
	if (error((int [3]){IF_NULL, CAN_EAT, INIT_PHIL}, phil->can_eat))
		return (1);
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
		return (finish_simulation(phil));
	phil->start_time = get_time(NULL);
	if (phil->start_time == -1)
		return (GET_TIME);
	simulation(phil);
	i = 0;
	while (i < phil->count)
	{
		pthread_join(phil->philosopher[i], NULL);
		i++;
	}
	pthread_join((pthread_t) phil->death_timer, NULL);
	return (finish_simulation(phil));
}
