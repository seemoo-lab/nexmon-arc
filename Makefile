all: buildtools 

buildtools: FORCE
	@printf "\033[0;31m  BUILDING BUILDTOOLS\033[0m\n"
	$(Q)make -C $@

FORCE:
