ifndef TOPDIR
	TOPDIR := $(abspath ../../../)
endif

ifndef DSTDIR
	DSTDIR := $(abspath ../)
endif

include $(TOPDIR)/config/makefile/common.mk

DIRS := ClientRequestHandlerFactory \
		ClientResponseHandlerFactory \
		ServerAcceptHandlerFactory

include $(TOPDIR)/config/makefile/objRecipes.mk
