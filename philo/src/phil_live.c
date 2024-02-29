/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phil_live.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 16:30:15 by JFikents          #+#    #+#             */
/*   Updated: 2024/02/29 09:16:03 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	take_nap(const int i, t_phil_schedule *phil)
{
	if (phil && phil->phil_has_fork[i] == i + 1)
		pthread_mutex_unlock(&phil->forks[i]);
	if (phil && phil->phil_has_fork[i] == i + 1)
		phil->phil_has_fork[i] = 0;
	if (phil && i == phil->count - 1)
	{
		if (phil && phil->phil_has_fork[0] == i + 1)
			pthread_mutex_unlock(&phil->forks[0]);
		if (phil && phil->phil_has_fork[0] == i + 1)
			phil->phil_has_fork[0] = 0;
	}
	else
	{
		if (phil && phil->phil_has_fork[i + 1] == i + 1)
			pthread_mutex_unlock(&phil->forks[i + 1]);
		if (phil && phil->phil_has_fork[i + 1] == i + 1)
			phil->phil_has_fork[i + 1] = 0;
	}
	print_state(phil, i, SLEEP);
	if (phil && !usleep(phil->sleep_time * 1000))
		return (SLEEP);
	return (0);
}

static void	end_phil(t_phil_schedule *phil, const int i)
{
	if (phil && phil->phil_has_fork[i] == i + 1)
	{
		pthread_mutex_unlock(&phil->forks[i]);
		phil->phil_has_fork[i] = 0;
	}
	if (phil && phil->count == 1)
		return ;
	if (phil && i == phil->count - 1 && phil->phil_has_fork[0] == i + 1)
	{
		pthread_mutex_unlock(&phil->forks[0]);
		phil->phil_has_fork[0] = 0;
	}
	else if (phil && i < phil->count - 1 && phil->phil_has_fork[i + 1] == i + 1)
	{
		pthread_mutex_unlock(&phil->forks[i + 1]);
		phil->phil_has_fork[i + 1] = 0;
	}
}

void	*odd_phil(void *arg)
{
	static int		times_ate = 0;
	static int		state = SLEEP;
	const int		i = ((t_phil_args *)arg)->i;
	t_phil_schedule	*phil;

	phil = ((t_phil_args *)arg)->phil;
	while (phil && !phil->someone_died)
	{
		if (state != EAT)
		{
			times_ate += eat_meal(i, phil, FALSE);
			state = EAT;
		}
		if (phil && times_ate >= phil->meal_count)
			return (eat_meal(i, phil, TRUE), end_phil(phil, i), NULL);
		if (state != SLEEP)
			state = take_nap(i, phil);
		if (state != THINK)
			state = print_state(phil, i, THINK);
	}
	return (eat_meal(i, phil, TRUE), end_phil(phil, i), NULL);
}

void	*even_phil(void *arg)
{
	static int		times_ate = 0;
	static int		state = THINK;
	const int		i = ((t_phil_args *)arg)->i;
	t_phil_schedule	*phil;

	phil = ((t_phil_args *)arg)->phil;
	while (phil && !phil->someone_died)
	{
		if (state != SLEEP)
			state = take_nap(i, phil);
		if (state != THINK)
			state = print_state(phil, i, THINK);
		if (state != EAT)
		{
			times_ate += eat_meal(i, phil, FALSE);
			state = EAT;
		}
		if (phil && times_ate >= phil->meal_count)
			return (end_phil(phil, i), NULL);
	}
	return (end_phil(phil, i), NULL);
}
