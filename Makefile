MODULES = libc kernel

# We don't actually need these flags. CLion needs them to understand
# the include structure, since it can't read inner Makefiles.
CFLAGS = -Ikernel/include -Ilibc/include -Ilibk/include -I./include

MAKEFLAGS = all

debug: MAKEFLAGS += debug

.PHONY: all clean debug iso debugiso $(MODULES)

all: $(MODULES)

debug: $(MODULES)

iso:
	cd ./tools; ./iso.sh

debugiso:
	cd ./tools; ./iso.sh debug

$(MODULES):
	$(MAKE) -C $@

clean:
	$(RM) -r ./build
	for module in $(MODULES); do \
  		$(MAKE) -C $$module clean; \
  	done

.PHONY: all debug clean iso debugiso