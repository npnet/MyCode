#ifndef _RS_PORTING_CFG_H_
#define _RS_PORTING_CFG_H_

/*
 * RS_SUPPORT_UPDATE_INFO_FS
 *
 * ���ع������Ƿ�֧�ְ�������Ϣ��¼���ļ�ϵͳ
 * �ڲ�֧�ֵ������,������Ϣ��洢��flash�У���ô��ʱҲ��ʾ�ܹ�֧��flash�Ĳ���
 */
//#define RS_SUPPORT_UPDATE_INFO_FS

/*
 * RS_SUPPORT_UPDATE_DATA_FS
 *
 * ���ع������Ƿ�֧�ְ��������洢���ļ�ϵͳ��
 * ���bootloader�׶λ���recoveryģʽ�ܹ�ʶ���ļ�ϵͳ����ôӦ��֧�ִ�ѡ��
 * �ڲ�֧�ֵ������,��������洢��flash�У���ô��ʱҲ��ʾ�ܹ�֧��flash�Ĳ���
 * 
 * ��ע������ܹ�֧��RS_SUPPORT_UPDATE_DATA_FS����ôRS_SUPPORT_UPDATE_INFO_FS��һ���ܹ�֧�֡�
 * ���֧��RS_SUPPORT_UPDATE_INFO_FS������RS_SUPPORT_UPDATE_INFO_FS��һ������֧�֣�MTK6216����һ�����ӣ� 
 * ��Ȼ���ļ�ϵͳ������bootloader��֧�֡�
 */
//#define RS_SUPPORT_UPDATE_DATA_FS

#endif