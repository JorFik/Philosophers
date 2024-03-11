/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phil_live.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 16:30:15 by JFikents          #+#    #+#             */
/*   Updated: 2024/03/11 17:50:51 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	end_phil(t_phil_schedule *phil, const int i)
{
	if (phil && phil->phil_has_fork[i] == i + 1)
	{
		pthread_mutex_unlock(&phil->forks[i]);
		phil->phil_has_fork[i] = 0;
	}
	if (phil && i == phil->count - 1 && phil->phil_has_fork[0] == i + 1)
	{
		pthread_mutex_unlock(&phil->forks[0]);
		phil->phil_has_fork[0] = 0;
	}
	else
	{
		pthread_mutex_unlock(&phil->forks[i + 1]);
		phil->phil_has_fork[i + 1] = 0;
	}
}

void	*phil_live(void *arg)
{
	t_phil_schedule	*phil;
	const int		i = ((t_phil_args *)arg)->i;
	int				state;

	phil = ((t_phil_args *)arg)->phil;
	state = EAT;
	if (i % 2)
		ft_usleep(50 + (phil->eat_time * 1000), phil, i);
	while (phil && !phil->someone_died)
	{
		if (state == EAT)
			state += eat_meal(i, phil);
		if (state == SLEEP)
		{
			state = print_state(phil, i, SLEEP);
			ft_usleep(phil->sleep_time * 1000, phil, i);
		}
		if (state == THINK)
		{
			state = print_state(phil, i, THINK);
			if (phil->count % 2)
				ft_usleep(phil->eat_time * 900, phil, i);
		}
	}
	return (end_phil(phil, i), NULL);
}
