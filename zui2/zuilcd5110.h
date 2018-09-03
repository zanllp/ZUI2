#pragma once
#ifndef ZUILCD5110_H_
#define ZUILCD5110_H_
/************************函数*******************************************/

//LCD初始化函数
void LcdInit(int RST0, int CE0, int DC0, int DIN0, int CLK0);
//LCD清屏函数
void Clear(void);
//设置字符位置函数
void SetPostion(unsigned char X, unsigned char Y);
/*******************数据到LCD输入参数*****dat数据，command 0命令1数据选择*********/
void WriteData(unsigned char dat, unsigned char command);


//点 xy坐标(0-83,0-47)
void point(int x, int y);
// 画图        xy坐标(0-83,0-5)，width宽度,length长度,l点阵数据,是否反显，是否从内存读取数据
void Draw(int x, int y, int width, int length, const char *l, bool reverse_display = 0, bool from_memory = 0);
// 变量输出    xy坐标(0-83,0-5), a数值，b要打印出多少位, 是否为浮点数，是否反显
void Var(int x, int y, double a, int b, boolean  float_number = 0, bool reverse_display = 0);
// 文本输出    xy坐标(0-83,0-5), a数值，l字符串，是否反显
void Text(int x, int y, char* l, boolean reverse_display = 0);



/**************************数据**************************************/
int RST ;
int CE ;
int DC ;
int DIN ;
int CLK ;
int cursor_pos[20][2];//存放当前页面所有光标的坐标
int cursor_num;//当前页面的光标数量
int cursor_num_temp;
int cursor_now = 1;//存放当前光标在当前页面的位数
int cursor_now_temp;
int page_now;//当前页面编号
int page_now_temp;
int page_last = 1;//上个页面编号
double number_temp;//消息框数据修改的缓存
unsigned time_temp;
boolean arrow_state;
boolean ixy;
int key = 12;//按键
int vertical_input = A0;//纵向选项电位器输入
int horiziontal_input = A1; //横向选项电位器输入
int limit_high = 600;//读取电位器模拟值的上限，超过时触发
int limit_low = 400;//下限

#endif // !ZUILCD5110_H_

