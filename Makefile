REMOTE_USER=root@bbe
REMOTE_DEST=~/dev/ex_fc

ifeq ($(MIX_ENV),prod)
all: build_pru1 build_pru_loader
else
all: build_fake_pru_loader
endif

.PHONY: build_pru1
build_pru1:
	make -C src/rc_pru1

.PHONY: build_pru_loader
build_pru_loader:
	make -C src/pru_loader

.PHONY: push
push:
	rsync -av --exclude=_build --exclude="priv/rc_pru1" --exclude="priv/pru_loader.so" --exclude=deps --exclude=".git" --delete . $(REMOTE_USER):$(REMOTE_DEST)
	#ssh $(REMOTE_USER) "cd $(REMOTE_DEST); MIX_ENV=prod mix deps.get"
	ssh $(REMOTE_USER) "cd $(REMOTE_DEST); MIX_ENV=prod mix compile"
	#ssh $(REMOTE_USER) "MIX_ENV=prod cd $(REMOTE_DEST) elixir --detached -S mix run"

.PHONY: run
run:
	MIX_ENV=prod iex -S mix run --no-halt

.PHONY: build_fake_pru_loader
build_fake_pru_loader:
	make -C src/fake_pru_loader
