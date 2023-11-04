ifndef TOPDIR
	TOPDIR := $(abspath ../)
endif

include $(TOPDIR)/config/makefile/common.mk

NAME :=	$(PROJECT_NAME)

DIRS :=	AConfig \
		ALogger \
		Client \
		Factory \
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
