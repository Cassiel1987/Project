//定义协议头 因为直接要传输的类容中有不确定长的的类容
//为了避免浪费空间选择分两部分传输，故定义一个头

#pragma once

const int MSG_LOGOIN = 0x01; //登录
const int MSG_SEND = 0x11;   //发送消息
const int MSG_CLOSE = 0x02;  //退出
const int MSG_UPDATE = 0x21; //更信息

#pragma pack(push,1)
typedef struct tagHeader {
	int type;//协议类型

	int nContentLen; //将要发送内容的长度
	char to_user[20];
	char from_user[20];
}HEADER, *LPHEADER;
#pragma pack(pop)