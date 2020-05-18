SUBDIRS=unique_ptr shared_ptr shared_ptr_wrapper

#
# To force clean and avoid "up to date" warning.
#
.PHONY: clean
.PHONY: clobber

all::
	@echo make all
	$(foreach var,$(SUBDIRS),echo $(var): ; cd $(var)/ && make $@ && cd ..;)

clean:
	@echo make clean
	$(foreach var,$(SUBDIRS),echo $(var): ; cd $(var)/ && make $@ && cd ..;)
