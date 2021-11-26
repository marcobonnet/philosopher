/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_action_philo.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 18:40:07 by mbonnet           #+#    #+#             */
/*   Updated: 2021/11/26 11:11:28 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	ft_take_one_forks(int index, t_philo *philo)
{
	pthread_mutex_lock(&philo->info->forks[index]);
	if (my_write(philo, "has taken a fork") == -1)
	{
		pthread_mutex_unlock(&philo->info->forks[index]);
		return (-1);
	}
	return (1);
}

int	my_take_forks(t_philo *philo)
{
	int	index_left;
	int	index_right;
	int	tmp;

	index_left = philo->id;
	index_right = philo->id + 1;
	if (philo->id == philo->info->nb_philo - 1)
		index_right = 0;
	if (philo->id % 2 == 0)
	{
		tmp = index_right;
		index_right = index_left;
		index_left = tmp;
	}
	if (ft_take_one_forks(index_left, philo) == -1)
		return (-1);
	else
	{
		if (ft_take_one_forks(index_right, philo) == -1)
		{
			pthread_mutex_unlock(&philo->info->forks[index_left]);
			return (-1);
		}
	}
	return (1);
}

int	my_pose_forks(t_philo *philo)
{
	int	index_left;
	int	index_right;
	int	tmp;

	index_left = philo->id;
	index_right = philo->id + 1;
	if (philo->id == philo->info->nb_philo - 1)
		index_right = 0;
	if (philo->id % 2 == 0)
	{
		tmp = index_right;
		index_right = index_left;
		index_left = tmp;
	}
	pthread_mutex_unlock(&philo->info->forks[index_right]);
	pthread_mutex_unlock(&philo->info->forks[index_left]);
	return (1);
}

int	my_eat(t_philo *philo)
{
	int	checker_nb_repas;

	pthread_mutex_lock(&philo->check_last_eat);
	philo->time_last_eat = get_time();
	pthread_mutex_unlock(&philo->check_last_eat);
	pthread_mutex_lock(&philo->check_nb_eat);
	philo->nb_eat++;
	checker_nb_repas = philo->nb_eat;
	pthread_mutex_unlock(&philo->check_nb_eat);
	if (my_write(philo, "is eating") == -1)
		return (my_pose_forks(philo) * -1);
	if (philo->info->nb_eat != -1 && checker_nb_repas >= philo->info->nb_eat)
	{
		my_died_shot(philo, -2);
		my_write_2(philo, checker_nb_repas);
		return (-1);
	}
	if (my_usleep(philo, philo->info->time_eat) == -1)
		return (my_pose_forks(philo) * -1);
	return (1);
}

int	my_sleep_and_think(t_philo *philo)
{
	if (my_write(philo, "is sleeping") == -1
		|| my_usleep(philo, philo->info->time_sleep) == -1)
		return (-1);
	my_write(philo, "is thinking");
	return (1);
}
