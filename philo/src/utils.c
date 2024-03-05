/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 12:03:32 by JFikents          #+#    #+#             */
/*   Updated: 2024/03/05 20:08:47 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	finish_simulation(t_phil_schedule *phil, int *original_can_eat)
{
	extern int	errno;
	int			i;

	i = -1;
	while (++i < phil->count)
		pthread_mutex_destroy(&phil->forks[i]);
	pthread_mutex_destroy(phil->print);
	pthread_mutex_destroy(phil->take_forks);
	if (phil->philosopher)
		free(phil->philosopher);
	if (phil->ate)
		free(phil->ate);
	if (phil->forks)
		free(phil->forks);
	if (phil->phil_has_fork)
		free(phil->phil_has_fork);
	if (original_can_eat)
		free(original_can_eat);
	memset(phil, 0, sizeof(t_phil_schedule));
	printf("\t\t\t\tSimulation finished\n");
	if (errno)
		return (errno);
	return (0);
}

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
