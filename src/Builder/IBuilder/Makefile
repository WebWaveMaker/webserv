ifndef TOPDIR
	TOPDIR := $(abspath ../../../)
endif

ifndef DSTDIR
	DSTDIR := $(abspath ../)
endif

include $(TOPDIR)/config/makefile/common.mk

DIRS := DefaultResponseBuilder \
		ErrorResponseBuilder \
		GetResponseBuilder \
		DeleteResponseBuilder \
		RedirectResponseBuilder \
		PutResponseBuilder

include $(TOPDIR)/config/makefile/objRecipes.mk
