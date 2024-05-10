/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_timer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 18:17:09 by JFikents          #+#    #+#             */
/*   Updated: 2024/05/10 14:58:25 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
	@note//_DESCRIPTION
	@brief #### Initializes the `t_death_timer` structure.
	@brief Allocates memory for the `last_meal` and `times_ate` members of the
		`utils` structure, and initializes the members with the values from the
		`phil` structure.
	@note//_PARAMETERS
	@param utils The `t_death_timer` structure to be initialized.
	@param phil The philosophers structure containing all the resources.
	@note//_RETURNS
	@return 0 if no error, otherwise 1.
 */
static int	init_death_timer(t_death_timer *utils,
	t_phil_schedule *phil)
{
	int				i;

	i = -1;
	memset(utils, 0, sizeof(t_death_timer));
	utils->last_meal = ft_calloc(phil->count, sizeof(useconds_t));
	utils->times_ate = ft_calloc(phil->count, sizeof(int));
	if (error((int [3]){IF_NULL, ALLOC_DEATH, DEATH_T}, utils->last_meal)
		|| error((int [3]){IF_NULL, ALLOC_DEATH, DEATH_T}, utils->times_ate))
		return (1);
	while (++i < phil->count)
	{
		utils->last_meal[i] = (useconds_t) get_time(NULL);
	}
	return (0);
}

/**
	@note//_DESCRIPTION
	@brief #### Frees the resources from the `t_death_timer` structure.
	@brief Frees the memory allocated for the `last_meal` and `times_ate` members
		of the `utils` structure, and sets all to 0 and sets
		`phil->someone_died` to 1.
	@note//_PARAMETERS
	@param utils The `t_death_timer` structure to be freed.
	@param phil The philosophers structure containing the `someone_died` member.
	@note//_NOTES
	@note The `phil->someone_died` is set to 1 to indicate that a philosopher has
		died, that way the threads can be stopped.
 */
static void	finish_death_timer(t_death_timer *utils, t_phil_schedule *phil)
{
	if (utils->last_meal)
		free(utils->last_meal);
	if (utils->times_ate)
		free(utils->times_ate);
	memset(utils, 0, sizeof(t_death_timer));
	phil->someone_died = 1;
}

/**
	@note//_DESCRIPTION
	@brief #### Checks if a philosopher has died.
	@brief Checks if a philosopher has died by comparing the time of the last meal
		with the current time.
	@note//_PARAMETERS
	@param phil The philosophers structure containing all the resources.
	@param i The index of the philosopher.
	@param now The current time.
	@param utils The `t_death_timer` structure containing the `last_meal` value.
	@note//_RETURNS
	@return 1 if the philosopher has died, otherwise 0.
	@note//_NOTES
	@note Locks the fork of the philosopher that has died so that the philosopher
		can't take the fork and eat while dead.
 */
static int	phil_died(t_phil_schedule *phil, const int i, useconds_t now,
	t_death_timer *utils)
{
	if (phil->ate[i] == IS_EATING)
		return (0);
	if (phil && now - utils->last_meal[i] >= phil->die_time)
	{
		phil->someone_died++;
		print_state(phil, i, DEATH);
		return (1);
	}
	return (0);
}

/**
	@note//_DESCRIPTION
	@brief #### Checks if a philosopher has had a meal.
	@brief Checks if a philosopher has had a meal by checking the `ate` member of
		the `phil` structure, and if the philosopher has had a meal, adds 1 to
		`times_ate` and sets the `last_meal` to the current time. It also checks
		if the philosopher has eaten the required amount of meals and if true,
		adds 1 to `full_phil`.
	@note//_PARAMETERS
	@param phil The philosophers structure containing all the resources.
	@param i The index of the philosopher.
	@param utils The `t_death_timer` structure containing the `last_meal` and
		`times_ate` values.
	@param now The current time.
 */
static void	phil_had_a_meal(t_phil_schedule *phil, const int i,
	t_death_timer *utils, useconds_t now)
{
	if (phil && phil->ate[i] == IS_EATING)
	{
		phil->ate[i] = HUNGRY;
		utils->last_meal[i] = now;
		if (++utils->times_ate[i] >= phil->meal_count)
		{
			phil->full_phil++;
		}
	}
}

void	*death_timer(t_phil_schedule	*phil)
{
	t_death_timer	utils;
	useconds_t		now;
	int				i;

	if (init_death_timer(&utils, phil))
		return (finish_death_timer(&utils, phil), NULL);
	i = 0;
	while (phil->full_phil != phil->count)
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
