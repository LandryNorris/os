MODULES = libc kernel

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