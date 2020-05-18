COMPILER_FLAGS=-std=c++2a -Werror -g -ggdb3 -O2  # AUTOGEN
    
CLANG_COMPILER_WARNINGS=-Wall # AUTOGEN
GCC_COMPILER_WARNINGS=-Wall # AUTOGEN
GXX_COMPILER_WARNINGS=-Wall # AUTOGEN
COMPILER_WARNINGS=$(GCC_COMPILER_WARNINGS) # AUTOGEN
COMPILER_WARNINGS=$(GXX_COMPILER_WARNINGS) # AUTOGEN
COMPILER_WARNINGS=$(CLANG_COMPILER_WARNINGS) # AUTOGEN
CC=clang # AUTOGEN
# CC=gcc # AUTOGEN
# CC=cc # AUTOGEN
# CC=g++ # AUTOGEN
    
EXE= # AUTOGEN
LDLIBS= # AUTOGEN
CFLAGS=$(COMPILER_FLAGS) $(COMPILER_WARNINGS) # AUTOGEN
NAME=example
OBJDIR=.o

TARGET_OBJECTS=$(OBJDIR)/main.o \
               $(OBJDIR)/traceback.o $(OBJDIR)/ptrcheck.o $(OBJDIR)/sprintf.o

EXTRA_CFLAGS=-std=c++11

$(OBJDIR)/%.o: %.cpp
	@echo $(CXX) $(EXTRA_CFLAGS) $(CFLAGS) -c -o $@ $<
	@$(CXX) $(EXTRA_CFLAGS) $(CFLAGS) -c -o $@ $<

#
# link
#
TARGET=$(NAME)$(EXE)
$(TARGET): $(TARGET_OBJECTS)
	$(CXX) $(TARGET_OBJECTS) $(LDLIBS) -o $(TARGET)

#
# To force clean and avoid "up to date" warning.
#
.PHONY: clean
.PHONY: clobber

clean:
	rm -rf $(TARGET) $(OBJDIR)/*.o

clobber: clean
	mkdir -p $(OBJDIR)

all: $(TARGET) 
# DO NOT DELETE

.o/main.o: my_traceback.h my_ptrcheck.h shared_ptr_wrapper.h
.o/ptrcheck.o: my_traceback.h my_sprintf.h /usr/include/string.h
.o/ptrcheck.o: /usr/include/_types.h /usr/include/sys/_types.h
.o/ptrcheck.o: /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h
.o/ptrcheck.o: /usr/include/sys/_posix_availability.h
.o/ptrcheck.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
.o/ptrcheck.o: /usr/include/sys/_pthread/_pthread_types.h
.o/ptrcheck.o: /usr/include/Availability.h
.o/ptrcheck.o: /usr/include/AvailabilityInternal.h
.o/ptrcheck.o: /usr/include/sys/_types/_size_t.h
.o/ptrcheck.o: /usr/include/sys/_types/_null.h
.o/ptrcheck.o: /usr/include/sys/_types/_rsize_t.h
.o/ptrcheck.o: /usr/include/machine/types.h /usr/include/i386/types.h
.o/ptrcheck.o: /usr/include/sys/_types/_int8_t.h
.o/ptrcheck.o: /usr/include/sys/_types/_int16_t.h
.o/ptrcheck.o: /usr/include/sys/_types/_int32_t.h
.o/ptrcheck.o: /usr/include/sys/_types/_int64_t.h
.o/ptrcheck.o: /usr/include/sys/_types/_u_int8_t.h
.o/ptrcheck.o: /usr/include/sys/_types/_u_int16_t.h
.o/ptrcheck.o: /usr/include/sys/_types/_u_int32_t.h
.o/ptrcheck.o: /usr/include/sys/_types/_u_int64_t.h
.o/ptrcheck.o: /usr/include/sys/_types/_intptr_t.h
.o/ptrcheck.o: /usr/include/sys/_types/_uintptr_t.h
.o/ptrcheck.o: /usr/include/sys/_types/_errno_t.h
.o/ptrcheck.o: /usr/include/sys/_types/_ssize_t.h /usr/include/strings.h
.o/ptrcheck.o: /usr/include/secure/_strings.h /usr/include/secure/_common.h
.o/ptrcheck.o: /usr/include/secure/_string.h
.o/sprintf.o: my_sprintf.h /usr/include/string.h /usr/include/_types.h
.o/sprintf.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
.o/sprintf.o: /usr/include/sys/_symbol_aliasing.h
.o/sprintf.o: /usr/include/sys/_posix_availability.h
.o/sprintf.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
.o/sprintf.o: /usr/include/sys/_pthread/_pthread_types.h
.o/sprintf.o: /usr/include/Availability.h /usr/include/AvailabilityInternal.h
.o/sprintf.o: /usr/include/sys/_types/_size_t.h
.o/sprintf.o: /usr/include/sys/_types/_null.h
.o/sprintf.o: /usr/include/sys/_types/_rsize_t.h /usr/include/machine/types.h
.o/sprintf.o: /usr/include/i386/types.h /usr/include/sys/_types/_int8_t.h
.o/sprintf.o: /usr/include/sys/_types/_int16_t.h
.o/sprintf.o: /usr/include/sys/_types/_int32_t.h
.o/sprintf.o: /usr/include/sys/_types/_int64_t.h
.o/sprintf.o: /usr/include/sys/_types/_u_int8_t.h
.o/sprintf.o: /usr/include/sys/_types/_u_int16_t.h
.o/sprintf.o: /usr/include/sys/_types/_u_int32_t.h
.o/sprintf.o: /usr/include/sys/_types/_u_int64_t.h
.o/sprintf.o: /usr/include/sys/_types/_intptr_t.h
.o/sprintf.o: /usr/include/sys/_types/_uintptr_t.h
.o/sprintf.o: /usr/include/sys/_types/_errno_t.h
.o/sprintf.o: /usr/include/sys/_types/_ssize_t.h /usr/include/strings.h
.o/sprintf.o: /usr/include/secure/_strings.h /usr/include/secure/_common.h
.o/sprintf.o: /usr/include/secure/_string.h
.o/traceback.o: my_traceback.h my_sprintf.h /usr/include/string.h
.o/traceback.o: /usr/include/_types.h /usr/include/sys/_types.h
.o/traceback.o: /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h
.o/traceback.o: /usr/include/sys/_posix_availability.h
.o/traceback.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
.o/traceback.o: /usr/include/sys/_pthread/_pthread_types.h
.o/traceback.o: /usr/include/Availability.h
.o/traceback.o: /usr/include/AvailabilityInternal.h
.o/traceback.o: /usr/include/sys/_types/_size_t.h
.o/traceback.o: /usr/include/sys/_types/_null.h
.o/traceback.o: /usr/include/sys/_types/_rsize_t.h
.o/traceback.o: /usr/include/machine/types.h /usr/include/i386/types.h
.o/traceback.o: /usr/include/sys/_types/_int8_t.h
.o/traceback.o: /usr/include/sys/_types/_int16_t.h
.o/traceback.o: /usr/include/sys/_types/_int32_t.h
.o/traceback.o: /usr/include/sys/_types/_int64_t.h
.o/traceback.o: /usr/include/sys/_types/_u_int8_t.h
.o/traceback.o: /usr/include/sys/_types/_u_int16_t.h
.o/traceback.o: /usr/include/sys/_types/_u_int32_t.h
.o/traceback.o: /usr/include/sys/_types/_u_int64_t.h
.o/traceback.o: /usr/include/sys/_types/_intptr_t.h
.o/traceback.o: /usr/include/sys/_types/_uintptr_t.h
.o/traceback.o: /usr/include/sys/_types/_errno_t.h
.o/traceback.o: /usr/include/sys/_types/_ssize_t.h /usr/include/strings.h
.o/traceback.o: /usr/include/secure/_strings.h /usr/include/secure/_common.h
.o/traceback.o: /usr/include/secure/_string.h /usr/include/stdio.h
.o/traceback.o: /usr/include/_stdio.h /usr/include/sys/_types/_va_list.h
.o/traceback.o: /usr/include/sys/stdio.h /usr/include/sys/_types/_off_t.h
.o/traceback.o: /usr/include/secure/_stdio.h /usr/include/stdlib.h
.o/traceback.o: /usr/include/sys/wait.h /usr/include/sys/_types/_pid_t.h
.o/traceback.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
.o/traceback.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
.o/traceback.o: /usr/include/i386/signal.h /usr/include/machine/_mcontext.h
.o/traceback.o: /usr/include/i386/_mcontext.h
.o/traceback.o: /usr/include/mach/machine/_structs.h
.o/traceback.o: /usr/include/mach/i386/_structs.h
.o/traceback.o: /usr/include/sys/_pthread/_pthread_attr_t.h
.o/traceback.o: /usr/include/sys/_types/_sigaltstack.h
.o/traceback.o: /usr/include/sys/_types/_ucontext.h
.o/traceback.o: /usr/include/sys/_types/_sigset_t.h
.o/traceback.o: /usr/include/sys/_types/_uid_t.h /usr/include/sys/resource.h
.o/traceback.o: /usr/include/stdint.h /usr/include/_types/_uint8_t.h
.o/traceback.o: /usr/include/_types/_uint16_t.h
.o/traceback.o: /usr/include/_types/_uint32_t.h
.o/traceback.o: /usr/include/_types/_uint64_t.h
.o/traceback.o: /usr/include/_types/_intmax_t.h
.o/traceback.o: /usr/include/_types/_uintmax_t.h
.o/traceback.o: /usr/include/sys/_types/_timeval.h
.o/traceback.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
.o/traceback.o: /usr/include/sys/_endian.h
.o/traceback.o: /usr/include/libkern/_OSByteOrder.h
.o/traceback.o: /usr/include/libkern/i386/_OSByteOrder.h
.o/traceback.o: /usr/include/alloca.h /usr/include/sys/_types/_ct_rune_t.h
.o/traceback.o: /usr/include/sys/_types/_rune_t.h
.o/traceback.o: /usr/include/sys/_types/_wchar_t.h
.o/traceback.o: /usr/include/sys/_types/_dev_t.h
.o/traceback.o: /usr/include/sys/_types/_mode_t.h /usr/include/execinfo.h
.o/traceback.o: /usr/include/cxxabi.h /usr/include/stddef.h
.o/traceback.o: /usr/include/sys/_types/_offsetof.h
.o/traceback.o: /usr/include/sys/_types/_ptrdiff_t.h
.o/traceback.o: /usr/include/sys/_types/_wint_t.h
.o/traceback.o: /usr/include/__cxxabi_config.h
