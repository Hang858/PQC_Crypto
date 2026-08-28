#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "operator_interface.h"

 /**
 * @brief ����4. Ӳ�����������  (�ܾ����Ȳ���)
 * �ڲ�ʵ�֣�
 * ����Ӳ��TRNGģ�飬�������������Ϊ���ӣ�
 * ���ڸ����ӣ�����α�������
 * 
 * @param buffer  ������洢������Ļ�������
 * @param size    ���룺��Ҫ��ȡ����������ȡ�
 * @return        0: �ɹ�; -1: Ӳ������/��Դ���㡣
 * �������ӿ��Ƿ�ֱ�ӵ���Ӳ��TRNGģ�飿
 * �������Ե��㷨δ��
 */
int OP_trng(void *buffer, int size)
{
    srand((unsigned int)time(NULL)); // 使用当前时间作为随机数种子
    unsigned char *buf = (unsigned char *)buffer;
    for (int i = 0; i < size; i++) {
        buf[i] = rand() % 256; // 生成0-255之间的随机字节
        //buf[i] = i % 256;
    }
    return OP_SUCCESS;
}
