ifndef TOPDIR
	TOPDIR := $(abspath ../../../../)
endif

ifndef DSTDIR
	DSTDIR := $(abspath ../../)
endif

include $(TOPDIR)/config/makefile/common.mk

SRCS := $(addprefix $(DSTDIR)/objs/, SharedData.cpp)

OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

-include $(DEPS)erverReadHandler

include $(TOPDIR)/config/makefile/objRecipes.mk
