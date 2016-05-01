#include "menu.h"
#define Button_S1 gpio_get(PTE0)
#define Button_S2 gpio_get(PTE1)
#define Button_S3 gpio_get(PTE2)
#define Button_S4 gpio_get(PTE3)
void Menu()
{
  //显示菜单界面
  OLED_Fill(0);
  OLED_6x8Str(40,0,"Menu");
  OLED_6x8Char(32,up+1,'-');
  OLED_6x8Char(39,up+1,'>');
  OLED_6x8Str(46,1,"1.SteerContol");
  OLED_6x8Str(46,2,"2.MotorContol");
  if(!gpio_get(PTE0))   //4
  {
    up=~up&0x01;
    OLED_Fill(0);
    OLED_6x8Str(40,0,"Menu");
    OLED_6x8Char(32,up+1,'-');
    OLED_6x8Char(39,up+1,'>');
    OLED_6x8Str(46,1,"1.SteerContol");
    OLED_6x8Str(46,2,"2.MotorContol");
    while(gpio_get(PTE3)&0x01);  //5
  }
  if(!gpio_get(PTE1))    //3
  {  DELAY_MS(100);
  if(!gpio_get(PTE1))
  {
     OLED_Fill(0);
    if(up==0)//舵机控制部分
    {   
     do{
        camera_get_img();                   //摄像头获取图像
        Steer_Turn();
        show_data();
        OLED_6x8Str(90,7,show);
        Show_Path();           //根据射线画三条线
        OLED_PrintImage(s);   //显示赛道
        
        if(!gpio_get(PTE1)&0x01)   //选择调参数
        {
            DELAY_MS(120);
         if(!gpio_get(PTE1)&0x01)
         {
           
        while(1)
        {
         OLED_Fill(0);
         OLED_6x8Str(40,0,"Menu");
         OLED_6x8Char(32,up+1,'-');
         OLED_6x8Char(39,up+1,'>');
         OLED_6x8Str(46,1,"1.Kp");
         OLED_6x8Str(46,2,"2.Kd");
         OLED_6x8Str(46,3,"3.kL");
         OLED_6x8Str(46,4,"4.kR");
         if(!gpio_get(PTE0)&0x01)
         {
           DELAY_MS(100);
         if(!gpio_get(PTE0)&0x01)
         {
         up++;
         up=up%4;
         }
         }
         if(!gpio_get(PTE1))//调整参数
         {  DELAY_MS(100);
         if(!gpio_get(PTE1))
          do{
          switch(up)
          {
          case 0: 
            if(!gpio_get(PTE0))    //4
               {
                DELAY_MS(100);
                if(!gpio_get(PTE0)) 
                Kp++;
                 }
            if(!gpio_get(PTE2))  //2
               {
                DELAY_MS(100);
                 if(!gpio_get(PTE2))
                  Kp--;
                 } 
            OLED_3number((char *)show,(int16)Kp);//转化为字符串
            OLED_6x8Str(80,1,show);
                    break;
          case 1:
            if(!gpio_get(PTE0))
               {
                DELAY_MS(100);
                if(!gpio_get(PTE0)) 
                Kd++;
                 }
            if(!gpio_get(PTE2))
               {
                DELAY_MS(100);
                 if(!gpio_get(PTE2))
                  Kd--;
                 } 
            OLED_3number((char *)show,Kd);//转化为字符串
            OLED_6x8Str(80,2,show);
                    break;
          case 2:
            if(!gpio_get(PTE0))
               {
                DELAY_MS(100);
                if(!gpio_get(PTE0)) 
                L++;
                 }
            if(!gpio_get(PTE2))
               {
                DELAY_MS(100);
                 if(!gpio_get(PTE2))
                  L--;
                 } 
            OLED_3number((char *)show,L);//转化为字符串
            OLED_6x8Str(80,3,show);
                    break;
          case 3:
            if(!gpio_get(PTE0))
               {
                DELAY_MS(100);
                if(!gpio_get(PTE0)) 
                R++;
                 }
            if(!gpio_get(PTE2))
               {
                DELAY_MS(100);
                 if(!gpio_get(PTE2))
                  R--;
                 } 
            OLED_3number((char *)show,R);//转化为字符串
            OLED_6x8Str(80,4,show);
                    break;    
          default:break;
          }
          }
           while(gpio_get(PTE3)&0x01);
         }  
         if(!gpio_get(PTE3)&0x01)
         {
           DELAY_MS(150);
         if(!gpio_get(PTE3)&0x01)
         { up=0;
           break;
         }
         }
        }
        }
        }
     }
        while(gpio_get(PTE3)&0x01);
    }
    else if(up==1) //速度控制部分
    {    up=0;
      while(1)
      {
         OLED_Fill(0);
         OLED_6x8Str(40,0,"Menu");
         OLED_6x8Char(12,up+1,'-');
         OLED_6x8Char(19,up+1,'>');
         OLED_6x8Str(26,1,"1.Ki_Mor");
         OLED_6x8Str(26,2,"2.Kp_Mor");
         OLED_6x8Str(26,3,"3.Set_Speed");
         if(!gpio_get(PTE0)&0x01)
         {
           DELAY_MS(100);
         if(!gpio_get(PTE0)&0x01)
         {
         up++;
         up=up%3;
         }
         }
        else if(!gpio_get(PTE1))//调整参数
         { 
           DELAY_MS(100);
         if(!gpio_get(PTE1))
         {
          do
          {
         switch(up)
         {
         case 0:
              if(!gpio_get(PTE0))
               {
                DELAY_MS(100);
                if(!gpio_get(PTE0)) 
                Ki_Mor++;
               
                 }
            if(!gpio_get(PTE2))
               {
                DELAY_MS(100);
                 if(!gpio_get(PTE2))
                  Ki_Mor--;
                  
                 } 
            OLED_3number((char *)show,Ki_Mor);//转化为字符串
            OLED_6x8Str(80,1,show);
                break;
         case 1:
            if(!gpio_get(PTE0))
               {
                DELAY_MS(100);
                if(!gpio_get(PTE0)) 
                Kp_Mor++;
                 }
            if(!gpio_get(PTE2))
               {
                DELAY_MS(100);
                 if(!gpio_get(PTE2))
                  Kp_Mor--;
                 } 
            OLED_3number((char *)show,Kp_Mor);//转化为字符串
            OLED_6x8Str(80,2,show);
                break;
         case 2:
            if(!gpio_get(PTE0))
               {
                DELAY_MS(100);
                if(!gpio_get(PTE0)) 
                {
                value_R++;
                value_L++;
                }
                 }
            if(!gpio_get(PTE2))
               {
                DELAY_MS(100);
                 if(!gpio_get(PTE2))
                 {
                   value_R--;
                   value_L--;
                 }
                 } 
            OLED_3number((char *)show,value_R);//转化为字符串
            OLED_6x8Str(80,3,show);
          
                break;
         default:break;
         }
          }while(gpio_get(PTE3)&0x01);
         }
         }
      if(!gpio_get(PTE3))
      {
        DELAY_MS(100);
        if(!gpio_get(PTE3))
          break;
      }
      }
    }
     while(gpio_get(PTE3)&0x01);
  }
  }
}
void show_data()
{
        OLED_3number((char *)show,(int16)Kp);//转化为字符串
        OLED_6x8Char(82,0,'p');
        OLED_6x8Str(90,0,show);
        OLED_3number((char *)show, Kd);//转化为字符串
        OLED_6x8Char(82,1,'d');
        OLED_6x8Str(90,1,show);
        OLED_3number((char *)show,Mid_farCount);//转化为字符串
        OLED_6x8Char(82,2,'M');
        OLED_6x8Str(90,2,show);
        OLED_3number((char *)show,L_Count);//转化为字符串
        OLED_6x8Char(82,3,'L');
        OLED_6x8Str(90,3,show);
        OLED_3number((char *)show,R_Count);//转化为字符串
        OLED_6x8Char(82,4,'R');
        OLED_6x8Str(90,4,show);
        OLED_3number((char *)show,(int16)Kp_temp);//转化为字符串
        OLED_6x8Char(82,5,'P');
        OLED_6x8Str(90,5,show);
        OLED_3number((char *)show,turn_error);//转化为字符串
        OLED_6x8Char(82,6,'T');
        OLED_6x8Str(90,6,show);
        OLED_3number((char *)show, Steer_PWM_Out-PWM_MID);//转化为字符串
        OLED_6x8Char(82,7,'S');
        OLED_6x8Str(90,7,show);
}



