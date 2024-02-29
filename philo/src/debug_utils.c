/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 20:49:22 by JFikents          #+#    #+#             */
/*   Updated: 2024/02/24 22:15:58 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_philosophers(t_phil_schedule *phil)
{
	printf("Philosopher count:	%d\n", phil->count);
	printf("Die delay:		%d\n", phil->die_time);
	printf("Eat Time:		%d\n", phil->eat_time);
	printf("Sleep Time:		%d\n", phil->sleep_time);
	printf("Meal count:		%d\n", phil->meal_count);
	printf("forks:			%p\n", phil->forks);
	printf("start_time:			%u\n", phil->start_time);
}
