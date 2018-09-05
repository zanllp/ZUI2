// 
// 
// 

#include "zui_lcd5110.h"

#include <avr/pgmspace.h>
int zui::RST;
int zui::CE;
int zui::DC;
int zui::DIN;
int zui::CLK;
int zui::cursor_pos[20][2];//存放当前页面所有光标的坐标
int zui::cursor_num = 0;//当前页面的光标数量
int zui::cursor_num_temp;
int zui::cursor_now = 1;//存放当前光标在当前页面的位数
int zui::cursor_now_temp;
int zui::page_now = 0;//当前页面编号
int zui::page_now_temp;
int zui::page_last = 1;//上个页面编号
boolean zui::first_record;//是否为当前页面首次记录光标位置
boolean zui::page_arrow = true;//当前页面是否使用箭头作为光标指示
int zui::key = 12;//ps2摇杆的按键
int zui::vrx_input = A0;//ps2的vrx电位器输入
int zui::vry_input = A1; //ps2的vry电位器输入
int zui::limit_high = 650;//读取电位器模拟值的上限，超过时触发
int zui::limit_low = 350;//下限
const char zui::decimal_point[]PROGMEM = { 0x00, 0x40, }; //小数点
const char zui::frame[]PROGMEM = { 0xFF, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xFF, 0xFF, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0xFF, 0xFF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFF,

};
const char zui::ASCIIDZ[67] = " !:abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.";
char zui::char_conver_temp[112];//字符转换缓存
const char zui::xz[8] PROGMEM = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, };//用到的地方不多就不重写名字了
const char zui::ASCII[92][6]PROGMEM = {
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
	{ 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
	{ 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
	{ 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
	{ 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
	{ 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
	{ 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
	{ 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
	{ 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
	{ 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
	{ 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
	{ 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
	{ 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
	{ 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
	{ 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
	{ 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
	{ 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
	{ 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
	{ 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
	{ 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
	{ 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
	{ 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
	{ 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
	{ 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
	{ 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
	{ 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
	{ 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
	{ 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
	{ 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
	{ 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
	{ 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
	{ 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
	{ 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
	{ 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
	{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
	{ 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
	{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
	{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
	{ 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
	{ 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
	{ 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
	{ 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
	{ 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
	{ 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
	{ 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
	{ 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
	{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
	{ 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
	{ 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
	{ 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
	{ 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
	{ 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
	{ 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
	{ 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
	{ 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
	{ 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
	{ 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
	{ 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
	{ 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
	{ 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
	{ 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
	{ 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
	{ 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
	{ 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
	{ 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
	{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
	{ 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
	{ 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
	{ 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
	{ 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
	{ 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
	{ 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
	{ 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
	{ 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
	{ 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
	{ 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
	{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
	{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
	{ 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
	{ 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
	{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
	{ 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
	{ 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
	{ 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
	{ 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
	{ 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
	{ 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
	{ 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
	{ 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
	{ 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 },    // horiz lines
};
const char zui::cursor_arrow[6]PROGMEM = { 0x10, 0x10, 0x10, 0x54, 0x38, 0x10 };//箭头样式的光标
zui::zui()
{
}
zui::~zui()
{
}
///设置ps2摇杆的连接的针脚   按键，vrx电位器输入，vry电位器输入
void zui::SetPS2(int _key, int _vrx_input, int _vry_input)
{
	key = _key;
	vry_input = _vry_input;
	vrx_input = _vrx_input;
}
//设置ps2摇杆触发的极限   上限，下限 范围都是0-1024.
void zui::SetTrigLimit(int _limit_high, int _limit_low)
{
	limit_high = _limit_high;
	limit_low = _limit_low;
}
//如果ps2的vrx摇杆向上，返回true.      （取名是编程最困难的工作之一）
boolean zui::PS2VRXUp()
{
	return analogRead(vrx_input) > limit_high;
}
//如果ps2的vrx摇杆向下，返回true.   
boolean zui::PS2VRXDown()
{
	return analogRead(vrx_input) < limit_low;
}
//如果ps2的vry摇杆向上，返回true.   
boolean zui::PS2VRYUp()
{
	return analogRead(vry_input) > limit_high;
}
//如果ps2的vry摇杆向下，返回true.   
boolean zui::PS2VRYDown()
{
	return analogRead(vry_input) < limit_low;
}


//LCD初始化函数
void zui::SetLcd(int RST0, int CE0, int DC0, int DIN0, int CLK0)
{
	RST = RST0;
	CE = CE0;
	DC = DC0;
	DIN = DIN0;
	CLK = CLK0;
	//先设置为输出
	pinMode(CLK, OUTPUT);
	pinMode(DIN, OUTPUT);
	pinMode(DC, OUTPUT);
	pinMode(CE, OUTPUT);
	pinMode(RST, OUTPUT);

	// 产生一个让LCD复位的低电平脉冲
	digitalWrite(RST, LOW);

	delayMicroseconds(1);
	digitalWrite(RST, HIGH);

	// 关闭LCD
	digitalWrite(CE, LOW);
	delayMicroseconds(1);

	// 使能LCD
	digitalWrite(CE, HIGH); //LCD_CE = 1;
	delayMicroseconds(1);
	WriteData(0x21, 0); // 使用扩展命令设置LCD模式
	WriteData(0xc8, 0); // 设置偏置电压
	WriteData(0x06, 0); // 温度校正
	WriteData(0x13, 0); // 1:48
	WriteData(0x20, 0); // 使用基本命令
	Clear();             // 清屏
	WriteData(0x0c, 0); // 设定显示模式，正常显示

							 // 关闭LCD
	digitalWrite(CE, LOW); //LCD_CE = 0;
}
//LCD清屏函数
void zui::Clear(void)
{
	unsigned int i;
	WriteData(0x0c, 0);
	WriteData(0x80, 0);
	for (i = 0; i < 504; i++)
	{
		WriteData(0, 1);
	}
}
//设置字符位置函数  xy坐标
void zui::SetPostion(unsigned char X, unsigned char Y)
{
	WriteData(0x40 | Y, 0);// column
	WriteData(0x80 | X, 0);// row
}
//数据写入到lcd dat数据，command 0命令1数据选择
void zui::WriteData(unsigned char dat, unsigned char command)//data ：写入的数据；command ：写数据 / 命令选择；
{
	unsigned char i;
	digitalWrite(CE, LOW); // 使能LCD_CE = 0
	if (command == 0)
	{
		digitalWrite(DC, LOW);// 传送命令 LCD_DC = 0;
	}
	else
	{
		digitalWrite(DC, HIGH);// 传送数据LCD_DC = 1;
	}

	for (i = 0; i < 8; i++)
	{
		if (dat & 0x80)
		{
			digitalWrite(DIN, HIGH);
		}
		else
		{
			digitalWrite(DIN, LOW);;
		}
		digitalWrite(CLK, LOW);
		dat = dat << 1;
		digitalWrite(CLK, HIGH);
	}
	digitalWrite(CE, HIGH);
}


//点          xy坐标
void zui::point(int x, int y)//点函数
{
	SetPostion(x, int(y / 8));
	WriteData(pgm_read_byte(&xz[int(y % 8)]), 1);
}
//画图        xy坐标(0-83,0-5)，width宽度,length长度,l点阵数据,是否反显，是否从内存读取数据
void zui::Draw(int x, int y, int width, int length, const char *l, bool reverse_display = 0, bool from_memory = 0)//xy图片左上角点的x（0-83）y（0-5）值，width为图标的宽度，length为图标的长度,l为点阵数据
{ //从flash中读取数据
	int i;
	int ii;
	int ys = length / 8;
	for (ii = 0; ii < ys; ii += 1)
	{
		for (i = ii * width + 0; i < width + ii * width; i++)
		{
			SetPostion(x + int(i % width), y + ii);
			if (reverse_display == 0 && from_memory == 0)//不反显，从闪存中读取数据
			{
				WriteData(pgm_read_byte(&l[i]), 1);
			}
			if (reverse_display == 1 && from_memory == 0)//反显，从闪存中读取数据
			{
				WriteData(0xff - pgm_read_byte(&l[i]), 1);
			}
			if (reverse_display == 0 && from_memory == 1)//不反显，从内存中读取数据
			{
				WriteData((l[i]), 1);
			}
			if (reverse_display == 1 && from_memory == 1)//反显，从内存中读取数据
			{
				WriteData((0xff - l[i]), 1);
			}
		}
	}
}
//变量输出    xy坐标(0-83,0-5), number数值，b要打印出多少位, 是否为浮点数，是否反显
void zui::Var(int x, int y, double number, int b, boolean  float_number = 0, bool reverse_display = 0)
{

	if (float_number)//浮点数
	{
		if (number < 0)//负号
		{
			number = -number;
			Draw(x, y, 6, 8, ASCII[13], reverse_display);
			x += 6;
		}
		int xs;//小数点所在其有效数字的位数后
		for (int i = 0; i < b; i++)//计算小数点前面有几位数字
		{
			double pd = number / pow(10, i);
			if (pd < 1)
			{
				xs = i;
				break;
			}
		}
		for (int i = 0; i < b + 1; i++)
		{
			if (i < xs)//小数点前面的数字
			{
				double z = u32(number / (pow(10, xs - i))) * pow(10, xs - i);
				Draw(int(x + 6 * i), y, 6, 8, ASCII[char(u16((number - z) / (pow(10, xs - (i + 1))) + 16))], reverse_display);
			}
			if (i > xs)//小数点后面的数字
			{
				double z = u16((number - int(number)) * pow(10, (i - xs - 1))) / pow(10, (i - xs - 1));
				Draw(int(x + 6 * i - 4), y, 6, 8, ASCII[char(int((number - int(number) - z) * (pow(10, (i - xs))) + 16))], reverse_display);
			}
			if (i == xs)//小数点
			{
				Draw(x + xs * 6, y, 2, 8, decimal_point, reverse_display);
			}

		}
	}
	else//整数
	{
		if (number < 0)
		{
			number = -number;
			Draw(x, y, 6, 8, ASCII[13], reverse_display);
			x += 6;
		}
		for (int i = 0; i < b; i++)
		{
			double z = u32(number / (pow(10, b - i))) * pow(10, b - i);
			Draw(int(x + 6 * i), y, 6, 8, ASCII[char(int((number - z) / (pow(10, b - (i + 1))) + 16))], reverse_display);
		}
	}
}
//文本输出    xy坐标(0-83,0-5), l字符串，是否反显
void zui::Text(int x, int y, char* l, boolean reverse_display = 0)
{
	String txtMsg = l;
	int char_num = txtMsg.length();
	for (int i = 0; i < char_num; i++)
	{
		for (int ia = 0; ia < 67; ia++)
		{
			char lx = l[i];
			char ly = ASCIIDZ[ia];
			if (lx == ly)
			{

				if (ia > 2 && ia < 29) //小写字母
				{
					for (int ib = 0; ib < 6; ib++)
					{
						char_conver_temp[i * 6 + ib] = pgm_read_byte(&ASCII[ia + 62][ib]);
					}

				}
				if (ia > 28 && ia < 55) //大写字母
				{
					for (int ib = 0; ib < 6; ib++)
					{
						char_conver_temp[i * 6 + ib] = pgm_read_byte(&ASCII[ia + 4][ib]);
					}
				}
				if (ia > 54 && ia < 65)//数字
				{
					for (int ib = 0; ib < 6; ib++)
					{
						char_conver_temp[i * 6 + ib] = pgm_read_byte(&ASCII[ia - 39][ib]);
					}

				}
				/**************符号********************/

				if (ia == 0)//空格
				{
					for (int ib = 0; ib < 6; ib++)
					{
						char_conver_temp[i * 6 + ib] = pgm_read_byte(&ASCII[0][ib]);
					}
				}
				if (ia == 1)//感叹号
				{
					for (int ib = 0; ib < 6; ib++)
					{
						char_conver_temp[i * 6 + ib] = pgm_read_byte(&ASCII[1][ib]);
					}
				}
				if (ia == 2)//冒号
				{
					for (int ib = 0; ib < 6; ib++)
					{
						char_conver_temp[i * 6 + ib] = pgm_read_byte(&ASCII[26][ib]);
					}
				}
				if (ia == 65)//小数点
				{
					for (int ib = 0; ib < 6; ib++)
					{
						char_conver_temp[i * 6 + ib] = pgm_read_byte(&ASCII[14][ib]);
					}
				}


				break;
			}

		}

	}
	Draw(x, y, char_num * 6, 8, char_conver_temp, reverse_display, 1);
}


//点击        button_now当前按钮是这个页面的第几个按钮，ps2摇杆的按键按下时返回true
boolean zui::Click(int button_now)
{
	if (!digitalRead(key) && cursor_now == button_now)//ps2摇杆在按下时是断开
	{
		Clear();
		return true;
	}
	else
	{
		return false;
	}
}
//变量按钮    xy坐标，number变量，b要打印出多少位，float_number是否为浮点数
void zui::ButtonVar(int x, int y, double number, int b, boolean float_number = 0) //光标标注,注册选项,保存各个光标的坐标
{
	if (PageSwitch())//切换了页面
	{
		if (first_record == 0)//首次记录需要将光标数量置0
		{
			cursor_num = 0;
			first_record = 1;
		}
		cursor_num++;
		cursor_pos[cursor_num - 1][0] = x;
		cursor_pos[cursor_num - 1][1] = y;
	}
	else
	{
		first_record = 0;
	}
	if (page_arrow)//如果这个页面使用的是箭头作为光标标识
	{
		x += 6;//光标占用6个像素
		Var(x, y, number, b, float_number);
	}
	else//反显
	{
		if (x == cursor_pos[cursor_now][0] && y == cursor_pos[cursor_now][1])//符合当前光标坐标的反显输出，不符合的正常输出
		{
			Var(x, y, number, b, float_number, 1);
		}
		else
		{
			Var(x, y, number, b, float_number);
		}
	}
}
//文本按钮    xy坐标，l字符串
void zui::ButtonText(int x, int y, char *l)//光标标注,注册选项,保存各个光标的坐标
{
	if (PageSwitch())
	{
		if (first_record == 0)
		{
			cursor_num = 0;
			first_record = 1;
		}
		cursor_num++;
		cursor_pos[cursor_num - 1][0] = x;
		cursor_pos[cursor_num - 1][1] = y;
	}
	else
	{
		first_record = 0;
	}
	if (page_arrow)
	{
		x += 6;
		Text(x, y, l, 0);
	}
	else
	{
		if (x == cursor_pos[cursor_now][0] && y == cursor_pos[cursor_now][1])
		{
			Text(x, y, l, 1);
		}
		else
		{
			Text(x, y, l, 0);
		}
	}
}
void copy(int* _Array, int* _Array1)
{
	for (int i = 0; i < 20; i++)
	{
		_Array1[i] = _Array[i];
	}
}

//数据更改对话框   number被修改的数据，b要打印出几位数，时候为浮点，每50ms步进程度
void zui::MsgBox(double &number, int b, boolean float_num, float range)//number要修改的数据，b数据的有效数位，c是否浮点数，range每隔50ms变动的幅度
{//arduino好像不能用模板，提示 error: 'T' was not declared in this scope
	int cursor_now_0 = cursor_now;
	int cursor_num_0 = cursor_num;
	int page_now_0 = page_now;
	int cursor_pos_0[20][2] ;
	copy(cursor_pos[0], cursor_pos_0[0]);//和cpp不一样
	copy(cursor_pos[1], cursor_pos_0[1]);
	//以上代码备份所有信息
	delay(10);
	Clear();
	double number_temp = number;
	Draw(12, 1, 60, 32, frame);
	SetPage(255);//数据框修改，没有像ui()这个这个函数来分配页面编号，必须手动指出，不然按钮的坐标和数量不会更新
	while (1)
	{
		delay(200);
		Var(30, 2, number_temp, b, float_num);
		while (PS2VRXUp())
		{
			number_temp += range;
			Draw(12, 1, 60, 32, frame);
			delay(50);
		}
		while(PS2VRXDown())
		{
			number_temp -= range;
			Draw(12, 1, 60, 32, frame);
			delay(50);
		}
		ButtonText(25, 4, "YES");
		ButtonText(50, 4, "NO");
		if (Click(0))//第0个按钮按下
		{//还原所有坐标位置信息
			copy(cursor_pos_0[0], cursor_pos[0]);
			copy(cursor_pos_0[1], cursor_pos[1]);
			cursor_now = cursor_now_0;
			cursor_num = cursor_num_0;
			page_now = page_now_0;
			PageAsyc();
			Clear();
			number = number_temp;
			break;
		}
		if (Click(1))
		{//还原所有坐标位置信息
			copy(cursor_pos_0[0], cursor_pos[0]);
			copy(cursor_pos_0[1], cursor_pos[1]);
			cursor_now = cursor_now_0;
			cursor_num = cursor_num_0;
			page_now = page_now_0;
			PageAsyc();
			Clear();
			break;
		}
		ReverseDisplayEnd();
	}
}
//数据更改对话框   number被修改的数据，b要打印出几位数，时候为浮点，每50ms步进程度
void zui::MsgBox(int &number, int b, boolean float_num, float range)//number要修改的数据，b数据的有效数位，c是否浮点数，range每隔50ms变动的幅度
{

	int cursor_now_0 = cursor_now;
	int cursor_num_0 = cursor_num;
	int page_now_0 = page_now;
	int cursor_pos_0[20][2];
	copy(cursor_pos[0], cursor_pos_0[0]);//和cpp不一样
	copy(cursor_pos[1], cursor_pos_0[1]);
	//以上代码备份所有信息
	delay(10);
	Clear();
	int number_temp = number;
	Draw(12, 1, 60, 32, frame);
	SetPage(255);//数据框修改，没有像ui()这个这个函数来分配页面编号，必须手动指出，不然按钮的坐标和数量不会更新
	while (1)
	{
		delay(200);
		Var(30, 2, number_temp, b, float_num);
		while (PS2VRXUp())
		{
			number_temp += range;
			Draw(12, 1, 60, 32, frame);
			delay(50);
		}
		while (PS2VRXDown())
		{
			number_temp -= range;
			Draw(12, 1, 60, 32, frame);
			delay(50);
		}
		ButtonText(25, 4, "YES");
		ButtonText(50, 4, "NO");
		if (Click(0))//第0个按钮按下
		{
			//还原所有坐标位置信息
			copy(cursor_pos_0[0], cursor_pos[0]);
			copy(cursor_pos_0[1], cursor_pos[1]);
			cursor_now = cursor_now_0;
			cursor_num = cursor_num_0;
			page_now = page_now_0;
			PageAsyc();
			number = number_temp;
			Clear();
			break;
		}
		if (Click(1))
		{//还原所有坐标位置信息
			copy(cursor_pos_0[0], cursor_pos[0]);
			copy(cursor_pos_0[1], cursor_pos[1]);
			cursor_now = cursor_now_0;
			cursor_num = cursor_num_0;
			page_now = page_now_0;
			PageAsyc();
			Clear();
			break;
		}
		ReverseDisplayEnd();
	}

}
//页面结束使用箭头作为光标指示
void zui::ArrowEnd()
{
	if (!page_arrow)
	{
		Clear();
	}
	page_arrow = true;//当前页面是箭头
	if (PS2VRYUp())
	{
		cursor_now++;
		if (cursor_now > cursor_num - 1)
		{
			cursor_now = 0;
		}
		Clear();
	}
	if (PS2VRYDown())
	{
		cursor_now--;
		if (cursor_now < 0)
		{
			cursor_now = cursor_num - 1;
		}
		Clear();
	}
	if (cursor_now > -1 && cursor_now < cursor_num)//限定范围防止访问到错误的内存
	{
		Draw(cursor_pos[cursor_now][0], cursor_pos[cursor_now][1], 6, 8, cursor_arrow);
	}
	if (page_now != 0&& page_now !=255) //主页不需要home键
	{
		ButtonText(54, 5, "home");//回家按钮
	}
	if (Click(cursor_num - 1))
	{
		page_now = 0;
	}

	PageAsyc();
}
//页面结束使用反显作为光标指示
void zui::ReverseDisplayEnd()//反显移动
{
	if (page_arrow)
	{
		Clear();//使用箭头会让这个控件推后6像素，因为控件是先绘制再指示光标样式，所以使用反显的时候可能会残留箭头样式遗留下来的几个像素，刷新就行
	}
	page_arrow = false;//当前页面不是箭头
	if (PS2VRYUp())
	{
		cursor_now++;
		if (cursor_now > cursor_num - 1)
		{
			cursor_now = 0;
		}
	}
	if (PS2VRYDown())
	{
		cursor_now--;
		if (cursor_now < 0)
		{
			cursor_now = cursor_num - 1;
		}
	}
	if (page_now != 0 &&page_now != 255) //主页，提示框不需要home键
	{
		ButtonText(60, 5, "home");
	}
	if (Click(cursor_num - 1))
	{
		page_now = 0;
	}
	PageAsyc();
}



//获取当前的页面编号，255是数据修改框，0是主页
int zui::GetPage()
{
	return page_now;
}
//设置当前的页面编号，255是数据修改框，0是主页不能与这两个冲突
void zui::SetPage(int _page_now)
{
	//first_record = true;
	page_now = _page_now;
}
//获取当前的光标位置 0开始
int zui::GetCursor()
{
	return cursor_now;
}
//设置当前页面的光标位置 
int zui::SetCursor(int _curosr_now)
{
	cursor_now=_curosr_now;
}
//获取当前页面的光标数量
int zui::GetCursorNum()
{
	return cursor_num;
}
//设置当前页面的光标数量,有那两个end（）的时候会自动计算，不需要手动设置
int zui::SetCursorNum(int _cursor_num)
{
	cursor_num = _cursor_num;
}
//页面同步  放在页面尾部，有那两个end（）的时候不需要手动设置
void zui::PageAsyc()
{
	page_last = page_now;
}
//如果切换了页面    返回ture
boolean zui::PageSwitch()
{
	return page_last != page_now;
}
