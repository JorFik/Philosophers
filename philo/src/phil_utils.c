/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phil_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 02:05:22 by JFikents          #+#    #+#             */
/*   Updated: 2024/03/07 13:21:18 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// static int	wait_to_eat(const int index, t_phil_schedule *phil, int *next)
// {
// 	int	i;
// 	int	needs_to_eat;

// 	if (phil->ate[index] == STARVING)
// 		return (1);
// 	i = *next;
// 	needs_to_eat = 0;
// 	while (phil->eat_order[i] && i < phil->count)
// 		if (phil->eat_order[i++] == index + 1)
// 			needs_to_eat = 1;
// 	while (needs_to_eat && phil->eat_order[*next] != index + 1
// 		&& !phil->someone_died)
// 		usleep(1);
// 	return (needs_to_eat);
// }

static int	take_forks(const int i, t_phil_schedule *phil, int *order)
{
	static int	next = 0;
	int			needs_to_eat = 0;
	int			index;

	if (next >= phil->count)
		next = 0;
	index = next;
	// needs_to_eat = wait_to_eat(i, phil, &next);
	if ((order[next++] != i + 1 && !needs_to_eat) || phil->count == 1)
		return (0);
	// pthread_mutex_lock(phil->take_forks);
	phil->ate[i] = IS_EATING;
	pthread_mutex_lock(&phil->forks[i]);
	phil->phil_has_fork[i] = i + 1;
	if (i == phil->count - 1)
	{
		pthread_mutex_lock(&phil->forks[0]);
		phil->phil_has_fork[0] = i + 1;
	}
	else
	{
		pthread_mutex_lock(&phil->forks[i + 1]);
		phil->phil_has_fork[i + 1] = i + 1;
	}
	// return (pthread_mutex_unlock(phil->take_forks), 1);
	return (1);
}

static int	leave_forks(const int i, t_phil_schedule *phil)
{
	pthread_mutex_unlock(&phil->forks[i]);
	phil->phil_has_fork[i] = 0;
	if (i == phil->count - 1)
	{
		pthread_mutex_unlock(&phil->forks[0]);
		phil->phil_has_fork[0] = 0;
	}
	else
	{
		pthread_mutex_unlock(&phil->forks[i + 1]);
		phil->phil_has_fork[i + 1] = 0;
	}
	return (0);
}

static void	get_order(t_phil_schedule *phil, const int index,
	pthread_mutex_t *get_order)
{
	int	i;

	i = 0;
	while (phil->eat_order[i])
		if (phil->eat_order[i++] == index + 1)
			return ;
	pthread_mutex_lock(get_order);
	while (phil->eat_order[i])
		if (phil->eat_order[i++] == index + 1)
			return ;
	if (i == 0
		|| ((phil->eat_order[i] > index + 2
				|| phil->eat_order[i] < index) && index != 0
			&& index != phil->count - 1)
		|| (index == 0 && phil->eat_order[i] != phil->count)
		|| (index == phil->count - 1 && phil->eat_order[i] != 1))
		phil->eat_order[i] = index + 1;
	pthread_mutex_unlock(get_order);
}

int	eat_meal(const int i, t_phil_schedule *phil)
{
	get_order(phil, i, phil->print);
	if (take_forks(i, phil, phil->eat_order))
	{
		print_state(phil, i, FORK);
		print_state(phil, i, FORK);
		print_state(phil, i, EAT);
		ft_usleep(phil->eat_time * 1000, phil, i);
		phil->ate[i] = IS_EATING;
		leave_forks(i, phil);
		return (1);
	}
	return (0);
}
