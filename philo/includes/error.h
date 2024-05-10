/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:55:08 by JFikents          #+#    #+#             */
/*   Updated: 2024/05/10 16:40:07 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

// ** ------------------------- MACROS AND ENUMS ------------------------- ** //

enum e_error
{
	ALLOC_PHIL,
	FLAG_ATE,
	ALLOC_FORKS,
	FLAG_FORK,
	ALLOC_DEATH,
	GET_TIME,
	MORE_ARGS,
	LESS_ARGS,
	NO_PHIL,
	CAN_EAT,
	NO_MEALS,
	INVALID_INPUT,
};

enum e_where
{
	INIT_PHIL,
	ARGS,
	SIMULATION,
	TIME_STAMP,
	DEATH_T,
	START_SIM,
	STOPWATCH,
	EXPECTED
};

enum e_check
{
	STATE,
	ERROR,
	WHERE,
	IF_NULL = -50102
};

//_--------------------------------------------------------------------------_//

#endif /* error.h */
