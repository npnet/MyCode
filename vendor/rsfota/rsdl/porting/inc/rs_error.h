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
#ifndef RS_ERROR_H_
#define RS_ERROR_H_

#define RS_ERR_FAILED                      -1  // δ�������ʹ���
#define RS_ERR_OK                           0   // �ɹ�
#define RS_DL_STATE_ERROR                   1   // ��ǰ״̬����
#define RS_SESSION_STATE_ERROR              2   // ÿһ��session��״̬����
#define RS_USER_CANCEL                      3   // �û�ȡ����ǰ����
#define RS_CARD_NOT_INSERT                  4   // SIM��û�в���
#define RS_CARD_NOT_RECOGNIZE               5   // ����ʶ��
#define RS_SYSTEM_BUSY                      6   // ϵͳ��æ
#define RS_IMEI_NOT_WRITTEN					7	// IMEI not written
#define RS_ERR_INVALID_PARAM                9  // ��Ч����


// ϵͳ
#define RS_ERR_UNSPECIFIC                   10  // δָ���Ĵ���
#define RS_ERR_MALLOC_FAIL                  11  // �ڴ�������
#define RS_ERR_OPEN_FILE                    12  // ���ļ�����
#define RS_ERR_CREATE_FILE                  13  // �����ļ�����
#define RS_ERR_SERVICE_NOT_AVAILABLE        14  // ���������ɴ�
#define RS_ERR_INVALID_FILE                 15  // ��Ч�ļ�
#define RS_ERR_INVALID_URL                  16  // ��ЧURL��ַ
#define RS_ERR_WRITE_DATA_FAILE             17  // �ļ�д�����ݴ���
#define RS_ERR_READ_DATA_FAILE              18  // �ļ�д�����ݴ���
#define RS_ERR_FLASH_INIT                   19  // flash��ʼ������
#define RS_ERR_FLASH_WRITE                  20  // falshд��ʧ��
#define RS_ERR_FLASH_READ                   21  // flash��ȡʧ��
#define RS_ERR_SPACE_NOT_ENGHOU				22  // �ռ��Ƿ����
#define RS_ERR_BATTERY_LEVEL_LOW			23  // ��ص�����



// ���紫��
#define RS_ERR_XPT_OPEN_SOCK                40  // ��socketʧ��
#define RS_ERR_XPT_OPEN_TIMEOUT             41  // ����socket��ʱ
#define RS_ERR_XPT_SEND_DATA                42  // ��������ʧ��
#define RS_ERR_XPT_RECV_SELECT              43  // select��ѯ����ʧ��
#define RS_ERR_XPT_RECV_DATA                44  // ��������ʧ��
#define RS_ERR_XPT_RECV_TIMEOUT             45  // �������ݳ�ʱ
#define RS_ERR_XPT_RECV_PENDING             46  // ������������
#define RS_ERR_XPT_SSL_INIT                 47  // SSL��ʼ��ʧ��
#define RS_ERR_XPT_SSL_CONNECT              48  // SSL����ʧ��
#define RS_ERR_XPT_GET_HOST_BY_NAME         49  // ��������ʧ��



#endif // RS_ERROR_H_
