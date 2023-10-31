all:
	cd build; emmake make -j8
	cp build/beshell.js ../../../beconsole.cloud/public/vm/last
	cp build/beshell.wasm ../../../beconsole.cloud/public/vm/last

cmake:
	cd build; emcmake cmake ..

fs: 
	mkdir -p tmp
	rm -rf tmp/*
	cp -af ../../filesystem/root/. tmp
	rm -rf tmp/home/become/* 
	rm -rf tmp/mnt/*
	find tmp -name "*.js.bin" -exec rm {} \;
	cp -rf src.fs/* tmp/
	rm -rf build/fs.zip
	rm -rf build/fs.git.zip
	cd tmp; zip -r ../build/fs.git.zip .
	rm -rf tmp/.git
	find tmp -name ".gitignore" -exec rm {} \;
	cd tmp; zip -r ../build/fs.zip .
	cp build/fs.zip ../../../beconsole.cloud/public/vm/last


help:
	@echo ". ../emsdk/emsdk_env.fish"
	@echo "make"
	@echo "make cmake"


	