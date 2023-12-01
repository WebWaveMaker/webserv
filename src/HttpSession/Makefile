ifndef TOPDIR
	TOPDIR := $(abspath ../../)
endif

include $(TOPDIR)/config/makefile/common.mk

SRCS := HttpSession.cpp

DIRS := SessionStore

LIBS :=	$(wildcard objs/*.o)
	
OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

-include $(DEPS)
include $(TOPDIR)/config/makefile/libRecipes.mk
