#include "Motor.h"
void Motor_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE); //使能PB PC端口时钟
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	//端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;	//端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
  GPIO_Init(GPIOC, &GPIO_InitStructure);					      //根据设定参数初始化GPIOC 
}
void PWM_Init(u16 arr,u16 psc)
{		 		
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	Motor_Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);  //使能GPIO外设时钟使能
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //TIM_CH1 //TIM_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

  TIM_CtrlPWMOutputs(TIM8,ENABLE);	//MOE 主输出使能	

	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH1预装载使能	
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH1预装载使能	
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH1预装载使能		
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH4预装载使能	 
	
	TIM_ARRPreloadConfig(TIM8, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM_Cmd(TIM8, ENABLE);  //使能TIM
}

/*
v =(0,100)
*/
void Go(int v){
	ENLB=1;
	ENLF=0;
	ENRF=0;
	ENRB=1;
	
	PWMLB=7200-72*v;
	PWMLF=72*v;
	PWMRF=72*v;
	PWMRB=7200-72*v;
}

void Back(int v){
	ENLB=0;
	ENLF=1;
	ENRF=1;
	ENRB=0;
	
	PWMLB=72*v;
	PWMLF=7200-72*v;
	PWMRF=7200-72*v;
	PWMRB=72*v;
}

/*
pwm control left motor 
pwm =(0,100)
*/
void TurnLeft(int v, int pwm){
	ENLB=1;
	ENLF=0;
	ENRF=0;
	ENRB=1;
	
	PWMLB=7200-(int)(0.72*v*pwm);
	PWMLF=(int)(0.72*v*pwm);
	PWMRF=72*v;
	PWMRB=7200-72*v;
	
}

void TurnRight(int v, int pwm){
	ENLB=1;
	ENLF=0;
	ENRF=0;
	ENRB=1;
	
	PWMLB=7200-72*v;
	PWMLF=72*v;
	PWMRF=(int)(0.72*v*pwm);
	PWMRB=7200-(int)(0.72*v*pwm);
}

void Stop(void){
	ENLB=1;
	ENLF=0;
	ENRF=0;
	ENRB=1;
	
	PWMLB=7200;
	PWMLF=0;
	PWMRF=0;
	PWMRB=7200;
}

//
void VerticalR(int v){
	ENLB=0;//re
	ENLF=0;
	ENRF=1;//re
	ENRB=1;
	
	PWMLB=72*v;
	PWMLF=72*v;
	PWMRF=7200-72*v;
	PWMRB=7200-72*v;
}
void VerticalRRight(int v,int pwm){
	ENLB=0;//re
	ENLF=0;
	ENRF=1;//re
	ENRB=1;
	
	PWMLB=(int)(0.72*v*pwm);
	PWMLF=72*v;
	PWMRF=7200-72*v;
	PWMRB=7200-(int)(0.72*v*pwm);
}
void VerticalRLeft(int v,int pwm){
	ENLB=0;//re
	ENLF=0;
	ENRF=1;//re
	ENRB=1;
	
	PWMLB=72*v;
	PWMLF=(int)(0.72*v*pwm);
	PWMRF=7200-(int)(0.72*v*pwm);
	PWMRB=7200-72*v;
}

void VerticalL(int v){
	ENLB=1;
	ENLF=1;//re
	ENRF=0;
	ENRB=0;//re
	
	PWMLB=7200-72*v;
	PWMLF=7200-72*v;
	PWMRF=72*v;
	PWMRB=72*v;
}
void VerticalLRight(int v,int pwm){
	ENLB=1;
	ENLF=1;//re
	ENRF=0;
	ENRB=0;//re
	
	PWMLB=7200-72*v;
	PWMLF=7200-(int)(0.72*v*pwm);
	PWMRF=(int)(0.72*v*pwm);
	PWMRB=72*v;
}
void VerticalLLeft(int v,int pwm){
	ENLB=1;
	ENLF=1;//re
	ENRF=0;
	ENRB=0;//re
	
	PWMLB=7200-(int)(0.72*v*pwm);
	PWMLF=7200-72*v;
	PWMRF=72*v;
	PWMRB=(int)(0.72*v*pwm);
}

void test(int a,int b, int c,int d){
	PWMLB=a;
	PWMLF=b;
	PWMRF=c;
	PWMRB=d;
}
void test_en(int a,int b, int c,int d){
	ENLB=a;
	ENLF=b;
	ENRF=c;
	ENRB=d;
	//test(100,100,100,100);
}

