ifndef TOPDIR
	TOPDIR := $(abspath ./)
endif

include $(TOPDIR)/config/makefile/common.mk

all bonus clean fclean re check:
	@$(MAKE) TOPDIR=`pwd` -C src $@

.PHONY: all clean fclean re bonus
