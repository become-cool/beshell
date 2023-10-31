CC = emcc
CFLAGS = -Wall

wsam_base_env.js: main.c
	$(CC) $(CFLAGS) -O0 -o $@ $< -lidbfs.js --preload-file files
	cp wsam_base_env.* ../../../beconsole.cloud/public/vm


clean:
	rm wsam_base_env.wasm wsam_base_env.js

