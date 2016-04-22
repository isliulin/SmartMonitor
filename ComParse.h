#pragma once


//判断指令的正确性
BOOL IsWrightCode(BYTE* m_ReceiveByte);



//解析串口接收字符以得到X方向参数
double DecodeX(BYTE* ReceiveByte);



//解析串口接受字符以得到Y方向参数
double DecodeY(BYTE* ReceiveByte);



//由X方向与Y方向参数解析除传感器与水平面倾角
double AngCosReturn(double x,double y);



//检测一个字符是不是十六进制字符(0-F)，若是返回相应的值，否则返回0x10(即F)；
char HexChar(char c);



//将一个字符串作为十六进制串转化为一个字节数组，字节间可用空格分隔，
//返回转换后的字节数组长度，同时设置字节数组长度。
int Str2Hex(CString str,CByteArray &data);



//判断字符串是否为数字
bool IsNumString( CString str );