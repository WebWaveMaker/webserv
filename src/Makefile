ifndef TOPDIR
	TOPDIR := $(abspath ../)
endif

include $(TOPDIR)/config/makefile/common.mk

NAME :=	$(PROJECT_NAME)

DIRS :=	AConfig \
		ALogger \
		Builder \
		Client \
		Factory \
		HttpMessage \
		HttpSession \
		Parser \
		Reactor \
		Server \
		ServerManager \
		Mime \
		utils \
		UserDB

LIBS :=	$(addsuffix .a, $(addprefix lib/, $(DIRS)))
SRCS := main.cpp

OBJS :=	$(SRCS:.cpp=.o)
DEPS :=	$(SRCS:.cpp=.d)

-include	$(DEPS)
include		$(TOPDIR)/config/makefile/recipes.mk
