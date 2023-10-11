# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jgo <jgo@student.42seoul.kr>               +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/07 19:34:14 by jgo               #+#    #+#              #
#    Updated: 2023/10/11 17:11:25 by jgo              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifndef TOPDIR
	TOPDIR := $(abspath ../../)
endif

include $(TOPDIR)/config/makefile/common.mk

SRCS := AccessLogger.cpp

OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

-include $(DEPS)
include $(TOPDIR)/config/makefile/libRecipes.mk
