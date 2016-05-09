#ifndef _menu_H_
#define _menu_H_
#include "common.h"
volatile uint8 ButtonPressed=0;
char number[10]="0123456789";
uint8 str[6]="0";
static const uint8 *const MenuText[]={
  "FREESCALE",
  "GOD_BLESS",
  "1.Steer_Control",
  "2.Speed_Control",
  "3.Important_Parameter",
  "4.Close_Menu"
};
static const uint8 *const SteerText[]={
  "1.Kp",
  "2.Kd",
  "3.KL",
  "4.KR"
};
static const uint8 *const SpeedText[]={
  "1.Ki_motor",
  "2.Kp_motor",
  "3.Set_Speed",
  "4.chasu"
};
void num2str(uint8 x,uint8 y,int32 num)
{
   str[5]='\0';
   uint8 i;
   for(i=5;i>0;i--)
   {
    str[i]=number[num%10];
    num/=10;
   }
   if(num<0)
      OLED_6x8Str(i-2,j,'-',0);
   else
      OLED_6x8Str(i-2,j,'+',0);
   OLED_6x8Str(i,j,str,0);
}
void numtochar(uint32 num,uint8 i)
{
    unsigned char str[6]="0";
    uint8 a;
    for(a=5;a>0;a--)
    {
        str[a]=number[num%10];
        num/=10;
    }
    OLED_6x8Str(85,i,str,0);
}
void menu()
{
  uint8 Select_Menu=0;
  ButtonPressed=0;
  Select_Menu=Menu_active((uint8 **)MenuText,5);
  if(Button_S3);//按下确定
  else
  {
  ButtonPressed=0;
  switch(Select_Menu)
  {
  case 2:Speed_adjust();break;
  case 3:Steer_adjust();break;
  case 4:break;
  case 5:Close_Menu();break;
  default:break;
  }
  }
}
void Menu_active(uint8 **MenuText,uint8 numltems)
{
 uint8 i;
 uint8 position=2;
 uint8 LastPosition=6;
 ButtonPressed=0;
 OLED_6x8Str(0,0,menuText[0],0); 
 OLED_6x8Str(0,1,menuText[1],0); 
 while(Button_S4)//退出
 {
  if(!Button_S1)//加
  {
   position++;
   if(position >= numItems)position = numItems;   
    ButtonPressed = 0;  
  }
  if(!Button_S2)//减
  {
   position--;
   if(position <2)position = 2;   
    ButtonPressed = 0;  
  }
   if(position != lastPosition)
        {
            for(i = 2; i <= numItems ; i++)
            {
                if(i != position)OLED_6x8Str(0,i,MenuText[i],0); 
                if(i == position)OLED_6x8Str(0,i,MenuText[i],1); 
            }
    lastPosition = position;
        }
        camera_get_img();
  
 }
 return position;
}

void Menu(void);
void show_data(void);
void Speed_adjust()
{
 ButtonPressed=0;
 uint8 NextOne=0;
 OLED_Fill(0x00);
 //显示菜单
 OLED_6x8Str(0,0,SpeedText[0],0); 
 OLED_6x8Str(0,1,SpeedText[1],0);
 OLED_6x8Str(0,2,SpeedText[2],0);
 OLED_6x8Str(0,3,SpeedText[3],0);
 while(Button_S4)//退出
 {
   camera_get_img(); 
   switch(NextOne)
   {
   case 0: 
     if(!Button_S1) {Ki_Mor++; ButtonPressed=0;}
     if(!Button_S2) {Ki_Mor--; ButtonPressed=0;}
     if(!Button_S3) {NextOne=1; ButtonPressed=0;}
    numtochar(Ki_Mor,0);
    OLED_6x8Str(0,0,SpeedText[0],1); 
    OLED_6x8Str(0,1,SpeedText[1],0);
    OLED_6x8Str(0,2,SpeedText[2],0);
    OLED_6x8Str(0,3,SpeedText[3],0);
    break;
   case 1:
     if(!Button_S1) {Kp_Mor++; ButtonPressed=0;}
     if(!Button_S2) {Kp_Mor--; ButtonPressed=0;}
     if(!Button_S3) {NextOne=2; ButtonPressed=0;}
    numtochar(Kp_Mor,0);
    OLED_6x8Str(0,0,SpeedText[0],0); 
    OLED_6x8Str(0,1,SpeedText[1],1);
    OLED_6x8Str(0,2,SpeedText[2],0);
    OLED_6x8Str(0,3,SpeedText[3],0);
    break;
   case 2:
     if(!Button_S1) {value_R++;value_L++; ButtonPressed=0;}
     if(!Button_S2) {value_R--;value_L--; ButtonPressed=0;}
     if(!Button_S3) {NextOne=3; ButtonPressed=0;}
    numtochar(value_R,0);
    OLED_6x8Str(0,0,SpeedText[0],0); 
    OLED_6x8Str(0,1,SpeedText[1],0);
    OLED_6x8Str(0,2,SpeedText[2],1);
    OLED_6x8Str(0,3,SpeedText[3],0);
    break;
   case 3:
     if(!Button_S1) {m++; ButtonPressed=0;}
     if(!Button_S2) {m--; ButtonPressed=0;}
     if(!Button_S3) {NextOne=0; ButtonPressed=0;}
    numtochar(m,0);
    OLED_6x8Str(0,0,SpeedText[0],0); 
    OLED_6x8Str(0,1,SpeedText[1],0);
    OLED_6x8Str(0,2,SpeedText[2],0);
    OLED_6x8Str(0,3,SpeedText[3],1);
    break;
   default:break;
   }
 }
 OLED_Fill(0x00);
}
void Steer_adjust()
{
ButtonPressed=0;
 uint8 NextOne=0;
 OLED_Fill(0x00);
 //显示菜单
 OLED_6x8Str(0,0,SteerText[0],0); 
 OLED_6x8Str(0,1,SteerText[1],0);
 OLED_6x8Str(0,2,SteerText[2],0);
 OLED_6x8Str(0,3,SteerText[3],0);
 while(Button_S4)//退出
 {
   camera_get_img(); 
   switch(NextOne)
   {
   case 0: 
     if(!Button_S1) {Kp++; ButtonPressed=0;}
     if(!Button_S2) {Kp--; ButtonPressed=0;}
     if(!Button_S3) {NextOne=1; ButtonPressed=0;}
    numtochar(Kp,0);
    OLED_6x8Str(0,0,SteerText[0],1); 
    OLED_6x8Str(0,1,SteerText[1],0);
    OLED_6x8Str(0,2,SteerText[2],0);
    OLED_6x8Str(0,3,SteerText[3],0);
    break;
   case 1:
     if(!Button_S1) {Kd++; ButtonPressed=0;}
     if(!Button_S2) {Kdr--; ButtonPressed=0;}
     if(!Button_S3) {NextOne=2; ButtonPressed=0;}
    numtochar(Kd,0);
    OLED_6x8Str(0,0,SpeedText[0],0); 
    OLED_6x8Str(0,1,SpeedText[1],1);
    OLED_6x8Str(0,2,SpeedText[2],0);
    OLED_6x8Str(0,3,SpeedText[3],0);
    break;
   case 2:
     if(!Button_S1) {L++; ButtonPressed=0;}
     if(!Button_S2) {L--; ButtonPressed=0;}
     if(!Button_S3) {NextOne=3; ButtonPressed=0;}
    numtochar(L,0);
    OLED_6x8Str(0,0,SteerText[0],0); 
    OLED_6x8Str(0,1,SteerText[1],0);
    OLED_6x8Str(0,2,SteerText[2],1);
    OLED_6x8Str(0,3,SteerText[3],0);
    break;
   case 3:
     if(!Button_S1) {R++; ButtonPressed=0;}
     if(!Button_S2) {R--; ButtonPressed=0;}
     if(!Button_S3) {NextOne=0; ButtonPressed=0;}
    numtochar(R,0);
    OLED_6x8Str(0,0,SpeedText[0],0); 
    OLED_6x8Str(0,1,SpeedText[1],0);
    OLED_6x8Str(0,2,SpeedText[2],0);
    OLED_6x8Str(0,3,SpeedText[3],1);
    break;
   default:break;
   }
 }
 OLED_Fill(0x00);
}
void Close_Menu()
{

}
#endif