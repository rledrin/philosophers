/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rledrin <rledrin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 13:54:49 by rledrin           #+#    #+#             */
/*   Updated: 2020/11/19 13:54:49 by rledrin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/functions.h"

static int		fill_param(t_one *one, char **av, int ac)
{
	one->philo = NULL;
	one->param->time_start = NULL;
	if (ft_atos(av[1], (size_t *)&one->param->number_of_philosophers))
		return (WRONG_ARG);
	if (one->param->number_of_philosophers < 2)
		return (WRONG_ARG);
	if ((ft_atos(av[2], &one->param->time_to_die)))
		return (WRONG_ARG);
	if ((ft_atos(av[3], &one->param->time_to_eat)))
		return (WRONG_ARG);
	if ((ft_atos(av[4], &one->param->time_to_sleep)))
		return (WRONG_ARG);
	if (ac == 6 && (ft_atos(av[5],
		&one->param->number_of_eat)))
		return (WRONG_ARG);
	else if (ac == 5)
		one->param->number_of_eat = -1;
	if (!(one->param->prot = m(sizeof(pthread_mutex_t))))
		return (ERROR_MALLOC);
	if (pthread_mutex_init(one->param->prot, NULL))
		return (ERROR_MUTEX);
	return (0);
}

static int		clean_philo(t_one *one)
{
	int			i;

	i = -1;
	while (++i < one->param->number_of_philosophers)
	{
		if (one->philo[i].thread)
			free(one->philo[i].thread);
		if (one->philo[i].right_fork->fork)
		{
			pthread_mutex_destroy(one->philo[i].right_fork->fork);
			free(one->philo[i].right_fork->fork);
		}
		if (one->philo[i].right_fork)
			free(one->philo[i].right_fork);
		if (one->philo[i].monitor)
			free(one->philo[i].monitor);
		if (one->philo[i].time_last_meal)
			free(one->philo[i].time_last_meal);
		if (one->philo[i].nb_eat)
			free(one->philo[i].nb_eat);
		if (one->philo[i].end)
			free(one->philo[i].end);
	}
	free(one->philo);
	return (0);
}

static int		clean_one(t_one *one)
{
	if (one && one->philo)
		clean_philo(one);
	if (one && one->param)
	{
		if (one->param->time_start)
			free(one->param->time_start);
		if (one->param->prot)
			pthread_mutex_destroy(one->param->prot);
		if (one->param->prot)
			free(one->param->prot);
		free(one->param);
	}
	if (one)
		free(one);
	return (0);
}

int				main(int ac, char **av)
{
	t_one		*one;
	int			ret;

	if (!(one = (t_one*)m(sizeof(t_one))) ||
		!(one->param = (t_param*)m(sizeof(t_param))))
		return (errors(ERROR_MALLOC) + clean_one(one));
	if (ac < 5 || ac > 6)
		return (errors(TOO_MANY_ARGS) + clean_one(one));
	one->philo = NULL;
	if ((ret = fill_param(one, av, ac)))
		return (errors(ret) + clean_one(one));
	if (!one->param->number_of_eat)
		return (clean_one(one));
	if ((ret = init_philo(one)))
		return (errors(ret) + clean_one(one));
	if ((ret = philo_start(one)))
		return (errors(ret) + clean_one(one));
	return (clean_one(one));
}
