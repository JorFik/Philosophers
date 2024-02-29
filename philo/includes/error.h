/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:55:08 by JFikents          #+#    #+#             */
/*   Updated: 2024/02/29 04:48:39 by JFikents         ###   ########.fr       */
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
};

enum e_where
{
	INIT_PHIL,
	ARGS,
	SIMULATION,
	TIME_STAMP,
	ODD,
	EVEN,
	EATING,
	DEATH_T,
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
