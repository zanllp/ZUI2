#pragma once
#ifndef ZUILCD5110_H_
#define ZUILCD5110_H_
/************************����*******************************************/

//LCD��ʼ������
void LcdInit(int RST0, int CE0, int DC0, int DIN0, int CLK0);
//LCD��������
void Clear(void);
//�����ַ�λ�ú���
void SetPostion(unsigned char X, unsigned char Y);
/*******************���ݵ�LCD�������*****dat���ݣ�command 0����1����ѡ��*********/
void WriteData(unsigned char dat, unsigned char command);


//�� xy����(0-83,0-47)
void point(int x, int y);
// ��ͼ        xy����(0-83,0-5)��width���,length����,l��������,�Ƿ��ԣ��Ƿ���ڴ��ȡ����
void Draw(int x, int y, int width, int length, const char *l, bool reverse_display = 0, bool from_memory = 0);
// �������    xy����(0-83,0-5), a��ֵ��bҪ��ӡ������λ, �Ƿ�Ϊ���������Ƿ���
void Var(int x, int y, double a, int b, boolean  float_number = 0, bool reverse_display = 0);
// �ı����    xy����(0-83,0-5), a��ֵ��l�ַ������Ƿ���
void Text(int x, int y, char* l, boolean reverse_display = 0);



/**************************����**************************************/
int RST ;
int CE ;
int DC ;
int DIN ;
int CLK ;
int cursor_pos[20][2];//��ŵ�ǰҳ�����й�������
int cursor_num;//��ǰҳ��Ĺ������
int cursor_num_temp;
int cursor_now = 1;//��ŵ�ǰ����ڵ�ǰҳ���λ��
int cursor_now_temp;
int page_now;//��ǰҳ����
int page_now_temp;
int page_last = 1;//�ϸ�ҳ����
double number_temp;//��Ϣ�������޸ĵĻ���
unsigned time_temp;
boolean arrow_state;
boolean ixy;
int key = 12;//����
int vertical_input = A0;//����ѡ���λ������
int horiziontal_input = A1; //����ѡ���λ������
int limit_high = 600;//��ȡ��λ��ģ��ֵ�����ޣ�����ʱ����
int limit_low = 400;//����

#endif // !ZUILCD5110_H_

