SUBDIRS=\
	hello_world_color \
	constexpr \
	range_based_for_loop \
	range_based_for_loop_custom_begin_end \
	std_bind \
	std_bind_with_a_method \
	std_bind_with_a_class_callback \
	std_move \
	std_forward \
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
