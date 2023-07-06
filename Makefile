MODULES = libk libc kernel

# We don't actually need these flags. CLion needs them to understand
# the include structure, since it can't read inner Makefiles.
CFLAGS = -Ikernel/include -Ilibc/include -Ilibk/include -I./include

.PHONY: all clean $(MODULES)

all: $(MODULES)

$(MODULES):
	$(MAKE) -C $@

clean:
	$(RM) -r ./build
	for module in $(MODULES); do \
  		$(MAKE) -C $$module clean; \
  	done

.PHONY: all clean