ifndef TOPDIR
	TOPDIR := $(abspath ../../)
endif

include $(TOPDIR)/config/makefile/common.mk

SRCS := utils.cpp

OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

-include $(DEPS)
include $(TOPDIR)/config/makefile/libRecipes.mk
