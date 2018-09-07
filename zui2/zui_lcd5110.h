// zui_lcd5110.h

#ifndef _ZUI_LCD5110_h
#define _ZUI_LCD5110_h
#include <avr/pgmspace.h>
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class zui
{
public:
	zui();
	~zui();
	//设置ps2摇杆的连接的针脚   按键，纵向选项电位器输入，横向选项电位器输入
	void SetPS2(int _key, int _vrx_input, int _vry_input);
	//设置ps2摇杆触发的极限   上限，下限 范围都是0-1024，默认650，350
	void SetTrigLimit(int _limit_high, int _limit_low);
	//如果ps2的x摇杆向上，返回true.      （取名是编程最困难的工作之一）
	boolean PS2VRXUp();
	//如果ps2的x摇杆向下，返回true.   
	boolean PS2VRXDown();
	//如果ps2的y摇杆向上，返回true.   
	boolean PS2VRYUp();
	//如果ps2的y摇杆向下，返回true.   
	boolean PS2VRYDown();
	//设置ps2摇杆控制光标的延时  _delay延长的时间单位ms   默认50
	void SetPS2Delay(int _delay);


	//LCD初始化函数
	void SetLcd(int RST0, int CE0, int DC0, int DIN0, int CLK0);
	//LCD清屏函数
	void Clear(void);
	//设置字符位置函数  xy坐标
	void SetPostion(unsigned char X, unsigned char Y);
	//数据写入到lcd dat数据，command 0命令1数据选择
	void WriteData(unsigned char dat, unsigned char command);


	//点          xy坐标
	void point(int x, int y);
	//画图        xy坐标(0-83,0-5)，width宽度,length长度,l点阵数据,是否反显，是否从内存读取数据
	void Draw(int x, int y, int width, int length, const char *l, bool reverse_display = 0, bool from_memory = 0);
	//变量输出    xy坐标(0-83,0-5), number数值，b要打印出多少位, 是否为浮点数，是否反显
	void Var(int x, int y, double number, int b, boolean  float_number = 0, bool reverse_display = 0);
	//文本输出    xy坐标(0-83,0-5), l字符串，是否反显
	void Text(int x, int y, char* l, boolean reverse_display = 0);


	//点击        button_now当前按钮是这个页面的第几个按钮，ps2摇杆的按键按下时返回true
	boolean Click(int button_now);
	//变量按钮    xy坐标，number变量，b要打印出多少位，float_number是否为浮点数
	void ButtonVar(int x, int y, double number, int b, boolean float_number = 0);
	//文本按钮    xy坐标，l字符串
	void ButtonText(int x, int y, char *l);
	//数据更改对话框   number被修改的数据，b要打印出几位数，时候为浮点，每50ms步进程度
	void MsgBox(double &number, int b, boolean float_num, float range);
	//数据更改对话框   number被修改的数据，b要打印出几位数，时候为浮点，每50ms步进程度
	void MsgBox(int &number, int b, boolean float_num, float range);
	//页面结束使用箭头作为光标指示
	void ArrowEnd();
	//页面结束使用反显作为光标指示
	void ReverseDisplayEnd();

	//获取当前的页面编号，255是数据修改框，0是主页
	int GetPage();
	//设置当前的页面编号，255是数据修改框，0是主页不能与这两个冲突
	void SetPage(int _page_now);
	//获取当前的光标位置 0开始
	int GetCursor();
	//获取当前页面的光标位置 
	int SetCursor(int _curosr_now);
	//获取当前页面的光标数量
	int GetCursorNum();
	//设置当前页面的光标数量
	int SetCursorNum(int _cursor_num);
	//页面同步  放在页面尾部，有那两个end（）的时候不需要手动设置
	void PageSync();
	//如果切换了页面    返回ture
	boolean PageSwitch();
	//到另一个页面 _page要到页面的页面编号
	void ToPage(int _page);
private:
	/*******************数据**************************/
	static int RST;
	static int CE;
	static int DC;
	static int DIN;
	static int CLK;
	static int cursor_pos;//记录cpu执行到那个坐标
	static int cursor_num;//当前页面的光标数量
	static int cursor_num_temp;
	static int cursor_now ;//存放当前光标在当前页面的位数
	static int cursor_now_temp;
	static int page_now;//当前页面编号
	static int page_now_temp;
	static int page_last ;//上个页面编号
	static boolean first_record;//是否为当前页面首次记录光标位置
	static boolean page_arrow ;//当前页面是否使用箭头作为光标指示
	static int key ;//ps2摇杆的按键
	static int vrx_input ;//ps2的vrx电位器
	static int vry_input; //ps2的vrx电位器
	static int limit_high;//读取电位器模拟值的上限，超过时触发
	static int limit_low;//下限
	static int PS2_delay;//ps2摇杆超出极限时的延时
	static boolean sync;//是否进行同步
	static const char decimal_point[]PROGMEM ; //小数点
	static const char frame[]PROGMEM ;
	static const char ASCIIDZ[67];
	static char char_conver_temp[112];//字符转换缓存
	static const char xz[8] PROGMEM;//用到的地方不多就不重写名字了
	static const char ASCII[92][6]PROGMEM ;
	static const char cursor_arrow[6]PROGMEM ;//箭头样式的光标
};


extern zui ui;//第一次发现还有这种骚操作
#endif

