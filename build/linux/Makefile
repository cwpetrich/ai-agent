MAKE_DIR := $(shell mkpath="."; count=0; while [ \( ! -e "$$mkpath/Makefile.top" \) -a \( "$$count" -lt 10 \) ]; do mkpath="$$mkpath/.."; count=`echo $$count | awk '{print $$1 + 1;}'`; done; echo $$mkpath;)

ifeq ($(wildcard $(MAKE_DIR)/Makefile.top),)
#
# bootstrap the Makefiles
#
-include ../../../ai-main/build/linux/mk/top.mk
include ../../../ai-main/build/linux/mk/make_rules.mk

all: make
	make configure

else
#
# normal operations
#
include $(MAKE_DIR)/Makefile.top

all: make configure progs

progs:
	(cd prog; $(MAKE) install)

PROJECT_LIST  := prog

clean spotless:
	for d in $(PROJECT_LIST); do \
		(cd $$d; $(MAKE) $@) \
	done

include $(MAKE_DIR)/Makefile.configure_rules
include $(MAKE_DIR)/Makefile.make_rules
include $(MAKE_DIR)/Makefile.dep

endif
