/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 10:14:21 by chon              #+#    #+#             */
/*   Updated: 2024/07/30 12:33:32 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	sum_num_arr(size_t *arr)
{
	int		count;
	int		i;
	size_t	sum;

	count = sizeof(*arr) / sizeof(size_t);
	i = -1;
	sum = 0;
	while (++i < count)
		sum += arr[i];
	return (sum);
}

size_t	cur_time()
{
	struct	timeval	time;

	if (gettimeofday(&time, NULL) < 0)
	{
		printf("gettimeofday error\n");
		return (0);
	}
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int	ft_usleep(size_t msec)
{
	size_t	start;

	start = cur_time();
	while ((cur_time() - start) < msec)
		usleep(300);
	return (0);
}

int	ft_atoi(char *str)
{
	int	num;
	int	i;

	num = 0;
	i = -1;
	while (str[++i])
		num = num * 10 + str[i] - '0'; 
	return (num);
}
