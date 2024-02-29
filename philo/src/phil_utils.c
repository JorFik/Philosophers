/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phil_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 02:05:22 by JFikents          #+#    #+#             */
/*   Updated: 2024/02/29 09:27:22 by JFikents         ###   ########.fr       */
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
	return (STATE);
}

static void	restart_can_eat(t_phil_schedule *phil, int *can_eat)
{
	int	i;
	int	everyone_ate;

	i = -1;
	everyone_ate = 1;
	while (++i < phil->count)
		if (can_eat[i])
			everyone_ate = 0;
	i = 0;
	while (i < phil->count && everyone_ate)
		can_eat[i++] = 1;
}

static int	take_forks(const int i, t_phil_schedule *phil, int *can_eat)
{
	if (!can_eat[i] || phil->count == 1)
		return (0);
	pthread_mutex_lock(phil->take_forks);
	pthread_mutex_lock(&phil->forks[i]);
	phil->phil_has_fork[i] = i + 1;
	if (i == phil->count - 1 && phil->phil_has_fork[0] != i + 1)
	{
		pthread_mutex_lock(&phil->forks[0]);
		phil->phil_has_fork[0] = i + 1;
	}
	else if (i != phil->count - 1 && phil->phil_has_fork[i + 1] != i + 1)
	{
		pthread_mutex_lock(&phil->forks[i + 1]);
		phil->phil_has_fork[i + 1] = i + 1;
	}
	can_eat[i] = 0;
	pthread_mutex_unlock(phil->take_forks);
	return (1);
}

int	eat_meal(const int i, t_phil_schedule *phil)
{
	restart_can_eat(phil, phil->can_eat);
	if (phil->can_eat && take_forks(i, phil, phil->can_eat))
	{
		print_state(phil, i, FORK);
		print_state(phil, i, FORK);
		print_state(phil, i, EAT);
		if (phil->ate[i] == 0)
			phil->ate[i] = 1;
		usleep(phil->eat_time * 1000);
		return (1);
	}
	return (0);
}
