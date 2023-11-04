ifndef TOPDIR
	TOPDIR := $(abspath ../../)
endif

include $(TOPDIR)/config/makefile/common.mk

DIRS := Dispatcher \
		AEventHandler \
		SyncEventDemultiplexer

LIBS :=	$(wildcard objs/*.o)

include $(TOPDIR)/config/makefile/libRecipes.mk
