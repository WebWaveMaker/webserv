# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jgo <jgo@student.42seoul.fr>               +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/02 20:10:46 by jgo               #+#    #+#              #
#    Updated: 2023/10/27 00:14:06 by jgo              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifndef TOPDIR
	TOPDIR := $(abspath ./)
endif

include $(TOPDIR)/config/makefile/common.mk

all bonus clean fclean re check:
	@$(MAKE) TOPDIR=`pwd` -C src $@

.PHONY: all clean fclean re bonus
