all:
	ninja -C build

config:
	cd build; cmake -G Ninja -S .. -B . ; cd -

debug:
	gdbgui build/beshell

help:
	@echo "make"
	@echo "make config		# Ninja"
	@echo "make debug		# gdbgui"
	@echo "rlwrap build/beshell"


	