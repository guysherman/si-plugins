#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

include Makefile.mk

all: libs plugins gen

# --------------------------------------------------------------

libs:
ifeq ($(HAVE_DGL),true)
	$(MAKE) -C dpf/dgl
endif

plugins: libs
	#$(MAKE) all -C plugins/Info
	#$(MAKE) all -C plugins/Latency
	#$(MAKE) all -C plugins/Meters
	#$(MAKE) all -C plugins/Parameters
	#$(MAKE) all -C plugins/States
	$(MAKE) all -C plugins/si-d1
	$(MAKE) all -C plugins/si-d2
	$(MAKE) all -C plugins/si-l1
	$(MAKE) all -C plugins/si-h1
	$(MAKE) all -C plugins/si-l2
	$(MAKE) all -C plugins/si-h2

gen: plugins dpf/utils/lv2_ttl_generator
ifneq ($(MINGW64),true)
ifneq ($(MINGW32),true)
	@$(CURDIR)/dpf/utils/generate-ttl.sh
endif
endif
ifeq ($(MACOS),true)
	@$(CURDIR)/dpf/utils/generate-vst-bundles.sh
endif

dpf/utils/lv2_ttl_generator:
	$(MAKE) -C dpf/utils/lv2-ttl-generator

# --------------------------------------------------------------

clean:
ifeq ($(HAVE_DGL),true)
	$(MAKE) clean -C dpf/dgl
endif
	$(MAKE) clean -C dpf/utils/lv2-ttl-generator
	#$(MAKE) clean -C plugins/Info
	#$(MAKE) clean -C plugins/Latency
	#$(MAKE) clean -C plugins/Meters
	#$(MAKE) clean -C plugins/Parameters
	#$(MAKE) clean -C plugins/States
	$(MAKE) clean -C plugins/si-d1
	$(MAKE) clean -C plugins/si-d2
	$(MAKE) clean -C plugins/si-l1
	$(MAKE) clean -C plugins/si-h1
	$(MAKE) clean -C plugins/si-l2
	$(MAKE) clean -C plugins/si-h2


# --------------------------------------------------------------

.PHONY: plugins
