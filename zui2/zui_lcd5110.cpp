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
int zui::cursor_pos[20][2];//��ŵ�ǰҳ�����й�������
int zui::cursor_num = 0;//��ǰҳ��Ĺ������
int zui::cursor_num_temp;
int zui::cursor_now = 1;//��ŵ�ǰ����ڵ�ǰҳ���λ��
int zui::cursor_now_temp;
int zui::page_now = 0;//��ǰҳ����
int zui::page_now_temp;
int zui::page_last = 1;//�ϸ�ҳ����
boolean zui::first_record;//�Ƿ�Ϊ��ǰҳ���״μ�¼���λ��
boolean zui::page_arrow = true;//��ǰҳ���Ƿ�ʹ�ü�ͷ��Ϊ���ָʾ
int zui::key = 12;//ps2ҡ�˵İ���
int zui::vrx_input = A0;//ps2��vrx��λ������
int zui::vry_input = A1; //ps2��vry��λ������
int zui::limit_high = 650;//��ȡ��λ��ģ��ֵ�����ޣ�����ʱ����
int zui::limit_low = 350;//����
const char zui::decimal_point[]PROGMEM = { 0x00, 0x40, }; //С����
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
char zui::char_conver_temp[112];//�ַ�ת������
const char zui::xz[8] PROGMEM = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, };//�õ��ĵط�����Ͳ���д������
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
const char zui::cursor_arrow[6]PROGMEM = { 0x10, 0x10, 0x10, 0x54, 0x38, 0x10 };//��ͷ��ʽ�Ĺ��
zui::zui()
{
}
zui::~zui()
{
}
///����ps2ҡ�˵����ӵ����   ������vrx��λ�����룬vry��λ������
void zui::SetPS2(int _key, int _vrx_input, int _vry_input)
{
	key = _key;
	vry_input = _vry_input;
	vrx_input = _vrx_input;
}
//����ps2ҡ�˴����ļ���   ���ޣ����� ��Χ����0-1024.
void zui::SetTrigLimit(int _limit_high, int _limit_low)
{
	limit_high = _limit_high;
	limit_low = _limit_low;
}
//���ps2��vrxҡ�����ϣ�����true.      ��ȡ���Ǳ�������ѵĹ���֮һ��
boolean zui::PS2VRXUp()
{
	return analogRead(vrx_input) > limit_high;
}
//���ps2��vrxҡ�����£�����true.   
boolean zui::PS2VRXDown()
{
	return analogRead(vrx_input) < limit_low;
}
//���ps2��vryҡ�����ϣ�����true.   
boolean zui::PS2VRYUp()
{
	return analogRead(vry_input) > limit_high;
}
//���ps2��vryҡ�����£�����true.   
boolean zui::PS2VRYDown()
{
	return analogRead(vry_input) < limit_low;
}


//LCD��ʼ������
void zui::SetLcd(int RST0, int CE0, int DC0, int DIN0, int CLK0)
{
	RST = RST0;
	CE = CE0;
	DC = DC0;
	DIN = DIN0;
	CLK = CLK0;
	//������Ϊ���
	pinMode(CLK, OUTPUT);
	pinMode(DIN, OUTPUT);
	pinMode(DC, OUTPUT);
	pinMode(CE, OUTPUT);
	pinMode(RST, OUTPUT);

	// ����һ����LCD��λ�ĵ͵�ƽ����
	digitalWrite(RST, LOW);

	delayMicroseconds(1);
	digitalWrite(RST, HIGH);

	// �ر�LCD
	digitalWrite(CE, LOW);
	delayMicroseconds(1);

	// ʹ��LCD
	digitalWrite(CE, HIGH); //LCD_CE = 1;
	delayMicroseconds(1);
	WriteData(0x21, 0); // ʹ����չ��������LCDģʽ
	WriteData(0xc8, 0); // ����ƫ�õ�ѹ
	WriteData(0x06, 0); // �¶�У��
	WriteData(0x13, 0); // 1:48
	WriteData(0x20, 0); // ʹ�û�������
	Clear();             // ����
	WriteData(0x0c, 0); // �趨��ʾģʽ��������ʾ

							 // �ر�LCD
	digitalWrite(CE, LOW); //LCD_CE = 0;
}
//LCD��������
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
//�����ַ�λ�ú���  xy����
void zui::SetPostion(unsigned char X, unsigned char Y)
{
	WriteData(0x40 | Y, 0);// column
	WriteData(0x80 | X, 0);// row
}
//����д�뵽lcd dat���ݣ�command 0����1����ѡ��
void zui::WriteData(unsigned char dat, unsigned char command)//data ��д������ݣ�command ��д���� / ����ѡ��
{
	unsigned char i;
	digitalWrite(CE, LOW); // ʹ��LCD_CE = 0
	if (command == 0)
	{
		digitalWrite(DC, LOW);// �������� LCD_DC = 0;
	}
	else
	{
		digitalWrite(DC, HIGH);// ��������LCD_DC = 1;
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


//��          xy����
void zui::point(int x, int y)//�㺯��
{
	SetPostion(x, int(y / 8));
	WriteData(pgm_read_byte(&xz[int(y % 8)]), 1);
}
//��ͼ        xy����(0-83,0-5)��width���,length����,l��������,�Ƿ��ԣ��Ƿ���ڴ��ȡ����
void zui::Draw(int x, int y, int width, int length, const char *l, bool reverse_display = 0, bool from_memory = 0)//xyͼƬ���Ͻǵ��x��0-83��y��0-5��ֵ��widthΪͼ��Ŀ�ȣ�lengthΪͼ��ĳ���,lΪ��������
{ //��flash�ж�ȡ����
	int i;
	int ii;
	int ys = length / 8;
	for (ii = 0; ii < ys; ii += 1)
	{
		for (i = ii * width + 0; i < width + ii * width; i++)
		{
			SetPostion(x + int(i % width), y + ii);
			if (reverse_display == 0 && from_memory == 0)//�����ԣ��������ж�ȡ����
			{
				WriteData(pgm_read_byte(&l[i]), 1);
			}
			if (reverse_display == 1 && from_memory == 0)//���ԣ��������ж�ȡ����
			{
				WriteData(0xff - pgm_read_byte(&l[i]), 1);
			}
			if (reverse_display == 0 && from_memory == 1)//�����ԣ����ڴ��ж�ȡ����
			{
				WriteData((l[i]), 1);
			}
			if (reverse_display == 1 && from_memory == 1)//���ԣ����ڴ��ж�ȡ����
			{
				WriteData((0xff - l[i]), 1);
			}
		}
	}
}
//�������    xy����(0-83,0-5), number��ֵ��bҪ��ӡ������λ, �Ƿ�Ϊ���������Ƿ���
void zui::Var(int x, int y, double number, int b, boolean  float_number = 0, bool reverse_display = 0)
{

	if (float_number)//������
	{
		if (number < 0)//����
		{
			number = -number;
			Draw(x, y, 6, 8, ASCII[13], reverse_display);
			x += 6;
		}
		int xs;//С������������Ч���ֵ�λ����
		for (int i = 0; i < b; i++)//����С����ǰ���м�λ����
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
			if (i < xs)//С����ǰ�������
			{
				double z = u32(number / (pow(10, xs - i))) * pow(10, xs - i);
				Draw(int(x + 6 * i), y, 6, 8, ASCII[char(u16((number - z) / (pow(10, xs - (i + 1))) + 16))], reverse_display);
			}
			if (i > xs)//С������������
			{
				double z = u16((number - int(number)) * pow(10, (i - xs - 1))) / pow(10, (i - xs - 1));
				Draw(int(x + 6 * i - 4), y, 6, 8, ASCII[char(int((number - int(number) - z) * (pow(10, (i - xs))) + 16))], reverse_display);
			}
			if (i == xs)//С����
			{
				Draw(x + xs * 6, y, 2, 8, decimal_point, reverse_display);
			}

		}
	}
	else//����
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
//�ı����    xy����(0-83,0-5), l�ַ������Ƿ���
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

				if (ia > 2 && ia < 29) //Сд��ĸ
				{
					for (int ib = 0; ib < 6; ib++)
					{
						char_conver_temp[i * 6 + ib] = pgm_read_byte(&ASCII[ia + 62][ib]);
					}

				}
				if (ia > 28 && ia < 55) //��д��ĸ
				{
					for (int ib = 0; ib < 6; ib++)
					{
						char_conver_temp[i * 6 + ib] = pgm_read_byte(&ASCII[ia + 4][ib]);
					}
				}
				if (ia > 54 && ia < 65)//����
				{
					for (int ib = 0; ib < 6; ib++)
					{
						char_conver_temp[i * 6 + ib] = pgm_read_byte(&ASCII[ia - 39][ib]);
					}

				}
				/**************����********************/

				if (ia == 0)//�ո�
				{
					for (int ib = 0; ib < 6; ib++)
					{
						char_conver_temp[i * 6 + ib] = pgm_read_byte(&ASCII[0][ib]);
					}
				}
				if (ia == 1)//��̾��
				{
					for (int ib = 0; ib < 6; ib++)
					{
						char_conver_temp[i * 6 + ib] = pgm_read_byte(&ASCII[1][ib]);
					}
				}
				if (ia == 2)//ð��
				{
					for (int ib = 0; ib < 6; ib++)
					{
						char_conver_temp[i * 6 + ib] = pgm_read_byte(&ASCII[26][ib]);
					}
				}
				if (ia == 65)//С����
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


//���        button_now��ǰ��ť�����ҳ��ĵڼ�����ť��ps2ҡ�˵İ�������ʱ����true
boolean zui::Click(int button_now)
{
	if (!digitalRead(key) && cursor_now == button_now)//ps2ҡ���ڰ���ʱ�ǶϿ�
	{
		Clear();
		return true;
	}
	else
	{
		return false;
	}
}
//������ť    xy���꣬number������bҪ��ӡ������λ��float_number�Ƿ�Ϊ������
void zui::ButtonVar(int x, int y, double number, int b, boolean float_number = 0) //����ע,ע��ѡ��,���������������
{
	if (PageSwitch())//�л���ҳ��
	{
		if (first_record == 0)//�״μ�¼��Ҫ�����������0
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
	if (page_arrow)//������ҳ��ʹ�õ��Ǽ�ͷ��Ϊ����ʶ
	{
		x += 6;//���ռ��6������
		Var(x, y, number, b, float_number);
	}
	else//����
	{
		if (x == cursor_pos[cursor_now][0] && y == cursor_pos[cursor_now][1])//���ϵ�ǰ�������ķ�������������ϵ��������
		{
			Var(x, y, number, b, float_number, 1);
		}
		else
		{
			Var(x, y, number, b, float_number);
		}
	}
}
//�ı���ť    xy���꣬l�ַ���
void zui::ButtonText(int x, int y, char *l)//����ע,ע��ѡ��,���������������
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

//���ݸ��ĶԻ���   number���޸ĵ����ݣ�bҪ��ӡ����λ����ʱ��Ϊ���㣬ÿ50ms�����̶�
void zui::MsgBox(double &number, int b, boolean float_num, float range)//numberҪ�޸ĵ����ݣ�b���ݵ���Ч��λ��c�Ƿ񸡵�����rangeÿ��50ms�䶯�ķ���
{//arduino��������ģ�壬��ʾ error: 'T' was not declared in this scope
	int cursor_now_0 = cursor_now;
	int cursor_num_0 = cursor_num;
	int page_now_0 = page_now;
	int cursor_pos_0[20][2] ;
	copy(cursor_pos[0], cursor_pos_0[0]);//��cpp��һ��
	copy(cursor_pos[1], cursor_pos_0[1]);
	//���ϴ��뱸��������Ϣ
	delay(10);
	Clear();
	double number_temp = number;
	Draw(12, 1, 60, 32, frame);
	SetPage(255);//���ݿ��޸ģ�û����ui()����������������ҳ���ţ������ֶ�ָ������Ȼ��ť������������������
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
		if (Click(0))//��0����ť����
		{//��ԭ��������λ����Ϣ
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
		{//��ԭ��������λ����Ϣ
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
//���ݸ��ĶԻ���   number���޸ĵ����ݣ�bҪ��ӡ����λ����ʱ��Ϊ���㣬ÿ50ms�����̶�
void zui::MsgBox(int &number, int b, boolean float_num, float range)//numberҪ�޸ĵ����ݣ�b���ݵ���Ч��λ��c�Ƿ񸡵�����rangeÿ��50ms�䶯�ķ���
{

	int cursor_now_0 = cursor_now;
	int cursor_num_0 = cursor_num;
	int page_now_0 = page_now;
	int cursor_pos_0[20][2];
	copy(cursor_pos[0], cursor_pos_0[0]);//��cpp��һ��
	copy(cursor_pos[1], cursor_pos_0[1]);
	//���ϴ��뱸��������Ϣ
	delay(10);
	Clear();
	int number_temp = number;
	Draw(12, 1, 60, 32, frame);
	SetPage(255);//���ݿ��޸ģ�û����ui()����������������ҳ���ţ������ֶ�ָ������Ȼ��ť������������������
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
		if (Click(0))//��0����ť����
		{
			//��ԭ��������λ����Ϣ
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
		{//��ԭ��������λ����Ϣ
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
//ҳ�����ʹ�ü�ͷ��Ϊ���ָʾ
void zui::ArrowEnd()
{
	if (!page_arrow)
	{
		Clear();
	}
	page_arrow = true;//��ǰҳ���Ǽ�ͷ
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
	if (cursor_now > -1 && cursor_now < cursor_num)//�޶���Χ��ֹ���ʵ�������ڴ�
	{
		Draw(cursor_pos[cursor_now][0], cursor_pos[cursor_now][1], 6, 8, cursor_arrow);
	}
	if (page_now != 0&& page_now !=255) //��ҳ����Ҫhome��
	{
		ButtonText(54, 5, "home");//�ؼҰ�ť
	}
	if (Click(cursor_num - 1))
	{
		page_now = 0;
	}

	PageAsyc();
}
//ҳ�����ʹ�÷�����Ϊ���ָʾ
void zui::ReverseDisplayEnd()//�����ƶ�
{
	if (page_arrow)
	{
		Clear();//ʹ�ü�ͷ��������ؼ��ƺ�6���أ���Ϊ�ؼ����Ȼ�����ָʾ�����ʽ������ʹ�÷��Ե�ʱ����ܻ������ͷ��ʽ���������ļ������أ�ˢ�¾���
	}
	page_arrow = false;//��ǰҳ�治�Ǽ�ͷ
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
	if (page_now != 0 &&page_now != 255) //��ҳ����ʾ����Ҫhome��
	{
		ButtonText(60, 5, "home");
	}
	if (Click(cursor_num - 1))
	{
		page_now = 0;
	}
	PageAsyc();
}



//��ȡ��ǰ��ҳ���ţ�255�������޸Ŀ�0����ҳ
int zui::GetPage()
{
	return page_now;
}
//���õ�ǰ��ҳ���ţ�255�������޸Ŀ�0����ҳ��������������ͻ
void zui::SetPage(int _page_now)
{
	//first_record = true;
	page_now = _page_now;
}
//��ȡ��ǰ�Ĺ��λ�� 0��ʼ
int zui::GetCursor()
{
	return cursor_now;
}
//���õ�ǰҳ��Ĺ��λ�� 
int zui::SetCursor(int _curosr_now)
{
	cursor_now=_curosr_now;
}
//��ȡ��ǰҳ��Ĺ������
int zui::GetCursorNum()
{
	return cursor_num;
}
//���õ�ǰҳ��Ĺ������,��������end������ʱ����Զ����㣬����Ҫ�ֶ�����
int zui::SetCursorNum(int _cursor_num)
{
	cursor_num = _cursor_num;
}
//ҳ��ͬ��  ����ҳ��β������������end������ʱ����Ҫ�ֶ�����
void zui::PageAsyc()
{
	page_last = page_now;
}
//����л���ҳ��    ����ture
boolean zui::PageSwitch()
{
	return page_last != page_now;
}
