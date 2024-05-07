/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 12:03:32 by JFikents          #+#    #+#             */
/*   Updated: 2024/05/07 19:38:23 by JFikents         ###   ########.fr       */
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

int	ft_usleep(useconds_t time)
{
	useconds_t	start;

	start = get_time(NULL);
	if (time > 5000)
		usleep(time - 5000);
	while (get_time(NULL) - start < time / 1000)
		usleep(50);
	return (0);
}

int	print_state(t_phil_schedule *phil, int i, int state)
{
	int	time;

	pthread_mutex_lock(phil->print);
	time = get_time(phil);
	if (!phil->someone_died && state == THINK && phil->full_phil != phil->count)
		printf("%d %d is thinking\n", time, i + 1);
	if (!phil->someone_died && state == EAT && phil->full_phil != phil->count)
		printf("%d %d is eating\n", time, i + 1);
	if (!phil->someone_died && state == SLEEP && phil->full_phil != phil->count)
		printf("%d %d is sleeping\n", time, i + 1);
	if (!phil->someone_died && state == FORK && phil->full_phil != phil->count)
		printf("%d %d has taken a fork\n", time, i + 1);
	if (state == DEATH && phil->full_phil != phil->count)
		printf("%d %d died\n", time, i + 1);
	if (!phil->someone_died && state == FULL && phil->full_phil != phil->count)
		printf("%d %d is full\n", time, i + 1);
	if (!phil->someone_died && state == ALL_FULL)
		printf("%d All philosophers are full\n", time);
	pthread_mutex_unlock(phil->print);
	if (state == SLEEP)
		return (THINK);
	return (++state);
}
