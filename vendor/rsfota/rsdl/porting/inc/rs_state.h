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
#ifndef _RS_STATE_H_
#define _RS_STATE_H_


/**
说明：描述了TOTA整个流程的各种状态
*/
typedef enum{
	RS_FWDL_STATE_IDEL,				// 空闲状态
	RS_FWDL_STATE_CHECKING,			// 检测中
	RS_FWDL_STATE_CHECKED,			// 检测完成
	RS_FWDL_STATE_DOWNLOADING,		// 下载中
	RS_FWDL_STATE_DOWNLOAD_PAUSE,	// 下载暂停
	RS_FWDL_STATE_DOWNLOADED,		// 下载完成
	RS_FWDL_STATE_INSTALLING,		// 安装中
	RS_FWDL_STATE_INSTALLED,		// 安装完成
	RS_FWDL_STATE_REPORTING,		// 上报中
	RS_FWDL_STATE_REPORT_PAUSE,		// 上报暂停，比如用户设置的总周期为10，这期间没有上报成功的情况下为此状态
	RS_FWDL_STATE_REPORTED			// 上报完成
} RS_FWDL_STATE;



/**
说明：开机之后如果上次有未完成的任务，会对上层有一个通知
*/
typedef enum{
	RS_FWDL_BREAK_STATE_DOWNLOADING,	// 提醒用户上次异常中断，是否重新启动下载
	RS_FWDL_BREAK_STATE_RESUME,			// 提醒用户上次有处于暂停状态的未完成的任务，是否继续
	RS_FWDL_BREAK_STATE_DOWNLOADED,		// 提醒用户上次下载完成，但是没有进行升级的任务
} RS_FWDL_BREAK_STATE;


#endif