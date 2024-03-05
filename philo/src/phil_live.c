/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phil_live.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 16:30:15 by JFikents          #+#    #+#             */
/*   Updated: 2024/03/05 19:57:57 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	print_state(t_phil_schedule *phil, int i, int state)
{
	int	time;

	time = get_time(phil);
	pthread_mutex_lock(phil->print);
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

static void	end_phil(t_phil_schedule *phil, const int i)
{
	printf("%d Philosopher %d has finished\n", get_time(phil), i + 1);
	if (phil && phil->phil_has_fork[i] == i + 1)
	{
		printf("%d left its own fork\n", i + 1);
		pthread_mutex_unlock(&phil->forks[i]);
		phil->phil_has_fork[i] = 0;
	}
	if (phil && i == phil->count - 1 && phil->phil_has_fork[0] == i + 1)
	{
		printf("%d left the first fork\n", i + 1);
		pthread_mutex_unlock(&phil->forks[0]);
		phil->phil_has_fork[0] = 0;
	}
	else
	{
		printf("%d left the right fork\n", i + 1);
		pthread_mutex_unlock(&phil->forks[i + 1]);
		phil->phil_has_fork[i + 1] = 0;
	}
}

void	*phil_live(void *arg)
{
	t_phil_schedule	*phil;
	const int		i = ((t_phil_args *)arg)->i;
	int				state;

	state = EAT;
	phil = ((t_phil_args *)arg)->phil;
	if (i % 2)
		usleep(50 + (phil->eat_time * 1000));
	while (phil && !phil->someone_died)
	{
		if (state == EAT)
			state += eat_meal(i, phil);
		if (state == SLEEP && !usleep(phil->sleep_time * 1000))
			state = print_state(phil, i, SLEEP);
		if (state == THINK && !usleep(phil->eat_time * 1000))
			state = print_state(phil, i, THINK);
	}
	return (end_phil(phil, i), NULL);
}
