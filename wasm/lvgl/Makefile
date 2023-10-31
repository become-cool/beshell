all: 
	cd build; emmake make -j8 ;
	cp build/lvgl_api.js ../../beconsole.next/public/lib/lvgl.js/ ;
	cp build/lvgl.ts ../../beconsole.next/src/ ;
	cp build/lvgl_api.wasm ../../beconsole.next/public/lib/lvgl.js/ ;
	cp build/lvgl_api.wasm.map ../../beconsole.next/public/lib/lvgl.js/ ;

cmake:
	cd build; emcmake cmake .

dev:
	emrun --port 8080 --serve_root / .

gen:
	cd ../generator/wasm; node generate-wasm.js
	cd ../generator/wasm; node generate-wasm-ts.js

