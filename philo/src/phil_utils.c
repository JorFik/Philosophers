/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phil_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 02:05:22 by JFikents          #+#    #+#             */
/*   Updated: 2024/03/05 19:50:21 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	wait_to_eat(const int index, t_phil_schedule *phil, int *who_ate)
{
	int	i;
	int	needs_to_eat;

	i = *who_ate;
	needs_to_eat = 0;
	while (phil->can_eat[i] && i < phil->count)
		if (phil->can_eat[i++] == index + 1)
			needs_to_eat = 1;
	while (needs_to_eat && phil->can_eat[*who_ate] != index + 1
		&& !phil->someone_died)
		usleep(1);
	return (needs_to_eat);
}

static int	take_forks(const int i, t_phil_schedule *phil, int *can_eat)
{
	static int	who_ate = 0;
	int			needs_to_eat;
	int			index;

	if (who_ate >= phil->count)
		who_ate = 0;
	index = who_ate;
	needs_to_eat = wait_to_eat(i, phil, &who_ate);
	if ((can_eat[who_ate++] != i + 1 && !needs_to_eat) || phil->count == 1)
		return (0);
	pthread_mutex_lock(phil->take_forks);
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
	pthread_mutex_unlock(phil->take_forks);
	return (1);
}

static int	leave_forks(const int i, t_phil_schedule *phil)
{
	printf("%d %d left its own fork\n", get_time(phil), i + 1);
	pthread_mutex_unlock(&phil->forks[i]);
	phil->phil_has_fork[i] = 0;
	if (i == phil->count - 1)
	{
		printf("%d %d left the first fork\n", get_time(phil), i + 1);
		pthread_mutex_unlock(&phil->forks[0]);
		phil->phil_has_fork[0] = 0;
	}
	else
	{
		printf("%d %d left the right fork\n", get_time(phil), i + 1);
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
	while (phil->can_eat[i])
		if (phil->can_eat[i++] == index + 1)
			return ;
	pthread_mutex_lock(get_order);
	while (phil->can_eat[i])
		if (phil->can_eat[i++] == index + 1)
			return ;
	if (i == 0
		|| ((phil->can_eat[i] > index + 2
				|| phil->can_eat[i] < index) && index != 0
			&& index != phil->count - 1)
		|| (index == 0 && phil->can_eat[i] != phil->count)
		|| (index == phil->count - 1 && phil->can_eat[i] != 1))
		phil->can_eat[i] = index + 1;
	pthread_mutex_unlock(get_order);
}

int	eat_meal(const int i, t_phil_schedule *phil)
{
	get_order(phil, i, phil->print);
	if (take_forks(i, phil, phil->can_eat))
	{
		print_state(phil, i, FORK);
		print_state(phil, i, FORK);
		print_state(phil, i, EAT);
		if (phil->ate[i] == 0)
			phil->ate[i] = 1;
		usleep(phil->eat_time * 1000);
		leave_forks(i, phil);
		return (1);
	}
	return (0);
}
