SUBDIRS=shared_ptr shared_ptr_wrapper

all::
	@echo make all
	$(foreach var,$(SUBDIRS),echo $(var): ; cd $(var)/ && make $@ && cd ..;)

clean:
	@echo make clean
	$(foreach var,$(SUBDIRS),echo $(var): ; cd $(var)/ && make $@ && cd ..;)
