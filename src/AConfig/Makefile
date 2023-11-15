ifndef TOPDIR
	TOPDIR := $(abspath ../../)
endif

include $(TOPDIR)/config/makefile/common.mk

DIRS := HttpConfig \
		LocationConfig \
		ServerConfig \
		ConfigSyntax 

SRCS := AConfig.cpp

LIBS :=	$(wildcard objs/*.o)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

-include $(DEPS)
include $(TOPDIR)/config/makefile/libRecipes.mk
