ifndef TOPDIR
	TOPDIR := $(abspath ../../../)
endif

ifndef DSTDIR
	DSTDIR := $(abspath ../)
endif

include $(TOPDIR)/config/makefile/common.mk

DIRS := ClientReadHandlerFactory \
		ClientWriteHandlerFactory \
		FileReadHandlerFactory \
		FileWriteHandlerFactory \
		ServerReadHandlerFactory \
		TimerHandlerFactory

include $(TOPDIR)/config/makefile/objRecipes.mk
