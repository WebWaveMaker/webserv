ifndef TOPDIR
	TOPDIR := $(abspath ../../../)
endif

ifndef DSTDIR
	DSTDIR := $(abspath ../)
endif

include $(TOPDIR)/config/makefile/common.mk

DIRS := ClientReadHandler \
		ClientRequestHandler \
		ClientResponseHandler \
		ClientWriteHandler \
		FileReadHandler \
		ServerAcceptHandler \
		ServerReadHandler \
		TestAcceptHandler

include $(TOPDIR)/config/makefile/objRecipes.mk
