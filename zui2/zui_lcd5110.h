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
	//����ps2ҡ�˵����ӵ����   ����������ѡ���λ�����룬����ѡ���λ������
	void SetPS2(int _key, int _vrx_input, int _vry_input);
	//����ps2ҡ�˴����ļ���   ���ޣ����� ��Χ����0-1024��Ĭ��650��350
	void SetTrigLimit(int _limit_high, int _limit_low);
	//���ps2��xҡ�����ϣ�����true.      ��ȡ���Ǳ�������ѵĹ���֮һ��
	boolean PS2VRXUp();
	//���ps2��xҡ�����£�����true.   
	boolean PS2VRXDown();
	//���ps2��yҡ�����ϣ�����true.   
	boolean PS2VRYUp();
	//���ps2��yҡ�����£�����true.   
	boolean PS2VRYDown();
	//����ps2ҡ�˿��ƹ�����ʱ  _delay�ӳ���ʱ�䵥λms   Ĭ��50
	void SetPS2Delay(int _delay);


	//LCD��ʼ������
	void SetLcd(int RST0, int CE0, int DC0, int DIN0, int CLK0);
	//LCD��������
	void Clear(void);
	//�����ַ�λ�ú���  xy����
	void SetPostion(unsigned char X, unsigned char Y);
	//����д�뵽lcd dat���ݣ�command 0����1����ѡ��
	void WriteData(unsigned char dat, unsigned char command);


	//��          xy����
	void point(int x, int y);
	//��ͼ        xy����(0-83,0-5)��width���,length����,l��������,�Ƿ��ԣ��Ƿ���ڴ��ȡ����
	void Draw(int x, int y, int width, int length, const char *l, bool reverse_display = 0, bool from_memory = 0);
	//�������    xy����(0-83,0-5), number��ֵ��bҪ��ӡ������λ, �Ƿ�Ϊ���������Ƿ���
	void Var(int x, int y, double number, int b, boolean  float_number = 0, bool reverse_display = 0);
	//�ı����    xy����(0-83,0-5), l�ַ������Ƿ���
	void Text(int x, int y, char* l, boolean reverse_display = 0);


	//���        button_now��ǰ��ť�����ҳ��ĵڼ�����ť��ps2ҡ�˵İ�������ʱ����true
	boolean Click(int button_now);
	//������ť    xy���꣬number������bҪ��ӡ������λ��float_number�Ƿ�Ϊ������
	void ButtonVar(int x, int y, double number, int b, boolean float_number = 0);
	//�ı���ť    xy���꣬l�ַ���
	void ButtonText(int x, int y, char *l);
	//���ݸ��ĶԻ���   number���޸ĵ����ݣ�bҪ��ӡ����λ����ʱ��Ϊ���㣬ÿ50ms�����̶�
	void MsgBox(double &number, int b, boolean float_num, float range);
	//���ݸ��ĶԻ���   number���޸ĵ����ݣ�bҪ��ӡ����λ����ʱ��Ϊ���㣬ÿ50ms�����̶�
	void MsgBox(int &number, int b, boolean float_num, float range);
	//ҳ�����ʹ�ü�ͷ��Ϊ���ָʾ
	void ArrowEnd();
	//ҳ�����ʹ�÷�����Ϊ���ָʾ
	void ReverseDisplayEnd();

	//��ȡ��ǰ��ҳ���ţ�255�������޸Ŀ�0����ҳ
	int GetPage();
	//���õ�ǰ��ҳ���ţ�255�������޸Ŀ�0����ҳ��������������ͻ
	void SetPage(int _page_now);
	//��ȡ��ǰ�Ĺ��λ�� 0��ʼ
	int GetCursor();
	//��ȡ��ǰҳ��Ĺ��λ�� 
	int SetCursor(int _curosr_now);
	//��ȡ��ǰҳ��Ĺ������
	int GetCursorNum();
	//���õ�ǰҳ��Ĺ������
	int SetCursorNum(int _cursor_num);
	//ҳ��ͬ��  ����ҳ��β������������end������ʱ����Ҫ�ֶ�����
	void PageSync();
	//����л���ҳ��    ����ture
	boolean PageSwitch();
	//����һ��ҳ�� _pageҪ��ҳ���ҳ����
	void ToPage(int _page);
private:
	/*******************����**************************/
	static int RST;
	static int CE;
	static int DC;
	static int DIN;
	static int CLK;
	static int cursor_pos;//��¼cpuִ�е��Ǹ�����
	static int cursor_num;//��ǰҳ��Ĺ������
	static int cursor_num_temp;
	static int cursor_now ;//��ŵ�ǰ����ڵ�ǰҳ���λ��
	static int cursor_now_temp;
	static int page_now;//��ǰҳ����
	static int page_now_temp;
	static int page_last ;//�ϸ�ҳ����
	static boolean first_record;//�Ƿ�Ϊ��ǰҳ���״μ�¼���λ��
	static boolean page_arrow ;//��ǰҳ���Ƿ�ʹ�ü�ͷ��Ϊ���ָʾ
	static int key ;//ps2ҡ�˵İ���
	static int vrx_input ;//ps2��vrx��λ��
	static int vry_input; //ps2��vrx��λ��
	static int limit_high;//��ȡ��λ��ģ��ֵ�����ޣ�����ʱ����
	static int limit_low;//����
	static int PS2_delay;//ps2ҡ�˳�������ʱ����ʱ
	static boolean sync;//�Ƿ����ͬ��
	static const char decimal_point[]PROGMEM ; //С����
	static const char frame[]PROGMEM ;
	static const char ASCIIDZ[67];
	static char char_conver_temp[112];//�ַ�ת������
	static const char xz[8] PROGMEM;//�õ��ĵط�����Ͳ���д������
	static const char ASCII[92][6]PROGMEM ;
	static const char cursor_arrow[6]PROGMEM ;//��ͷ��ʽ�Ĺ��
};


extern zui ui;//��һ�η��ֻ�������ɧ����
#endif

