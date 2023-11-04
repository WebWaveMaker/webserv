ifndef TOPDIR
	TOPDIR := $(abspath ../../)
endif

include $(TOPDIR)/config/makefile/common.mk

DIRS := ErrorLogger \
		AccessLogger

SRCS := ALogger.cpp

LIBS :=	$(wildcard objs/*.o)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

-include $(DEPS)
include $(TOPDIR)/config/makefile/libRecipes.mk
