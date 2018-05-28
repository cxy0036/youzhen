#include "main.h"
#include "SN8F5702.H"
#include "delay.h"


//extern uint8_t ;
uint32_t Level3_count = 1728000;//30min    1152000;
uint32_t Level3_off_count = 172800;//3min  230400;
uint8_t Level = 0;
uint8_t Power = 0;
uint8_t LEVEL_UP = 0;
uint8_t POWER_UP = 0;
uint32_t Five_mins_time = 0;
uint32_t Six_hour_time = 27500000;//8h delay
uint8_t Five_mins_no = 0; 
uint16_t Level1_timeout_high = 3840;//4s
uint16_t Level1_timeout_low = 9600;//10s        5660;//6s
uint16_t Level2_timeout_high = 3840;//4s       6140;//6.5s
uint16_t Level2_timeout_low = 7680;//8s        3360;//3.5s
uint16_t Level3_timeout_high = 3840;//4s       7480;//8s
uint16_t Level3_timeout_low = 5760;//6s        1920;//2s


#define POWER_PIN			P14 //P12 
#define LEVEL_PIN			P12 //P14 
#define LED1_PIN			P10 //P00 
#define LED2_PIN			P05 
#define LED3_PIN			P00 //P10 


void main(void)
{
	Delay_Init();
	P11  = 0;
  P1M |=0X02;//P1.1OUTPUT
  
  P1M &= ~ 0x04;//P12INPUT SW3
  P1UR |= 0x04;
  
  P1M &= ~ 0x10;//P14INPUT  SW2
  P1UR |= 0x10;
  
  //P1UR &= ~ 0x01;  
	//P0UR &= ~ 0x21; 

	P10 =0;
  P00 =0;
  P05 =0;
  P1M |=0X01;//P1.0OUTPUT
	P0M |=0X21;//P00,P05OUTPUT
  	
  while(1)
  {
     //WDTR = 0x5A;//??
		if(Six_hour_time == 0)
		{
		  Power =0;//power off
			P00 = 0 ;//LED OFF
			P05 = 0 ;
			P10 = 0 ;
      P11 = 0 ;	
			Level = 0 ;	
		}
  //电源键按下
		if(POWER_PIN == 1)//按键短按有效
	   {
			 POWER_UP = 1;
		 }
    if((POWER_PIN == 0)&&(POWER_UP == 1))//SW3 
    {
       Delay_ms(2);
       if((POWER_PIN == 0)&&(POWER_UP == 1))//SW3
       { 
				 POWER_UP = 0;
				 if(Power == 0)
				 {
					  Level = 1;
					  P11  = 1;
					  Six_hour_time = 27500000;
            Power =1;//power on 
					  Five_mins_time =145500;
				    Level1_timeout_high = 3840;//4s
						Level1_timeout_low = 9600;//10s       5660;//6s
						Level2_timeout_high = 3840;//4s       6140;//6.5s
						Level2_timeout_low = 7680;//8s        3360;//3.5s
						Level3_timeout_high = 3840;//4s       7480;//8s
						Level3_timeout_low = 5760;//6s        1920;//2s
					  Five_mins_no = 0;
				 }else{
				   Power =0;//power off
					 LED1_PIN = 0 ;//LED OFF
					 LED2_PIN = 0 ;
					 LED3_PIN = 0 ;
           P11 = 0 ;	
					 Level = 0 ;	 
				 }				 
       }
      Delay_ms(100);
	  }
	//档位键按下
    if(Power == 1)		
    {
			Six_hour_time--;//6 hour no opetate ,power off
			if(LEVEL_PIN == 1)//短按有效
	     {
				 LEVEL_UP = 1;
			 }
	    if((LEVEL_PIN == 0)&&(LEVEL_UP == 1))//SW2 
	     {
	       Delay_ms(2);
	       if((LEVEL_PIN == 0)&&(LEVEL_UP == 1))//SW2 
	       {
					 Six_hour_time = 27500000;
					 LEVEL_UP = 0;
	         Level++;
	       }
				 if(Level ==4)//加到三挡，返回1
	       {
	         Level =1;
	       }
		     Delay_ms(100);
       }
			 if(Five_mins_time != 0)
			 {
			   Five_mins_time--;//5 mins delay
				 Delay_ms(2);
			 }else{
				   Five_mins_no = 1;
				  }
			 switch(Level)
						 {
							case 1: 
								      LED1_PIN = 1 ;//LED1
											LED2_PIN = 0 ;
											LED3_PIN = 0 ;	
											break;
							case 2: 
								      LED1_PIN = 0 ;//LED2
											LED2_PIN = 1 ;
											LED3_PIN = 0 ;
											break;;
							case 3: 
								      LED1_PIN = 0 ;//LED3
											LED2_PIN = 0 ;
											LED3_PIN = 1 ;
											break;
							default :
											P00 = 0 ;//LED OFF
											P05 = 0 ;
											P10 = 0 ;
											P11 = 0 ;		
											break;	
						 }
			    if(Five_mins_no == 1)
					{
							switch(Level)
						 {
							case 1: //on 4.5s ,off 5.5s
											Level2_timeout_high = 3840;//4s   6140;//6.5s
											Level2_timeout_low = 7680;//8s    3360;//3.5s
											Level3_timeout_high = 3840;//4s   7480;//8s
											Level3_timeout_low =  5760;//6s   1920;//2s     
							        if(Level1_timeout_high != 0)//on 4.5s
											{
												P11 = 1 ;
											  Level1_timeout_high--;
											  Delay_ms(1);
											}
							        if((Level1_timeout_low != 0)&&(Level1_timeout_high == 0))//off 5.5s
											{
												P11 = 0;
												Level1_timeout_low--;
											  Delay_ms(1);
											}
											if((Level1_timeout_high == 0)&&(Level1_timeout_low == 0))//next period
											{
												Level1_timeout_high = 3840;//4s
												Level1_timeout_low = 9600;//10s    5660;//6s
											}
											break;
							case 2: //on 7.2s ,off 2.8s
								      Level1_timeout_high = 3840;//4s
											Level1_timeout_low = 9600;//10s       5660;//6s
											Level3_timeout_high = 3840;//4s       7480;//8s
											Level3_timeout_low = 5760;//6s        1920;//2s
											if(Level2_timeout_high != 0)
											{
												P11 = 1 ;
											  Level2_timeout_high--;
											  Delay_ms(1);
											}
							        if((Level2_timeout_low != 0)&&(Level2_timeout_high == 0))
											{
												Level2_timeout_low--;
											  P11 = 0;
											  Delay_ms(1);
											}
											if((Level2_timeout_high == 0)&&(Level2_timeout_low == 0))
											{
												Level2_timeout_high = 3840;//4s       6240;//6.5s
												Level2_timeout_low = 7680;//8s        3360;//3.5s
											}
											break;
							case 3: //on 9s ,off 1s
											Level1_timeout_high = 3840;//4s
											Level1_timeout_low = 9600;//10s       5660;//6s
											Level2_timeout_high = 3840;//4s       6140;//6.5s
											Level2_timeout_low = 7680;//8s        3360;//3.5s
							        if(Level3_count == 0)//20 mins
											{
											  P11 = 0;
												Level3_off_count--;
												Delay_ms(1);
												if(Level3_off_count == 0)//4 mins
												{
												  Level3_count = 1728000;//30min     1152000;
													Level3_off_count = 172800;//3min   230400;
												}
												break;
											}
											Level3_count--;
											if(Level3_timeout_high != 0)
											{
												P11 = 1 ;
											  Level3_timeout_high--;
											  Delay_ms(1);
											}
							        if((Level3_timeout_low != 0)&&(Level3_timeout_high == 0))
											{
												Level3_timeout_low--;
											  P11 = 0;
											  Delay_ms(1);
											}
											if((Level3_timeout_high == 0)&&(Level3_timeout_low == 0))
											{
												Level3_timeout_high = 3840;//4s       7480;
											  Level3_timeout_low = 5760;//6s        1920;
											}
											break;
							default :
											Level1_timeout_high = 3840;//4s
											Level1_timeout_low = 9600;//10s       5660;//6s
											Level2_timeout_high = 3840;//4s       6140;//6.5s
											Level2_timeout_low = 7680;//8s        3360;//3.5s
											Level3_timeout_high = 3840;//4s       7480;//8s
											Level3_timeout_low = 5760;//6s        1920;//2s
//								      Level1_timeout_high = 3840;//4s
//											Level1_timeout_low = 5660;//6s
//											Level2_timeout_high = 6140;//6.5s
//											Level2_timeout_low = 3360;//3.5s
//											Level3_timeout_high = 7480;//8s
//											Level3_timeout_low = 1920;//2s
											break;	
						 }
					}
	 
     }
   }
}
