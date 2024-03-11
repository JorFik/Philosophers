/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phil_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 02:05:22 by JFikents          #+#    #+#             */
/*   Updated: 2024/03/11 18:51:54 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
	@note//_DESCRIPTION
	@brief #### Takes the forks.
	@brief Locks the forks and sets the `phil_has_fork` corresponding to those
		forks to the philosopher's index.
	@note//_PARAMETERS
	@param phil The philosophers structure containing all the resources.
	@param i The index of the philosopher.
	@note//_RETURNS
	@return 1 if the forks were taken, otherwise 0.
	@note//_NOTES
	@note Sets the `ate[i]` to `IS_EATING` as soon as the forks are taken,
		indicating that the philosopher has started to eat needed by the
		`death_timer` function.
 */
static int	take_forks(const int i, t_phil_schedule *phil)
{
	if (phil->count == 1)
		return (0);
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
	phil->ate[i] = IS_EATING;
	return (1);
}

/**
	@note//_DESCRIPTION
	@brief #### Leaves the forks.
	@brief Unlocks the forks and sets the `phil_has_fork` corresponding to those
		forks to 0.
	@note//_PARAMETERS
	@param i The index of the philosopher.
	@param phil The philosophers structure containing all the resources.
	@note//_RETURNS
	@return 0.
	@note//_WARNINGS
	@warning if the function is miscalled (the philosopher does not have the
		forks), this will lead to undefined behavior.
 */
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
		ft_usleep(phil->eat_time * 1000, phil);
		leave_forks(i, phil);
		return (1);
	}
	return (0);
}
