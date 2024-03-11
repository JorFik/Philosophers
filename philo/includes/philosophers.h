/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 11:58:31 by JFikents          #+#    #+#             */
/*   Updated: 2024/03/11 19:10:53 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

// ** ---------------------------- LIBRARIES ---------------------------- ** //

# include <stdlib.h> // malloc, free
# include <string.h> // memset
# include <stdio.h> // printf
# include <unistd.h> // usleep, write
# include <sys/time.h> // gettimeofday
# include <pthread.h> // pthread_* functions
# include "error.h" // error functions and messages

//_--------------------------------------------------------------------------_//

// ** ------------------------- MACROS AND ENUMS ------------------------- ** //

enum e_printed_stamps
{
	DEATH,
	FORK,
	THINK,
	EAT,
	SLEEP,
	FULL,
	ALL_FULL,
};

enum e_eating
{
	HUNGRY = 0,
	IS_EATING,
};

//_--------------------------------------------------------------------------_//

// ** ------------------------- DATA STRUCTURES ------------------------- ** //

/**
	@note//_DESCRIPTION
	@brief #### The main structure for the philosophers.
	@brief The main structure for the philosophers, containing all the resources
		needed for the simulation and some extra information to facilitate the
		communication between the threads and the debug processes.
	@note//_PARAMETERS
	@param count The number of philosophers.
	@param meal_count The number of meals each philosopher has to eat.
	@param full_phil The number of philosophers that have eaten the required
		number of meals.
	@param someone_died A flag to indicate if a philosopher has died.
	@param phil_has_fork An array to keep track of which philosopher has which
		fork.
	@param ate An array to keep track of which philosopher is eating.
	@param philosopher An array of threads for the philosophers.
	@param sleep_time The time the philosopher has to sleep.
	@param eat_time The time the philosopher has to eat.
	@param die_time The time the philosopher has to eat before dying.
	@param start_time The time the simulation started.
	@param forks An array of mutexes for the forks.
	@param print A mutex to make sure that the output is not interrupted by other
		threads.
	@note//_NOTES
	@note `meal_count` is set to -1 if the philosophers don't have to eat a specific
		number of meals.
 */
typedef struct s_phil_schedule
{
	int				count;
	int				meal_count;
	int				full_phil;
	int				someone_died;
	int				*phil_has_fork;
	int				*ate;
	pthread_t		*philosopher;
	useconds_t		sleep_time;
	useconds_t		eat_time;
	useconds_t		die_time;
	useconds_t		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print[1];
}				t_phil_schedule;

/**
	@note//_DESCRIPTION
	@brief #### The structure containing all information for the philosopher
		threads.
	@brief The structure containing the index of the philosopher and the
		philosophers structure.
	@note//_PARAMETERS
	@param i The index of the philosopher.
	@param phil The philosophers structure.
 */
typedef struct s_phil_args
{
	int				i;
	t_phil_schedule	*phil;
}				t_phil_args;

/**
	@note//_DESCRIPTION
	@brief #### The structure for the death timer.
	@brief A structure containing all the tools necessary to keep track of the
		time and check if a philosopher has died or if the simulation has to
		stop.
	@note//_PARAMETERS
	@param times_ate An array to keep track of how many times each philosopher
		has eaten.
	@param last_meal An array to keep track of the last time each philosopher
		ate.
 */
typedef struct s_death_timer
{
	int				*times_ate;
	useconds_t		*last_meal;
}				t_death_timer;

//_--------------------------------------------------------------------------_//

// ** ---------------------------- FUNCTIONS ---------------------------- ** //

/**
	@note//_DESCRIPTION
	@brief #### Usleep but with adjustments.
	@brief Same as `usleep` but it adjust the waiting time to not have a
		extra time to wait, and try to keep the time as accurate as possible.
	@note//_PARAMETERS
	@param time Time to sleep in microseconds.
	@param phil The philosophers structure.
	@note//_RETURNS
	@return 1 if it has stopped, otherwise 0.
	@note//_NOTES
	@note It adjust the time to be a multiple of 50000 microseconds or 50
		milliseconds.
 */
int			ft_usleep(useconds_t time, t_phil_schedule *phil);

/**
	@note//_DESCRIPTION
	@brief #### Takes a meal.
	@brief Takes both forks and eats a meal.
	@note//_PARAMETERS
	@param i The index of the philosopher.
	@param phil The philosophers structure.
	@note//_NOTES
	@note It communicates through `phil_ate` and `phil_has_fork`.
	@note It uses the `forks` mutex to make sure that the forks are not taken by
		another philosopher.
	@note It uses the `phil_has_fork` array to keep track of which philosopher
		has which fork.
	@note It sets `ate` to `IS_EATING` as soon as the philosopher has taken the
		both fork.
	@note//_RETURN
	@return 1 if the philosopher has eaten, otherwise 0.
 */
int			eat_meal(const int i, t_phil_schedule *phil);

/**
	@note//_DESCRIPTION
	@brief #### Print the state of a philosopher.
	@brief Prints the time stamp, the philosopher's index and the state of the
		philosopher and makes sure that the output is not interrupted by other
		threads.
	@note//_PARAMETERS
	@param phil The philosophers structure.
	@param i The index of the philosopher.
	@param state The state of the philosopher.
	@note//_NOTES
	@note It uses the `print` mutex to make sure that the output is not
		interrupted by other threads.
	@note For more information, see `enum e_printed_stamps` and the
		function itself to see all printable states.
	@note//_RETURN
	@return The state of the philosopher.
 */
int			print_state(t_phil_schedule *phil, int i, int state);

/**
	@note//_DESCRIPTION
	@brief #### Creates the time stamp.
	@brief Takes the current time, substracts the start time and returns the
		result in milliseconds.
	@note//_PARAMETERS
	@param phil The philosophers structure containing the start time.
	@note//_RETURN
	@return The time stamp in milliseconds.
	@note//_NOTES
	@note If `phil` is `NULL`, it will return the current time in milliseconds.
 */
useconds_t	get_time(t_phil_schedule *phil);

/**
	@note//_DESCRIPTION
	@brief #### Keep track of the time and tells the program when to stop.
	@brief Has a timer for each philosopher and checks if a philosopher has died
		or if the philosophers had enough meals, if so, it will tell the program
		to stop
	@note//_PARAMETERS
	@param phil The philosophers structure containing all the resources.
	@note//_NOTES
	@note If a philosopher hasn't eaten for `phil->die_time` milliseconds, he
		will die.
	@note It restarts the timer if the philosopher has eaten.
	@note If a philosopher dies, it will print the time stamp and who died, then
		it will tell the program to stop.
	@note It communicates with the other threads through `phil_ate` and
		`someone_died`.
 */
void		*death_timer(t_phil_schedule *phil);

/**
	@note//_DESCRIPTION
	@brief #### Start function for philosophers.
	@brief This function simulates the life of a philosopher, it makes the
		philosopher think, eat and sleep.
	@note//_PARAMETERS
	@param arg The structure containing the philosopher's index and the
		philosophers structure.
	@note//_NOTES
	@note It communicates with `death_timer` and other `philosophers` through
		`phil_ate`, `phil_has_fork` and `someone_died`.
 */
void		*phil_live(void *arg);

/**
	@note//_DESCRIPTION
	@brief #### Print an error message and return an error number.
	@brief Prints an error message, sets `errno` to `check[ERROR]` and returns
		the error number if an error occurs, otherwise it does nothing and
		returns 0.
	@note//_PARAMETERS
	@param check[3] The error check array, containing the state, error number
		and the location of the error, in that order.
	@note//_NOTES
	@note It does nothing if `check[STATE]` is not `-1` or `IF_NULL`, if it is 
		`IF_NULL` then `if_null` should be `NULL` to print the error message.
	@note Set `check[ERROR]` to `-1` if you don't want to overwrite `errno`.
	@note for more information see the errors.c file and the error.h file.
	@note//_RETURN
	@return 0 if no error, otherwise the error number.
 */
int			error(int check[3], void *if_null);

/**
	@note//_DESCRIPTION
	@brief #### Allocate memory and sets it to zero.
	@brief Allocates memory for an array of count elements of size bytes each
	and sets the memory to zero.
	@note//_PARAMETERS
	@param count The number of elements to allocate.
	@param size The size of each element in bytes.
	@note//_RETURNS
	@return A pointer to the allocated memory.
	@note//_WARNINGS
	@warning Undefined behavior if count or size is 0.
	@warning Undefined behavior if the allocation fails.
 */
void		*ft_calloc(size_t count, size_t size);

/**
	@note//_DESCRIPTION
	@brief #### Print the philosophers structure.
	@brief Prints all the fields of the philosophers structure.
	@note//_PARAMETERS
	@param phil The philosophers structure to be printed.
	@note//_NOTES
	@note This Function is used for debugging purposes and it is not part of the
		of the final program.
 */
void		print_philosophers(t_phil_schedule *phil);

/**
	@note//_DESCRIPTION
	@brief Converts a string to an integer.
	This function takes a string as input and converts it to an integer.
	It skips any leading white spaces and stops converting when it encounters
	a non-digit character. The converted integer is returned.
	@note//_PARAMETERS
	@param str The string to be converted.
	@note//_RETURNS
	@return The converted integer value.
	@note//_NOTES
	@note The function handles only one sign, either + or -.
	@note The function handles multiple blank characters (ASCII 32, 9-13).
	@note//_WARNINGS
	@warning Undefined behavior if str is NULL.
 */
int			ft_atoi(char *str);

//_--------------------------------------------------------------------------_//

//? * --------------------- ALLOWED EXTERN FUNCTIONS --------------------- * ?//

/*									NOTE
!	The following functions are from the libraries included, the doocumentation
! here is only for reference. For exact information about the functions, see the
! library's documentation. 
*/

/**
	@note//_DESCRIPTION
	@brief #### Set a number of bytes to a specific value.
	@brief Sets the first len bytes of the block of memory pointed by b to the
	specified value (interpreted as an unsigned char).
	@note//_PARAMETERS
	@param b The pointer to the block of memory to fill.
	@param c The value to be set.
	@param len The number of bytes to be set to the value.
	@note//_NOTES
	@note `c` is converted to an unsigned char before being written to the memory.
	@note//_RETURN
	@return The value of `b`.
//?		void	*memset(void *b, int c, size_t len);
 */

int			printf(const char *format, ...);

void		*malloc(size_t size);

void		free(void *ptr);

ssize_t		write(int fd, const void *buf, size_t count);

/**
	@note//_DESCRIPTION
	@brief #### Delay execution for a number of microseconds.
	@brief Delays the execution of the program for the number of microseconds or
		until a signal is delivered to the calling process.
	@note//_PARAMETERS
	@param microseconds The number of microseconds to delay the execution.
	@note//_NOTES
	@note errno is set by this function if an error occurs.
	@note//_RETURN
	@return 0 on success, -1 on error.
	@note//_WARNING
	@warning may not work for values of `microseconds` greater than 1,000,000.
 */
int			usleep(useconds_t microseconds);

/**
	@note//_DESCRIPTION
	@brief #### Get the current time.
	@brief Stores the current time in the timeval structure pointed to by tv
		or/and the current timezone in the timezone structure pointed to by tz.
	@note//_PARAMETERS
	@param tv A pointer to a timeval structure where the current time will be
		stored.
	@param tz A pointer to a timezone structure where the current timezone will
		be stored.
	@note//_NOTES
	@note if `tz` is not NULL, the current timezone will be stored in the
		structure pointed to by `tz`.
	@note if `tv` is not NULL, the current time will be stored in the structure
		pointed to by `tv`.
	@note//_RETURN
	@return 0 on success, -1 on error.
//?		int		gettimeofday(struct timeval *tv, struct timezone *tz);
 */

/**
	@note//_DESCRIPTION
	@brief #### Create a new thread.
	@brief Creates a new thread, with attributes specified by `attr`, and places
		the thread ID in the location pointed to by `thread`, and starts the
		thread by calling the function `start_routine` with `arg` as its
		argument.
	@note//_PARAMETERS
	@param thread A pointer to a pthread_t structure where the thread ID will be
		stored.
	@param attr A pointer to a pthread_attr_t structure containing the thread
		attributes. If NULL, the default attributes are used.
	@param start_routine A pointer to the function to be called by the new
		thread.
	@param arg The argument to be passed to the `start_routine` function.
	@note//_NOTES
	@note If `attr` is NULL, the default attributes are used.
	@note If `start_routine` returns, the effect is as if there was an implicit
		call to pthread_exit() using the return value of `start_routine` as the
		exit status.
	@note Upon thread exit the storage associated with the thread ID must be
		either reclaimed by another thread with `pthread_join` or must be given
		back to the system with `pthread_detach`.
	@note//_RETURN
	@return 0 on success, otherwise an error number is returned.
	@note//_WARNING
	@warning
 */
int			pthread_create(pthread_t *thread, const pthread_attr_t *attr,
				void *(*start_routine) (void *), void *arg);

/**
	@brief #### Suspend execution of the calling thread until the target thread
		terminates.
	@note//_DESCRIPTION
	@brief The `pthread_join` function waits for the thread specified by `thread`
		to terminate. If that thread has already terminated, then `pthread_join`
		returns immediately.
	@note//_PARAMETERS
	@param thread The thread to wait for.
	@param ret_val A pointer to a pointer where the exit status of
		the target thread will be stored.
	@note//_NOTES
	@note If `ret_val` is not NULL, the exit status of the target thread will be
		stored in the location pointed to by `ret_val`.
	@note//_RETURN
	@return 0 on success, otherwise an error number is returned.
	@note//_WARNING
	@warning Multiple threads can not wait for the same thread with
		`pthread_join` it will result in undefined behavior.
	@warning If the caller is canceled, the target thread will not be detached.
 */
int			pthread_join(pthread_t thread, void **ret_val);

/**
	@brief #### Detach the thread.
	@note//_DESCRIPTION
	@brief It tells the system that storage for the thread ID can be reclaimed
		when the thread terminates.
	@note//_PARAMETERS
	@param thread The thread to be detached.
	@note//_NOTES
	@note It will not make the thread end if it is still running.
	@note//_RETURN
	@return 0 on success, otherwise an error number is returned.
	@note//_WARNING
	@warning Multiple calls to `pthread_detach` with the same thread ID will
		result in undefined behavior.
 */
int			pthread_detach(pthread_t thread);

/**
	@brief #### Create a new mutex.
	@note//_DESCRIPTION
	@brief Creates a new mutex, with attributes specified by `attr`, and places
		the mutex in the location pointed to by `mutex`.
	@note//_PARAMETERS
	@param mutex A pointer to a `pthread_mutex_t` structure where the mutex will be
		stored.
	@param attr A pointer to a `pthread_mutexattr_t` structure containing the
		mutex attributes. If `NULL`, the default attributes are used.
	@note//_RETURN
	@return 0 on success, otherwise an error number is returned.
 */
int			pthread_mutex_init(pthread_mutex_t *mutex,
				const pthread_mutexattr_t *attr);

/**
	@brief #### Destroy the mutex.
	@note//_DESCRIPTION
	@brief Frees the resources allocated for the mutex specified by `mutex`.
	@note//_PARAMETERS
	@param mutex A pointer to the mutex to be destroyed.
	@note//_NOTES
	@note It will fail if the mutex is locked.
	@note//_RETURN
	@return 0 on success, otherwise an error number is returned.
 */
int			pthread_mutex_destroy(pthread_mutex_t *mutex);

/**
	@brief #### Lock the mutex.
	@note//_DESCRIPTION
	@brief Locks the mutex specified by `mutex`. If the mutex is already locked,
		the calling thread will block until the mutex becomes available.
	@note//_PARAMETERS
	@param mutex A pointer to the mutex to be locked.
	@note//_NOTES
	@note If the mutex is already locked, the calling thread will block until
		the mutex becomes available.
	@note//_RETURN
	@return 0 on success, otherwise an error number is returned.
	@note//_WARNING
	@warning warning
 */
int			pthread_mutex_lock(pthread_mutex_t *mutex);

/**
	@brief #### Unlock the mutex.
	@note//_DESCRIPTION
	@brief If the current thread holds the lock on the mutex specified by
		`mutex`, then the `pthread_mutex_unlock` function unlocks the mutex.
	@note//_PARAMETERS
	@param mutex A pointer to the mutex to be unlocked.
	@note//_RETURN
	@return 0 on success, otherwise an error number is returned.
	@note//_WARNING
	@warning Calling `pthread_mutex_unlock` with a mutex that is not locked by
		the calling thread will result in undefined behavior.
 */
int			pthread_mutex_unlock(pthread_mutex_t *mutex);

//_--------------------------------------------------------------------------_//

#endif /* philosophers.h */
