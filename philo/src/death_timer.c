/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_timer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 18:17:09 by JFikents          #+#    #+#             */
/*   Updated: 2024/03/05 20:32:32 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	init_death_timer(t_death_timer *utils,
	t_phil_schedule **phil, void *arg)
{
	int				i;
	t_phil_schedule	*philo;

	i = -1;
	*phil = (t_phil_schedule *)arg;
	philo = *phil;
	memset(utils, 0, sizeof(t_death_timer));
	utils->last_meal = ft_calloc(philo->count, sizeof(useconds_t));
	if (error((int [3]){IF_NULL, ALLOC_DEATH, DEATH_T}, philo->death_timer))
		return (1);
	while (++i < philo->count)
		utils->last_meal[i] = (useconds_t) get_time(NULL);
	utils->times_ate = ft_calloc(philo->count, sizeof(int));
	if (error((int [3]){IF_NULL, ALLOC_DEATH, DEATH_T}, utils->times_ate))
		return (1);
	return (0);
}

static void	finish_death_timer(t_death_timer *utils, t_phil_schedule *phil)
{
	if (utils->last_meal)
		free(utils->last_meal);
	if (utils->times_ate)
		free(utils->times_ate);
	memset(utils, 0, sizeof(t_death_timer));
	phil->someone_died = 1;
}

static int	phil_died(t_phil_schedule *phil, const int i, useconds_t now,
	t_death_timer *utils)
{
	if (phil && now - utils->last_meal[i] >= phil->die_time)
	{
		phil->someone_died++;
		print_state(phil, i, DEATH);
		return (1);
	}
	return (0);
}

static void	phil_had_a_meal(t_phil_schedule *phil, const int i,
	t_death_timer *utils, useconds_t now)
{
	if (phil && phil->ate[i] == 1)
	{
		phil->ate[i] = 0;
		utils->last_meal[i] = now;
		if (++utils->times_ate[i] == phil->meal_count)
		{
			phil->full_phil++;
		}
	}
}

void	*death_timer(void	*arg)
{
	t_phil_schedule	*phil;
	t_death_timer	utils;
	useconds_t		now;
	int				i;

	if (init_death_timer(&utils, &phil, arg))
		return (finish_death_timer(&utils, phil), NULL);
	i = 0;
	while (phil->full_phil != phil->meal_count)
	{
		now = get_time(NULL);
		phil_had_a_meal(phil, i, &utils, now);
		if (phil->full_phil == phil->count)
			phil->someone_died++;
		if (phil_died(phil, i, now, &utils))
			return (finish_death_timer(&utils, phil), NULL);
		if (++i == phil->count)
			i = 0;
	}
	return (finish_death_timer(&utils, phil), NULL);
}
