# Philosophers
Philosophers is a multithreaded dining philosophers problem simulator. The problem is a classic multi-process synchronization problem which was originally formulated in 1965 by Edsger Dijkstra.

## Problem Statement
There are `N` philosophers sitting around a round table. Each philosopher has a dish of spaghetti. A fork is placed between each pair of adjacent philosophers, and each philosopher must alternately think, eat and sleep. However, a philosopher can only eat spaghetti when they have both left and right forks. Each fork can be held by only one philosopher and so a philosopher can use the fork only if it's not being used by another philosopher. After an individual philosopher finishes eating, they need to put down both forks so the forks become available to others. A philosopher can take the fork on their right or the one on their left as they become available, but can't start eating before getting both forks.

_For more information, see [Dining Philosophers Problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem)._

## Specifications
1. The program takes in the following arguments:

	- `N` - the number of philosophers
	- `die_time` - time to die (in milliseconds). If a philosopher doesn’t start eating `die_time` milliseconds after starting his last meal or the beginning of the simulation, it dies
	- `eat_time` - time the philosopher will need to eat (in milliseconds). During that time they will need to keep the two forks
	- `sleep_time` - time the philosopher will need to sleep (in milliseconds)
	- `meal_count` - (optional) number of times each philosopher must eat. If all philosophers eat at least `meal_count` times, the simulation will stop

1. The philosophers are numbered from 1 to `N`.
1. The simulation uses threads and mutexes.*
1. Each philosopher should be a thread.*
1. Each fork is represented by a mutex.*

_** = This specifications are only for the mandatory part of the project, for more information about the bonus part see [BONUS](./philo_bonus/BONUS.md)._

## Usage
To compile the program, run:
``` C
make -C philo
```

To start the simulation, run:
``` C
./philo/philo N die_time eat_time sleep_time [meal_count]
```
_Substitute `N`, `die_time`, `eat_time`, `sleep_time`, and `meal_count`(optional) with the desired values._

### Output
The program provides the following output** for each philosopher:

- When a philosopher takes a fork
- When a philosopher is eating
- When a philosopher is sleeping
- When a philosopher is thinking
- When a philosopher dies

_** = Each output is preceded by a timestamp (in milliseconds)._

## Note
This project is part of the curriculum for the 42 School. It's designed to help students learn about multithreading and synchronization in C.

## License
This project is licensed under the MIT License. This means you can use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the final products. However, you need to include this license in any substantial uses of the work. For more details, please see the [MIT License](./LICENSE).
