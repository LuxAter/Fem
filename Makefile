
SHELL=/bin/bash
ifndef .VERBOSE
    .SILENT:
endif
ROOT=$(shell pwd)
CXX=clang++
CC=clang
CCIGNORE=
CXXIGNORE=
CCFLAGS=-fPIC -Wall -Wpedantic --static
CXXFLAGS=-std=c++17 -fPIC -Wall -Wpedantic --static -O3
LINK=-lz -llua5.3
SOURCE=src
INCLUDE_DIR=include
INCLUDE=-I$(ROOT)/$(BUILD)/libpng/include -I/usr/include/lua5.3/
BUILD=build
COMMON_INCLUDE=-I$(ROOT)/$(INCLUDE_DIR) $(INCLUDE)

SCAN_COLOR=\033[1;35m
BUILD_COLOR=\033[32m
CLEAN_COLOR=\033[1;33m
LINK_COLOR=\033[1;32m
INSTALL_COLOR=\033[1;36m
CMD_COLOR=\033[1;34m
HELP_COLOR=\033[1;34m

define scan_target
printf "%b%s%b\n" "$(SCAN_COLOR)" "Scaning dependencies for target $(1)" "\033[0m"
endef
define complete_target
printf "%s\n" "Built target $(1)"
endef
define clean_target
printf "%b%s%b\n" "$(CLEAN_COLOR)" "Cleaning target $(1)" "\033[0m"
endef
define install_target
printf "%b%s%b\n" "$(INSTALL_COLOR)" "Installing target $(1)" "\033[0m"
endef
define uninstall_target
printf "%b%s%b\n" "$(INSTALL_COLOR)" "Unnstalling target $(1)" "\033[0m"
endef
define print_build_c
str=$$(realpath --relative-to="$(ROOT)" "$(1)");    printf "%b%s%b\n" "$(BUILD_COLOR)" "Building C object $$str" "\033[0m"
endef
define print_build_cpp
str=$$(realpath --relative-to="$(ROOT)" "$(1)");    printf "%b%s%b\n" "$(BUILD_COLOR)" "Building C++ object $$str" "\033[0m"
endef
define print_link_lib
str=$$(realpath --relative-to="$(ROOT)" "$(1)");    printf "%b%s%b\n" "$(LINK_COLOR)" "Linking static library $$str" "\033[0m"
endef
define print_link_exe
str=$$(realpath --relative-to="$(ROOT)" "$(1)");    printf "%b%s%b\n" "$(LINK_COLOR)" "Linking executable $$str" "\033[0m"
endef
define print_run_cmd
printf "%b%s%b\n" "$(CMD_COLOR)" "Running '$(1)'" "\033[0m"
endef
define help
printf "%b%*s%b: %s\n" "$(HELP_COLOR)" 20 "$(1)" "\033[0m" "$(2)"
endef

all: build-fem
clean: clean-fem
install: install-fem
uninstall: uninstall-fem

# FEM {{{

FEM=fem
FEM_FILES=src/main.cpp
FEM_OBJS=$(FEM_FILES:%=$(ROOT)/$(BUILD)/%.o)
-include $(FEM_OBJS:.o=.d)

build-fem: build-libfem.a pre-fem $(FEM)
	$(call complete_target,$(shell basename $(FEM)))

clean-fem:
	$(call clean_target,$(shell basename $(FEM)))
	if [ -e "$(FEM)" ]; then rm $(FEM); fi

pre-fem:
	$(call scan_target,$(shell basename $(FEM)))

$(FEM): $(FEM_OBJS) FORCE
	$(call print_link_exe,$(shell basename $(FEM)))
	$(CXX) $(FEM_OBJS) $(LIBFEM.A) $(LINK) $(COMMON_INCLUDE) -o $(FEM)

install-fem: build-fem
	$(call install_target,$(shell basename $(FEM)))
	mkdir -p $(INSTALL_PATH)/bin/
	cp $(FEM) $(INSTALL_PATH)/bin

uninstall-fem:
	$(call uninstall_target,$(shell basename $(FEM)))
	if [ -e "$(INSTALL_PATH)/bin/$(shell basename $(FEM))" ]; then rm $(INSTALL_PATH)/bin/$(shell basename $(FEM)); fi

# }}}
# LIBFEM.A {{{

LIBFEM.A=$(BUILD)/libfem.a
LIBFEM.A_FILES=$(filter-out src/main.cpp, $(shell find "src/" -name "*.cpp"))
LIBFEM.A_OBJS=$(LIBFEM.A_FILES:%=$(ROOT)/$(BUILD)/%.o)
-include $(LIBFEM.A_OBJS:.o=.d)

build-libfem.a: build-libpng.a pre-libfem.a $(LIBFEM.A)
	$(call complete_target,$(shell basename $(LIBFEM.A)))

clean-libfem.a: clean-libpng.a
	$(call clean_target,$(shell basename $(LIBFEM.A)))
	if [ -e "$(LIBFEM.A)" ]; then rm $(LIBFEM.A); fi

pre-libfem.a:
	$(call scan_target,$(shell basename $(LIBFEM.A)))

$(LIBFEM.A): $(LIBFEM.A_OBJS) FORCE
	$(call print_link_lib,$(shell basename $(LIBFEM.A)))
	ar rcs $@ $(LIBFEM.A_OBJS)
	mkdir -p $(ROOT)/tmp/libpng.a && cd $(ROOT)/tmp/libpng.a && ar x $(ROOT)/build/libpng.a/lib/libpng.a && ar qc $(ROOT)/$@ $(ROOT)/tmp/libpng.a/*.o && rm -rf $(ROOT)/tmp/libpng.a

install-libfem.a: build-libfem.a
	$(call install_target,$(shell basename $(LIBFEM.A)))
	mkdir -p $(INSTALL_PATH)/lib/
	mkdir -p $(INSTALL_PATH)/include/$(NAME)/
	cp $(LIBFEM.A) $(INSTALL_PATH)/lib
	if [ ! -z "$(INCLUDE_DIR)" ]; then cp -R $(INCLUDE_DIR)/ $(INSTALL_PATH)/include/$(NAME)/; fi
	if [ ! -z "$(shell find $(SOURCE_DIR) -name "*.h")" ]; then cd $(SOURCE_DIR) && cp --parents $(sehll cd $(SOURCE_DIR) && find . -name "*.h") $(INSTALL_PATH)/include/$(NAME); fi
	if [ ! -z "$(shell find $(SOURCE_DIR) -name "*.hpp")" ]; then cd $(SOURCE_DIR) && cp --parents $(sehll cd $(SOURCE_DIR) && find . -name "*.hpp") $(INSTALL_PATH)/include/$(NAME); fi

uninstall-libfem.a:
	$(call uninstall_target,$(shell basename $(LIBFEM.A)))
	if [ ! -e "$(INSTALL_PATH)/lib/$(shell basename $(LIBFEM.A))" ]; then rm $(INSTALL_PATH)/lib/$(shell basename $(LIBFEM.A)); fi
	if [ ! -e "$(INSTALL_PATH)/include/$(NAME)" ]; then rm $(INSTALL_PATH)/include/$(NAME) -r; fi

# }}}
# LIBPNG.A {{{

build-libpng.a: pre-libpng.a
	if [ ! -f "libpng/configure" ]; then $(call print_run_cmd,autogen.sh) && cd libpng && ./autogen.sh; fi
	if [ ! -f "libpng/Makefile" ]; then $(call print_run_cmd,configure) && cd libpng && ./configure --prefix=$(ROOT)/build/libpng.a; fi
	if [ ! -d "$(ROOT)/build/libpng.a" ]; then $(call print_run_cmd,make) && cd libpng && make install; fi
	$(call complete_target,libpng.a)

clean-libpng.a:
	$(call clean_target,libpng.a)
	if [ -e "libpng/Makefile" ]; then cd libpng && make clean && rm Makefile; fi
	if [ -d "$(ROOT)/build/libpng.a" ]; then rm $(ROOT)/build/libpng.a -r; fi

pre-libpng.a:
	$(call scan_target,libpng.a)

# }}}

$(ROOT)/$(BUILD)/%.cpp.o: %.cpp
	mkdir -p $(@D)
	$(call print_build_cpp,$@)
	$(CXX) $(CXXFLAGS) -MMD -c $(COMMON_INCLUDE) $< -o $@

$(ROOT)/$(BUILD)/%.c.o: %.c
	mkdir -p $(@D)
	$(call print_build_c,$@)
	$(CC) $(CCFLAGS) -MMD -c $(COMMON_INCLUDE) $< -o $@

FORCE:
