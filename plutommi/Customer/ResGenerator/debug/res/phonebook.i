# 1 "temp/res/phonebook.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "temp/res/phonebook.c"
# 594 "temp/res/phonebook.c"
# 1 "../../mmi/inc/mmi_features.h" 1
# 69 "../../mmi/inc/mmi_features.h"
# 1 "../../mmi/inc/MMI_features_switch.h" 1
# 67 "../../mmi/inc/MMI_features_switch.h"
# 1 "../../mmi/inc/MMI_features_type.h" 1
# 68 "../../mmi/inc/MMI_features_switch.h" 2
# 70 "../../mmi/inc/mmi_features.h" 2
# 595 "temp/res/phonebook.c" 2
# 1 "../customerinc/custresdef.h" 1
# 195 "../customerinc/custresdef.h"
# 1 "../../mmi/inc/MMI_features.h" 1
# 196 "../customerinc/custresdef.h" 2
# 596 "temp/res/phonebook.c" 2
# 1 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h" 1
# 444 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
typedef enum
{
    MMI_PHB_IMAGE_NO_SELECT,
    MMI_PHB_IMAGE_FOR_ENTRY,
    MMI_PHB_IMAGE_SELECT_PATH,
    MMI_PHB_IMAGE_CALLER_GROUP,
    MMI_PHB_SWFLASH_SELECT_PATH,
    MMI_PHB_AVATAR_SELECT_PATH,
    MMI_PHB_IMAGE_LOCATION_ENUM_TOTAL
} mmi_phb_image_location_enum;


typedef enum
{
    MMI_PHB_VOICE_CALL = 0,




    MMI_PHB_SEND_MSG,
    MMI_PHB_EDIT,
    MMI_PHB_TOOLBAR_ENUM_TOTAL
} mmi_phb_fte_toolbar_enum;
# 482 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
typedef struct mmi_phb_setting_cntx
{
    mmi_menu_id highlighted_menu_id;
    U16 prefer_storage;







}mmi_phb_setting_cntx_struct;
# 522 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern void mmi_phb_launch_entry(void);
extern srv_phb_filter_struct *mmi_phb_list_build_filter(
                                  U8 storage,
                                  U32 field_filter,
                                  srv_phb_group_filter_struct* group_filter,
                                  U16* key_word,
                                  U16 input_mode);

extern void mmi_phb_list_free_filter(srv_phb_filter_struct *filter);


extern void mmi_phb_list_highlight_handler(S32 item_index);
# 543 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern mmi_ret mmi_phb_choose_number_scrn_proc(mmi_event_struct *param);

extern void mmi_phb_entry_choose_field_ex(
                mmi_id sg_id,
                WCHAR *name,
                U8 storage,
                const U8* select_type,
                U32 field_mask,
                FunctionPtr entry_func,
                FunctionPtr lsk_func,
                FunctionPtr rsk_func);

extern void mmi_phb_entry_choose_field(
                mmi_id sg_id,
                U16 store_index,
                U32 field_type,
                FunctionPtr entry_func,
                FunctionPtr lsk_func,
                FunctionPtr rsk_func,
                FunctionPtr send_key_func,
                MMI_BOOL show_if_empty,
                MMI_BOOL start_primitive);

extern void mmi_phb_entry_list_ext(
                mmi_id sg_id,
                U16 entry_count,
                U16 str_title,
                U16 img_title,
                U16 str_lsk,
                U16 img_lsk,
                U16 scr_id,
                U16 img_csk,
                HighlightPtr highlight_ptr,
                FunctionPtr lsk_ptr,
                FunctionPtr csk_ptr,
                FunctionPtr send_key_ptr,
                FunctionPtr ip_key_ptr,
                GetAsyncItemFuncPtr item_ptr,
                GetAsyncHintFuncPtr hint_ptr,
                FunctionPtr entry_ptr,
                FunctionPtr exit_ptr,
                MMI_BOOL alpha_index);
# 605 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern void mmi_phb_list_select_entry(void);




extern void mmi_phb_op_cntx_init(mmi_phb_op_action_cntx_struct *op_cntx);
extern void mmi_phb_op_start(mmi_phb_op_action_cntx_struct *op_action, MMI_ID group_id);







extern void mmi_phb_sms_send_data_for_entry(CHAR *number, MMI_ID group_id);
extern void mmi_phb_sms_send_data_choose_entry(U16 store_index, MMI_ID group_id);
# 638 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern void mmi_phb_dial_normal_call(void);
extern void mmi_phb_dial_ip_call(void);

extern void mmi_phb_entry_op_option(void);
# 655 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern void mmi_phb_op_save_contact_ex(void);

extern void mmi_phb_entry_op_save_contact_cancel(void);
extern void mmi_phb_entry_op_save_contact_confirm(void);
extern void mmi_phb_entry_op_view_entry_options(void);
# 675 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern void mmi_phb_op_view_pre_entry(void);
extern U32 mmi_phb_get_field_mask_by_field_type(U8 field_type);
extern U8 mmi_phb_get_field_type_by_field_mask(U32 field_mask);
extern U16 mmi_phb_get_str_by_field_type(U8 field_type, U8 storage);
extern U16 mmi_phb_get_img_by_field_type(U8 field_type);
extern U16 mmi_phb_get_img_by_number_type(mmi_phb_num_type_enum number_type);


extern MMI_BOOL mmi_phb_op_check_pre_save(mmi_phb_contact_info_struct *contact, U8 storage, MMI_BOOL popup_error);
extern MMI_BOOL mmi_phb_check_valid_number(U16 *number, U8 storage);
# 695 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern void mmi_phb_op_email_custom_function(void);
# 715 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern void mmi_phb_contact_build_res(void);
extern void mmi_phb_contact_res_config(void);
extern void mmi_phb_contact_res_value_assign(
                mmi_id sg_id,
                mmi_phb_caller_res_struct *caller_res,
                mmi_phb_contact_info_struct *contact_cntx);
extern void mmi_phb_contact_res_value_save(
                mmi_phb_contact_info_struct *contact_cntx,
                mmi_phb_caller_res_struct *caller_res);

extern void mmi_phb_entry_image_view(void);
extern void mmi_phb_entry_image_option(void);


extern void mmi_phb_image_select_from_file_mamager(void);





extern void mmi_phb_ring_tone_highlight_hdlr(S32 ringToneIndex);
# 749 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern void mmi_phb_setting_init(void);
extern U8 mmi_phb_get_speak_name(void);
extern U16 mmi_phb_get_preferred_storage(void);







extern U32 mmi_phb_contact_build_field_mask(mmi_phb_contact_info_struct *contact);
extern void mmi_phb_truncate_number_field_to_max_len(U16 *ucs2_num_dest, U16 *ucs2_num_src, U16 number_length);
extern U32 mmi_phb_get_name_length(U8 storage);
extern void mmi_phb_contact_get_item_image(U16 store_index, U8** image_buffer1, U8** image_buffer2);
extern mmi_phb_contact_info_struct* mmi_phb_contact_get_contact(U16 store_index);
extern void mmi_phb_contact_free_contact(mmi_phb_contact_info_struct *contact);
extern void mmi_phb_contact_get_contact_info(U16 store_index, mmi_phb_contact_info_struct* contact);
extern srv_phb_contact_info_struct* mmi_phb_contact_build(mmi_phb_contact_info_struct *contact);



extern void mmi_phb_contact_srv_to_mmi(srv_phb_contact_data_struct *srv_contact, mmi_phb_contact_info_struct *mmi_contact);
extern MMI_BOOL mmi_phb_check_storage_full(U8 storage, MMI_BOOL is_popup);
extern srv_phb_contact_info_struct *mmi_phb_get_contact_build_buffer(mmi_phb_contact_info_struct* contact);
extern void mmi_phb_get_contact_free_buffer(srv_phb_contact_info_struct *contact);


extern void mmi_phb_show_in_progress(mmi_id, U16);

extern void mmi_phb_entry_not_ready(U16 str_id);
# 792 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern void mmi_phb_entry_group_list_non_tab(void);





extern void mmi_phb_sos_list_init(void);
extern MMI_BOOL mmi_phb_check_extra_number_available(void);

extern void *mmi_phb_construct_group_editor(phb_storage_enum storage);
extern void mmi_phb_popup_display(UI_string_type text, mmi_event_notify_enum event_id);
extern void mmi_phb_popup_display_ext(MMI_STR_ID title_id, mmi_event_notify_enum event_id);
extern void mmi_phb_confirm_display(UI_string_type text, mmi_id parent_id, FuncPtr lsk_func, FuncPtr rsk_func, MMI_BOOL is_enable_csk);
extern void mmi_phb_confirm_display_ext(MMI_STR_ID title_id, mmi_id parent_id, FuncPtr lsk_func, FuncPtr rsk_func, MMI_BOOL is_enable_csk);
extern void mmi_phb_confirm_display_handle(mmi_alert_result_evt_struct *alert_result);
# 824 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern void mmi_phb_op_add_pre_entry(mmi_phb_save_contact_cntx_struct *save_cntx);
extern void mmi_phb_op_add_new_entry_ext(MMI_ID group_id);





extern mmi_ret mmi_phb_sim_editor_proc(mmi_event_struct *evt);
extern mmi_ret mmi_phb_phone_editor_proc(mmi_event_struct *evt);
# 842 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern void mmi_phb_view_entry_hdlr(mmi_phb_view_contact_cntx_struct *cntx, mmi_phb_op_action_enum op_action);
extern void mmi_phb_entry_op_view_entry(mmi_scrn_essential_struct* scrn_data);





extern void mmi_phb_view_contact_op_edit_entry_ext(void);
extern void mmi_phb_op_edit_pre_entry_ext(MMI_ID group_id);

extern void mmi_phb_op_edit_highlight_hdlr(S32);






extern void mmi_phb_op_delete_update_index(U16 store_index);
# 869 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern void mmi_phb_entry_image_view_default_pic_ext(mmi_scrn_essential_struct *scrn_data);

extern void mmi_phb_edit_option_image_highlight_hdlr(mmi_id menu_id);



extern BOOL mmi_phb_image_check_image_file(U16 store_index);
extern void mmi_phb_image_view_from_phb_folder(mmi_scrn_essential_struct *scrn_data);
extern void mmi_phb_image_select_from_file_mamager(void);
extern void mmi_phb_image_select_from_file_mamager_done(void);




extern BOOL mmi_phb_image_update_by_index(U16);
extern void mmi_phb_image_delete_by_index(U16);
extern BOOL mmi_phb_image_duplicate_by_index(U16 dst_record, U16 src_record);
# 894 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern MMI_BOOL mmi_phb_image_check_valid_image(CHAR *path, MMI_BOOL auto_resize);
extern void mmi_phb_image_preview_callback(S32 result);
extern void mmi_phb_image_init_folder(void);
# 970 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern void mmi_phb_at_cmd_edit(void *);
extern void mmi_phb_at_cmd_add(void *);
extern void mmi_phb_at_cmd_delete(void *);
extern void mmi_phb_at_cmd_delete_all(void *);




extern void mmi_phb_op_delete_all_update_index(U8 storage);
extern void mmi_phb_op_delete_all_update_optioal_id(void);
extern void mmi_phb_op_update_optional_field(U16 store_index, U8 storage, U8 type);
extern void mmi_phb_clear_optional_ids(U16 store_index);
extern void mmi_phb_util_clear_info_buffer(void);





extern BOOL mmi_phb_copy_optional_ids(U16 dst_store_index, U16 src_store_index);
extern void mmi_phb_op_set_entry_prepare_name_number_by_index(void *dst_msg, U16 store_index);
extern void mmi_phb_op_set_entry_rsp_name_number(void *rsp_msg, U16 store_index);
extern U16 mmi_phb_op_get_store_index_and_increase_counter(U8 storage, U16 record_index);
extern U16 mmi_phb_op_clear_lookup_table_by_index(U16 store_index);







extern void mmi_phb_ind_startup_finish_after_name_sort(void);
# 1032 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern S32 mmi_phb_compare_chars(U16 char1, U16 char2);
# 1043 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern void mmi_phb_lookup_table_sort(void);
extern void mmi_phb_lookup_table_quicksort(U16, U16);
extern void mmi_phb_lookup_table_insertionsort(U16, U16);
extern void mmi_phb_lookup_table_swap_node(U16, U16);
extern void mmi_phb_util_read_compare_length(void);






extern void mmi_phb_highlight_setting(void);
# 1092 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern void mmi_phb_ringtone_select_from_file_mamager(void);
extern void mmi_phb_edit_option_ringtone_highlight_hdlr(mmi_id menu_id);


extern void mmi_phb_stop_ring_tone_id(void);





extern void mmi_phb_build_image_list(void);
# 1136 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern void mmi_phb_entry_fmgr_save_option(void);
extern void mmi_phb_exit_fmgr_save_option(void);
extern void mmi_phb_fmgr_pre_add_entry(void);
extern void mmi_phb_fmgr_pre_edit_entry(U16 store_index);
extern S32 mmi_phb_fmgr_edit_list_get_hint(S32 item_index, UI_string_type *hint_array);
extern void mmi_phb_fmgr_pre_edit_callergrp(void);







extern U16 mmi_phb_convert_to_0x81_get_base(U16 code);
extern U8 mmi_phb_convert_to_0x81_get_offset(U16 base, U16 code);
# 1183 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern MMI_BOOL mmi_phb_is_num_in_bcd(const U16* source, U8 num, U16 len_in_bytes);
# 1193 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern MMI_BOOL mmi_phb_og_start_browser(U16 store_index, MMI_ID group_id);
# 1261 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern void mmi_phb_op_copy_all_confirm(MMI_ID group_id);
extern void mmi_phb_op_move_all_confirm(MMI_ID group_id);
extern U16 mmi_phb_op_copy_move_all_get_confirm_str(U16 dst_storage);
extern void mmi_phb_move_all_pre_req(MMI_ID group_id);
extern void mmi_phb_move_all_copy_call_back(U16 error_result, U16 store_index);
extern void mmi_phb_move_all_call_back(U16 result, U16 store_index);
# 1275 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern void mmi_phb_show_mass_processing(mmi_id sg_id, U16 msg, U16 total_count);

extern void mmi_phb_mass_processing_abort(void);
extern void mmi_phb_end_key_save_contact(void);

extern mmi_ret mmi_phb_update_mass_processing(mmi_event_struct* param);



extern MMI_BOOL mmi_phb_check_storage_status(U8, mmi_phb_check_storage_enum, MMI_BOOL);
extern MMI_BOOL mmi_phb_op_all_check_storage_status(U8 storage, mmi_phb_check_storage_enum check_type, MMI_BOOL need_popup, MMI_ID group_id);
extern MMI_BOOL mmi_phb_get_fdn_status(U8 storage);


extern void mmi_phb_entry_delete_all_no_sim(void);


extern void mmi_phb_continue_send_multi_message_confirm(U16, MenucuiPtr, MenucuiPtr, MMI_ID);
# 1305 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern void mmi_phb_entry_choose_caller_pic(void);

extern void mmi_phb_entry_image_view_ext(MMI_ID group_id);







extern mmi_ret mmi_phb_edit_menu_cui_proc(mmi_event_struct *evt);
extern mmi_ret mmi_phb_number_editor_proc(mmi_event_struct *evt);
extern void mmi_phb_sg_entry_setting(MMI_ID group_id);
extern void mmi_phb_entry_group_speed_dial(void);
# 1332 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern void mmi_phb_entry_group_my_number(void);
extern void mmi_phb_entry_group_extra_number(void);





extern mmi_ret mmi_phb_mark_several_menu_cui_proc(mmi_event_struct *evt);
# 1352 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern mmi_ret mmi_phb_choose_storage_menu_cui_proc(mmi_event_struct *evt);
extern mmi_ret mmi_phb_main_list_menu_cui_proc(mmi_event_struct *evt);
extern mmi_ret mmi_phb_view_contact_menu_cui_proc(mmi_event_struct *evt);






extern mmi_ret mmi_phb_image_edit_menu_cui_proc(mmi_event_struct *evt);



extern mmi_ret mmi_phb_fmgr_edit_menu_cui_proc(mmi_event_struct *evt);
# 1377 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern void mmi_phb_op_add_new_entry(void);






extern void mmi_phb_select_image_preview(mmi_scrn_essential_struct* scrn_data);
extern void mmi_phb_select_image(MMI_ID group_id);


extern void mmi_phb_search_contact_pre_run(void);
extern void mmi_phb_search_contact_run(void);
extern void mmi_phb_search_contact_continue(MMI_ID group_id);





extern MMI_ID mmi_phb_entry_menu_cui_generic(MMI_ID parent_gid, MMI_MENU_ID menu_id, U16 title_id);

extern MMI_ID mmi_phb_entry_menu_cui_generic_ext(
                MMI_ID parent_gid,
                cui_menu_type_enum menu_type,
                MMI_MENU_ID menu_id,
                U16 title_id,
                void* parent_data);
# 1438 "../../mmi/phonebook/phonebookinc/PhoneBookProt.h"
extern MMI_BOOL mmi_phb_sim_ctrl_not_available_hide_menu(mmi_sim_enum mmi_sim, MMI_ID inline_gid, U16 menu_id, U16 string_id);
extern void mmi_phb_new_entry_choose_storage_check(mmi_id sender_id, U16 menu_id, U16 string_id);
# 597 "temp/res/phonebook.c" 2
# 1 "../../mmi/phonebook/core/PhoneBookCore.h" 1
# 414 "../../mmi/phonebook/core/PhoneBookCore.h"
typedef enum
{
    MMI_PHB_FIELD_NAME,






    MMI_PHB_FIELD_NUMBER,
# 441 "../../mmi/phonebook/core/PhoneBookCore.h"
    MMI_PHB_FIELD_PIC,
    MMI_PHB_FIELD_RING,



    MMI_PHB_FIELD_GROUP,
# 483 "../../mmi/phonebook/core/PhoneBookCore.h"
    MMI_PHB_FIELD_TOTAL
} mmi_phb_field_enum;
# 501 "../../mmi/phonebook/core/PhoneBookCore.h"
typedef enum
{
    MMI_PHB_SG_MAIN,
    MMI_PHB_SG_RES,
    MMI_PHB_SG_MARK_SEVERAL,
    MMI_PHB_SG_GROUP,
    MMI_PHB_SG_SETTING,

    MMI_PHB_SG_CUI_SAVE,
    MMI_PHB_SG_CUI_VIEW,
    MMI_PHB_SG_CUI_LIST,
    MMI_PHB_SG_CUI_LIST_MULTI,
    MMI_PHB_SG_CUI_LIST_GROUP_TAB,

    MMI_PHB_SG_TOTAL
} mmi_phb_sg_type_enum;

typedef enum
{
    MMI_PHB_CHECK_STORAGE_EMPTY,
    MMI_PHB_CHECK_STORAGE_FULL,
    MMI_PHB_CHECK_STORAGE_BOTH,
    MMI_PHB_CHECK_STORAGE_TOTAL
}mmi_phb_check_storage_enum;

typedef enum
{
    MMI_PHB_OP_NONE,
    MMI_PHB_OP_ADD,
    MMI_PHB_OP_EDIT,
    MMI_PHB_OP_DELETE,
    MMI_PHB_OP_COPY,
    MMI_PHB_OP_MOVE,
    MMI_PHB_OP_COPY_ALL,
    MMI_PHB_OP_DELETE_ALL,
    MMI_PHB_OP_MOVE_ALL,
    MMI_PHB_OP_MULTI_DELETE,
    MMI_PHB_OP_MULTI_COPY,
    MMI_PHB_OP_MULTI_MOVE,

    MMI_PHB_OP_CALL,
    MMI_PHB_OP_CALL_SEND_KEY,
    MMI_PHB_OP_VIDEO_CALL,
    MMI_PHB_OP_IP_CALL,
    MMI_PHB_OP_VOICE_CALL,
    MMI_PHB_OP_MSG,
    MMI_PHB_OP_SMS,
    MMI_PHB_OP_MMS,
    MMI_PHB_OP_EMAIL,
    MMI_PHB_OP_SEND_CARD,
    MMI_PHB_OP_IM,
    MMI_PHB_OP_POSTCARD,

    MMI_PHB_OP_TOTAL
} mmi_phb_op_action_enum;


typedef enum
{
    MMI_PHB_STATE_IDLE,
    MMI_PHB_STATE_READ = 0x01,
    MMI_PHB_STATE_UPDATE = 0x02,
    MMI_PHB_STATE_COPY_MULTI = 0x04,
    MMI_PHB_STATE_DELETE_MULTI = 0x08,




    MMI_PHB_STATE_DM = 0x20,
    MMI_PHB_STATE_TOTAL
} mmi_phb_state_enum;

typedef enum
{
    MMI_PHB_GRP_MEM_OP_NONE,
    MMI_PHB_GRP_MEM_OP_MS_ADD,
    MMI_PHB_GRP_MEM_OP_MS_MOVE,
    MMI_PHB_GRP_MEM_OP_MS_REMOVE,
    MMI_PHB_GRP_MEM_OP_ADD,
    MMI_PHB_GRP_MEM_OP_MOVE,
    MMI_PHB_GRP_MEM_OP_REMOVE,
    MMI_PHB_GRP_MEM_OP_REMOVE_ALL,
    MMI_PHB_GRP_MEM_OP_TOTAL_TOTAL
} mmi_phb_group_member_op_enum;
# 613 "../../mmi/phonebook/core/PhoneBookCore.h"
typedef struct
{
    MMI_ID sg_id;
    U16 path[SRV_FMGR_PATH_MAX_LEN + 1];
    U8 type;
} mmi_phb_select_image_struct;


typedef struct
{
    U16 store_index;
    MMI_BOOL ip_call;
} mmi_phb_choose_number_struct;


typedef struct
{
    mmi_phb_op_action_enum op_action;
    U16 *data;
    U32 data_type;
    U16 store_index;
    MMI_BOOL ip_call;
} mmi_phb_op_action_cntx_struct;


typedef struct
{
    U8 storage;
    U32 field_type;
    srv_phb_contact_data_struct* phb_data;
} mmi_phb_unsaved_data_struct;

typedef struct mmi_phb_unsaved_data_node_struct
{
    struct mmi_phb_unsaved_data_node_struct* next;
    mmi_phb_unsaved_data_struct* object;
} mmi_phb_unsaved_data_node_struct;


typedef struct
{
    mmi_id sg_id;
    U8 src_storage;
    U8 dst_storage;
    U8 state;
        U8 extra_handler;
    mmi_phb_op_action_enum action;
    U16 store_index;
    MMI_BOOL canceled;
    MMI_BOOL end_key_press;

    U16 total_count;
    U16 active_index;

}mmi_phb_op_cntx_struct;


typedef struct
{
    mmi_id sg_id;
    MenucuiPtr LSK_function;
    MenucuiPtr CSK_function;
    MenucuiPtr RSK_function;
}mmi_phb_confirm_user_data_struct;


typedef struct
{
    U16 name[MMI_PHB_NAME_FIELD_LENGTH + 1];






    U16 number[MMI_PHB_NUMBER_LENGTH + 1 + 1];
# 719 "../../mmi/phonebook/core/PhoneBookCore.h"
    U32 group_id;
    U16 ring_tone_id;
    U16 ring_tone[SRV_FMGR_PATH_MAX_LEN + 1];
    U16 picture_id;
    U16 picture[SRV_FMGR_PATH_MAX_LEN + 1];
# 741 "../../mmi/phonebook/core/PhoneBookCore.h"
} mmi_phb_contact_info_struct;




typedef enum
{
    MMI_PHB_RES_RING,
    MMI_PHB_RES_IMAGE,
    MMI_PHB_RES_AVATAR_IMG,
    MMI_PHB_RES_VIDEO,
    MMI_PHB_RES_SWFLASH,
    MMI_PHB_RES_AVATAR_VIDEO,
    MMI_PHB_RES_TYPE_TOTAL
} mmi_phb_res_type_enum;

typedef struct
{
    U8 total_ring_tone;
    U16 ring_tone_list[60];
    U8 *ring_tone_name_list[60];
# 773 "../../mmi/phonebook/core/PhoneBookCore.h"
    U8 total_image_id;
    U16 image_id_list[30];
    U8 *image_name_list[30];

} mmi_phb_resource_struct;

typedef struct
{
    mmi_id sg_id;
    mmi_id child_id;

    U16 curr_res_type;

    U8 done;
    U16 highlight_index;

    U16 ring_id;
    S32 selected_ring_index;
    U8 ring_location;
    U16 ring_temp_path[SRV_FMGR_PATH_MAX_LEN + 1];
    U16 ring_path[SRV_FMGR_PATH_MAX_LEN + 1];

    S32 selected_pattern_index;

    S32 selected_alert_index;
# 808 "../../mmi/phonebook/core/PhoneBookCore.h"
    U16 img_id;
    S32 selected_pic_index;
    U8 image_location;
    U16 img_temp_path[SRV_FMGR_PATH_MAX_LEN + 1];
    U16 img_path[SRV_FMGR_PATH_MAX_LEN + 1];
} mmi_phb_caller_res_struct;




typedef struct
{
    mmi_id sg_id;
    U16 res_path[SRV_FMGR_PATH_MAX_LEN + 1];
    U16 recv_res_id;
    U8 recv_res_from;
} mmi_phb_filemgr_caller_res_struct;
# 919 "../../mmi/phonebook/core/PhoneBookCore.h"
typedef struct
{
    mmi_id sg_id;
    mmi_id parent_id;
    mmi_id child_id;
    U8 storage;
    U8 display_type;
    U16 store_index;
    U16 list_index;
    U16 *contact_array;
    U16 contact_count;

    U32 filter;

    U32 user_data;

    U16 qsearch_input_buffer[MMI_PHB_NAME_LENGTH + 1];
    srv_phb_group_filter_struct group_filter;




    U16 select_count;
    U8 bitmask[(MMI_PHB_ENTRIES + 7) / 8];
    U16 mark_count;
    U16 mark_index;
    U16 *mark_array;
    U8 *mark_field;




    U16 lsk_img;
    U16 lsk_label;




    U8 group_id;
    U8 *group_array;
# 972 "../../mmi/phonebook/core/PhoneBookCore.h"
    MMI_BOOL evt_out;
} mmi_phb_list_cntx_struct;




typedef struct
{
    mmi_id sg_id;
    mmi_id parent_id;
    mmi_id child_id;
    U32 style;
    mmi_id editor_id;
    mmi_id menu_storage_id;
    mmi_id menu_add_replace_id;
    U32 user_data;
    MMI_BOOL is_popup;
    U16 store_index;
    U16 highlight_str;
    U8 highlight_type;
    U8 first_highlight_type;
    U32 field_mask;
    U8 done_flag;
    MMI_BOOL bday_changed;



    MMI_BOOL add_detail;
    mmi_phb_caller_res_struct *res;
    mmi_phb_contact_info_struct *contact;




    U8 storage;
    mmi_phb_op_action_enum action;



    MMI_BOOL edit_temp_done;
    U16 edit_temp_buff[100];
    void *input_buffer;
    MMI_BOOL evt_out;
    MMI_BOOL is_from_email;



} mmi_phb_save_contact_cntx_struct;




typedef struct
{
    mmi_id sg_id;
    mmi_id parent_id;
    mmi_id child_id;
    U16 store_index;
    U8 highlight_type;
    U16 highlight_str;
    U8 first_highlight_type;
    U16 *select_data;
    mmi_phb_caller_res_struct *res;
    U32 field_mask;
    mmi_phb_contact_info_struct *contact;
# 1050 "../../mmi/phonebook/core/PhoneBookCore.h"
} mmi_phb_view_contact_cntx_struct;


typedef struct
{
    U8 refresh_type;
    U16 old_store_index;
    U16 new_store_index;

    mmi_id sg_id;
} mmi_phb_refresh_struct;

typedef struct
{
    U16 age;
    U16 sg_type;
    U16 parent_id;

    mmi_phb_refresh_struct refresh_data;
    FunctionPtr refresh_cb;

    void *private_data;

    mmi_event_struct* delay_event;
} mmi_phb_sg_data_struct;

typedef struct
{
    U8 highlight_index;
    U16 *number_to_dial[MMI_PHB_FIELD_TOTAL];
    U32 number_type[MMI_PHB_FIELD_TOTAL];
} mmi_phb_choose_field_struct;

typedef struct
{
    MMI_BOOL ready;
    MMI_BOOL processing;





    U16 state;




    mmi_phb_sg_data_struct* sg_stack[GRP_ID_PHB_END - GRP_ID_PHB_BEGIN];






} mmi_phb_cntx_mgr_struct;

typedef struct
{
    U32 count;
    U8 group_id_list[MMI_PHB_GROUP_COUNT];
    mmi_phb_group_member_op_enum op;
    MMI_BOOL finished;
} mmi_phb_group_member_data_struct;





extern mmi_phb_cntx_mgr_struct g_phb_cntx_mgr;
extern mmi_phb_view_contact_cntx_struct *g_mmi_phb_view_contact_p;
extern mmi_phb_list_cntx_struct *g_mmi_phb_list_p;
extern mmi_phb_contact_info_struct *g_mmi_phb_contact_p;
extern mmi_phb_save_contact_cntx_struct *g_mmi_phb_save_contact_p;
extern mmi_phb_resource_struct g_mmi_phb_resource;
extern mmi_phb_caller_res_struct *g_mmi_phb_res_p;
extern mmi_phb_choose_field_struct g_mmi_phb_choose_field;
extern mmi_phb_op_cntx_struct g_mmi_phb_op_cntx;
# 1140 "../../mmi/phonebook/core/PhoneBookCore.h"
extern void mmi_phb_cntx_mgr_init(void);
# 1152 "../../mmi/phonebook/core/PhoneBookCore.h"
extern U32 mmi_phb_cntx_mgr_create_handle(U16 sg_type);
# 1164 "../../mmi/phonebook/core/PhoneBookCore.h"
extern void mmi_phb_cntx_mgr_release_handle(U32 handle);
# 1176 "../../mmi/phonebook/core/PhoneBookCore.h"
extern U32 mmi_phb_cntx_mgr_get_handle_by_sg_id(U16 sg_id);
# 1188 "../../mmi/phonebook/core/PhoneBookCore.h"
extern U32 mmi_phb_cntx_mgr_get_handle(void);

extern mmi_phb_sg_data_struct * mmi_phb_cntx_mgr_get_instance(mmi_id sg_id);
# 1203 "../../mmi/phonebook/core/PhoneBookCore.h"
extern void mmi_phb_cntx_mgr_set_data(U32 handle, void* data);
# 1215 "../../mmi/phonebook/core/PhoneBookCore.h"
extern void* mmi_phb_cntx_mgr_get_data(U32 handle);
# 1228 "../../mmi/phonebook/core/PhoneBookCore.h"
extern void mmi_phb_cntx_mgr_set_parent_id(U32 handle, mmi_id parent_id);
# 1240 "../../mmi/phonebook/core/PhoneBookCore.h"
extern mmi_id mmi_phb_cntx_mgr_get_parent_id(U32 handle);
# 1252 "../../mmi/phonebook/core/PhoneBookCore.h"
extern U16 mmi_phb_cntx_mgr_get_sg_tpye(U32 handle);
# 1265 "../../mmi/phonebook/core/PhoneBookCore.h"
extern mmi_id mmi_phb_cntx_mgr_get_sg_id(U32 handle);
# 1277 "../../mmi/phonebook/core/PhoneBookCore.h"
extern mmi_phb_refresh_struct* mmi_phb_cntx_mgr_get_refresh_data(U32 handle);
# 1291 "../../mmi/phonebook/core/PhoneBookCore.h"
extern MMI_BOOL mmi_phb_cntx_mgr_controller(U16 state, MMI_BOOL set, void *data);
# 1308 "../../mmi/phonebook/core/PhoneBookCore.h"
extern U16 mmi_phb_cntx_mgr_get_state(void);

extern MMI_BOOL mmi_phb_cntx_mgr_state_is_set(U16 state);
# 1322 "../../mmi/phonebook/core/PhoneBookCore.h"
extern void mmi_phb_cntx_mgr_set_state(U16 state);
# 1334 "../../mmi/phonebook/core/PhoneBookCore.h"
extern void mmi_phb_cntx_mgr_reset_state(U16 state);

extern mmi_id mmi_phb_cntx_mgr_get_current_sg_id(void);
# 1362 "../../mmi/phonebook/core/PhoneBookCore.h"
extern void mmi_phb_cntx_mgr_at(U8 operation);
# 1374 "../../mmi/phonebook/core/PhoneBookCore.h"
extern void mmi_phb_cntx_mgr_clear_history(void);
extern void mmi_phb_cntx_mgr_refresh(mmi_phb_op_cntx_struct *op_cntx, U16 *contact_array, U16 contact_count, U16 store_index);
extern void mmi_phb_cntx_mgr_input_method_change(void);




extern mmi_phb_list_cntx_struct* cui_phb_list_select_contact_create_cntx(U8 list_type);
extern mmi_ret cui_phb_list_select_contact_proc(mmi_event_struct *evt);
extern mmi_ret cui_phb_list_select_group_member_proc(mmi_event_struct *evt);
extern void cui_phb_list_select_contact_set_cntx(mmi_phb_list_cntx_struct *list_cntx);
extern mmi_ret mmi_phb_launch_proc(mmi_event_struct *evt);

extern MMI_BOOL mmi_phb_display_popup_cb(
            mmi_id parent_id,
            UI_string_type string,
            mmi_event_notify_enum event_id,
            mmi_proc_func proc,
            void* user_data);

extern MMI_BOOL mmi_phb_usb_app_unavailable_popup(S32 result);
extern void mmi_phb_ucm_app_entry_error_message(void);

extern void mmi_phb_fmgr_cui_res_handle(cui_file_selector_result_event_struct *res);





extern void mmi_phb_cntx_mgr_post_evt(mmi_id sg_id, MMI_BOOL evt_wait, mmi_event_struct *evt);
# 1412 "../../mmi/phonebook/core/PhoneBookCore.h"
extern void mmi_phb_confirm_user_data_init(mmi_phb_confirm_user_data_struct * arg);
extern mmi_ret mmi_phb_comfirm_proc(mmi_event_struct *evt);
extern U16 mmi_phb_get_storage_full_str_id(U8 storage);
extern MMI_BOOL mmi_phb_check_storage(MMI_BOOL is_or, U8 storage);
# 598 "temp/res/phonebook.c" 2


<?xml version="1.0" encoding="UTF-8"?>


<APP id="APP_PHONEBOOK"



     package_name = "native.mtk.phonebook"
     name = "STR_SCR_PBOOK_CAPTION"

     img = "IMG_ID_PHB_APP_LAUNCH"

     launch = "mmi_phb_pre_entry_main_screen"






>



    <!--Include Area-->





    <INCLUDE file="GlobalResDef.h,SettingResDef.h,mmi_rp_cui_dialer_def.h,mmi_rp_app_idle_homescreen_def.h"/>

    <!-----------------------------------------------------String Resource Area----------------------------------------------------->
# 664 "temp/res/phonebook.c"
    <STRING id="STR_ID_PHB_ADD_DETAILS"/>
    <STRING id="STR_ID_PHB_DELETE_DETAILS"/>
# 717 "temp/res/phonebook.c"
    <STRING id="STR_ID_PHB_SEND_MULTI_MESSAGE_QUERY_1"/>
    <STRING id="STR_ID_PHB_SEND_MULTI_MESSAGE_QUERY_2"/>
    <STRING id="STR_ID_PHB_SEND_MULTI_MESSAGE_QUERY_3"/>
# 761 "temp/res/phonebook.c"
    <STRING id="STR_ID_PHB_SETTINGS"/>






    <STRING id="STR_ID_PHB_STORAGE_BOTH"/>
# 795 "temp/res/phonebook.c"
    <STRING id="STR_ID_PHB_COPY_SEVERAL"/>
    <STRING id="STR_ID_PHB_MOVE_SEVERAL"/>
# 844 "temp/res/phonebook.c"
    <STRING id="STR_ID_PHB_DELETE_ALL"/>
    <STRING id="STR_ID_PHB_NO_CONTACT"/>
    <STRING id="STR_ID_PHB_INVALID_LOCATION"/>
# 893 "temp/res/phonebook.c"
    <STRING id="STR_ID_PHB_TO_PHONE"/>
    <STRING id="STR_ID_PHB_FROM_PHONE"/>
# 914 "temp/res/phonebook.c"
    <STRING id="STR_ID_PHB_TO_SIM"/>
    <STRING id="STR_ID_PHB_FROM_SIM"/>
# 973 "temp/res/phonebook.c"
    <STRING id="STR_ID_PHB_UNNAMED"/>




    <STRING id="STR_ID_PHB_OPTION_VIEW_PIC"/>
# 1015 "temp/res/phonebook.c"
    <STRING id="STR_ID_PHB_FMGR_IMAGE_SELECT_FILE"/>





    <STRING id="STR_ID_PHB_FMGR_IMAGE_NO_SELECT"/>

    <STRING id="STR_ID_PHB_FMGR_TO_NEW_ENTRY"/>
    <STRING id="STR_ID_PHB_FMGR_TO_SELECT_ENTRY"/>




    <STRING id="STR_ID_PHB_FMGR_IMAGE_SELECTED"/>

    <STRING id="STR_SCR_PBOOK_CAPTION"/>
    <STRING id="STR_MITEM101"/>
    <STRING id="STR_ID_PHB_ADD_NEW_CONTACT"/>
    <STRING id="STR_ID_PHB_NAME_NUM_REQUIRE"/>
# 1059 "temp/res/phonebook.c"
    <STRING id="STR_PBOOK_FULL_MSG"/>
    <STRING id="STR_SIM_FULL_MSG"/>
    <STRING id="STR_PHONE_FULL_MSG"/>
    <STRING id="STR_ID_PHB_NO_ENTRY_TO_SELECT"/>
    <STRING id="STR_SCR_PBOOK_VIEW_CAPTION"/>
    <STRING id="STR_SCR_INLINE_EDIT_CAPTION"/>
    <STRING id="STR_SCR_ENTRY_VIEW_CAPTION"/>

    <STRING id="STR_ID_PBAP_BQB_TEST"/>

    <STRING id="STR_PROCESSING_PHONEBOOK"/>



    <STRING id="STR_SCR_ADD_NEW_ENTRY_CAPTION"/>





    <STRING id="STR_ID_PHB_NAME"/>
    <STRING id="STR_ID_PHB_NUMBER"/>


    <STRING id="STR_ID_PHB_ADD_MORE_DETAILS"/>
    <STRING id="STR_ID_PHB_SAVE_TO_PHONE_QUERY"/>
    <STRING id="STR_ID_PHB_MOVE_FROM_SIM_TO_PHONE_QUERY"/>

    <STRING id="STR_ID_PHB_VCARD"/>
    <STRING id="STR_ID_PHB_VCARD_EDIT_MYCARD"/>
# 1098 "temp/res/phonebook.c"
    <STRING id="STR_ID_PHB_COPY_NUM_WITH_QUESTION_MARK"/>
    <STRING id="STR_ID_PHB_COPY_NUM_WITH_W"/>

    <STRING id="STR_ID_PHB_MOVE_NUM_WITH_QUESTION_MARK"/>
# 1120 "temp/res/phonebook.c"
    <STRING id="STR_ID_PHB_OP_IMAGE_FAIL"/>
    <STRING id="STR_ID_PHB_OP_VIDEO_FAIL"/>
    <STRING id="STR_ID_PHB_OP_IMAGE_VIDEO_FAIL"/>
    <STRING id="STR_ID_PHB_NAME_MODIFIED"/>
# 1135 "temp/res/phonebook.c"
    <STRING id="STR_NOT_SAVED_NUMBER_TOO_LONG"/>




    <STRING id="STR_COPIED_NAME_MODIFIED"/>



    <STRING id="STR_ID_PHB_PARTIAL_COPIED"/>
# 1163 "temp/res/phonebook.c"
    <STRING id="STR_ID_PHB_EXTRA_NUMBER"/>
# 1247 "temp/res/phonebook.c"
    <STRING id="STR_ID_PHB_ADD_MORE_NUMBER"/>
    <STRING id="STR_ID_PHB_ADN_FORBID"/>



    <STRING id="STR_EMAIL_ADRESS"/>
# 1274 "temp/res/phonebook.c"
    <STRING id="STR_ID_PHB_SEP_COLON"/>
    <STRING id="STR_ID_PHB_INVLID_PREFFER_STORAGE"/>
# 1333 "temp/res/phonebook.c"
    <STRING id="STR_ID_PHB_VIP_CONTACT_CHANGE"/>
# 1365 "temp/res/phonebook.c"
    <STRING id="STR_ID_PHB_CALLER_PIC"/>
    <STRING id="STR_ID_PHB_CALLER_RING"/>






    <STRING id="STR_SCR8054_CALL_FDL_CAPTION"/>
    <STRING id="STR_MENU9167_FIXED_DIAL"/>
    <STRING id="STR_ID_SECSET_REPLACE_WILDCARD"/>
    <STRING id="STR_SETTING_FDL_LIST"/>
    <STRING id="STR_FDL_IS_ON"/>
    <STRING id="STR_FDL_IS_OFF"/>
# 1441 "temp/res/phonebook.c"
        <MEMORY cui="max(CUI_IMAGE_CLIP, CUI_CAMERACUI)"/>





    <!-----------------------------------------------------Image Resource Area------------------------------------------------------>




    <IMAGE id="IMG_ID_PHB_APP_LAUNCH">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\VUI\\\\Launcher\\\\Mainmenu\\\\Icons\\\\Contact.png"</IMAGE>




    <IMAGE id="IMG_ID_PHB_CSK_ADD">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Phonebook\\\\PHB_CSK_ADD.bmp"</IMAGE>
    <IMAGE id="IMG_ID_PHB_CSK_EDIT">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Phonebook\\\\PHB_CSK_EDIT.bmp"</IMAGE>
    <IMAGE id="IMG_ID_PHB_CSK_MARK">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Phonebook\\\\PHB_CSK_MARK.bmp"</IMAGE>
# 1527 "temp/res/phonebook.c"
    <IMAGE id="IMG_SCR_PBOOK_CAPTION">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\TitleBar\\\\TB_PB.PNG"</IMAGE>




    <IMAGE id="IMG_PHB_SETTINGS">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Phonebook\\\\SB_SET.bmp"</IMAGE>


    <IMAGE id="IMG_NAME">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Phonebook\\\\PBView\\\\PV_Name.bmp"</IMAGE>
    <IMAGE id="IMG_MOBILE_NUMBER">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Phonebook\\\\PBView\\\\PV_PHONE.bmp"</IMAGE>



    <IMAGE id="IMG_MORE_DETAILS">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Phonebook\\\\PBView\\\\PV_More.bmp"</IMAGE>

    <IMAGE id="IMG_ID_PHB_NEW_NUMBER">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Phonebook\\\\PBView\\\\PV_NEW_NUMBER.bmp"</IMAGE>
# 1556 "temp/res/phonebook.c"
    <IMAGE id="IMG_PICTURE">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Phonebook\\\\PBView\\\\PV_PIC.bmp"</IMAGE>

    <IMAGE id="IMG_RING_TONE">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Phonebook\\\\PBView\\\\PV_RING.bmp"</IMAGE>
# 1590 "temp/res/phonebook.c"
    <IMAGE id="IMG_STORAGE_SIM">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Phonebook\\\\PBView\\\\PV_INSIM.bmp"</IMAGE>
    <IMAGE id="IMG_STORAGE_HANDSET">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Phonebook\\\\PBView\\\\PV_INPH.bmp"</IMAGE>
# 1742 "temp/res/phonebook.c"
    <!------------------------------------------------------Timer Resource Area------------------------------------------------------>
                <TIMER id="PHB_NOTIFYDURATION_TIMER"/>
                <TIMER id="PHB_READ_SPEED_DIAL_TIMER"/>
                <TIMER id="PHB_LIST_SPEAK_NAME_TIMER"/>
                <TIMER id="PHB_1ST_STARTUP_ASK_TIMER"/>
                <TIMER id="PHB_1ST_STARTUP_ASK_TIMER_SIM2"/>
                <TIMER id="PHB_1ST_STARTUP_ASK_TIMER_SIM3"/>
                <TIMER id="PHB_1ST_STARTUP_ASK_TIMER_SIM4"/>
                <TIMER id="PHB_CHOOSE_FIELD_TIMER"/>






    <!------------------------------------------------------Menu Resource Area------------------------------------------------------>


    <MENUITEM id="MENU_ID_PHB_SLIM_EDIT" str="STR_GLOBAL_EDIT"/>
    <MENUITEM id="MENU_ID_PHB_SLIM_RINGTONE" str="STR_GLOBAL_SELECT"/>
# 1802 "temp/res/phonebook.c"
    <MENU id="MENU_ID_PHB_EDIT_OPTION" type="OPTION" str="STR_GLOBAL_OPTIONS">

                <MENUITEM_ID>MENU_ID_PHB_SLIM_EDIT</MENUITEM_ID>
                <MENUITEM_ID>MENU_ID_PHB_SLIM_RINGTONE</MENUITEM_ID>

        <MENUITEM_ID>MENU_ID_PHB_EDIT_OK</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_EDIT_NUM_OP_SET_DFT</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_EDIT_NUM_OP_CANCEL_DFT</MENUITEM_ID>




        <MENUITEM_ID>MENU_ID_PHB_EDIT_RINGTONE_OPTION_SELECT</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_EDIT_IMAGE_OPTION_VIEW</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_EDIT_IMAGE_OPTION_SELECT</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_EDIT_IMAGE_OPTION_CAMERA</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_EDIT_VIDEO_OPTION_PLAY</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_EDIT_VIDEO_OPTION_SELECT</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_EDIT_VIDEO_OPTION_AUDIO_SET</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_EDIT_ADD_IMPS</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_EDIT_DELETE_IMPS</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_EDIT_ADD_BIRTHDAY_REMINDER</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_ADD_MORE_DETAILS</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_DELETE_DETAILS</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_EDIT_NUM_OP_CHANGE_TYPE</MENUITEM_ID>



        <MENUITEM_ID>MENU_ID_PHB_EDIT_CANCEL</MENUITEM_ID>
    </MENU>

    <MENUITEM id="MENU_ID_PHB_ADD_MORE_DETAILS" str="STR_ID_PHB_ADD_DETAILS"/>
    <MENUITEM id="MENU_ID_PHB_DELETE_DETAILS" str="STR_ID_PHB_DELETE_DETAILS"/>
# 1850 "temp/res/phonebook.c"
    <MENUITEM id="MENU_ID_PHB_EDIT_OK" str="STR_GLOBAL_SAVE"/>



    <MENUITEM id="MENU_ID_PHB_EDIT_CANCEL" str="STR_GLOBAL_CANCEL"/>

    <MENUITEM id="MENU_ID_PHB_EDIT_IMAGE_OPTION_VIEW" str="STR_ID_PHB_OPTION_VIEW_PIC"/>


    <MENUITEM id="MENU_ID_PHB_EDIT_RINGTONE_OPTION_SELECT" str="STR_ID_PHB_FMGR_IMAGE_SELECT_FILE"/>
    <MENUITEM id="MENU_ID_PHB_EDIT_IMAGE_OPTION_SELECT" str="STR_ID_PHB_FMGR_IMAGE_SELECT_FILE"/>
# 2043 "temp/res/phonebook.c"
    <MENU id="MENU_ID_PHB_SETTINGS" type="APP_SUB" str="STR_ID_PHB_SETTINGS">
# 2056 "temp/res/phonebook.c"
            <MENUITEM_ID>MENU_ID_PHB_MY_NUMBER</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_EXTRA_NUMBER</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_SPEAK_NAME</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_SAVE_CONTACT_NOTIFY</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_MEMORY_STATUS</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_COPY_SEVERAL</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_MOVE_SEVERAL</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_DELETE_ALL</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_DELETE_ALL_NO_SIM</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_BACKUP</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_RESTORE</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_SYNC</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_OPTRSER_PIM</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_NAME_DISPLAY</MENUITEM_ID>
    </MENU>

    <MENU id="MENU_ID_PHB_SETTINGS_NO_SIM" type="APP_SUB" str="STR_ID_PHB_SETTINGS">



            <MENUITEM_ID>MENU_ID_PHB_MY_NUMBER</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_SPEAK_NAME</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_SAVE_CONTACT_NOTIFY</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_MEMORY_STATUS</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_COPY_SEVERAL</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_MOVE_SEVERAL</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_DELETE_ALL</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_DELETE_ALL_NO_SIM</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_BACKUP</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_RESTORE</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_SYNC</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_NAME_DISPLAY</MENUITEM_ID>
    </MENU>
# 2193 "temp/res/phonebook.c"
    <MENU id="MENU_ID_PHB_SPEED_DIAL_OP" type="OPTION" str="STR_GLOBAL_OPTIONS">
            <MENUITEM_ID>MENU_ID_PHB_SPEED_DIAL_OP_EDIT</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_SPEED_DIAL_OP_DEL</MENUITEM_ID>
    </MENU>

    <MENUITEM id="MENU_ID_PHB_SPEED_DIAL_OP_EDIT" str="STR_GLOBAL_EDIT"/>
    <MENUITEM id="MENU_ID_PHB_SPEED_DIAL_OP_DEL" str="STR_GLOBAL_REMOVE"/>






    <MENU id="MENU_ID_PHB_MY_NUMBER" type="OPTION" str="STR_ID_PHB_VCARD">
            <MENUITEM_ID>MENU_ID_PHB_EDIT_MY_NUMBER</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_SEND_MY_NUMBER</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_DELETE_MY_NUMBER</MENUITEM_ID>
    </MENU>

    <MENUITEM id="MENU_ID_PHB_EDIT_MY_NUMBER" str="STR_ID_PHB_VCARD_EDIT_MYCARD"/>
# 2223 "temp/res/phonebook.c"
    <MENUITEM id="MENU_ID_PHB_DELETE_MY_NUMBER" str="STR_GLOBAL_DELETE"/>
# 2243 "temp/res/phonebook.c"
    <MENUITEM id="MENU_ID_PHB_MEMORY_STATUS" str="STR_GLOBAL_MEMORY_STATUS"/>
# 2277 "temp/res/phonebook.c"
    <MENU id="MENU_ID_PHB_COPY_SEVERAL" type="APP_SUB" str="STR_ID_PHB_COPY_SEVERAL">
            <MENUITEM_ID>MENU_ID_PHB_COPY_SIMTOPHONE</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_COPY_PHONETOSIM</MENUITEM_ID>
    </MENU>
    <MENU id="MENU_ID_PHB_MOVE_SEVERAL" type="APP_SUB" str="STR_ID_PHB_MOVE_SEVERAL">
            <MENUITEM_ID>MENU_ID_PHB_MOVE_SIMTOPHONE</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_MOVE_PHONETOSIM</MENUITEM_ID>
    </MENU>
# 2323 "temp/res/phonebook.c"
    <MENUITEM id="MENU_ID_PHB_COPY_SIMTOPHONE" str="STR_ID_PHB_TO_PHONE"/>
    <MENUITEM id="MENU_ID_PHB_COPY_PHONETOSIM" str="STR_ID_PHB_TO_SIM"/>
    <MENUITEM id="MENU_ID_PHB_MOVE_SIMTOPHONE" str="STR_ID_PHB_TO_PHONE"/>
    <MENUITEM id="MENU_ID_PHB_MOVE_PHONETOSIM" str="STR_ID_PHB_TO_SIM"/>


    <MENU id="MENU_ID_PHB_DELETE_ALL" type="APP_SUB" str="STR_ID_PHB_DELETE_ALL">
            <MENUITEM_ID>MENU_ID_PHB_DELETE_ALL_SIM</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_DELETE_ALL_SIM2</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_DELETE_ALL_SIM3</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_DELETE_ALL_SIM4</MENUITEM_ID>
            <MENUITEM_ID>MENU_ID_PHB_DELETE_ALL_PHONE</MENUITEM_ID>



    </MENU>

    <MENUITEM id="MENU_ID_PHB_DELETE_ALL_NO_SIM" str="STR_ID_PHB_DELETE_ALL"/>
# 2351 "temp/res/phonebook.c"
    <MENUITEM id="MENU_ID_PHB_DELETE_ALL_SIM" str="STR_ID_PHB_FROM_SIM"/>

    <MENUITEM id="MENU_ID_PHB_DELETE_ALL_PHONE" str="STR_ID_PHB_FROM_PHONE"/>
# 2362 "temp/res/phonebook.c"
    <MENUITEM id="MITEM101_PBOOK_SEARCH_ENTRY" str="STR_MITEM101"/>
    <MENU id="MITEM101_SEARCH_OPTIONS" type="OPTION" str="STR_GLOBAL_OPTIONS">
        <MENUITEM_ID>MITEM1010_SEARCH</MENUITEM_ID>
        <MENUITEM_ID>MITEM1011_INPUT_MEHTHOD</MENUITEM_ID>
    </MENU>

    <MENUITEM id="MITEM1010_SEARCH" str="STR_GLOBAL_SEARCH"/>
    <MENUITEM id="MITEM1011_INPUT_MEHTHOD" str="STR_GLOBAL_INPUT_METHOD"/>



    <MENU id="MITEM102_PBOOK_NEW_ENTRY" type="APP_SUB" str="STR_ID_PHB_ADD_NEW_CONTACT"
        shortcut="ON" shortcut_img="IMG_ID_PHB_NEW_NUMBER" launch="mmi_phb_sc_add_new_entry">
        <MENUITEM_ID>MITEM_ADD_ENTRY_TO_SIM</MENUITEM_ID>
        <MENUITEM_ID>MENU_MTPNP_PB_ADD_SIM2</MENUITEM_ID>






        <MENUITEM_ID>MITEM_ADD_ENTRY_TO_PHONE</MENUITEM_ID>



    </MENU>
# 2407 "temp/res/phonebook.c"
    <MENU id="MENU_ID_PHB_ADD_NEW_ENTRY_SELECT" type="OPTION" str="STR_GLOBAL_SELECT">
        <MENUITEM_ID>MITEM_ADD_ENTRY_TO_SIM</MENUITEM_ID>
        <MENUITEM_ID>MENU_MTPNP_PB_ADD_SIM2</MENUITEM_ID>






        <MENUITEM_ID>MITEM_ADD_ENTRY_TO_PHONE</MENUITEM_ID>
    </MENU>
# 2450 "temp/res/phonebook.c"
    <MENUITEM id="MITEM_ADD_ENTRY_TO_SIM" str="STR_ID_PHB_TO_SIM"/>

    <MENUITEM id="MITEM_ADD_ENTRY_TO_PHONE" str="STR_ID_PHB_TO_PHONE"/>
# 2495 "temp/res/phonebook.c"
    <MENU id="MITEM101_PBOOK_VIEW_OPTIONS" type="OPTION" str="STR_GLOBAL_OPTIONS">

        <MENUITEM_ID>MENU_ID_PHB_ADD_NEW_ENTRY_SELECT</MENUITEM_ID>




        <MENUITEM_ID>MENU_ID_PHB_VIEW_CONTACT</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_OPTION_SEND_MESSAGE</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_OPTION_SEND_SMS</MENUITEM_ID>



        <MENUITEM_ID>MENU_ID_PHB_OPTION_DIAL</MENUITEM_ID>



        <MENUITEM_ID>MENU_ID_PHB_OPTION_SEND_POSTCARD</MENUITEM_ID>
        <MENUITEM_ID>MITEM1011_PBOOK_EDIT</MENUITEM_ID>
        <MENUITEM_ID>MITEM1012_PBOOK_DELETE</MENUITEM_ID>
        <MENUITEM_ID>MITEM1013_PBOOK_COPY_ENTRY</MENUITEM_ID>



        <MENUITEM_ID>MENU_ID_PHB_SET_VT_CALL</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_CANCEL_VT_CALL</MENUITEM_ID>



        <MENUITEM_ID>MENU_ID_VRSD_DIAL_PHB_OPTION</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_ADD_BLACK_LIST</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_ADD_VIDEO_BLACK_LIST</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_MARK_SEVERAL</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_MARK_SEVERAL_PRE</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_CALLER_GROUPS</MENUITEM_ID>

        <MENUITEM_ID>MENU_ID_PHB_SETTINGS</MENUITEM_ID>
# 2557 "temp/res/phonebook.c"
        <MENUITEM_ID>MENU_ID_PHB_PRINT</MENUITEM_ID>




    </MENU>

    <MENU id="MENU_ID_PHB_ENTRY_VIEW_OPTIONS" type="OPTION" str="STR_GLOBAL_OPTIONS">
        <MENUITEM_ID>MENU_ID_PHB_VIEW_CONTACT_EDIT</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_VIEW_CONTACT_SEND_MESSAGE</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_VIEW_CONTACT_SEND_SMS</MENUITEM_ID>



        <MENUITEM_ID>MENU_ID_PHB_VIEW_CONTACT_DIAL</MENUITEM_ID>



        <MENUITEM_ID>MENU_ID_PHB_OPTION_SEND_EMAIL</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_OPTION_SEND_IM</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_OPTION_START_BROWSER</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_OPTION_VIEW_PIC</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_OPTION_PLAY_VIDEO</MENUITEM_ID>
# 2592 "temp/res/phonebook.c"
    </MENU>

    <MENUITEM id="MENU_ID_PHB_VIEW_CONTACT" str="STR_GLOBAL_VIEW"/>
# 2617 "temp/res/phonebook.c"
    <MENUITEM id="MITEM1011_PBOOK_EDIT" str="STR_GLOBAL_EDIT"/>


    <MENUITEM id="MENU_ID_PHB_VIEW_CONTACT_EDIT" str="STR_GLOBAL_EDIT"/>
    <MENUITEM id="MITEM1012_PBOOK_DELETE" str="STR_GLOBAL_DELETE"/>
    <MENU id="MITEM1013_PBOOK_COPY_ENTRY" type="OPTION" str="STR_GLOBAL_COPY">
        <MENUITEM_ID>MENU_ID_PHB_COPY_TO_PHONE</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_COPY_TO_SIM</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_COPY_TO_SIM2</MENUITEM_ID>






        <MENUITEM_ID>MENU_ID_PHB_COPY_TO_FMGR</MENUITEM_ID>



    </MENU>
# 2647 "temp/res/phonebook.c"
    <MENUITEM id="MENU_ID_PHB_COPY_TO_SIM" str="STR_ID_PHB_TO_SIM"/>

    <MENUITEM id="MENU_ID_PHB_COPY_TO_PHONE" str="STR_ID_PHB_TO_PHONE"/>
# 2750 "temp/res/phonebook.c"
    <MENUITEM id="MITEM1014_PBOOK_MOVE" str="STR_GLOBAL_MOVE"/>






    <MENUITEM id="MENU_ID_PHB_OPTION_DIAL" str="STR_GLOBAL_DIAL"/>
    <MENUITEM id="MENU_ID_PHB_VIEW_CONTACT_DIAL" str="STR_GLOBAL_DIAL"/>
# 2773 "temp/res/phonebook.c"
    <MENUITEM id="MENU_ID_PHB_OPTION_VIEW_PIC" str="STR_ID_PHB_OPTION_VIEW_PIC"/>
# 2798 "temp/res/phonebook.c"
    <MENUITEM id="MENU_ID_PHB_OPTION_SEND_SMS" str="STR_GLOBAL_SEND_TEXT_MESSAGE"/>
    <MENUITEM id="MENU_ID_PHB_VIEW_CONTACT_SEND_SMS" str="STR_GLOBAL_SEND_TEXT_MESSAGE"/>
# 2891 "temp/res/phonebook.c"
    <MENU id="MENU_ID_PHB_IMAGE_OPTION" type="OPTION" str="STR_GLOBAL_OPTIONS">
                <MENUITEM_ID>MENU_ID_PHB_EDIT_OK</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_IMAGE_OPTION_VIEW</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_IMAGE_OPTION_SELECT</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_IMAGE_OPTION_DELETE</MENUITEM_ID>
    </MENU>

    <MENUITEM id="MENU_ID_PHB_IMAGE_OPTION_VIEW" str="STR_ID_PHB_OPTION_VIEW_PIC"/>
    <MENUITEM id="MENU_ID_PHB_IMAGE_OPTION_SELECT" str="STR_ID_PHB_VIP_CONTACT_CHANGE"/>
    <MENUITEM id="MENU_ID_PHB_IMAGE_OPTION_DELETE" str="STR_GLOBAL_REMOVE"/>

    <MENU id="MENU_ID_PHB_FMGR_SAVE_OPTION" type="OPTION" str="STR_GLOBAL_OPTIONS">
        <MENUITEM_ID>MENU_ID_PHB_FMGR_SAVE_NEW</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_FMGR_SAVE_LIST</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_PHB_FMGR_SAVE_CALLGRP</MENUITEM_ID>
    </MENU>

    <MENUITEM id="MENU_ID_PHB_FMGR_SAVE_NEW" str="STR_ID_PHB_FMGR_TO_NEW_ENTRY"/>
    <MENUITEM id="MENU_ID_PHB_FMGR_SAVE_LIST" str="STR_ID_PHB_FMGR_TO_SELECT_ENTRY"/>
# 2924 "temp/res/phonebook.c"
    <MENU id="MENU_ID_PHB_EXTRA_NUMBER" type="APP_SUB" str="STR_ID_PHB_EXTRA_NUMBER">
# 2934 "temp/res/phonebook.c"
        <MENUITEM_ID>MENU9167_FIXED_DIAL</MENUITEM_ID>
# 2951 "temp/res/phonebook.c"
    </MENU>
# 2981 "temp/res/phonebook.c"
    <MENU id="MENU9167_FIXED_DIAL" type="APP_SUB" str="STR_MENU9167_FIXED_DIAL">
        <MENUITEM_ID>MENU_SETTING_FDL_MODE</MENUITEM_ID>
        <MENUITEM_ID>MENU_SETTING_FDL_LIST</MENUITEM_ID>
    </MENU>
# 3072 "temp/res/phonebook.c"
    <MENU id="MENU_SETTING_FDL_MODE" type="APP_SUB" str="STR_GLOBAL_MODE">
        <MENUITEM_ID>MENU_SETTING_FDL_MODE_ON</MENUITEM_ID>
        <MENUITEM_ID>MENU_SETTING_FDL_MODE_OFF</MENUITEM_ID>
    </MENU>
    <MENUITEM id="MENU_SETTING_FDL_MODE_ON" str="STR_GLOBAL_ON"/>
    <MENUITEM id="MENU_SETTING_FDL_MODE_OFF" str="STR_GLOBAL_OFF"/>
    <MENUITEM id="MENU_SETTING_FDL_LIST" str="STR_SETTING_FDL_LIST"/>

    <MENU id="MENU8234_FDL_VIEW_OPTIONS_MAIN" type="OPTION" str="STR_GLOBAL_OPTIONS">
        <MENUITEM_ID>MENU8234_FDL_VIEW_OPTIONS_EDIT</MENUITEM_ID>
        <MENUITEM_ID>MENU8234_FDL_VIEW_OPTIONS_ADD</MENUITEM_ID>
        <MENUITEM_ID>MENU8234_FDL_VIEW_OPTIONS_DELETE</MENUITEM_ID>
    </MENU>

    <MENUITEM id="MENU8234_FDL_VIEW_OPTIONS_ADD" str="STR_GLOBAL_ADD"/>
    <MENUITEM id="MENU8234_FDL_VIEW_OPTIONS_EDIT" str="STR_GLOBAL_EDIT"/>
    <MENUITEM id="MENU8234_FDL_VIEW_OPTIONS_DELETE" str="STR_GLOBAL_DELETE"/>
# 3149 "temp/res/phonebook.c"
    <!------------------------------------------------------Other Resource---------------------------------------------------------->
# 3158 "temp/res/phonebook.c"
        <RECEIVER id="EVT_ID_PHB_READY" proc="mmi_phb_ready_hdlr"/>





        <RECEIVER id="EVT_ID_PHB_MULTI_OP_STATUS" proc="mmi_phb_update_mass_processing"/>




        <RECEIVER id="EVT_ID_GPIO_BEFORE_LCD_SLEEP_IN" proc="mmi_phb_processing_hdlr"/>
    <RECEIVER id="EVT_ID_GPIO_AFTER_LCD_SLEEP_IN" proc="mmi_phb_processing_hdlr"/>
# 3240 "temp/res/phonebook.c"
</APP>
