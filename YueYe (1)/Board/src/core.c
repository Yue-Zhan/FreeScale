#include "common.h"
#include "include.h"
#include  "core.h"



/*判断*/
uint8 ud = 0;
uint8  limit_flag = 0;
/*图像*/
uint8 imgbuff[CAMERA_SIZE];                             //定义存储接收图像的数组[图像占用空间大小]
uint8 img[CAMERA_W*CAMERA_H];                           //由于鹰眼摄像头是一字节8个像素，因而需要解压为 1字节1个像素，方便处理
uint8 img_OLED[CAMERA_H][CAMERA_W]={0,0};                      //图矩阵
void shanwai_sendimg(uint8 *imgaddr, uint32 imgsize)
{
    #define CMD_IMG     1
    uint8 cmdf[2] = {CMD_IMG, ~CMD_IMG};    //山外上位机 使用的命令
    uint8 cmdr[2] = {~CMD_IMG, CMD_IMG};    //山外上位机 使用的命令

    uart_putbuff(VCAN_PORT, cmdf, sizeof(cmdf));    //先发送命令

    uart_putbuff(VCAN_PORT, imgaddr, imgsize); //再发送图像

    uart_putbuff(VCAN_PORT, cmdr, sizeof(cmdr));    //先发送命令

}
void img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
    uint8 colour[2] = {1, 0}; //0 和 1 分别对应的颜色
    //注：山外的摄像头 0 表示 白色，1表示 黑色
    uint8 tmpsrc;
    while(srclen --)
    {
        tmpsrc = *src++;
        *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
}
uint8 size = 4;
uint8 model = 1;
#define boma_get(x) ((GPIO_PDIR_REG(GPIOX_BASE(x)) >> PTn(x)) & 0x01)
__RAMFUNC void Scan()
{
    int i = 0,j = 0,part_num = 0;
    uint16 x[10] = {0},y[60] = {0};
    uint8 center_x = 0,center_y = 0;
    /*************模式1**************/
    if(model == 0)
    {
        /*************不解压**************/
        if(model == 0)
        {
          for(i = 0;i < 10; i++)
            for(j = 0;j < 60; j++)
               x[i] += imgbuff[j*10 + i];
          for(i = 0;i < 9;i++)
          {
             if(x[i+1] < x[i])
               center_x = i+1;
          }
          if(center_x == 0 && x[center_x] == 15300)
          {         
              if(speed_out > SPEED_SLOW)
              {
                speed_out = SPEED_SLOW;
                go();
              }
              if(steer_mid != STEER_RIGHT_MAX)
               right();
          }
          else
          {
             if(center_x < 4)
             {
                 left_s(center_x);
             }
             else if(center_x > 5)
             {
                 right_s(center_x);
             }
             else go_s();
//             wdog_feed();
          }
        }
        /*************解压*************/
        else
        {
          for(i = 0;i < 10;i++)
            y[0] += imgbuff[i];
          for(i = 1;i < 60; i++)
          {
            for(j = 0;j < 10; j++)
              y[i] += imgbuff[i*10 + j];
            if(y[i] < y[i - 1])
              center_y = i;
          }      
          if(center_y == 0 && y[center_y] == 15300)
          {         
              if(speed_out > SPEED_SLOW)
              {
                speed_out = SPEED_SLOW;
                go();
              }
              if(steer_mid != STEER_LEFT_MAX)
               left();
          }
          else
          {
            uint8 l = 0,r = 9;
            for(i = 0;i < 10 ;i++)
              if(imgbuff[center_y*10 + i] < 0xFF){l = i;break;}
            for(;i < 10;i++)
              if(imgbuff[center_y*10 + i] == 0xFF){r = i - 1;break;}
            uint8 l_8 = imgbuff[center_y + l],r_8 = imgbuff[center_y + r];
            for(i = 0;i < 8;i++)
              if((l_8 << i) & 0x80 == 1)
                break;
            l = l * 8 + i - 1;
            for(i = 0;i < 8;i++)
              if((r_8 >> i) & 0x01 == 1)
                break;
            r = r * 8 - i + 1;
            center_x = (l + r) / 2;   
            turn_s(center_x);
//            wdog_feed();
          }
        }
    }
    else if(model == 1)
    {
        size = 40;
        for(;i < 600;i++)
        {
           if(imgbuff[i] < 0xFF)
           {
              int s = i,l = i%10,r = i%10;
              int l_max = l,r_max = r;
              while(1)
              {
    //            while(imgbuff[s/10 + l-1] < 0xFF && l > 0)
    //              l = l-1;
    //            while(imgbuff[s/10 + r+1] < 0xFF && r < 9)
    //              r = r+1;
                int t = 0;
                while(imgbuff[s/10*10+ t] == 0xFF && t < 9)
                  t++;
                l = t;
                while(imgbuff[s/10*10 + t] < 0xFF && t < 9)
                  t++;
                r = t;
                if(s%10 < l || s%10 > r)
                  break;
                l_max = l < l_max ? l : l_max;
                r_max = r > r_max ? r : r_max;
                s = s - s%10 + l + (r - l) / 2 + 1;
                s = s + 10;
              }
              int x_i,x_j;
              for(x_i = i/10;x_i <= s/10;x_i++)
                for(x_j = l_max;x_j <= r_max;x_j++)
                  imgbuff[x_i*10+x_j] = 0xFF;
              if(size < (s - i) * (r_max - l_max))
              {
                size = (s - i) * (r_max - l_max);
//                center_x = i + (s/10*10 - i/10*10) / 2 + (r_max - l_max) / 2 - 1;
                center_x = i%10; 
                part_num++;
              }
           }
        }
        if(part_num != 0 && size > 40)
        {
//            wdog_feed();
      //      ((GPIOX[PTX(PTC15)])->PDOR) |= (1 << ((PTC15)&0x1f)); 
            if(center_x < 4)
            {
                left_s(center_x);
            }
            else if(center_x > 5)
            {
                right_s(center_x);
            }
            else go_s();
        }
        else 
        {
            if(speed_out > SPEED_SLOW)
            {
              speed_out = SPEED_SLOW;
              go();
            }
            if(steer_mid != STEER_RIGHT_MAX)
              right();
//            switch(turn_direction)
//            {
//              case 2: 
//                  if(steer_mid != STEER_RIGHT_MAX)
//                      right();
//                  turn_direction = 0;
//                  break;
//              case 3: 
//                  if(steer_mid != STEER_LEFT_MAX)
//                    left();
//                  turn_direction = 0;
//                  break;
//              default : 
//                  if(steer_mid != STEER_RIGHT_MAX)
//                      right();
//                  turn_direction = 0;
//                  break;
//            }
        }
    //    else ((GPIOX[PTX(PTC15)])->PDOR) &= ~(1 << ((PTC15)&0x1f));  
    }
//    if(part_num != 0 && size > 4)
//    {
////      ((GPIOX[PTX(PTC15)])->PDOR) |= (1 << ((PTC15)&0x1f)); 
//      center_x = center_x % 10;
//      if(center_x < 4)
//      {
//          left_s(center_x);
//      }
//      else if(center_x > 5)
//      {
//          right_s(center_x);
//      }
//      else go_s();
//    }
////    else ((GPIOX[PTX(PTC15)])->PDOR) &= ~(1 << ((PTC15)&0x1f));     
} 
__RAMFUNC void go_s()
{     
    if(speed_out < SPEED_STABLE)
    {
      speed_out = SPEED_STABLE;
      go();
    }
    steer_mid = steer_mid_debug;
    FTM_PWM_Duty(SD5_FTM,SD5_CH,steer_mid);
}
__RAMFUNC void left_s(uint8 center)
{    
//    if(speed_out != 500){
//      speed_out = 500;
//      FTM_PWM_Duty(FTM2,FTM_CH0,speed_out);
//      FTM_PWM_Duty(FTM2,FTM_CH1,0);
//    }
    if(steer_mid < STEER_LEFT_MAX) 
    {
      if(center < 3)
        steer_mid = steer_mid + 250;
      else
        steer_mid = steer_mid + 100;
      FTM_PWM_Duty(SD5_FTM,SD5_CH,steer_mid);
    }
}
__RAMFUNC void right_s(uint8 center)
{
//    if(speed_out != 500){
//      speed_out = 500;
//      FTM_PWM_Duty(FTM2,FTM_CH0,speed_out);
//      FTM_PWM_Duty(FTM2,FTM_CH1,0);
//    }
    if(steer_mid > STEER_RIGHT_MAX)
    {
      if(center > 6)
        steer_mid = steer_mid - 250;
      else
        steer_mid = steer_mid - 100;
      FTM_PWM_Duty(SD5_FTM,SD5_CH,steer_mid);
    }
}
__RAMFUNC void turn_s(uint8 center)
{
    if(center < 20){
      if(steer_mid + 500 <= STEER_RIGHT_MAX)
        steer_mid = steer_mid + 500;
    }
    else if(center < 36){
      if(steer_mid + 250 <= STEER_RIGHT_MAX)
        steer_mid = steer_mid + 250;
    }
    else if(center > 60){
      if(steer_mid - 500 >= STEER_LEFT_MAX)
        steer_mid = steer_mid - 500;
    }
    else if(center > 44){
      if(steer_mid - 250 >= STEER_LEFT_MAX)
        steer_mid = steer_mid - 250;
    }
    else {
      steer_mid = steer_mid_debug;
      if(speed_out < SPEED_STABLE)
      {
        speed_out = SPEED_STABLE;
        go();
      }
    }
    FTM_PWM_Duty(SD5_FTM,SD5_CH,steer_mid);
}
/*超声波模块*/ 
uint32 ultrasonic_flag;
uint16 ultrasonic_distance;
void Ultrasonic_Init()
{
    gpio_init(TRIG,GPO,0);
    gpio_init(ECHG,GPI,0);
    gpio_init(TRIG_LEFT,GPO,0);
    gpio_init(ECHG_LEFT,GPI,0);
    gpio_init(TRIG_RIGHT,GPO,0);
    gpio_init(ECHG_RIGHT,GPI,0);
    ultrasonic_distance = 1200;
}
void Ultrasonic_Compute()
{
    uint16 ul;                       //暂存
    //////////////////////////////////////////////
    ultrasonic_flag = 0;
    gpio_set(TRIG,1);               //产生触发脉冲
    DELAY_US(15);
    gpio_set(TRIG,0);
    while(gpio_get(ECHG) == 0);             //等待电平变高，低电平一直等待
    pit_time_start(PIT1);                 //开始计时
    while(gpio_get(ECHG) == 1 && pit_time_get_us(PIT1) <= 8000)              //等待电平变低，高电平一直等待
    {
//        ultrasonic_flag++;
//        if(ultrasonic_flag >UL_FLAGWAIT)
//            break; 
    };             
    ul = pit_time_get_us(PIT1);    //停止计时，获取计时时间    
    if(turn_direction < 2)
      turn_direction = 1;
    pit_time_close(PIT1);
    /////////////////////////////////////////////
    ultrasonic_flag = 0;
    gpio_set(TRIG_LEFT,1);               //产生触发脉冲
    DELAY_US(15);
    gpio_set(TRIG_LEFT,0);
    while(gpio_get(ECHG_LEFT) == 0);             //等待电平变高，低电平一直等待
    pit_time_start(PIT1);                 //开始计时
    while(gpio_get(ECHG_LEFT) == 1 && pit_time_get_us(PIT1) <= 8000)              //等待电平变低，高电平一直等待
    {
//        ultrasonic_flag++;
//        if(ultrasonic_flag >UL_FLAGWAIT)
//            break; 
    };   
    ul = pit_time_get_us(PIT1) > ul ? ul : pit_time_get_us(PIT1);    //停止计时，获取计时时间
    pit_time_close(PIT1);
    /////////////////////////////////////////////
    ultrasonic_flag = 0;
    gpio_set(TRIG_RIGHT,1);               //产生触发脉冲
    DELAY_US(15);
    gpio_set(TRIG_RIGHT,0);
    while(gpio_get(ECHG_RIGHT) == 0);             //等待电平变高，低电平一直等待
    pit_time_start(PIT1);                 //开始计时
    while(gpio_get(ECHG_RIGHT) == 1 && pit_time_get_us(PIT1) <= 8000)              //等待电平变低，高电平一直等待
    {
//        ultrasonic_flag++;
//        if(ultrasonic_flag >UL_FLAGWAIT)
//            break; 
    };  
    if(pit_time_get_us(PIT1) < ul){
      ul = pit_time_get_us(PIT1);
      if(turn_direction < 2)
        turn_direction = 0; 
    }
    pit_time_close(PIT1);
    /////////////////////////////////////////////
//    if(ultrasonic_flag <UL_FLAGWAIT )
//    {
        ultrasonic_distance = ul * 340 /2/1000;
//    }       
}
/*电机*/
void motor_init()//TPM0,20khz
{
    FTM_PWM_init(FTM2,FTM_CH0,20*1000,0);
    FTM_PWM_init(FTM2,FTM_CH1,20*1000,0);
    FTM_PWM_Duty(FTM2,FTM_CH0,0);
    FTM_PWM_Duty(FTM2,FTM_CH1,0);
}
/*速度控制*/
uint8 speed_flag = 0;
uint16 speed_out = SPEED_STABLE;
uint16 limit_speed = 2500;
void speed_set()
{
    if(speed_flag == 0)
    {}
    if(speed_flag == 1)
    {}
    if(speed_flag == 2)
    {}
    if(speed_flag == 3)
    {}
}
void speed_limit()
{
    if(limit_flag == 0)
      limit_speed = 2500;
    else
      limit_speed = 3000;
}
/*OLED*/
uint8 menu_choose = 0;
uint8 parameter_choose = 0;
uint8 scan_choose = 0;
uint8 menu_model = 0;
uint8 oled_show_img_flag = 0;
void OLED_Init()
{
    OLED_init();
    OLED_model();
}
void OLED_show()
{
    switch(menu_model)
    {
      case 0:
        OLED_model();
        oled_show_img_flag = 0;
        break;
      case 1:
        OLED_scan_model();
        oled_show_img_flag = 0;
        break;
      case 2:
        OLED_Parameter();
        oled_show_img_flag = 0;
        break;
      case 3:
        oled_show_img_flag = 1;
        break;
    }
    
}
void OLED_model()
{
    OLED_6x8Str(24,0,"Model Choose",0);
    OLED_6x8Str(24,1,"Car Parameter",0);
    OLED_6x8Str(24,2,"OV7725 Img",0);
    switch(menu_choose)
    {
      case 0:
        OLED_6x8Str(24,0,"Model Choose",1);
        if(key_check(KEY_A) == KEY_DOWN)
        {
          OLED_Fill(0);
          menu_model = 1;
        }
        break;
      case 1:
        OLED_6x8Str(24,1,"Car Parameter",1);
        if(key_check(KEY_A) == KEY_DOWN)
        {
          OLED_Fill(0);
          menu_model = 2;
        }
        break;
      case 2:
        OLED_6x8Str(24,2,"OV7725 Img",1);  
        if(key_check(KEY_A) == KEY_DOWN)
        {
          OLED_Fill(0);
          menu_model = 3;
        }
        break;       
    }
}
void OLED_scan_model()
{
    OLED_6x8Str(0,0,"MODEL1",0);
    OLED_6x8Str(0,1,"MODEL2",0);
    OLED_6x8Str(0,2,"Return Menu",0);
    switch(model)
    {
      case 0:
        OLED_6x8Str(42,0," ",1);
        OLED_6x8Str(42,1," ",0);
        break;
      case 1:
        OLED_6x8Str(42,0," ",0);
        OLED_6x8Str(42,1," ",1);
        break;
    }
    switch(scan_choose)
    {
      case 0:
        OLED_6x8Str(0,0,"MODEL1",1);
        if(key_check(KEY_A) == KEY_DOWN)
        {
          OLED_6x8Str(42,0," ",1);
          OLED_6x8Str(42,1," ",0);
          model = 0;
        }
        break;
      case 1:
        OLED_6x8Str(0,1,"MODEL2",1);
        if(key_check(KEY_A) == KEY_DOWN)
        {
          OLED_6x8Str(42,0," ",0);
          OLED_6x8Str(42,0," ",1);
          model = 1;
        }
        break;
      case 2:
        OLED_6x8Str(0,2,"Return Menu",1);
        if(key_check(KEY_A) == KEY_DOWN)
        {
          OLED_Fill(0);
          menu_model =0;
        }
        break;
    }
}
void OLED_show_img()
{  
    img_extract(img, imgbuff,CAMERA_SIZE);
    for(int i=0;i<CAMERA_H;i++)          //行 
      for(int j=0;j<CAMERA_W;j++)    //列 
        img_OLED[i][j]= img[i*CAMERA_W+j];
    OLED_PrintImage(img_OLED);
    if(key_check(KEY_A) == KEY_DOWN)
    {
        OLED_Fill(0);
        menu_model = 0;
    }
}
void OLED_Parameter()
{
    OLED_6x8Str(0,0,"STE",0);
    OLED_6x8Str(0,1,"SPE",0);
    OLED_6x8Str(0,2,"ULT",0);
    OLED_6x8Str(0,3,"WUC",0);
    OLED_6x8Str(0,4,"DIS",0);
    OLED_6x8Str(0,5,"NUM",0);
    OLED_6x8Str(0,6,"ENC",0);
    OLED_6x8Str(0,7,"Return Menu",0);
    
    OLED_6x8Str(64,0,"LIM",0);
    OLED_6x8Str(64,1,"TUR",0);
    OLED_6x8Str(64,2,"MOD",0);
    OLED_6x8Str(64,3,"XXX",0);
    OLED_6x8Str(64,4,"XXX",0);
    OLED_6x8Str(64,5,"XXX",0);
    OLED_6x8Str(64,6,"XXX",0);
    OLED_6x8Str(64,7,"XXX",0);
    OLED_xs_number(24,0,steer_mid);
    OLED_xs_number(24,1,speed_out);
    OLED_xs_number(24,2,ultrasonic_distance);
    OLED_xs_number(24,3,steer_wucha);
    OLED_xs_number(24,4,turn_distance+steer_wucha);
    OLED_xs_number(24,5,stop_time);
    OLED_xs_number(24,6,speed_count);
          
    OLED_xs_number(88,0,limit_speed);
    OLED_xs_number(88,1,turn_distance);
    OLED_xs_number(88,2,model);
    switch(parameter_choose)
    {
      case 0:
        OLED_6x8Str(0,0,"STE",1);break;
      case 1:
        OLED_6x8Str(0,1,"SPE",1);break;
      case 2:
        OLED_6x8Str(0,2,"ULT",1);break;
      case 3:
        OLED_6x8Str(0,3,"WUC",1);break;
      case 4:
        OLED_6x8Str(0,4,"DIS",1);break;
      case 5:
        OLED_6x8Str(0,5,"NUM",1);break;
      case 6:
        OLED_6x8Str(0,6,"ENC",1);break;
      case 7:
        OLED_6x8Str(0,7,"Return Menu",1);
        if(key_check(KEY_A) == KEY_DOWN)
        {
          OLED_Fill(0);
          menu_model = 0;
        }
        break;
      case 8:
        OLED_6x8Str(64,0,"LIM",1);break;
      case 9:
        OLED_6x8Str(64,1,"TUR",1);break;
      case 10:
        OLED_6x8Str(64,2,"XXX",1);break;
      case 11:
        OLED_6x8Str(64,3,"XXX",1);break;
      case 12:
        OLED_6x8Str(64,4,"XXX",1);break;
      case 13:
        OLED_6x8Str(64,5,"XXX",1);break;
      case 14:
        OLED_6x8Str(64,6,"XXX",1);break;
      case 15:
        OLED_6x8Str(64,7,"XXX",1);break;
    }
}
/*按键*/
void checkKey()
{
    switch(menu_model)
    {
      case 0:
        switch(menu_choose)
        {
          case 0:
            if(key_check(KEY_U) == KEY_DOWN)
              menu_choose = 2;
            else if(key_check(KEY_D) == KEY_DOWN)
              menu_choose++;
            break;
          case 1:
            if(key_check(KEY_U) == KEY_DOWN)
              menu_choose--;
            else if(key_check(KEY_D) == KEY_DOWN)
              menu_choose++;
            break;
          case 2:
            if(key_check(KEY_U) == KEY_DOWN)
              menu_choose--;
            else if(key_check(KEY_D) == KEY_DOWN)
              menu_choose = 0;
            break;
        }
        break;
      case 1:
        switch(scan_choose)
        {
          case 0:
            if(key_check(KEY_U) == KEY_DOWN)
              scan_choose = 2;
            else if(key_check(KEY_D) == KEY_DOWN)
              scan_choose++;
            break;
          case 1:
            if(key_check(KEY_U) == KEY_DOWN)
              scan_choose--;
            else if(key_check(KEY_D) == KEY_DOWN)
              scan_choose++;
            break;
          case 2:
            if(key_check(KEY_U) == KEY_DOWN)
              scan_choose--;
            else if(key_check(KEY_D) == KEY_DOWN)
              scan_choose = 0;
            break;
        }
        break;
      case 2:
        switch(parameter_choose)
        {
            case 0:
              if(key_check(KEY_U) == KEY_DOWN)
                parameter_choose = 7;
              else if(key_check(KEY_D) == KEY_DOWN)
                parameter_choose++;
              else if(key_check(KEY_R) == KEY_DOWN)
                parameter_choose+=8;
              break;
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
              if(key_check(KEY_U) == KEY_DOWN)
                parameter_choose--;
              else if(key_check(KEY_D) == KEY_DOWN)
                parameter_choose++;
              else if(key_check(KEY_R) == KEY_DOWN)
                parameter_choose+=8;
              break;
            case 7:
              if(key_check(KEY_U) == KEY_DOWN)
                parameter_choose--;
              else if(key_check(KEY_D) == KEY_DOWN)
                parameter_choose = 0;
              else if(key_check(KEY_R) == KEY_DOWN)
                parameter_choose+=8;
              break;
            case 8:
              if(key_check(KEY_U) == KEY_DOWN)
                parameter_choose = 15;
              else if(key_check(KEY_D) == KEY_DOWN)
                parameter_choose++;
              else if(key_check(KEY_L) == KEY_DOWN)
                parameter_choose-=8;
              break;
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
              if(key_check(KEY_U) == KEY_DOWN)
                parameter_choose--;
              else if(key_check(KEY_D) == KEY_DOWN)
                parameter_choose++;
              else if(key_check(KEY_L) == KEY_DOWN)
                parameter_choose-=8;
              break;
            case 15:
              if(key_check(KEY_U) == KEY_DOWN)
                parameter_choose--;
              else if(key_check(KEY_D) == KEY_DOWN)
                parameter_choose = 8;
              else if(key_check(KEY_L) == KEY_DOWN)
                parameter_choose-=8;
              break;
        }
        break;
    }
      
}
/*舵机*/
uint16 steer_out = STEER_MID;
uint16 steer_mid_debug = STEER_MID;
uint16 steer_mid = STEER_MID;
uint16 turn_distance = 500;
uint16 steer_wucha = 0;
int debug_num = 0;
void Steer_Init()
{
    FTM_PWM_init(FTM1,FTM_CH0,100,STEER_MID);
}
/*转向*/
uint16 stop_time = 300;
uint8 turn_direction = 0;
void Turn()
{
    if(ultrasonic_distance <= turn_distance + steer_wucha)
    {
      DisableInterrupts;
      slow();
      turn_direction = 0;
//      switch(turn_direction)
//      {
//        case 2: right();turn_direction = 0;break;
//        case 3: left();turn_direction = 0;break;
//      }
      DELAY_MS(500);
//      front();
      go();
      EnableInterrupts;
    }
    else 
    { 
//      if(steer_mid != steer_mid_debug){
//        FTM_PWM_Duty(FTM0,FTM_CH2,steer_mid_debug);
//      }
      if(speed_out < SPEED_STABLE)
      {
        speed_out = SPEED_STABLE;
      }
      if(ud == 0)
        go();
      else 
        back();
    }
}
void slow()
{
    if(ud == 0)
    {
      if(speed_out != 0){
      FTM_PWM_Duty(FTM2,FTM_CH0,SPEED_SLOW);
      FTM_PWM_Duty(FTM2,FTM_CH1,0);
      }
    }
    switch(turn_direction)
    {
      case 0: left();turn_direction = 2;break;
      case 1: right();turn_direction = 3;break;
    }
    DELAY_MS(500);
}
void stop()
{
    FTM_PWM_Duty(FTM2,FTM_CH0,0);
    FTM_PWM_Duty(FTM2,FTM_CH1,0);
//    FTM_PWM_Duty(SD5_FTM,SD5_CH,steer_mid_debug);
    if(speed_count >= 800){
      speed_out = 2000;
      if(ud == 0)
        back();
      else
        go();
      ud = ud == 0 ? 1 : 0;
      DELAY_MS(stop_time + speed_out / 10);
    }
    if(speed_count >= 400){
      speed_out = SPEED_STABLE;
      if(ud == 0)
        back();
      else
        go();
      DELAY_MS(stop_time + speed_out / 10);
    }
    speed_out = 0;
    go();
}
void go()
{
//    int i;
//    for(i =0;i <= 1000;i+=10)
//    if(speed_out == 0)
//      speed_out = 1000;
    FTM_PWM_Duty(FTM2,FTM_CH0,speed_out);
    FTM_PWM_Duty(FTM2,FTM_CH1,0);
    ud = 0;
}
void back()
{   
//    int i;
//    for(i =0;i <= 1000;i+=10)
//    if(speed_out == 0)
//      speed_out = 1000;
    FTM_PWM_Duty(FTM2,FTM_CH0,0);
    FTM_PWM_Duty(FTM2,FTM_CH1,speed_out);
    ud = 1;
}
void left()
{
    steer_mid = STEER_LEFT_MAX;
    FTM_PWM_Duty(SD5_FTM,SD5_CH,steer_mid);
}
void right()
{
    steer_mid = STEER_RIGHT_MAX;
    FTM_PWM_Duty(SD5_FTM,SD5_CH,steer_mid);
}
void front()
{
    steer_mid = steer_mid_debug;
    FTM_PWM_Duty(SD5_FTM,SD5_CH,steer_mid);
}
/*编码器*/
uint8 pit0_flag = 0;
uint32 speed_count = 0;
void Encoder_Compute()
{
   if( pit0_flag > 0 )                                                           
   {
      speed_count = DMA_count_get(DMA_CH1);
      DMA_count_reset(DMA_CH1);
      pit0_flag = 0;                                //清空pit中断标志位
    }
}
/*拨码开关*/
//void boma_key()
//{
//    if(!gpio_get(boma_1))
//    {
//       
//    }
//    if(!gpio_get(boma_5))
//    {
//        speed_flag = 0;
//    }
//    if(!gpio_get(boma_6))
//    {
//        speed_flag = 1;
//    }
//    if(!gpio_get(boma_7))
//    {
//        speed_flag = 2;
//    }
//    if(!gpio_get(boma_8))
//    {
//        speed_flag = 3;
//    }    
//}
/*中断函数*/
void PORTA_IRQHandler()
{
    uint8  n = 0;    //引脚号
    uint32 flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //清中断标志位
    n = 29;                                             //场中断
    if(flag & (1 << n))                                 //PTA29触发中断
    {
        camera_vsync();
    }
}
void DMA0_IRQHandler()
{
    camera_dma();
    if(oled_show_img_flag == 1)
      OLED_show_img();
    Scan();    
}
void DMA1_IRQHandler()
{
    DMA_IRQ_CLEAN(DMA_CH1);                             //清除通道传输中断标志位    (这样才能再次进入中断)
    DMA_EN(DMA_CH1);                                    //使能通道CHn 硬件请求      (这样才能继续触发DMA传输)
}
void PIT0_IRQHandler()
{
    pit0_flag = 1;
    Ultrasonic_Compute();
    PIT_Flag_Clear(PIT0);
}
void uart4_handler()
{
    char ch;
    if(uart_querychar(UART4,&ch)){
    switch(ch)
    {
      case 'U': speed_out = SPEED_STABLE;go();break;
      case 'D': speed_out = SPEED_STABLE;back();break;
      case 'L': if(steer_mid == steer_mid_debug){
                    steer_mid = STEER_LEFT_MAX - 250;
                    FTM_PWM_Duty(SD5_FTM,SD5_CH,steer_mid);
                }
                break;
      case 'R': if(steer_mid == steer_mid_debug){
                    steer_mid = STEER_RIGHT_MAX + 250;
                    FTM_PWM_Duty(SD5_FTM,SD5_CH,steer_mid);
                }
               break;
      case 'F': front();break;
      case 'A': stop_time+=10;break;
      case 'M': stop_time-=10;break;
      case 'S': speed_out = 0;stop();break;
      case 'N': if(limit_flag == 0) limit_flag = 1;else limit_flag = 0;break;
      case '1': 
            if(speed_out > 0)
                speed_out-=500;
            if(ud == 0)
                go();
            else back();
                break;
      case '2': 
            if(speed_out < limit_speed)
                speed_out+=500;
             if(ud == 0)
                go();
             else 
                back();
             break;
      case '3': steer_mid_debug--;steer_mid = steer_mid_debug;FTM_PWM_Duty(FTM0,FTM_CH2,steer_mid_debug);break;
      case '4': steer_mid_debug++;steer_mid = steer_mid_debug;FTM_PWM_Duty(FTM0,FTM_CH2,steer_mid_debug);break;
     }
    }
    steer_wucha = (int)(160 * 0.583 * speed_out / 1000 + debug_num);
    uart_tx_irq_dis(UART4);
}
/*仿真*/
//void simular()
//{
//    string speed_out_s 
//}