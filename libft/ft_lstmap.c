/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashalaab <ashalaab@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 12:43:38 by ashalaab          #+#    #+#             */
/*   Updated: 2024/06/14 12:43:38 by ashalaab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_list	*ft_lstnewval(void *val, void (*del)(void *))
{
	t_list	*node;

	if (!val)
		return (NULL);
	node = ft_lstnew(val);
	if (!node)
		del(val);
	return (node);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*map;
	t_list	*curr;

	if (!lst)
		return (NULL);
	map = ft_lstnewval(f(lst->content), del);
	if (!map)
		return (NULL);
	curr = map;
	lst = lst->next;
	while (lst)
	{
		curr->next = ft_lstnewval(f(lst->content), del);
		curr = curr->next;
		if (curr == NULL)
		{
			ft_lstclear(&map, del);
			return (NULL);
		}
		lst = lst->next;
	}
	return (map);
}
