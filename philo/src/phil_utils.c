/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phil_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 02:05:22 by JFikents          #+#    #+#             */
/*   Updated: 2024/03/10 16:40:01 by JFikents         ###   ########.fr       */
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

int	print_state(t_phil_schedule *phil, int i, int state)
{
	int	time;

	pthread_mutex_lock(phil->print);
	time = get_time(phil);
	if (!phil->someone_died && state == THINK && phil->full_phil != phil->count)
		printf("%d %d is thinking\n", time, i + 1);
	if (!phil->someone_died && state == EAT && phil->full_phil != phil->count)
		printf("%d %d is eating\n", time, i + 1);
	if (!phil->someone_died && state == SLEEP && phil->full_phil != phil->count)
		printf("%d %d is sleeping\n", time, i + 1);
	if (!phil->someone_died && state == FORK && phil->full_phil != phil->count)
		printf("%d %d has taken a fork\n", time, i + 1);
	if (state == DEATH && phil->full_phil != phil->count)
		printf("%d %d died\n", time, i + 1);
	if (!phil->someone_died && state == FULL && phil->full_phil != phil->count)
		printf("%d %d is full\n", time, i + 1);
	if (!phil->someone_died && state == ALL_FULL)
		printf("%d All philosophers are full\n", time);
	pthread_mutex_unlock(phil->print);
	if (state == SLEEP)
		return (THINK);
	return (++state);
}

static int	take_forks(const int i, t_phil_schedule *phil)
{
	if (phil->count == 1)
		return (0);
	pthread_mutex_lock(&phil->forks[i]);
	phil->ate[i] = IS_EATING;
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

int	eat_meal(const int i, t_phil_schedule *phil)
{
	if (take_forks(i, phil))
	{
		print_state(phil, i, FORK);
		print_state(phil, i, FORK);
		print_state(phil, i, EAT);
		ft_usleep(phil->eat_time * 1000, phil, i);
		leave_forks(i, phil);
		return (1);
	}
	return (0);
}
