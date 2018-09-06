/*
 Name:		zui2.ino
 Created:	2018/9/7 00:39:25
 Author:	zanllp
*/
//���Ƶ�zui2.ino���棬���õ�vs2017��visual micro�е�bugֻ����ô��

#include "zui_lcd5110.h"
#include<avr/pgmspace.h>

void UI()
{
	/***************����ҳ���Ž����л�****************/
	switch (ui.GetPage())//ҳ���л�������ҳ����
	{
	case 0:
		home();//��ҳ
		break;
	case 1:
		timer();
		break;
	case 2:
		brightness();
		break;
	default://Ĭ���൱��û����ҳ����
		ui.SetPage(0);//����ָ��home��ҳ����Ϊ0����Ȼ����home��
		home();//��ҳ
		break;
	}
}
boolean led_open = false;//led��״̬
void home()
{
	ui.Text(0, 0, "This a led");
	ui.Text(0, 1, "light controller");
	ui.Text(0, 2, "based on zui.");
	ui.Text(0, 3, "status:");
	if (led_open)
	{
		ui.ButtonText(50, 3, "open");
	}
	else
	{
		ui.ButtonText(50, 3, "false");
	}
	if (ui.Click(0))
	{
		led_open = !led_open;
	}
	ui.ButtonText(0, 4, "To timer");
	if (ui.Click(1))
	{
		ui.ToPage(1);
	}
	ui.ButtonText(0, 5, "To brightness");
	if (ui.Click(2))
	{
		ui.ToPage(2);
	}
	ui.ReverseDisplayEnd();//ҳ�����,��ǰҳ��ʹ�÷���,ʹ�������λ����������п���,�������ҳ��ĩβ
}
void timer()
{
	ui.Text(0, 0, "this is");
	ui.Text(0, 1, "timer page");
	ui.ReverseDisplayEnd();
}
int bright_num = 100;//����
void brightness()
{
	ui.Text(0, 0, "this is");
	ui.Text(0, 1, "brightness");
	ui.Text(0, 2, "page");
	ui.Var(40, 3, bright_num, 3);//ʵʱˢ��
	ui.ButtonText(0, 4, "click me");
	if (ui.Click(0))//��0����������
	{
		ui.MsgBox(bright_num, 3, 0, 10);//�������޸Ŀ����޸�����
	}
	ui.ReverseDisplayEnd();
}
void setup()
{
	ui.SetLcd(6, 7, 5, 4, 3);//rst,ce,dc,din,clk
	ui.SetPS2(12, A1, A0);//���ڿ��ƽ����ps2ҡ�˵İ���������ѡ���λ����ţ�����ѡ���λ�����
	Serial.begin(9600);
}

void loop()
{
	UI();//��������
}
