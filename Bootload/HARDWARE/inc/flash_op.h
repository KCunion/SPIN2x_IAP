#ifndef __FLASH_OP_H__
#define __FLASH_OP_H__
#include "sys.h"  
#include "stdbool.h"

//��ָ����ַ��������
//wFlashAddr:��ȡ��ַ
extern uint16_t Flash_ReadHalfWord(uint32_t wFlashAddr);
//������д��
//wWriteAddr:��ʼ��ַ
//phwBuffer:����ָ��
//hwNumber:����(16λ)��
extern void Flash_Write_NoCheck(uint32_t wWriteAddr, uint16_t *phwBuffer, uint16_t hwNumber);
//���д���Ƿ�ɹ����ɹ�����true,���ɹ�����false
//wWriteAddr:��ʼ��ַ
//phwBuffer:����ָ��
//hwNumber:����(16λ)��
extern bool Flash_Write_Check(uint32_t wWriteAddr, uint16_t *phwBuffer, uint16_t hwNumber);
//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//phwBuffer:����ָ��
//NumToWrite:����(16λ)��
extern void Flash_Read(uint32_t ReadAddr, uint16_t *phwBuffer, uint16_t NumToRead);
//FLASH ѡ���ֽڰ��ֱ�̲���
//wAddress��ѡ���ֽڵ�ַ
//hwData��Ҫд�������
FLASH_Status Flash_ProgramOptionHalfWord(uint32_t wAddress, uint16_t hwData);
//ʹ����Ƭ������
extern void Flash_ReadOut_Protection(void);
#endif
