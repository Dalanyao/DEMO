#include "app_remoter.h"
#include "board.h"
#include "stdlib.h"
#include "gpio.h"
#include "app_select.h"
#include "print.h"
#include "tal_beacon_remoter.h"
#include "wakeup_timer.h"
#include "lvd.h"



// static u8 r_onoff_and_group_id = 0x00;    //hight 4 bit is onoff    low 4 bit is group id
// static u8 r_temp_value = 50;
// static u8 r_lum_value = 50;

// static u8 r_fan_onoff_and_fan_mode = 0x01;	//hight 4 bit is onoff  low 4 bit is fan_mode

static u8 led_blink_times = 0;//指示灯闪烁次数

typedef struct
{
	//system used,1 words
	u8 ret_onoff_and_group_id;    //hight 4 bit is onoff    low 4 bit is group id
	u8 ret_lum_value;
	u8 ret_temp_value;
	u8 ret_fan_onoff_and_fan_mode;
}ty_retention_memory;
ty_retention_memory ty_ret_mem_data;


u8 app_led_blink_times_get(void)
{
	return led_blink_times;//指示灯闪烁次数
}

void app_led_blink_times_set(u8 blink_times)
{
	led_blink_times = blink_times;//指示灯闪烁次数
}


void app_remoter_ram_restore(void)
{
	//read sys reset cause
	uint32_t reg_value = read_reg(WAKEUP_SOURE_STATE);
	uint8_t* p_retmem = (uint8_t*)&ty_ret_mem_data;

	//reset by wakeup timer
	if(reg_value == WAKEUP_BY_WUT)
	{
		//get saved state by word from retention memory
		for(uint8_t i = 0; i < 16; i++)
		{
			p_retmem[i] = *((volatile uint32_t *)(RETENTION_MEMEORY_ADDR + (i<<2)));
		}
		print("RESET WT\n");
	}
	//first power on
	else if(reg_value == WAKEUP_BY_FPO)
	{
		memset(p_retmem,0x0,RETENTION_MEMEORY_SIZE);
		ty_ret_mem_data.ret_lum_value = 50;
		ty_ret_mem_data.ret_temp_value = 50;
		// ty_ret_mem_data.ret_fan_onoff_and_fan_mode = 1<<4;
		// ty_ret_mem_data.ret_onoff_and_group_id = 1<<4;
		print("------device onoff:%d\r\n",ty_ret_mem_data.ret_lum_value);
		print("RESET 1ST\n");
	}

#if 0
	//log out reterntion memory by byte
	uint8_t* p_u8retmem = (uint8_t*)&ty_ret_mem_data;
	print("RETMEM:");
	for(uint8_t i = 0; i < 3; i++)
	{
		print("%x ",p_u8retmem[i]);
	}
	print("\r\n");
#endif
    
	// r_onoff_and_group_id = ty_ret_mem_data.ret_onoff_and_group_id;
	// r_lum_value = ty_ret_mem_data.ret_lum_value;
	// r_temp_value = ty_ret_mem_data.ret_temp_value;
	// r_fan_onoff_and_fan_mode = ty_ret_mem_data.r_fan_onoff_and_fan_mode;
	// print("------device onoff:%d\r\n",r_lum_value);
}

void app_remoter_ram_save(void)
{
	// ty_ret_mem_data.ret_onoff_and_group_id = r_onoff_and_group_id;
	// ty_ret_mem_data.ret_lum_value = r_lum_value;
	// ty_ret_mem_data.ret_temp_value = r_temp_value;
	// ty_ret_mem_data.r_fan_onoff_and_fan_mode = r_fan_onoff_and_fan_mode;
	uint8_t* p_retmem = (uint8_t*)&ty_ret_mem_data;

	for(uint8_t i = 0; i < 16; i++)							//把休眠后需要保存的数据保存到Retation中
	{
		*((volatile uint32_t *)(RETENTION_MEMEORY_ADDR + (i<<2))) = p_retmem[i];
	}
}



/*
 命令枚举
总开关:         0x00,0xff,0x04,0x00(),0x00,0x00,0x00
夜灯:  一下两条命令在里面
定时:           0x00,0xff,0x06,0x00(),0x00(),0x00,0x00
亮度+:          0x00,0x01,0x09,0x01,0x00,0x14,0x00
亮度-:          0x00,0x01,0x09,0x02,0x00,0x14,0x00
亮度无极+:      0x00,0x01,0x0A,0x00(),0x00,0x14,0x64
亮度无极-:      0x00,0x01,0x0A,0x01(),0x00,0x14,0x00
色温+:          0x00,0x01,0x0B,0x01,0x14,0x00,0x00
色温-:          0x00,0x01,0x0B,0x02,0x14,0x00,0x00
色温无极+:      0x00,0x01,0x0C,0x00(),0x14,0x64,0x00
色温无极-:      0x00,0x01,0x0C,0x01(),0x14,0x00,0x00
喜好收藏:       0x00,0xff,0x05,0x01,0x00(),0x00,0x00
喜好执行:       0x00,0xff,0x05,0x02,0x00(),0x00,0x00
分组1:          0x00(),0xff,0x04,0x00,0x00,0x00,0x00
情景切换:       0x00,0x01,0x15,0x01,0x00(),0x00,0x00
彩色步进循环:   0x00,0x01,0x0D,0x01,0x00(),0x00,0x00
彩色无极循环:   0x00,0x01,0x0F,0x00(),0x14,0x00,0x00//只有连续加减,无法实现循环
色温步进循环:   0x00,0x01,0x0B,0x01,0x14,0x00,0x00
色温无极循环:   0x00,0x01,0x0C,0x00(),0x14,0x00,0x00
情景:           0x00,0x01,0x15,0x01,0x00(),0x00,0x00
 */




// u8 cmd_dp_onoff[10] = {0x00,0x01,0x00,0x04,0x00,0x00,0x00,0x00};           //group_id + dp_id_onoff
// u8 cmd_dp_timing[8] = {0x00,0x07,0x02,0x04,0x00,0x00,0x00,0x00};          //group_id + countdown_time
// u8 cmd_dp_lum_step[10] = {0x00,0x13,0x02,0x03,0x02,0x01,0x90,0x00};          //group_id + lightness
// u8 cmd_dp_temp_step[10] = {0x00,0x04,0x01,0x04,0x00,0x00,0x00,0x00};          //group_id + temp

// dp_fan
// u8 cmd_dp_fan_speed[8] = {0x00,0x35,0x01,0x01,0x01,0x00,0x00,0x00};

void app_remoter_single_click(kb_data_t* kb_data)
{	
    if(kb_data->cnt > 1)
        return;
    u8 cmd_type = DEVICE_CMD_TYPE_CTRL;
    // u8 *params;
    //u8 *dp_params;
    // u8 params_len = 7;
    u8 dp_params_len = 8;
	u8 r_onoff =0 ;
	u8 r_fan_onoff = 0;
	u8 r_fan_mode = 0;
	u8 cmd_dp_buffer[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

    cmd_type = DEVICE_CMD_TYPE_CTRL;
    //dp_params = &cmd_dp_buffer[0];

    switch(kb_data->keycode[0]){
        case 1:{	//灯开关
				r_onoff = (ty_ret_mem_data.ret_onoff_and_group_id&0xff)>>4;
                
				//*(dp_params+1) = 0x01;		//dp
				//*(dp_params+2) = 0x00;		//type
				//*(dp_params+3) = 0x01;		//len
                //*(dp_params+4) = r_onoff;	//date
                
                cmd_dp_buffer[1] = 0x01;
                cmd_dp_buffer[2] = 0x00;
                cmd_dp_buffer[3] = 0x01;
                cmd_dp_buffer[4] = r_onoff;
                
				r_onoff++;
				r_onoff = r_onoff%2;
				r_onoff =r_onoff<<4;
				ty_ret_mem_data.ret_onoff_and_group_id = r_onoff + 0;
               }
            break;
		//case 2:{	//风扇开关
		//		r_fan_onoff = (ty_ret_mem_data.ret_fan_onoff_and_fan_mode&0xff)>>4;
		//		*(dp_params+1) = 0x3C;
		//		*(dp_params+2) = 0x01;
		//		*(dp_params+3) = 0x01;
        //        *(dp_params+4) = r_fan_onoff;

		//		r_fan_onoff++;
		//		r_fan_onoff = r_fan_onoff%2;
		//		r_fan_onoff = r_fan_onoff<<4;
		//		ty_ret_mem_data.ret_fan_onoff_and_fan_mode = r_fan_onoff + r_fan_mode;
         //      }
         //   break;
		case 3:{	//色温加

				//*(dp_params+1) = 0x04;
				//*(dp_params+2) = 0x01;
				//*(dp_params+3) = 0x04;

                cmd_dp_buffer[1] = 0x04;
                cmd_dp_buffer[2] = 0x01;
                cmd_dp_buffer[3] = 0x04;
                
                if(ty_ret_mem_data.ret_temp_value!=100)
                {
                    ty_ret_mem_data.ret_temp_value = ty_ret_mem_data.ret_temp_value+10;
                }
				
                //*(dp_params+6) = ((ty_ret_mem_data.ret_temp_value*10)>>8)&0xff;
                //*(dp_params+7) = (ty_ret_mem_data.ret_temp_value*10);
                cmd_dp_buffer[6] = ((ty_ret_mem_data.ret_temp_value*10)>>8)&0xff;
                cmd_dp_buffer[7] = (ty_ret_mem_data.ret_temp_value*10);
                print("------device temperature up\r\n");
               }
            break;
        case 4:{	//色温减
                //*(dp_params+1) = 0x04;
				//*(dp_params+2) = 0x01;
				//*(dp_params+3) = 0x04;
                cmd_dp_buffer[1] = 0x04;
                cmd_dp_buffer[2] = 0x01;
                cmd_dp_buffer[3] = 0x04;
                if(ty_ret_mem_data.ret_temp_value!=0)
                {
                    ty_ret_mem_data.ret_temp_value = ty_ret_mem_data.ret_temp_value-10;
                }

                //*(dp_params+6) = ((ty_ret_mem_data.ret_temp_value*10)>>8)&0xff;
                //*(dp_params+7) = (ty_ret_mem_data.ret_temp_value*10);		
				cmd_dp_buffer[6] = ((ty_ret_mem_data.ret_temp_value*10)>>8)&0xff;
                cmd_dp_buffer[7] = (ty_ret_mem_data.ret_temp_value*10);
                print("------device remperature down\r\n");
               }
            break;
        case 5:{			//亮度加
				//*(dp_params+1) = 0x13;
				//*(dp_params+2) = 0x02;
				//*(dp_params+3) = 0x03;
                cmd_dp_buffer[1] = 0x13;
                cmd_dp_buffer[2] = 0x02;
                cmd_dp_buffer[3] = 0x03;
				if(ty_ret_mem_data.ret_lum_value == 1)
				{
					ty_ret_mem_data.ret_lum_value = 10;
				}
				else{
					if(ty_ret_mem_data.ret_lum_value!=100)
					{
						ty_ret_mem_data.ret_lum_value = ty_ret_mem_data.ret_lum_value+10;
					}
				}
				
                //*(dp_params+5) = ((ty_ret_mem_data.ret_lum_value*10)>>8)&0xff;
                //*(dp_params+6) = (ty_ret_mem_data.ret_lum_value*10);
                cmd_dp_buffer[5] = ((ty_ret_mem_data.ret_lum_value*10)>>8)&0xff;
                cmd_dp_buffer[6] = (ty_ret_mem_data.ret_lum_value*10);
				print("------device lum up\r\n");
               }
            break;
        case 6:{	//亮度减
                //*(dp_params+1) = 0x13;
				//*(dp_params+2) = 0x02;
				//*(dp_params+3) = 0x03;
                cmd_dp_buffer[1] = 0x13;
                cmd_dp_buffer[2] = 0x02;
                cmd_dp_buffer[3] = 0x03;

                if(ty_ret_mem_data.ret_lum_value>=20)
                {
                    ty_ret_mem_data.ret_lum_value = ty_ret_mem_data.ret_lum_value-10;
                }
                else{
                    ty_ret_mem_data.ret_lum_value =1;
                }
				
                //*(dp_params+5) = (ty_ret_mem_data.ret_lum_value*10>>8)&0xff;
                //*(dp_params+6) = ty_ret_mem_data.ret_lum_value*10;
                cmd_dp_buffer[5] = (ty_ret_mem_data.ret_lum_value*10>>8)&0xff;
                cmd_dp_buffer[6] = ty_ret_mem_data.ret_lum_value*10;
                print("------device lum down\r\n");
               }
            break;
		//case 7:{	//fan_speed 1
		//		// dp_params = &cmd_dp_fan_speed[0];
        ///        *(dp_params+1) = 0x3E;
		//		*(dp_params+2) = 0x02;
		//		*(dp_params+3) = 0x04;

         //       *(dp_params+7) = 1;
        //       }
         //   break;
		//case 8:{	//fan_speed 2
        //        // dp_params = &cmd_dp_fan_speed[0];
        //        *(dp_params+1) = 0x3E;
		//		*(dp_params+2) = 0x02;
		//		*(dp_params+3) = 0x04;

         //       *(dp_params+7) = 2;
         //      }
        //    break;
		//case 9:{	//fan_speed 3
                // dp_params = &cmd_dp_fan_speed[0];
         //       *(dp_params+1) = 0x3E;
		//		*(dp_params+2) = 0x02;
		//		*(dp_params+3) = 0x04;

         //       *(dp_params+7) = 3;
         //      }
          //  break;
		//case 10:{	//fan_speed 4
       //        // dp_params = &cmd_dp_fan_speed[0];
       //         *(dp_params+1) = 0x3E;
		//		*(dp_params+2) = 0x02;
		//		*(dp_params+3) = 0x04;

        //        *(dp_params+7) = 4;
         //      }
         ///   break;
		//case 11:{	//fan_speed 5
        //        // dp_params = &cmd_dp_fan_speed[0];
        //        *(dp_params+1) = 0x3E;
		//		*(dp_params+2) = 0x02;
		//		*(dp_params+3) = 0x04;

        //        *(dp_params+7) = 5;
       //       }
        //    break;
		//case 12:{	//fan_direction
		//		*(dp_params+1) = 0x3F;
		//		*(dp_params+2) = 0x04;
		//		*(dp_params+3) = 0x01;

         //       *(dp_params+4) = 0x02;
         //      }
         //   break;
       // case 13:{	//fan_mode
		//		*(dp_params+1) = 0x3D;
		//		*(dp_params+2) = 0x04;
		//		*(dp_params+3) = 0x01;

		//		r_fan_mode = ty_ret_mem_data.ret_fan_onoff_and_fan_mode&0x0f;
        //        *(dp_params+4) = r_fan_mode;

		//		r_fan_mode++;
		//		r_fan_mode = r_fan_mode%2;
		//		ty_ret_mem_data.ret_fan_onoff_and_fan_mode = r_fan_onoff + r_fan_mode;
         //       }
          //  break;
		//case 14:{	//倒计时 1h
		//	// dp_params = &cmd_dp_timing[0];
		//	*(dp_params+1) = 0x07;
		//	*(dp_params+2) = 0x02;
		//	*(dp_params+3) = 0x04;
			
			// led_blink_times = 2;//指示灯闪烁次数-1
//
		//	*(dp_params+6) = 0x0E;	
		//	*(dp_params+7) = 0x10;
					
		//	}
		//	break;
		//case 15:{	//倒计时 2h
		//	// dp_params = &cmd_dp_timing[0];
		//	*(dp_params+1) = 0x07;
		//	*(dp_params+2) = 0x02;
		//	*(dp_params+3) = 0x04;
			
			// led_blink_times = 2;//指示灯闪烁次数-1

		//	*(dp_params+6) = 0x1C;
		//	*(dp_params+7) = 0x20;
					
		//	}
		//	break;
		//case 16:{	//倒计时 4h
		//	// dp_params = &cmd_dp_timing[0];
		//	*(dp_params+1) = 0x07;
		//	*(dp_params+2) = 0x02;
		//	*(dp_params+3) = 0x04;
		//	
			// led_blink_times = 2;//指示灯闪烁次数-1

		//	*(dp_params+6) = 0x38;
		//	*(dp_params+7) = 0x40;

		//	}
		//	break;
		//case 17:{	//倒计时 8h
			// dp_params = &cmd_dp_timing[0];
		//	*(dp_params+1) = 0x07;
		//	*(dp_params+2) = 0x02;
		//	*(dp_params+3) = 0x04;
			
			// led_blink_times = 2;//指示灯闪烁次数-1

		///	*(dp_params+6) = 0x70;
		//	*(dp_params+7) = 0x80;

		//	}
		//	break;

        default:
            return;
            break;
    }

    //print("ctrl------------------:");

   // for(u8 i=0;i<dp_params_len;i++)
    //    print("%x ",dp_params[i]);
   // print("\r\n");

    // for(u8 i=0;i<params_len;i++)
    //     print("%x ",params[i]);
    //print("\r\n");
	ty_beacon_ctrl(cmd_type, NULL, 0,cmd_dp_buffer,dp_params_len);

}

void app_remoter_long_press_start(kb_data_t* kb_data)
{
    u8 cmd_type = DEVICE_CMD_TYPE_CTRL;
    //u8 *params;
    u8 params_len = 3;
	//u8 *dp_params;
    u8 dp_params_len = 8;
	 u8 cmd_dp_buffer[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	
    u8 pair1[3] = {0x02,0x01,0xff};
    u8 send_pair[2] = {0x00,0x09};

    //dp_params = &cmd_dp_buffer[0];

    switch(kb_data->cnt){
        case 1:{
                switch(kb_data->keycode[0]){
					case 2:{	//配对	
							cmd_type = DEVICE_CMD_TYPE_PAIR;
							pair1[0] = 0x02;
							//params = &pair1[0];
							//params_len = 3;
							print("------device 1 group pair\r\n");
							}
                        break;
					//case 14:
					//case 15:
					//case 16:
					//case 17:
					//		*(dp_params+1) = 0x07;
					//		*(dp_params+2) = 0x02;
					//		*(dp_params+3) = 0x04;		

					//		led_blink_times = 2;//指示灯闪烁次数-1

					//		*(dp_params+6) = 0x00;
					//		*(dp_params+7) = 0x00;
					//	break;

					//case 18:{	//配网
					//		cmd_type = DEVICE_CMD_TYPE_SEND_PAIR;
					//		params = &send_pair[0];
					//		params_len = 2;
					//		led_blink_times = 2;//指示灯闪烁次数-1
					//		}
                     //   break;
                    default:
                        return;
                        break;
                }
               }
            break;
		case 2:{
            if(((7 == kb_data->keycode[0])&&(2 == kb_data->keycode[1]))||((2 == kb_data->keycode[0])&&(7 == kb_data->keycode[1]))){
                cmd_type = DEVICE_CMD_TYPE_PAIR;
				pair1[0] = 0x03;
                //params = &pair1[0];
                //params_len = 3;
				led_blink_times = 4;//指示灯闪烁次数-1
				print("------device 1 group unpair\r\n");
            }else{	
				return;
			}
			}break;
        default:
            return;
            break;
    }
	//print("ctrl------------------:");
	//for(u8 i=0;i<params_len;i++)
	//	print("%x ",params[i]);
	//print("\r\n");
	if (cmd_type == DEVICE_CMD_TYPE_PAIR || cmd_type == DEVICE_CMD_TYPE_SEND_PAIR) 
		ty_beacon_ctrl(cmd_type, pair1, params_len,NULL,0);
	else
		ty_beacon_ctrl(cmd_type, NULL, 0,cmd_dp_buffer,dp_params_len);
}

//void app_remoter_long_press_release(kb_data_t* kb_data)
//{
    //u8 cmd_type = DEVICE_CMD_TYPE_CTRL;
   // u8 *params;
   // u8 params_len = 7;
   // switch(kb_data->cnt){
    //    case 1:{
                // switch(kb_data->keycode[0]){
				// 	case 18:{
				// 			// params = &cmd_temp_stepless_cycle[0];
				// 			// *params = r_onoff_and_group_id&0x0f;
				// 			// *(params+3) = 0x02;
				// 			print("------Temperature cycle stepless adjustment\r\n");
				// 			}
                //         break;
                //     default:
                //         return;
                //         break;
                // }
      //         }
      //      break;
      //  default:
       //     return;
       //     break;
    //}
   // ty_beacon_ctrl(cmd_type, params, params_len,NULL,0);
//}


void ty_beacon_key_event(PressEvent key_state, kb_data_t* kb_data)
{
    // print("key state=%d \r\n",key_state);
    // print("key cnt=%d \r\n",kb_data->cnt);
    // print("keycode:");
	// for(u8 i=0;i<5;i++)
	// print("%x ",kb_data->keycode[i]);
	// print("\r\n");

    switch(key_state){
        case SINGLE_CLICK:
			// led_blink_times = 1;//指示灯闪烁次数
			//print("SINGLE_CLICK\r\n");
            app_remoter_single_click(kb_data);
            break;
        //case DOUBLE_CLICK:
         //   print("DOUBLE_CLICK\r\n");
         //   break;
        case LONG_PRESS_START:
            //print("LONG_PRESS_START\r\n");
			// led_blink_times = 1;//指示灯闪烁次数		
            app_remoter_long_press_start(kb_data);
            break;
        //case LONG_PRESS_RELEASE:
			// app_remoter_long_press_release(kb_data);
         //   break;
        default:
            break;
    }
	
}


