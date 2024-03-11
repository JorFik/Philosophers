/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:51:01 by JFikents          #+#    #+#             */
/*   Updated: 2024/03/11 18:50:07 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
	@note//_DESCRIPTION
	@brief #### Returns the error message.
	@brief Cheks which error code was given and returns the corresponding error
		message.
	@note//_PARAMETERS
	@param error The error code.
	@note//_RETURNS
	@return The error message, or "Error: Unknown error" if the error code is
		unknown.
 */
static char	*get_error(int error)
{
	if (error == ALLOC_PHIL)
		return ("Error: Failed to allocate memory for philosophers");
	if (error == FLAG_ATE)
		return ("Error: Failed to allocate memory for ate flags");
	if (error == ALLOC_FORKS)
		return ("Error: Failed to allocate memory for forks");
	if (error == FLAG_FORK)
		return ("Error: Failed to allocate memory for phil has fork");
	if (error == ALLOC_DEATH)
		return ("Error: Failed to allocate memory for death timer");
	if (error == GET_TIME)
		return ("Error: Failed to get current time");
	if (error == MORE_ARGS)
		return ("Error: Too many arguments,");
	if (error == LESS_ARGS)
		return ("Error: Too few arguments,");
	if (error == NO_PHIL)
		return ("Error: No philosophers, no simulation, no fun, no purpose.");
	if (error == CAN_EAT)
		return ("Error: Allocation failed for can eat flags");
	return ("Error: Unknown error");
}

/**
	@note//_DESCRIPTION
	@brief #### Returns the location of the error.
	@brief Cheks which location code was given and returns the corresponding
		location message.
	@note//_PARAMETERS
	@param where The location code.
	@note//_RETURNS
	@return The location message, or "At unknown location" if the location code
		is unknown.
 */
static char	*get_where(int where)
{
	if (where == INIT_PHIL)
		return ("at initize philos.");
	if (where == ARGS)
		return ("expected 4 to 5 arguments.");
	if (where == SIMULATION)
		return ("you beat the system, are you happy?, I feel empty.");
	if (where == TIME_STAMP)
		return ("for the time stamp.");
	if (where == DEATH_T)
		return ("at init death timer.");
	if (where == START_SIM)
		return ("before starting simulation.");
	if (where == STOPWATCH)
		return ("when trying to use stop watch.");
	return ("at unknown location.");
}

/**
	@note//_DESCRIPTION
	@brief #### Capitalizes the first letter of a string.
	@brief Changes the first letter of the string to uppercase.
	@note//_PARAMETERS
	@param str The string to be capitalized.
	@note//_RETURNS
	@return The capitalized string.
 */
static char	*capitalize(char *str)
{
	str[0] -= 32;
	return (str);
}

int	error(int check[3], void *if_null)
{
	extern int	errno;

	if (check[STATE] == -1 || (check[STATE] == IF_NULL && if_null == NULL))
	{
		if (check[ERROR] != -1)
			printf("%s %s\n", get_error(check[ERROR]), get_where(check[WHERE]));
		else
			printf("%s\n", capitalize(get_where(check[WHERE])));
		if (check[ERROR] != -1)
			errno = check[ERROR];
		return (errno);
	}
	return (0);
}
