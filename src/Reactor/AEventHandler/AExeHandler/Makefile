ifndef TOPDIR
	TOPDIR := $(abspath ../../../../)
endif

ifndef DSTDIR
	DSTDIR := $(abspath ../../)
endif

include $(TOPDIR)/config/makefile/common.mk

DIRS :=	ClientRequestHandler \
		ClientResponseHandler \
		ServerAcceptHandler \
		TimeoutHandler

SRCS := $(addprefix $(DSTDIR)/objs/, AExeHandler.cpp)

OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

-include $(DEPS)
include $(TOPDIR)/config/makefile/objRecipes.mk
