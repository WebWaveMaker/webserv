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
		PostResponseBuilder \
		PutResponseBuilder \
		RedirectResponseBuilder
		CgiResponseBuilder

include $(TOPDIR)/config/makefile/objRecipes.mk
