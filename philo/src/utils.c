/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 12:03:32 by JFikents          #+#    #+#             */
/*   Updated: 2024/03/11 17:51:16 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

useconds_t	get_time(t_phil_schedule *phil)
{
	static useconds_t		now;
	struct timeval			time;
	useconds_t				time_stamp;
	int						check;

	check = gettimeofday(&time, NULL);
	if (error((int [3]){check, GET_TIME, TIME_STAMP}, NULL))
		return (-1);
	now = (time.tv_usec / 1000) + (time.tv_sec * 1000);
	if (phil)
		time_stamp = now - phil->start_time;
	else
		time_stamp = now;
	return (time_stamp);
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = malloc(count * size);
	if (!ptr)
		return (NULL);
	memset(ptr, 0, count * size);
	return (ptr);
}

int	ft_atoi(char *str)
{
	int	new_n_shiny_usable_int;
	int	sign;
	int	i;

	i = 0;
	new_n_shiny_usable_int = 0;
	sign = 1;
	while ((*str == ' ' || (*str >= 9 && *str <= 13)) && *str)
		str++;
	if ((*str == '-' || *str == '+') && *str)
		if (*str++ == '-')
			sign = -1;
	while (str[i] && str[i] >= 48 && str[i] <= 57)
	{
		new_n_shiny_usable_int *= 10;
		new_n_shiny_usable_int += (int)str[i++] - 48;
	}
	return (new_n_shiny_usable_int * sign);
}

int	ft_usleep(useconds_t time, t_phil_schedule *phil, const int index)
{
	useconds_t	start;
	long long	adjustment;

	start = 0;
	adjustment = get_time(phil);
	adjustment = 50 - (adjustment % 50);
	ft_stop_watch(phil, index, STOP);
	adjustment = adjustment * 1000;
	ft_stop_watch(phil, index, START);
	adjustment += time - 50000;
	usleep(adjustment);
	if (phil->someone_died)
		return (1);
	return (0);
}

int	ft_stop_watch(t_phil_schedule *phil, const int i, int restart)
{
	struct timeval	time;

	if (restart == START)
	{
		gettimeofday(&time, NULL);
		phil->stop_watch[i] = (time.tv_usec) + (time.tv_sec * 1000000);
		return (1);
	}
	else
	{
		gettimeofday(&time, NULL);
		phil->stop_watch[i] -= (time.tv_usec) + (time.tv_sec * 1000000);
	}
	return (0);
}
