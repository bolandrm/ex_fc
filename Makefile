all: build_pru0

.PHONY: build_pru0
build_pru0:
	make -C src/rc_pru0
