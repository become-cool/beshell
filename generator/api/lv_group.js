module.exports = {
    "lv_group_del": [null, [], "void", false],
    "lv_group_set_default": [null, [], "void", false],
    "lv_group_add_obj": [null, [["lv_obj_t *","obj"]], "void", false],
    "lv_group_swap_obj": [null, [["lv_obj_t *","obj1"],["lv_obj_t *","obj2"]], "void", true],
    "lv_group_remove_obj": [null, [["lv_obj_t *","obj"]], "void", true],
    "lv_group_remove_all_objs": [null, [], "void", false],
    "lv_group_focus_obj": [null, [["lv_obj_t *","obj"]], "void", true],
    "lv_group_focus_next": [null, [], "void", false],
    "lv_group_focus_prev": [null, [], "void", false],
    "lv_group_focus_freeze": [null, [["bool","en"]], "void", false],
    "lv_group_send_data": [null, [["uint32_t","c"]], "lv_res_t", false],
    // "lv_group_set_focus_cb": [null, [["lv_group_focus_cb_t","focus_cb"]], "void", false],
    "lv_group_set_refocus_policy": [null, [["lv_group_refocus_policy_t","policy"]], "void", false],
    "lv_group_set_editing": [null, [["bool","edit"]], "void", false],
    "lv_group_set_wrap": [null, [["bool","en"]], "void", false],
    "lv_group_get_focused": [null, [], "_lv_obj_t *", false],
    // "lv_group_get_focus_cb": [null, [], "lv_group_focus_cb_t", false],
    "lv_group_get_editing": [null, [], "bool", false],
    "lv_group_get_wrap": [null, [], "bool", false],
    "lv_group_get_obj_count": [null, [], "uint32_t", false],
}
