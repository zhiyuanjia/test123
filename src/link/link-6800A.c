#include "link_def-6800A.h"
#include "../osd/osd.h"

MultiChHdDvr_Context gHdDvrUsecaseContext1 = {0 };

static Void multich_hddvr_reset_link_prms()
{
	int i;

	// OSD
	MULTICH_INIT_STRUCT(IpcFramesOutLinkRTOS_CreateParams,gHdDvrUsecaseContext1.ipcFramesOutVpssPrm[0]);
	MULTICH_INIT_STRUCT(IpcFramesInLinkRTOS_CreateParams,gHdDvrUsecaseContext1.ipcFramesInDspPrm[0]);
	IpcBitsOutLinkRTOS_CreateParams_Init(&gHdDvrUsecaseContext1.ipcBitsOutDspPrm);
	for (i = 0; i < OSA_ARRAYSIZE(gHdDvrUsecaseContext1.dspAlgPrm);i++)
	{
		AlgLink_CreateParams_Init(&gHdDvrUsecaseContext1.dspAlgPrm[i]);
	}

	// 解码
	DecLink_CreateParams_Init(&gHdDvrUsecaseContext1.decPrm);
	MULTICH_INIT_STRUCT(IpcLink_CreateParams,gHdDvrUsecaseContext1.ipcOutVideoPrm);
	MULTICH_INIT_STRUCT(IpcLink_CreateParams,gHdDvrUsecaseContext1.ipcInVpssPrm);
	IpcBitsOutLinkHLOS_CreateParams_Init(&gHdDvrUsecaseContext1.ipcBitsOutHostPrm);
	MULTICH_INIT_STRUCT(IpcBitsInLinkRTOS_CreateParams,gHdDvrUsecaseContext1.ipcBitsInVideoPrm);

	// 编码
	MULTICH_INIT_STRUCT(SclrLink_CreateParams, gHdDvrUsecaseContext1.sclrPrm);
	MULTICH_INIT_STRUCT(IpcLink_CreateParams,gHdDvrUsecaseContext1.ipcOutVpssPrm);
	MULTICH_INIT_STRUCT(IpcLink_CreateParams,gHdDvrUsecaseContext1.ipcInVideoPrm);
	MULTICH_INIT_STRUCT(IpcBitsOutLinkRTOS_CreateParams,gHdDvrUsecaseContext1.ipcBitsOutVideoPrm);
	MULTICH_INIT_STRUCT(IpcBitsInLinkHLOS_CreateParams,gHdDvrUsecaseContext1.ipcBitsInHostPrm[0]);
	EncLink_CreateParams_Init(&gHdDvrUsecaseContext1.encPrm);

	// 采集, DEI, NSF, MERGE, SELECT等
	CaptureLink_CreateParams_Init(&gHdDvrUsecaseContext1.capturePrm);
	for (i = 0; i < OSA_ARRAYSIZE(gHdDvrUsecaseContext1.nsfPrm);i++)
	{
		NsfLink_CreateParams_Init(&gHdDvrUsecaseContext1.nsfPrm[i]);
	}
	for (i = 0; i < OSA_ARRAYSIZE(gHdDvrUsecaseContext1.deiPrm);i++)
	{
		DeiLink_CreateParams_Init(&gHdDvrUsecaseContext1.deiPrm[i]);
	}
	for (i = 0; i < OSA_ARRAYSIZE(gHdDvrUsecaseContext1.selectPrm);i++)
	{
		SelectLink_CreateParams_Init(&gHdDvrUsecaseContext1.selectPrm[i]);
	}

	// 显示
	for (i = 0; i < OSA_ARRAYSIZE(gHdDvrUsecaseContext1.swMsPrm);i++)
	{
		SwMsLink_CreateParams_Init(&gHdDvrUsecaseContext1.swMsPrm[i]);
	}
	for (i = 0; i < OSA_ARRAYSIZE(gHdDvrUsecaseContext1.displayPrm);i++)
	{
		DisplayLink_CreateParams_Init(&gHdDvrUsecaseContext1.displayPrm[i]);
	}
	for (i = 0; i < OSA_ARRAYSIZE(gHdDvrUsecaseContext1.avsyncCfg);i++)
	{
		AvsyncLink_LinkSynchConfigParams_Init(&gHdDvrUsecaseContext1.avsyncCfg[i]);
	}
}

static Void multich_hddvr_set_link_ids()
{
	Bool   enableOsdAlgLink = gVsysModuleContext.vsysConfig.enableOsd;
	Bool   enableScdAlgLink = gVsysModuleContext.vsysConfig.enableScd;
	int    i;

	// 解码
	gVdecModuleContext.decId        = SYSTEM_LINK_ID_VDEC_0;
	gHdDvrUsecaseContext1.ipcOutVideoId= SYSTEM_VIDEO_LINK_ID_IPC_OUT_M3_0;
	gHdDvrUsecaseContext1.ipcInVpssId  = SYSTEM_VPSS_LINK_ID_IPC_IN_M3_0;
	gVdecModuleContext.ipcBitsOutHLOSId     = SYSTEM_HOST_LINK_ID_IPC_BITS_OUT_0;
	gVdecModuleContext.ipcBitsInRTOSId      = SYSTEM_VIDEO_LINK_ID_IPC_BITS_IN_0;

	// 编码
	gVencModuleContext.encId        = SYSTEM_LINK_ID_VENC_0;
	gHdDvrUsecaseContext1.ipcOutVpssId = SYSTEM_VPSS_LINK_ID_IPC_OUT_M3_0;
	gHdDvrUsecaseContext1.ipcInVideoId = SYSTEM_VIDEO_LINK_ID_IPC_IN_M3_0;
	gVencModuleContext.ipcBitsOutRTOSId     = SYSTEM_VIDEO_LINK_ID_IPC_BITS_OUT_0;
	gVencModuleContext.ipcBitsInHLOSId   = SYSTEM_HOST_LINK_ID_IPC_BITS_IN_0;

	gVcapModuleContext.captureId    = SYSTEM_LINK_ID_CAPTURE;

	for (i = 0; i < NUM_NSF_LINK;i++)
	{
		gVcapModuleContext.nsfId[i]         = SYSTEM_LINK_ID_NSF_0 + i;
	}

	// 采集, DEI, NSF, MERGE, SELECT等
	for (i = 0; i < NUM_MERGE_LINK;i++)
	{
		gHdDvrUsecaseContext1.mergeId[i] = SYSTEM_VPSS_LINK_ID_MERGE_0 + i;
	}
	for (i = 0; i < NUM_DUP_LINK;i++)
	{
		gHdDvrUsecaseContext1.dupId[i] = SYSTEM_VPSS_LINK_ID_DUP_0 + i;
	}
	gVcapModuleContext.deiId[DEIHQ_FULL_RES]     = SYSTEM_LINK_ID_DEI_HQ_0;
#if 1
	gVcapModuleContext.deiId[DEIHQ_Q_RES]        = SYSTEM_LINK_ID_DEI_0;
#endif
	gVcapModuleContext.deiId[DEI_FULL_RES]       = SYSTEM_LINK_ID_DEI_HQ_1;
	for (i = 0; i < NUM_SELECT_LINK;i++)
	{
		gHdDvrUsecaseContext1.selectId[i]   = SYSTEM_VPSS_LINK_ID_SELECT_0 + i;
	}

	// OSD
        gVcapModuleContext.ipcFramesOutVpssId[0] = SYSTEM_VPSS_LINK_ID_IPC_FRAMES_OUT_0;
        gVcapModuleContext.ipcFramesInDspId[0]   = SYSTEM_DSP_LINK_ID_IPC_FRAMES_IN_0;
        gVcapModuleContext.dspAlgId[0]           = SYSTEM_LINK_ID_ALG_0;

	// 显示
	//gVdisModuleContext.setMosaicFxn	= MultiCh_hdDvrSetMosaicParams;
	gVdisModuleContext.swMsId[ETAH_SWMS_ID_HDMI]		= SYSTEM_LINK_ID_SW_MS_MULTI_INST_0;
	gVdisModuleContext.swMsId[ETAH_SWMS_ID_MOSAIC]		= SYSTEM_LINK_ID_SW_MS_MULTI_INST_1;
	gVdisModuleContext.swMsId[ETAH_SWMS_ID_SD]		= SYSTEM_LINK_ID_SW_MS_MULTI_INST_2;
	gVdisModuleContext.displayId[ETAH_SWMS_ID_HDMI]		= SYSTEM_LINK_ID_DISPLAY_0;		// ON CHIP HDMI
	//gVdisModuleContext.displayId[ETAH_SWMS_ID_MOSAIC]	= SYSTEM_LINK_ID_DISPLAY_1;		// SD HDMI
	gVdisModuleContext.displayId[1]	= SYSTEM_LINK_ID_DISPLAY_1;
	gVdisModuleContext.displayId[ETAH_SWMS_ID_SD]		= SYSTEM_LINK_ID_DISPLAY_2;		// SD HDMI
	gVcapModuleContext.sclrId[0]    = SYSTEM_LINK_ID_SCLR_INST_0;
}

Void multich_hddvr_register_created_link(MultiChHdDvr_Context *pContext, UInt32 linkID)
{
	OSA_assert(pContext->createdLinkCount < OSA_ARRAYSIZE(pContext->createdLinks));
	pContext->createdLinks[pContext->createdLinkCount] = linkID;
	pContext->createdLinkCount++;
}

static Void multich_hddvr_connect_links()
{
	DeiLink_OutputScaleFactor outScaleFactorDeiSc, outScaleFactorVipSc;
	gVcapModuleContext.vcapConfig.enableConfigExtVideoDecoder = FALSE;

	// 视频捕获
	multich_hddvr_set_capture_prm(&gHdDvrUsecaseContext1.capturePrm);
	gHdDvrUsecaseContext1.capturePrm.outQueParams[0].nextLink = gHdDvrUsecaseContext1.dupId[DUP_FULL_RES_CAPTURE];
	MULTICH_HDDVR_CREATE_LINK(gVcapModuleContext.captureId, gHdDvrUsecaseContext1.capturePrm);

	// DUP: 第一份给预览，第二份给编码(内部会实现主次码流)
	gHdDvrUsecaseContext1.dupPrm[DUP_FULL_RES_CAPTURE].inQueParams.prevLinkId    = gVcapModuleContext.captureId;
	gHdDvrUsecaseContext1.dupPrm[DUP_FULL_RES_CAPTURE].inQueParams.prevLinkQueId = 0;
	gHdDvrUsecaseContext1.dupPrm[DUP_FULL_RES_CAPTURE].numOutQue                 = 2;
	gHdDvrUsecaseContext1.dupPrm[DUP_FULL_RES_CAPTURE].outQueParams[0].nextLink  = gHdDvrUsecaseContext1.mergeId[MERGE_DISPLAY];
	gHdDvrUsecaseContext1.dupPrm[DUP_FULL_RES_CAPTURE].outQueParams[1].nextLink  = gHdDvrUsecaseContext1.selectId[SELECT_FULL_RES_CAPTURE];
	gHdDvrUsecaseContext1.dupPrm[DUP_FULL_RES_CAPTURE].notifyNextLink            = TRUE;
	MULTICH_HDDVR_CREATE_LINK(gHdDvrUsecaseContext1.dupId[DUP_FULL_RES_CAPTURE], gHdDvrUsecaseContext1.dupPrm[DUP_FULL_RES_CAPTURE]);

	// 4路编码: 分给两个dei处理: deihq和dei
	gHdDvrUsecaseContext1.selectPrm[SELECT_FULL_RES_CAPTURE].numOutQue = 2;
	gHdDvrUsecaseContext1.selectPrm[SELECT_FULL_RES_CAPTURE].inQueParams.prevLinkId    = gHdDvrUsecaseContext1.dupId[DUP_FULL_RES_CAPTURE];
	gHdDvrUsecaseContext1.selectPrm[SELECT_FULL_RES_CAPTURE].inQueParams.prevLinkQueId = 1;
	gHdDvrUsecaseContext1.selectPrm[SELECT_FULL_RES_CAPTURE].outQueParams[0].nextLink  = gVcapModuleContext.deiId[DEIHQ_FULL_RES];
	gHdDvrUsecaseContext1.selectPrm[SELECT_FULL_RES_CAPTURE].outQueParams[1].nextLink  = gVcapModuleContext.deiId[DEI_FULL_RES];
	gHdDvrUsecaseContext1.selectPrm[SELECT_FULL_RES_CAPTURE].outQueChInfo[0].outQueId  = 0;
	gHdDvrUsecaseContext1.selectPrm[SELECT_FULL_RES_CAPTURE].outQueChInfo[0].numOutCh  = 2;
	gHdDvrUsecaseContext1.selectPrm[SELECT_FULL_RES_CAPTURE].outQueChInfo[0].inChNum[0]= 0;
	gHdDvrUsecaseContext1.selectPrm[SELECT_FULL_RES_CAPTURE].outQueChInfo[0].inChNum[1]= 1;
	gHdDvrUsecaseContext1.selectPrm[SELECT_FULL_RES_CAPTURE].outQueChInfo[1].outQueId  = 1;
	gHdDvrUsecaseContext1.selectPrm[SELECT_FULL_RES_CAPTURE].outQueChInfo[1].numOutCh  = 2;
	gHdDvrUsecaseContext1.selectPrm[SELECT_FULL_RES_CAPTURE].outQueChInfo[1].inChNum[0]= 2;
	gHdDvrUsecaseContext1.selectPrm[SELECT_FULL_RES_CAPTURE].outQueChInfo[1].inChNum[1]= 3;
	MULTICH_HDDVR_CREATE_LINK(gHdDvrUsecaseContext1.selectId[SELECT_FULL_RES_CAPTURE], gHdDvrUsecaseContext1.selectPrm[SELECT_FULL_RES_CAPTURE]);

	// DEIHQ, DEI 缩放和去隔行，内部还会复制为主次码流
	outScaleFactorVipSc.scaleMode = DEI_SCALE_MODE_RATIO;		// 主码流: 1:1 scaling, output is 420
	outScaleFactorVipSc.ratio.widthRatio.numerator     = 1;
	outScaleFactorVipSc.ratio.widthRatio.denominator   = 1;
	outScaleFactorVipSc.ratio.heightRatio.numerator    = 1;
	outScaleFactorVipSc.ratio.heightRatio.denominator  = 1;
	outScaleFactorDeiSc.scaleMode = DEI_SCALE_MODE_RATIO;		// 次码流: 1/2 scaling, output is 422, need NSF link
	outScaleFactorDeiSc.ratio.widthRatio.numerator     = 1;
	outScaleFactorDeiSc.ratio.widthRatio.denominator   = 2;
	outScaleFactorDeiSc.ratio.heightRatio.numerator    = 1;
	outScaleFactorDeiSc.ratio.heightRatio.denominator  = 2;
	multich_hddvr_set_dei_prm(&gHdDvrUsecaseContext1.deiPrm[DEIHQ_FULL_RES], &outScaleFactorDeiSc, &outScaleFactorVipSc);
	gHdDvrUsecaseContext1.deiPrm[DEIHQ_FULL_RES].inQueParams.prevLinkId    = gHdDvrUsecaseContext1.selectId[SELECT_FULL_RES_CAPTURE];
	gHdDvrUsecaseContext1.deiPrm[DEIHQ_FULL_RES].inQueParams.prevLinkQueId = 0;
	gHdDvrUsecaseContext1.deiPrm[DEIHQ_FULL_RES].outQueParams[DEI_LINK_OUT_QUE_VIP_SC].nextLink = gHdDvrUsecaseContext1.mergeId[MERGE_ENCODE];
	gHdDvrUsecaseContext1.deiPrm[DEIHQ_FULL_RES].outQueParams[DEI_LINK_OUT_QUE_DEI_SC].nextLink = gHdDvrUsecaseContext1.mergeId[MERGE_Q_RES_CAPTURE];
	//gHdDvrUsecaseContext1.deiPrm[DEIHQ_FULL_RES].outQueParams[DEI_LINK_OUT_QUE_DEI_SC].nextLink = SYSTEM_VPSS_LINK_ID_NULL_0;	// 本地不要次码流
	MULTICH_HDDVR_CREATE_LINK(gVcapModuleContext.deiId[DEIHQ_FULL_RES], gHdDvrUsecaseContext1.deiPrm[DEIHQ_FULL_RES]);		// DEIHQ
	multich_hddvr_set_dei_prm(&gHdDvrUsecaseContext1.deiPrm[DEI_FULL_RES]  , &outScaleFactorDeiSc, &outScaleFactorVipSc);
	gHdDvrUsecaseContext1.deiPrm[DEI_FULL_RES].inQueParams.prevLinkId    = gHdDvrUsecaseContext1.selectId[SELECT_FULL_RES_CAPTURE];
	gHdDvrUsecaseContext1.deiPrm[DEI_FULL_RES].inQueParams.prevLinkQueId = 1;
	gHdDvrUsecaseContext1.deiPrm[DEI_FULL_RES].outQueParams[DEI_LINK_OUT_QUE_VIP_SC].nextLink = gHdDvrUsecaseContext1.mergeId[MERGE_ENCODE];
	gHdDvrUsecaseContext1.deiPrm[DEI_FULL_RES].outQueParams[DEI_LINK_OUT_QUE_DEI_SC].nextLink = gHdDvrUsecaseContext1.mergeId[MERGE_Q_RES_CAPTURE];
	//gHdDvrUsecaseContext1.deiPrm[DEI_FULL_RES].outQueParams[DEI_LINK_OUT_QUE_DEI_SC].nextLink = SYSTEM_VPSS_LINK_ID_NULL_0;		// 本地不要次码流
	MULTICH_HDDVR_CREATE_LINK(gVcapModuleContext.deiId[DEI_FULL_RES], gHdDvrUsecaseContext1.deiPrm[DEI_FULL_RES]);			// DEI

#if 1
	// 次码流合并
	gHdDvrUsecaseContext1.mergePrm[MERGE_Q_RES_CAPTURE].numInQue = 2;
	gHdDvrUsecaseContext1.mergePrm[MERGE_Q_RES_CAPTURE].inQueParams[0].prevLinkId    = gVcapModuleContext.deiId[DEIHQ_FULL_RES];
	gHdDvrUsecaseContext1.mergePrm[MERGE_Q_RES_CAPTURE].inQueParams[0].prevLinkQueId = DEI_LINK_OUT_QUE_DEI_SC;
	gHdDvrUsecaseContext1.mergePrm[MERGE_Q_RES_CAPTURE].inQueParams[1].prevLinkId    = gVcapModuleContext.deiId[DEI_FULL_RES];
	gHdDvrUsecaseContext1.mergePrm[MERGE_Q_RES_CAPTURE].inQueParams[1].prevLinkQueId = DEI_LINK_OUT_QUE_DEI_SC;
	gHdDvrUsecaseContext1.mergePrm[MERGE_Q_RES_CAPTURE].outQueParams.nextLink        = gVcapModuleContext.nsfId[NSF_Q_RES_CAPTURE];
	gHdDvrUsecaseContext1.mergePrm[MERGE_Q_RES_CAPTURE].notifyNextLink               = TRUE;
	MULTICH_HDDVR_CREATE_LINK(gHdDvrUsecaseContext1.mergeId[MERGE_Q_RES_CAPTURE], gHdDvrUsecaseContext1.mergePrm[MERGE_Q_RES_CAPTURE]);

	// 次码流422转420(编码需要420)
	multich_hddvr_set_nsf_prm(&gHdDvrUsecaseContext1.nsfPrm[NSF_Q_RES_CAPTURE], NUM_BUFS_PER_CH_NSF_Q_RES_CAPTURE, 30);
	gHdDvrUsecaseContext1.nsfPrm[NSF_Q_RES_CAPTURE].inQueParams.prevLinkId    = gHdDvrUsecaseContext1.mergeId[MERGE_Q_RES_CAPTURE];
	gHdDvrUsecaseContext1.nsfPrm[NSF_Q_RES_CAPTURE].inQueParams.prevLinkQueId = 0;
	gHdDvrUsecaseContext1.nsfPrm[NSF_Q_RES_CAPTURE].outQueParams[0].nextLink  = gHdDvrUsecaseContext1.mergeId[MERGE_ENCODE];
	MULTICH_HDDVR_CREATE_LINK(gVcapModuleContext.nsfId[NSF_Q_RES_CAPTURE], gHdDvrUsecaseContext1.nsfPrm[NSF_Q_RES_CAPTURE]);	// 出来本地次码流
#endif

	// 解码和预览，预览出来拼接主码流和拼接次码流
	multich_hddvr_connect_decode_links();
	multich_hddvr_connect_display_links();

	// 编码前的合并
	int enc_cnt = 0;
	gHdDvrUsecaseContext1.mergePrm[MERGE_ENCODE].inQueParams[enc_cnt].prevLinkId	= gVcapModuleContext.deiId[DEIHQ_FULL_RES];	// 主码流前2路
	gHdDvrUsecaseContext1.mergePrm[MERGE_ENCODE].inQueParams[enc_cnt].prevLinkQueId	= DEI_LINK_OUT_QUE_VIP_SC;
	enc_cnt++;
	gHdDvrUsecaseContext1.mergePrm[MERGE_ENCODE].inQueParams[enc_cnt].prevLinkId	= gVcapModuleContext.deiId[DEI_FULL_RES];	// 主码流后2路
	gHdDvrUsecaseContext1.mergePrm[MERGE_ENCODE].inQueParams[enc_cnt].prevLinkQueId	= DEI_LINK_OUT_QUE_VIP_SC;
	enc_cnt++;
	gHdDvrUsecaseContext1.mergePrm[MERGE_ENCODE].inQueParams[enc_cnt].prevLinkId	= gVcapModuleContext.deiId[DEIHQ_Q_RES];	// 拼接主码流
	gHdDvrUsecaseContext1.mergePrm[MERGE_ENCODE].inQueParams[enc_cnt].prevLinkQueId	= DEI_LINK_OUT_QUE_VIP_SC;
	enc_cnt++;
	gHdDvrUsecaseContext1.mergePrm[MERGE_ENCODE].inQueParams[enc_cnt].prevLinkId	= gVcapModuleContext.nsfId[NSF_Q_RES_CAPTURE];	// 4路次码流
	gHdDvrUsecaseContext1.mergePrm[MERGE_ENCODE].inQueParams[enc_cnt].prevLinkQueId= 0;
	enc_cnt++;
	gHdDvrUsecaseContext1.mergePrm[MERGE_ENCODE].inQueParams[enc_cnt].prevLinkId	= gVcapModuleContext.nsfId[NSF_MOSAIC_ENCODE];	// 拼接次码流
	gHdDvrUsecaseContext1.mergePrm[MERGE_ENCODE].inQueParams[enc_cnt].prevLinkQueId	= 0;
	enc_cnt++;
	gHdDvrUsecaseContext1.mergePrm[MERGE_ENCODE].numInQue				= enc_cnt;
#ifdef ETAH_6800A_USE_OSD
	gHdDvrUsecaseContext1.mergePrm[MERGE_ENCODE].outQueParams.nextLink		= gVcapModuleContext.ipcFramesOutVpssId[0];	// 有osd
#else
	gHdDvrUsecaseContext1.mergePrm[MERGE_ENCODE].outQueParams.nextLink		= gHdDvrUsecaseContext1.ipcOutVpssId;		// 无osd
#endif
	//gHdDvrUsecaseContext1.mergePrm[MERGE_ENCODE].outQueParams.nextLink		= SYSTEM_VPSS_LINK_ID_NULL_0;			// 不编码
	gHdDvrUsecaseContext1.mergePrm[MERGE_ENCODE].notifyNextLink			= TRUE;
	MULTICH_HDDVR_CREATE_LINK(gHdDvrUsecaseContext1.mergeId[MERGE_ENCODE], gHdDvrUsecaseContext1.mergePrm[MERGE_ENCODE]);

	if(gHdDvrUsecaseContext1.mergePrm[MERGE_ENCODE].outQueParams.nextLink == gVcapModuleContext.ipcFramesOutVpssId[0]) {
		multich_hddvr_connect_osd_links();	// OSD
	}
	multich_hddvr_connect_encode_links();
}

static Void multich_set_links_framerate()
{
}

static Void multich_set_dec2disp_chmap()
{
}

static char *gMultiCh_cpuName[SYSTEM_PLATFORM_CPU_REV_MAX] = {
	"ES1.0",
	"ES1.1",
	"ES2.0",
	"ES2.1",
	"UNKNOWN",
};

static int MultiCh_detectBoard_6800A()
{
	int status;
	int cpuRev;

	SystemVpss_PlatformInfo  platformInfo;

	status = System_linkControl(
			SYSTEM_LINK_ID_M3VPSS
			, SYSTEM_M3VPSS_CMD_GET_PLATFORM_INFO
			, &platformInfo
			, sizeof(platformInfo)
			, TRUE
			);
	UTILS_assert(status==OSA_SOK);

	// Get CPU version
	cpuRev = platformInfo.cpuRev;			// sdk里面的api接口获取cpu版本号错误，正确的用dmesg里面查看。
	if (cpuRev >= SYSTEM_PLATFORM_CPU_REV_MAX)
	{
		cpuRev = SYSTEM_PLATFORM_CPU_REV_UNKNOWN;
	}
	printf(" %u: MCFW  : CPU Revision [%s] !!! \r\n", OSA_getCurTimeInMsec(), gMultiCh_cpuName[cpuRev]);

	return 0;
}

void MultiCh_createHdDvr1()			// MultiCh_createHdDvr
{
	multich_hddvr_reset_link_prms();		// link 参数
	multich_hddvr_set_link_ids();			// link ID
	MultiCh_detectBoard_6800A();

	printf("============System init links entry>>>>>>>>>>>>\n");
	System_linkControl(SYSTEM_LINK_ID_M3VPSS, SYSTEM_M3VPSS_CMD_RESET_VIDEO_DEVICES, NULL, 0, TRUE);
	System_linkControl(SYSTEM_LINK_ID_M3VIDEO, SYSTEM_COMMON_CMD_SET_CH2IVAHD_MAP_TBL, &systemVid_encDecIvaChMapTbl, sizeof(SystemVideo_Ivahd2ChMap_Tbl), TRUE);

	multich_hddvr_connect_links();			// 设置和连接
	printf("============System init links leave<<<<<<<<<<<<\n");
}

void MultiCh_deleteHdDvr1()
{
	UInt32 i;

	for (i = 0; i < gHdDvrUsecaseContext1.createdLinkCount; i++)
	{
		printf("delete link: %d-0x%x\n", gHdDvrUsecaseContext1.createdLinks[i], gHdDvrUsecaseContext1.createdLinks[i]);
		System_linkDelete (gHdDvrUsecaseContext1.createdLinks[i]);
	}
	gHdDvrUsecaseContext1.createdLinkCount = 0;
}

