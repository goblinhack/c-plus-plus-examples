COMPILER_FLAGS=-std=c++2a -Werror -g -ggdb3 # AUTOGEN
    
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
CXXFLAGS=$(COMPILER_FLAGS) $(COMPILER_WARNINGS) # AUTOGEN
NAME=example

TARGET_OBJECTS=main.o

EXTRA_CXXFLAGS=

%.o: %.cpp
	@echo $(CXX) $(EXTRA_CXXFLAGS) $(CXXFLAGS) -c -o $@ $<
	@$(CXX) $(EXTRA_CXXFLAGS) $(CXXFLAGS) -c -o $@ $<

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
	rm -f *.o $(TARGET)

clobber: clean

all: $(TARGET) 
# DO NOT DELETE

.o/main.o: /usr/include/string.h /usr/include/_types.h
.o/main.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
.o/main.o: /usr/include/sys/_symbol_aliasing.h
.o/main.o: /usr/include/sys/_posix_availability.h
.o/main.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
.o/main.o: /usr/include/sys/_pthread/_pthread_types.h
.o/main.o: /usr/include/Availability.h /usr/include/AvailabilityInternal.h
.o/main.o: /usr/include/sys/_types/_size_t.h /usr/include/sys/_types/_null.h
.o/main.o: /usr/include/sys/_types/_rsize_t.h /usr/include/machine/types.h
.o/main.o: /usr/include/i386/types.h /usr/include/sys/_types/_int8_t.h
.o/main.o: /usr/include/sys/_types/_int16_t.h
.o/main.o: /usr/include/sys/_types/_int32_t.h
.o/main.o: /usr/include/sys/_types/_int64_t.h
.o/main.o: /usr/include/sys/_types/_u_int8_t.h
.o/main.o: /usr/include/sys/_types/_u_int16_t.h
.o/main.o: /usr/include/sys/_types/_u_int32_t.h
.o/main.o: /usr/include/sys/_types/_u_int64_t.h
.o/main.o: /usr/include/sys/_types/_intptr_t.h
.o/main.o: /usr/include/sys/_types/_uintptr_t.h
.o/main.o: /usr/include/sys/_types/_errno_t.h
.o/main.o: /usr/include/sys/_types/_ssize_t.h /usr/include/strings.h
.o/main.o: /usr/include/secure/_strings.h /usr/include/secure/_common.h
.o/main.o: /usr/include/secure/_string.h ../common/common.h
