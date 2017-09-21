/*
 *
 * 版权(c) 2015 红石阳光（北京）科技有限公司
 * 版权(c) 2011-2015 红石阳光（北京）科技有限公司版权所有
 * 
 * 本文档包含信息是私密的且为红石阳光私有，且在民法和刑法中定义为商业秘密信息。
 * 任何人士不得擅自复印，扫描或以其他任何方式进行传播，否则红石阳光有权追究法律责任。
 * 阅读和使用本资料必须获得相应的书面授权，承担保密责任和接受相应的法律约束。
 *
 */
#ifndef RS_ERROR_H_
#define RS_ERROR_H_

#define RS_ERR_FAILED                      -1  // 未定义类型错误
#define RS_ERR_OK                           0   // 成功
#define RS_DL_STATE_ERROR                   1   // 当前状态错误
#define RS_SESSION_STATE_ERROR              2   // 每一个session的状态错误
#define RS_USER_CANCEL                      3   // 用户取消当前操作
#define RS_CARD_NOT_INSERT                  4   // SIM卡没有插入
#define RS_CARD_NOT_RECOGNIZE               5   // 卡不识别
#define RS_SYSTEM_BUSY                      6   // 系统正忙
#define RS_IMEI_NOT_WRITTEN					7	// IMEI not written
#define RS_ERR_INVALID_PARAM                9  // 无效参数


// 系统
#define RS_ERR_UNSPECIFIC                   10  // 未指定的错误
#define RS_ERR_MALLOC_FAIL                  11  // 内存分配错误
#define RS_ERR_OPEN_FILE                    12  // 打开文件错误
#define RS_ERR_CREATE_FILE                  13  // 创建文件错误
#define RS_ERR_SERVICE_NOT_AVAILABLE        14  // 服务器不可达
#define RS_ERR_INVALID_FILE                 15  // 无效文件
#define RS_ERR_INVALID_URL                  16  // 无效URL地址
#define RS_ERR_WRITE_DATA_FAILE             17  // 文件写入数据错误
#define RS_ERR_READ_DATA_FAILE              18  // 文件写入数据错误
#define RS_ERR_FLASH_INIT                   19  // flash初始化错误
#define RS_ERR_FLASH_WRITE                  20  // falsh写入失败
#define RS_ERR_FLASH_READ                   21  // flash读取失败
#define RS_ERR_SPACE_NOT_ENGHOU				22  // 空间是否充足
#define RS_ERR_BATTERY_LEVEL_LOW			23  // 电池电量低



// 网络传输
#define RS_ERR_XPT_OPEN_SOCK                40  // 打开socket失败
#define RS_ERR_XPT_OPEN_TIMEOUT             41  // 连接socket超时
#define RS_ERR_XPT_SEND_DATA                42  // 发送数据失败
#define RS_ERR_XPT_RECV_SELECT              43  // select查询数据失败
#define RS_ERR_XPT_RECV_DATA                44  // 接收数据失败
#define RS_ERR_XPT_RECV_TIMEOUT             45  // 接收数据超时
#define RS_ERR_XPT_RECV_PENDING             46  // 接收数据阻塞
#define RS_ERR_XPT_SSL_INIT                 47  // SSL初始化失败
#define RS_ERR_XPT_SSL_CONNECT              48  // SSL连接失败
#define RS_ERR_XPT_GET_HOST_BY_NAME         49  // 域名解析失败



#endif // RS_ERROR_H_
