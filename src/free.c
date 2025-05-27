/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:11:12 by brcoppie          #+#    #+#             */
/*   Updated: 2025/05/27 17:17:50 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_store(t_store *store)
{
	if (store->env_tab)
		ft_free_tab(store->env_tab);
	free(store);
}

void	free_data(t_data *data)
{
	if (data->store)
		free_store(data->store);
	free(data);
}
