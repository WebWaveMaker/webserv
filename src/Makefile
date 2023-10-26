# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sanghwal <sanghwal@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/23 14:05:59 by jgo               #+#    #+#              #
#    Updated: 2023/10/20 17:40:22 by sanghwal         ###   ########seoul.kr   #
#                                                                              #
# **************************************************************************** #

ifndef TOPDIR
	TOPDIR := $(abspath ../)
endif

include $(TOPDIR)/config/makefile/common.mk

NAME :=	$(PROJECT_NAME)

DIRS :=	AConfig \
		ALogger \
		Client \
		HttpMessage \
		Parser \
		Reactor \
		Server \
		ServerManager \
		utils

LIBS :=	$(addsuffix .a, $(addprefix lib/, $(DIRS)))
SRCS := main.cpp

OBJS :=	$(SRCS:.cpp=.o)
DEPS :=	$(SRCS:.cpp=.d)

-include	$(DEPS)
include		$(TOPDIR)/config/makefile/recipes.mk
