//����Э��ͷ ��Ϊֱ��Ҫ������������в�ȷ�����ĵ�����
//Ϊ�˱����˷ѿռ�ѡ��������ִ��䣬�ʶ���һ��ͷ

#pragma once

const int MSG_LOGOIN = 0x01; //��¼
const int MSG_SEND = 0x11;   //������Ϣ
const int MSG_CLOSE = 0x02;  //�˳�
const int MSG_UPDATE = 0x21; //����Ϣ

#pragma pack(push,1)
typedef struct tagHeader {
	int type;//Э������

	int nContentLen; //��Ҫ�������ݵĳ���
	char to_user[20];
	char from_user[20];
}HEADER, *LPHEADER;
#pragma pack(pop)