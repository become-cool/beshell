set(EXPORTED_FUNCTIONS "EXPORTED_FUNCTIONS='[_main, _malloc, _free, _memcpy")
set(EXPORTED_FUNCTIONS "${EXPORTED_FUNCTIONS}, _js_main_loop_tick, _wasm_set_fs_prefix, _be_telnet_wasm_send, _be_exit, _be_lv_tick")

# AUTO GENERATE CODE START [EXPORTED FUNCTIONS] --------
# AUTO GENERATE CODE END [EXPORTED FUNCTIONS] --------

set(EXPORTED_FUNCTIONS "${EXPORTED_FUNCTIONS}]'")
set(LINK_FLAGS "${LINK_FLAGS} -s ${EXPORTED_FUNCTIONS}")