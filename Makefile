SUBDIRS=range_based_for_loop \
	range_based_for_loop_custom_begin_end \
	std_move \
	std_unique_ptr \
	std_shared_ptr \
	std_shared_ptr_wrapper

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
