ifndef TOPDIR
	TOPDIR := $(abspath ../../../../)
endif

ifndef DSTDIR
	DSTDIR := $(abspath ../../)
endif

include $(TOPDIR)/config/makefile/common.mk

DIRS := ClientReadHandler \
		ClientWriteHandler \
		FileReadHandler \
		FileWriteHandler \
		ServerReadHandler

include $(TOPDIR)/config/makefile/objRecipes.mk
