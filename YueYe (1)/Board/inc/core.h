#include "common.h"
#include "include.h"

/************变量声明*************/
/*图像参数*/
extern uint8 imgbuff[CAMERA_SIZE];                             //定义存储接收图像的数组[图像占用空间大小]
extern uint8 img[CAMERA_W*CAMERA_H];                           //由于鹰眼摄像头是一字节8个像素，因而需要解压为 1字节1个像素，方便处理
extern uint8 img_OLED[CAMERA_H][CAMERA_W];                      //图矩阵
/*速度参数*/
#define SPEED_STABLE 1500
#define SPEED_SLOW 800
extern uint8 speed_flag;
extern uint16 speed_out;
/*舵机参数*/
#define SD5_FTM   FTM0
#define SD5_CH    FTM_CH2
#define SD5_HZ    (200)
#define STEER_MID 3150
#define STEER_RIGHT_MAX 2650
#define STEER_LEFT_MAX 3650
extern uint16 steer_mid_debug;
extern uint16 steer_mid;
extern uint16 limit_speed;
extern uint16 turn_distance;
extern uint16 steer_wucha;
extern int debug_num;
/*判断*/
extern uint8 ud;
extern uint8 limit_flag;
/*电机参数*/
/*超声波模块参数*/
#define TRIG PTE4
#define ECHG PTE5
#define TRIG_LEFT PTE2
#define ECHG_LEFT PTE3
#define TRIG_RIGHT PTE0
#define ECHG_RIGHT PTE1
//#define UL_FLAGWAIT    0xFFFFF    
extern uint32 ultrasonic_timevar;
extern uint32 ultrasonic_flag;
extern uint16 ultrasonic_distance;
/************函数声明*************/
/*超声波模块*/ 
void Ultrasonic_Init();
void Ultrasonic_Compute();
/*电机*/
void motor_init(void); 
/*速度控制*/
void speed_set(void);
void speed_limit();
/*OLED*/
extern uint8 menu_choose;
extern uint8 parameter_choose;
extern uint8 scan_choose;
extern uint8 menu_model;
extern uint8 oled_show_img_flag;
void OLED_Init();
void OLED_show();
void OLED_model();
void OLED_scan_model();
void OLED_show_img();
void OLED_Parameter();
/*图像*/
extern uint8 size;
extern uint8 model;
#define WHIRL_SIZE 100
void shanwai_sendimg(uint8 *imgaddr, uint32 imgsize);          //发送图像到上位机
void img_extract(uint8 *dst, uint8 *src, uint32 srclen);       //提取图像二值化图像解压
__RAMFUNC void Scan(); 
__RAMFUNC void go_s();
__RAMFUNC void left_s(uint8);
__RAMFUNC void right_s(uint8);
__RAMFUNC void turn_s(uint8);
/*舵机*/
void Steer_Init();
/*转向*/
extern uint16 stop_time;
extern uint8 turn_direction;
void Turn();
__RAMFUNC void go();
void back();
void stop();
__RAMFUNC void left();
__RAMFUNC void right();
void slow();
void front();
/*编码器*/
extern uint8 pit0_flag;
extern uint32 speed_count;
void Encoder_Compute();
/*拨码开关*/
#define boma_1  PTD0            //菜单
#define boma_2  PTD1            //发送图像到上位机
#define boma_3  PTD2            //开环闭环
#define boma_4  PTD3            //待定
#define boma_5  PTD4            //测试速度
#define boma_6  PTD5            //低速度
#define boma_7  PTD6            //高速度
#define boma_8  PTD7            //极限速度
void boma_key();
/*按键*/
void checkKey();
/*中断函数*/
void PIT0_IRQHandler();
void PIT2_IRQHandler();
void PORTA_IRQHandler();
void DMA0_IRQHandler();
void DMA1_IRQHandler();
void uart4_handler();
/*上位机*/
void simular();