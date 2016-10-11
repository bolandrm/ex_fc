REMOTE_USER=root@bbe
REMOTE_DEST=~/dev/ex_fc

ifeq ($(MIX_ENV),prod)
all: build_pru0 build_pru_loader
else
all: build_fake_pru_loader
endif

.PHONY: build_pru0
build_pru0:
	make -C src/rc_pru0

.PHONY: build_pru_loader
build_pru_loader:
	make -C src/pru_loader

.PHONY: up
up:
	rsync -av --progress --exclude=_build --exclude=priv --exclude=deps --exclude=".git" --delete . $(REMOTE_USER):$(REMOTE_DEST)
	ssh $(REMOTE_USER) "MIX_ENV=prod cd $(REMOTE_DEST) mix deps.get"
	ssh $(REMOTE_USER) "MIX_ENV=prod cd $(REMOTE_DEST) mix compile"
	#ssh $(REMOTE_USER) "MIX_ENV=prod cd $(REMOTE_DEST) elixir --detached -S mix run"

.PHONY: run
run:
	MIX_ENV=prod iex -S mix run --no-halt

.PHONY: build_fake_pru_loader
build_fake_pru_loader:
	make -C src/fake_pru_loader
