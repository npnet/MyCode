/*
 *
 * ��Ȩ(c) 2015 ��ʯ���⣨�������Ƽ����޹�˾
 * ��Ȩ(c) 2011-2015 ��ʯ���⣨�������Ƽ����޹�˾��Ȩ����
 * 
 * ���ĵ�������Ϣ��˽�ܵ���Ϊ��ʯ����˽�У������񷨺��̷��ж���Ϊ��ҵ������Ϣ��
 * �κ���ʿ�������Ը�ӡ��ɨ����������κη�ʽ���д����������ʯ������Ȩ׷���������Ρ�
 * �Ķ���ʹ�ñ����ϱ�������Ӧ��������Ȩ���е��������κͽ�����Ӧ�ķ���Լ����
 *
 */
#ifndef _RS_STATE_H_
#define _RS_STATE_H_


/**
˵����������TOTA�������̵ĸ���״̬
*/
typedef enum{
	RS_FWDL_STATE_IDEL,				// ����״̬
	RS_FWDL_STATE_CHECKING,			// �����
	RS_FWDL_STATE_CHECKED,			// ������
	RS_FWDL_STATE_DOWNLOADING,		// ������
	RS_FWDL_STATE_DOWNLOAD_PAUSE,	// ������ͣ
	RS_FWDL_STATE_DOWNLOADED,		// �������
	RS_FWDL_STATE_INSTALLING,		// ��װ��
	RS_FWDL_STATE_INSTALLED,		// ��װ���
	RS_FWDL_STATE_REPORTING,		// �ϱ���
	RS_FWDL_STATE_REPORT_PAUSE,		// �ϱ���ͣ�������û����õ�������Ϊ10�����ڼ�û���ϱ��ɹ��������Ϊ��״̬
	RS_FWDL_STATE_REPORTED			// �ϱ����
} RS_FWDL_STATE;



/**
˵��������֮������ϴ���δ��ɵ����񣬻���ϲ���һ��֪ͨ
*/
typedef enum{
	RS_FWDL_BREAK_STATE_DOWNLOADING,	// �����û��ϴ��쳣�жϣ��Ƿ�������������
	RS_FWDL_BREAK_STATE_RESUME,			// �����û��ϴ��д�����ͣ״̬��δ��ɵ������Ƿ����
	RS_FWDL_BREAK_STATE_DOWNLOADED,		// �����û��ϴ�������ɣ�����û�н�������������
} RS_FWDL_BREAK_STATE;


#endif