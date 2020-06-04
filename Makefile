SUBDIRS=\
	hello_world_color \
	std_file_read \
	scoped_enums \
	initializer_lists \
	initializer_lists_with_custom_vector \
	constexpr \
	decltype \
	range_based_for_loop \
	range_based_for_loop_custom_begin_end \
	lambda \
	std_for_each_and_transform \
	std_map_with_custom_key \
	std_map_reverse_sort \
	std_unordered_map \
	std_multiset \
	std_set \
	std_unordered_set \
	std_sort_with_custom_iterators \
	std_function_and_using \
	std_bind \
	std_bind_with_a_method \
	std_bind_with_a_class_callback \
	std_move \
	std_forward \
	std_unique_ptr \
	std_unique_ptr_with_custom_deallocator \
	std_unique_ptr_for_file_handling \
	std_shared_ptr \
	std_shared_ptr_wrapper \
	std_thread

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
