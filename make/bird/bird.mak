# Define source file lists to SRC_LIST
SRC_LIST = bird\base\src\Bird_socket.c \
                   bird\base\src\Bird_std.c \
                   bird\base\src\Bird_main.c \
                   bird\base\src\Bird_gpscell.c \
                   bird\base\src\hd8020_agps.c \
                   bird\base\src\Bird_nbrcell.c \
                   bird\base\src\Bird_task.c \
                   bird\base\src\Bird_oilcut.c \
                   bird\base\src\Bird_mileage.c \
                   bird\yd\src\Yd_base.c \
                   bird\yd\src\Yd_sms.c \
                   bird\yd\src\Yd_datadeal.c \
                   bird\yd\src\Yd_tboxpro.c \
                   bird\yd\src\Yd_aes128.c \
                   bird\yd\src\Yd_uart_main.c \
                   bird\yd\src\Yd_move_alarm.c \
                   bird\yd\src\Yd_vibration_alarm.c \
                   bird\yd\src\Yd_speed_alarm.c \
                   bird\yd\src\Yd_out_vol_alarm.c \
                   bird\yd\src\Yd_power_off.c \
                   bird\yd\src\Yd_low_power_alarm.c \
                   bird\yd\src\Yd_az_alarm.c \
                   bird\yd\src\Yd_rollover_alarm.c \
                   bird\yd\src\Yd_gps_control.c \
                   bird\yd\src\Yd_fly_mode_control.c \
                   bird\yd\src\Yd_reboot_control.c \
                   bird\yd\src\Yd_run_static_event.c \
                   bird\yd\src\Yd_nv_handler.c \
                   bird\yd\src\Yd_defense.c \
                   bird\yd\src\Yd_acc_control.c \
                   bird\yd\src\Yd_sos_alarm.c \
                   bird\yd\src\Yd_collision_alarm.c \
                   bird\yd\src\Yd_learn_command.c \
                   bird\yd\src\Yd_wakeup_handle.c \
                   bird\yd\src\Yd_sleep_state.c \
                   bird\yd\src\Yd_readposfile.c \
                   bird\yd\src\Yd_login_handle.c \
                   bird\yd\src\Yd_heart.c 

ifeq ($(strip $(BIRD_DW06A_SUPPORT)),TRUE)
	SRC_LIST += bird\yd\src\Yd_main_DW06A.c
endif

ifeq ($(strip $(BIRD_DW18_SUPPORT)),TRUE)
	SRC_LIST += bird\yd\src\Yd_main_DW18.c
endif

ifneq ($(strip $(BIRD_DW06A_SUPPORT)),TRUE)
	ifneq ($(strip $(BIRD_DW18_SUPPORT)),TRUE)
		SRC_LIST += bird\yd\src\Yd_main.c
	endif
endif

#  Define include path lists to INC_DIR
INC_DIR = bird\base\inc \
		    bird\yd\inc

# Define the specified compile options to COMP_DEFS
COMP_DEFS =

# Define the source file search paths to SRC_PATH
SRC_PATH = bird\base\src  \
                    bird\yd\src
