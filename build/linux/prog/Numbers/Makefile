MAKE_DIR := $(shell mkpath="."; count=0; while [ \( ! -e "$$mkpath/Makefile.top" \) -a \( "$$count" -lt 10 \) ]; do mkpath="$$mkpath/.."; count=`echo $$count | awk '{print $$1 + 1;}'`; done; echo $$mkpath;)
include $(MAKE_DIR)/Makefile.top

PROJECT_LIST  := NumbersSolver NumbersDriver

all install clean spotless:
	for d in $(PROJECT_LIST); do \
		$(MAKE) $(MAKE_FLAGS) -f Makefile.$$d $@ ; \
	done

$(PROJECT_LIST):
	$(MAKE) -f Makefile.$@

include $(MAKE_DIR)/Makefile.dep

# Local Variables:
# mode:makefile
# End:
