# Philosophers Bonus
# (At this point only the mandatory part is implemented.)
This is the bonus component of the Philosophers project. In this part of the project, processes instead of threads and semaphores instead of mutexes are used to solve the dining philosophers problem.

## Specifications
1. The program takes the same arguments as the mandatory part:

	- `N` - the number of philosophers
	- `die_time` - time to die (in milliseconds). If a philosopher doesn’t start eating `die_time` milliseconds after starting his last meal or the beginning of the simulation, it dies
	- `eat_time` - time the philosopher will need to eat (in milliseconds). During that time they will need to keep the two forks
	- `sleep_time` - time the philosopher will need to sleep (in milliseconds)
	- `meal_count` - (optional) number of times each philosopher must eat. If all philosophers eat at least `meal_count` times, the simulation will stop
1. The philosophers are numbered from 1 to `N`.
1. The simulation uses processes and semaphores.
1. Each philosopher should be a process.
1. Each fork is represented by a semaphore.

## Usage
To compile the program, run:
``` C
make -C philo_bonus
```

To start the simulation, run:
``` C
./philo_bonus/philo_bonus N die_time eat_time sleep_time [meal_count]
```

_Substitute `N`, `die_time`, `eat_time`, `sleep_time`, and `meal_count` (optional) with the desired values._

### Output
The program provides the following output* for each philosopher:

- When a philosopher takes a fork
- When a philosopher is eating
- When a philosopher is sleeping
- When a philosopher is thinking
- When a philosopher dies
_* = Each output is preceded by a timestamp (in milliseconds)._

## Note
This project is part of the curriculum for the 42 School. It's designed to help students learn about multithreading and synchronization in C.

## License
This project is licensed under the MIT License. This means you can use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the final products. However, you need to include this license in any substantial uses of the work. For more details, please see the [MIT License](../LICENSE).
