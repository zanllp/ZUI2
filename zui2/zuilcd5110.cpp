#include <zuilcd5110.h>
#include<avr/pgmspace.h>

/*********************函数***********************/

//LCD初始化函数
void LcdInit(int RST0, int CE0, int DC0, int DIN0, int CLK0)
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
void Clear(void)
{
	unsigned int i;
	WriteData(0x0c, 0);
	WriteData(0x80, 0);
	for (i = 0; i < 504; i++)
	{
		WriteData(0, 1);
	}
}
//设置字符位置函数
void SetPostion(unsigned char X, unsigned char Y)
{
	WriteData(0x40 | Y, 0);// column
	WriteData(0x80 | X, 0);// row
}
//数据到LCD输入参数(dat数据，command 0命令1数据选择)
void WriteData(unsigned char dat, unsigned char command)//data ：写入的数据；command ：写数据 / 命令选择；
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

//点 xy坐标
void point(int x, int y)//点函数
{
	SetPostion(x, int(y / 8));
	WriteData(pgm_read_byte(&xz[int(y % 8)]), 1);
}




// 画图        xy坐标(0-83,0-5)，width宽度,length长度,l点阵数据,是否反显，是否从内存读取数据
void Draw(int x, int y, int width, int length, const char *l, bool reverse_display = 0, bool from_memory = 0)//xy图片左上角点的x（0-83）y（0-5）值，width为图标的宽度，length为图标的长度,l为点阵数据
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

// 变量输出    xy坐标(0-83,0-5), a数值，b要打印出多少位, 是否为浮点数，是否反显
void Var(int x, int y, double a, int b, boolean  float_number = 0, bool reverse_display = 0)
{

	if (float_number)//浮点数
	{
		if (a < 0)//负号
		{
			a = -a;
			Draw(x, y, 6, 8, ASCII[13], reverse_display);
			x += 6;
		}
		int xs;//小数点所在其有效数字的位数后
		for (int i = 0; i < b; i++)//计算小数点前面有几位数字
		{
			double pd = a / pow(10, i);
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
				double z = u32(a / (pow(10, xs - i))) * pow(10, xs - i);
				Draw(int(x + 6 * i), y, 6, 8, ASCII[char(u16((a - z) / (pow(10, xs - (i + 1))) + 16))], reverse_display);
			}
			if (i > xs)//小数点后面的数字
			{
				double z = u16((a - int(a)) * pow(10, (i - xs - 1))) / pow(10, (i - xs - 1));
				Draw(int(x + 6 * i - 4), y, 6, 8, ASCII[char(int((a - int(a) - z) * (pow(10, (i - xs))) + 16))], reverse_display);
			}
			if (i == xs)//小数点
			{
				Draw(x + xs * 6, y, 2, 8, decimal_point, reverse_display);
			}

		}
	}
	else//整数
	{
		if (a < 0)
		{
			a = -a;
			Draw(x, y, 6, 8, ASCII[13], reverse_display);
			x += 6;
		}
		for (int i = 0; i < b; i++)
		{
			double z = u32(a / (pow(10, b - i))) * pow(10, b - i);
			Draw(int(x + 6 * i), y, 6, 8, ASCII[char(int((a - z) / (pow(10, b - (i + 1))) + 16))], reverse_display);
		}
	}
}

// 文本输出    xy坐标(0-83,0-5), a数值，l字符串，是否反显
void Text(int x, int y, char* l, boolean reverse_display = 0)
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
	if (reverse_display)
	{
		Draw(x, y, char_num * 6, 8, char_conver_temp, 1, 1);
	}
	else
	{
		Draw(x, y, char_num * 6, 8, char_conver_temp, 0, 1);
	}

}




/*******************数据**************************/
const char decimal_point[]PROGMEM = { 0x00, 0x40, }; //小数点
const char arrow[2][16]PROGMEM = { { 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x08, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, },
{ 0x00, 0x00, 0x00, 0x20, 0x10, 0x08, 0x04, 0x02, 0x02, 0x04, 0x08, 0x10, 0x20, 0x00, 0x00, 0x00, }
};
const char frame[]PROGMEM = { 0xFF, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
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
const char ASCIIDZ[67] = " !:abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.";
char char_conver_temp[112];//字符转换缓存
const char xz[8] PROGMEM = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, };//用到的地方不多就不重写名字了
const char ASCII[92][6]PROGMEM = {
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
const char gbf[6]PROGMEM = { 0x10, 0x10, 0x10, 0x54, 0x38, 0x10 };//光标符
//const char null[6]PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };//空