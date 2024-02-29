/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:51:01 by JFikents          #+#    #+#             */
/*   Updated: 2024/02/29 10:01:25 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
		return ("Error: No philosophers, no simulation, no fun, no porpuse.");
	if (error == CAN_EAT)
		return ("Error: Allocation failed for can eat flags");
	return ("Error: Unknown error");
}

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
	if (where == ODD)
		return ("for an odd philosopher.");
	if (where == EVEN)
		return ("for an even philosopher.");
	if (where == DEATH_T)
		return ("at init death timer.");
	return ("at unknown location.");
}

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
