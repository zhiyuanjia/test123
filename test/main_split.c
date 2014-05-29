#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decode.h"
#include "encode.h"
#include "split.h"
#include "system.h"
#include "src/common_sdk.h"
#include "src/common/system_def.h"

#include "../src/osd/osd.h"
#include "../src/decode/vdec_vdis.h"
#include "ti_swosd_logo_224x30_rgb.h"

void test_set_osd_content()
{
	int i, j;
	// 一个OSD内容
	unsigned char *osdTestBlock = NULL;
	int size = g_dev.osd_win_max_width * g_dev.osd_win_max_height * 3;
	osdTestBlock = malloc(size);
	printf("osd width: %d, height: %d, size: %d\n", g_dev.osd_win_max_width, g_dev.osd_win_max_height, size);
	unsigned char *p = osdTestBlock;

#if 0
	for(i = 0; i < g_dev.osd_win_max_width * g_dev.osd_win_max_height; i++) {
		int pos_x = i % g_dev.osd_win_max_width;
		if(pos_x >= 0 && pos_x < g_dev.osd_win_max_width / 3) {
			// red
			p[3 * i + 0] = 0xff;
			p[3 * i + 1] = 0x00;
			p[3 * i + 2] = 0x00;
		} else if(pos_x >= g_dev.osd_win_max_width / 3 && pos_x < g_dev.osd_win_max_width * 2 / 3) {
			// green
			p[3 * i + 0] = 0x00;
			p[3 * i + 1] = 0xff;
			p[3 * i + 2] = 0x00;
		} else {
			// blue
			p[3 * i + 0] = 0x00;
			p[3 * i + 1] = 0x00;
			p[3 * i + 2] = 0xff;
		}
	}
#else
	printf("copy gMCFW_swosdTiLogoRgb_224x30_rgb size: %d\n", sizeof(gMCFW_swosdTiLogoRgb_224x30_rgb));
	memcpy(osdTestBlock, gMCFW_swosdTiLogoRgb_224x30_rgb, sizeof(gMCFW_swosdTiLogoRgb_224x30_rgb));
#endif

	ENCODE_TITLE_PARAM param;
	param.raster = osdTestBlock;
	param.size = 224 * 30 * 3;
	param.x = 50;
	param.y = 50;
	param.width = 224;
	param.height = 30;
	param.alpha = 0x80 / 2;
	param.enable = 1;
	param.colorkey = 0xffffff;

	int TOTAL_VENC_CHANNEL = (g_dev.vi_channel_num + g_dev.swms_encode_num) * g_dev.stream_type;
	for(i = 0; i < TOTAL_VENC_CHANNEL; i++) {
		for(j = 0; j < g_dev.osd_win_max_window; j++) {
		//for(j = 0; j < 1; j++) {
			printf("EncodeSetTitle(); i: %d, j: %d\n", i, j);
			if(0 == j)
				param.enable = 1;
			else
				param.enable = 0;
			EncodeSetTitle(i, 0, j, &param);
		}
	}
}

void *test_set_split_1_thread(void *arg)
{
	VIDEO_RECT rect[SWMS_MAX_WIN_PER_LINK];

	// 1个大窗口
	rect[0].channelNum = 0;
	rect[0].startX = 0;
	rect[0].startY = 0;
	rect[0].width = 1920 * 4 / 4 + 0;
	rect[0].height = 1080 * 4 / 4 + 0;

	SplitSetCompositorMosaic(0, 1, rect);
	SplitSetCompositorMosaic(1, 1, rect);
	sleep(9999999);
	return NULL;
}

void *test_system_debug_thread(void *arg)
{
	while(1) {
#if 1
		sleep(200);
		printf("\n\n\n\n\n\n\n\n");
		SystemOutputVpssVicpInfo(0xffffffff);
		printf("\n\n\n\n\n\n\n\n");
#endif
#if 0
		sleep(10);
		printf("\n\n\n\n\n\n\n\n");
		SystemDebug2(0xffffffff);
		printf("\n\n\n\n\n\n\n\n");
#endif
		sleep(10);
	}
}

void *test_set_split_all_thread2(void *arg)
{
	return NULL;
	VIDEO_RECT rect[SWMS_MAX_WIN_PER_LINK];
	__attribute__((unused)) int i;
	__attribute__((unused)) int j;

	int ETAH_SCREEN_WIDTH;
	int ETAH_SCREEN_HEIGHT;

	while(1) {
		// ------------------------------------------------
		printf("\n\n\n\n\n\n\n\n");
		// 拼接1，显示分辨率1080p
		// 左上1个大窗口
		rect[0].channelNum = 0;
		rect[0].startX = 0;
		rect[0].startY = 0;
		rect[0].width = 1920 * 3 / 4;
		rect[0].height = 1080 * 3 / 4;

		// 右边3个小窗口
		for(i = 1; i < 4; i++) {
			rect[i].channelNum = i;
			rect[i].startX = 1920 * 3 / 4;
			rect[i].startY = 1080 * (i - 1) / 4;
			rect[i].width = 1920 / 4;
			rect[i].height = 1080 / 4;
		}

		// 下边4个小窗口
		for(i = 4; i < 8; i++) {
			rect[i].channelNum = i;
			rect[i].startX = 1920 * (i - 4) / 4;
			rect[i].startY = 1080 * 3 / 4;
			rect[i].width = 1920 / 4;
			rect[i].height = 1080 / 4;
		}

		SplitSetCompositorMosaic(0, 8, rect);
		sleep(5);

		// ------------------------------------------------
		printf("\n\n\n\n\n\n\n\n");
		// 拼接1，显示分辨率1080p
		int ETAH_SCREEN_WIDTH	= 1920;
		int ETAH_SCREEN_HEIGHT	= 1080;

		for(i = 0; i < 3; i++) {
			for(j = 0; j < 3; j++) {
				if((i * 3 + j) >= 8)
					continue;

				rect[i * 3 + j].channelNum	= (i * 3 + j);		// 第几个通道和对应通道的位置
				rect[i * 3 + j].startX		= ETAH_SCREEN_WIDTH   / 3 * j;
				rect[i * 3 + j].startY		= ETAH_SCREEN_HEIGHT  / 3 * i;
				rect[i * 3 + j].width		= ETAH_SCREEN_WIDTH   / 3;
				rect[i * 3 + j].height		= ETAH_SCREEN_HEIGHT  / 3;
			}
		}
		SplitSetCompositorMosaic(0, 8, rect);
		sleep(5);
	}
}

void *test_set_split_all_thread(void *arg)
{
	VIDEO_RECT rect[SWMS_MAX_WIN_PER_LINK];
	__attribute__((unused)) int i;
	__attribute__((unused)) int j;
#if 0
	int ETAH_SCREEN_WIDTH	= 1280 - 8;
	int ETAH_SCREEN_HEIGHT	= 720;
#else
	int ETAH_SCREEN_WIDTH	= 100;			// 百分比
	int ETAH_SCREEN_HEIGHT	= 100;			// 百分比
#endif

	while(1) {
#if 0
		rect[0].channelNum	= 0;		// 第几个通道和对应通道的位置
		rect[0].startX		= 0;
		rect[0].startY		= 0;
		rect[0].width		= ETAH_SCREEN_WIDTH;
		rect[0].height		= ETAH_SCREEN_HEIGHT;

		SplitSetCompositorMosaic(0, 1, rect);
		SplitSetCompositorMosaic(1, 1, rect);
		usleep(1000 * 3000);
		sleep(9999);
#endif
#if 1
		for(i = 0; i < 3; i++) {
			for(j = 0; j < 3; j++) {
				if((i * 3 + j) >= 8)
					continue;

				rect[i * 3 + j].channelNum	= (i * 3 + j);		// 第几个通道和对应通道的位置
				rect[i * 3 + j].startX		= ETAH_SCREEN_WIDTH   / 3 * j;
				rect[i * 3 + j].startY		= ETAH_SCREEN_HEIGHT  / 3 * i;
				rect[i * 3 + j].width		= ETAH_SCREEN_WIDTH   / 3;
				rect[i * 3 + j].height		= ETAH_SCREEN_HEIGHT  / 3;
			}
		}
		/*
		rect[0].channelNum = 0;
		rect[1].channelNum = 4;
		rect[2].channelNum = 1;
		rect[3].channelNum = 5;
		rect[4].channelNum = 2;
		rect[5].channelNum = 6;
		rect[6].channelNum = 3;
		rect[7].channelNum = 7;
		*/
		//rect[4].channelNum = -2;
		//rect[7].channelNum = 2;

		SplitSetCompositorMosaic(0, 8, rect);
		SplitSetCompositorMosaic(1, 8, rect);
		SplitSetCompositorMosaic(2, 8, rect);
		SplitSetDisplayResolution(0, ETAH_DISPLAY_STD_1080P_60);
		SplitSetDisplayResolution(1, ETAH_DISPLAY_STD_1080P_60);
		SplitSetDisplayResolution(2, 0);
		//SplitSetCompositorMosaic(1, 8, rect);
		usleep(1000 * 3000);
		sleep(999999);
#endif
#if 0
		for(i = 0; i < 4; i++) {
			for(j = 0; j < 4; j++) {
				if((i * 4 + j) >= 8)
					continue;

				rect[i * 4 + j].channelNum	= (i * 4 + j);		// 第几个通道和对应通道的位置
				rect[i * 4 + j].startX		= ETAH_SCREEN_WIDTH   / 4 * j;
				rect[i * 4 + j].startY		= ETAH_SCREEN_HEIGHT  / 4 * i;
				rect[i * 4 + j].width		= ETAH_SCREEN_WIDTH   / 4;
				rect[i * 4 + j].height		= ETAH_SCREEN_HEIGHT  / 4;
			}
		}
		/*
		rect[0].channelNum = 0;
		rect[1].channelNum = 4;
		rect[2].channelNum = 1;
		rect[3].channelNum = 5;
		rect[4].channelNum = 2;
		rect[5].channelNum = 6;
		rect[6].channelNum = 3;
		rect[7].channelNum = 7;
		*/

		SplitSetCompositor1(8, rect, VSYS_STD_1080P_60);
		SplitSetCompositor2(8, rect, VSYS_STD_1080P_60);
		usleep(1000 * 3000);
		sleep(9999);
#endif
#if 0
		while(1) {
			CaptureSetVideoPortStandard(0, 1920, 1080);
			sleep(5);
			CaptureSetVideoPortStandard(0, 1280, 720);
			sleep(5);
		}
		sleep(9999);
#endif
#if 1
		// 左上1个大窗口
		rect[0].channelNum = 0;
		rect[0].startX = 0;
		rect[0].startY = 0;
		rect[0].width = 1920 * 3 / 4;
		rect[0].height = 1080 * 3 / 4;

		// 右边3个小窗口
		for(i = 1; i < 4; i++) {
			rect[i].channelNum = i;
			rect[i].startX = 1920 * 3 / 4;
			rect[i].startY = 1080 * (i - 1) / 4;
			rect[i].width = 1920 / 4;
			rect[i].height = 1080 / 4;
		}

		// 下边4个小窗口
		for(i = 4; i < 8; i++) {
			rect[i].channelNum = i;
			rect[i].startX = 1920 * (i - 4) / 4;
			rect[i].startY = 1080 * 3 / 4;
			rect[i].width = 1920 / 4;
			rect[i].height = 1080 / 4;
		}

		SplitSetCompositorMosaic(0, 8, rect);
		SplitSetCompositorMosaic(1, 8, rect);
		usleep(1000 * 3000);
		sleep(9999);
#endif

#if 1
		// 左上1个大窗口
		rect[0].channelNum = 4;
		rect[0].startX = 0;
		rect[0].startY = 0;
		rect[0].width = 1920 * 4 / 4 + 0;
		rect[0].height = 1080 * 4 / 4 + 0;

		// 右边1个小窗口
		rect[1].channelNum = 0;
		rect[1].startX = 1920 * 3 / 4;
		rect[1].startY = 1080 * 0 / 4;
		rect[1].width = 1920 / 4;
		rect[1].height = 1080 / 4;

		SplitSetCompositorMosaic(0, 2, rect);
		SplitSetCompositorMosaic(1, 2, rect);
		usleep(1000 * 3000);
#endif

#if 1
		// 右边1个小窗口
		rect[0].channelNum = 0;
		rect[0].startX = 1920 * 3 / 4;
		rect[0].startY = 1080 * 0 / 4;
		rect[0].width = 1920 / 4;
		rect[0].height = 1080 / 4;

		// 左上1个大窗口
		rect[1].channelNum = 4;
		rect[1].startX = 0;
		rect[1].startY = 0;
		rect[1].width = 1920 * 4 / 4 + 0;
		rect[1].height = 1080 * 4 / 4 + 0;

		SplitSetCompositorMosaic(0, 2, rect);
		SplitSetCompositorMosaic(1, 2, rect);
		usleep(1000 * 3000);
#endif

#if 1
		// 左上1个大窗口
		rect[0].channelNum = 0;
		rect[0].startX = 0;
		rect[0].startY = 0;
		rect[0].width = 1920 * 4 / 4 + 0;
		rect[0].height = 1080 * 4 / 4 + 0;

		// 右边1个小窗口
		rect[1].channelNum = 4;
		rect[1].startX = 1920 * 3 / 4;
		rect[1].startY = 1080 * 0 / 4;
		rect[1].width = 1920 / 4;
		rect[1].height = 1080 / 4;

		SplitSetCompositorMosaic(0, 2, rect);
		SplitSetCompositorMosaic(1, 2, rect);
		usleep(1000 * 3000);
#endif

#if 1
		// 右边1个小窗口
		rect[0].channelNum = 4;
		rect[0].startX = 1920 * 3 / 4;
		rect[0].startY = 1080 * 0 / 4;
		rect[0].width = 1920 / 4;
		rect[0].height = 1080 / 4;

		// 左上1个大窗口
		rect[1].channelNum = 0;
		rect[1].startX = 0;
		rect[1].startY = 0;
		rect[1].width = 1920 * 4 / 4 + 0;
		rect[1].height = 1080 * 4 / 4 + 0;

		SplitSetCompositorMosaic(0, 2, rect);
		SplitSetCompositorMosaic(1, 2, rect);
		usleep(1000 * 3000);
#endif

#if 1
		// 1个大窗口
		rect[0].channelNum = 4;
		rect[0].startX = 0;
		rect[0].startY = 0;
		rect[0].width = 1920 * 4 / 4 + 0;
		rect[0].height = 1080 * 4 / 4 + 0;

		SplitSetCompositorMosaic(0, 1, rect);
		SplitSetCompositorMosaic(1, 1, rect);
		usleep(1000 * 3000);
#endif

#if 1
		// 1个大窗口
		rect[0].channelNum = 0;
		rect[0].startX = 0;
		rect[0].startY = 0;
		rect[0].width = 1920 * 4 / 4 + 0;
		rect[0].height = 1080 * 4 / 4 + 0;

		SplitSetCompositorMosaic(0, 1, rect);
		SplitSetCompositorMosaic(1, 1, rect);
		usleep(1000 * 3000);
#endif
	}
}


int debugcnt = 0;
int My_EncodeGetBuffer_Callback2(VIDEO_STREAM_PARAM *buf)
{
	//printf("My_EncodeGetBuffer_Callback2()============buf->chnId: %d, buf->stream_type: %d, size: %d\n", buf->chnId, buf->stream_type, buf->filledBufSize);
#if 0
	debugcnt++;
	static int all_channel_statics[256][2]	= {0 };
	all_channel_statics[buf->chnId][buf->stream_type]++;
	if(6 * 30 == debugcnt) {
		printf("---------write statistics: ");
		int i;
		for(i = 0; i < 8; i++) {
			if(all_channel_statics[i][0] > 0 || all_channel_statics[i][1] > 0) {
				printf("channel-%d: %d/%d, ", i, all_channel_statics[i][0], all_channel_statics[i][1]);
			}
		}
		printf("----------------\n");
		debugcnt = 0;
	}
#endif

#if 0
	if(0 == buf->chnId && 0 == buf->stream_type) {
		// 不要打开下面的，要不buffer会被a8 hold住，导致m3video取不到buffer
		static FILE *all_channel_fp[256][2]	= {0 };
		if(NULL == all_channel_fp[buf->chnId][buf->stream_type]) {
			char filename1[256] = "";
			sprintf(filename1, "/video_fp-%02d-%02d.h264", buf->chnId, buf->stream_type);
			all_channel_fp[buf->chnId][buf->stream_type] = fopen(filename1, "wb");
		}

		fwrite(buf->bufVirtAddr, 1, buf->filledBufSize, all_channel_fp[buf->chnId][buf->stream_type]);
		fflush(all_channel_fp[buf->chnId][buf->stream_type]);
	}
#endif
	return 0;
}

#include "bsreader.h"
#include "bswriter.h"
#include "rtmp/Struct.h"
#define	MAX_INPUT_CHANNEL_NR		32
BITSTREAMWRITER			*RTMPBitstreamWriter[MAX_INPUT_CHANNEL_NR] = {0 };
BITSTREAMWRITER_HANDLE		bhs[MAX_INPUT_CHANNEL_NR] = {0 };
BITSTREAMWRITER *GetRTMPBitWriter();

//int My_EncodeGetBuffer_Callback(VCODEC_BITSBUF_S *buf)
int My_EncodeGetBuffer_Callback(VIDEO_STREAM_PARAM *buf)
{
	// buf->chnId
	// buf->bufVirtAddr
	// buf->filledBufSize
#if 0
	if(NULL == RTMPBitstreamWriter[buf->chnId]) {
		RTMPBitstreamWriter[buf->chnId] = GetRTMPBitWriter();
		if(NULL == RTMPBitstreamWriter[buf->chnId])
			printf("RTMPBitstreamWriter get failed, channel: %d.\n", buf->chnId);

		if(NULL == (bhs[buf->chnId] = RTMPBitstreamWriter[buf->chnId]->create()))
			printf("bhs create failed, channel: %d.\n", buf->chnId);

		char rtmp_server[256] = "";
		sprintf(rtmp_server, "url=rtmp://%s/teach_app/s%d,width=%d,height=%d,framerate=30"
			, "2.1.1.5"
			, buf->chnId
			, 1920
			, 1080
			);
		printf("rtmp info: %s\n", rtmp_server);

		if(0 == RTMPBitstreamWriter[buf->chnId]->open(bhs[buf->chnId], rtmp_server))
			printf("RTMPBitstreamWriter open failed, channel: %d.\n", buf->chnId);
	}

	if(NULL != RTMPBitstreamWriter[buf->chnId]) {
		RTMPBitstreamWriter[buf->chnId]->newBlock(bhs[buf->chnId], buf->bufVirtAddr, buf->filledBufSize);
	}
#endif
	return 0;
}

Int32 Vsys_printDetailedStatistics();
Int32 Vsys_printBufferStatistics();

static int decode_flag = 0;
void *test_write_decode_buffer_thread(void *arg)
{
	int channel = (int)arg;
	int total = 0;
	int i;

	char fp_stream_filename[64] = "";
	char fp_index_filename[64] = "";

	sprintf(fp_stream_filename, "./ti-%02d.h264",		0);//channel);
	sprintf(fp_index_filename,  "./ti-%02d.h264.hdr",	0);//channel);

	FILE *fp_stream = fopen(fp_stream_filename, "r");
	FILE *fp_index  = fopen(fp_index_filename,  "r");

	char *line = NULL;
	ssize_t read;
	size_t len;
	char *buf2 = (char *)malloc(1024 * 1024);

	printf("******************************test_write_decode_buffer_thread(%s,%s)*************************************\n", fp_stream_filename, fp_index_filename);
	//sleep(10);
	printf("now starting decode.\n");
	//sleep(3);
	if(NULL == fp_stream || NULL == fp_index) {
		printf("open file failed.\n");
		return NULL;
	} else {
		printf("open file succ.\n");
	}
	/*
	i = 5;
	while(i-- > 0) {
		printf("prepare for decode: %d\n", i);
		sleep(1);
	}
	*/

	printf("decode channel: %d, after 0 second start\n", channel);
	printf("start decode.\n");
	i = 1000;
	//while(i-- > 0) {
	while(decode_flag) {
		printf("getline.\n");
		if(-1 == (read = getline(&line, &len, fp_index))) {
			printf("WARNING: end of file? goto start.\n");
			fseek(fp_stream, 0, SEEK_SET);
			fseek(fp_index,  0, SEEK_SET);
			continue;
		}

		int len2 = atoi(line);
		//printf("get line succ, pack len: %d\n", len2);
		len2 = fread(buf2, 1, len2, fp_stream);
		total++;
		//printf("******channel: %d[%d],  read size: %d\n", channel, total, len2);

		DecodePutBuffer(channel, buf2, len2);
		usleep(1000 * 33);
		//getchar();

		/*
		if(300 == total) {
			printf("====================Vsys_printDetailedStatistics=======================\n");
			Vsys_printDetailedStatistics();
			printf("====================Vsys_printBufferStatistics=========================\n");
			Vsys_printBufferStatistics();
		}
		*/
	}
	printf("decode thread exit, channel: %d\n", channel);
}

Int32 Vcap_setWrbkCaptScParamsDefault(UInt32 inW, UInt32 inH, UInt32 outW, UInt32 outH)
{
    int status = 0;
    CaptureLink_ScParams scPrms;

    scPrms.queId = 1;
    scPrms.chId = 0;
    scPrms.scEnable = TRUE;
    scPrms.inWidth = inW;
    scPrms.inHeight = inH;
    scPrms.outWidth = outW;
    scPrms.outHeight = outH;

    if ((gVcapModuleContext.captureId != SYSTEM_LINK_ID_INVALID) &&
        (TRUE == gVcapModuleContext.isWrbkCaptEnable))
    {
        status = System_linkControl(
                    gVcapModuleContext.captureId,
                    CAPTURE_LINK_CMD_SET_SC_PARAMS,
                    &scPrms,
                    sizeof(scPrms),
                    TRUE);
    }

    return (status);
}

Int32 Vsys_printDetailedStatistics2()
{
    UInt32 devId;

    dpline();
    if(gVcapModuleContext.captureId!=SYSTEM_LINK_ID_INVALID)
    {
        System_linkControl(gVcapModuleContext.captureId,
                            CAPTURE_LINK_CMD_PRINT_ADV_STATISTICS,
                            NULL, 0, TRUE);
    }

    dpline();
    for(devId=0; devId<MAX_NSF_LINK; devId++)
    {
        if(gVcapModuleContext.nsfId[devId]!=SYSTEM_LINK_ID_INVALID)
        {
             System_linkControl(gVcapModuleContext.nsfId[devId],
                                NSF_LINK_CMD_PRINT_STATISTICS,
                                NULL, 0, TRUE);
        }
    }

    dpline();
    for(devId=0; devId<MAX_DEI_LINK; devId++)
    {
        if(gVcapModuleContext.deiId[devId]!=SYSTEM_LINK_ID_INVALID)
        {
            System_linkControl(gVcapModuleContext.deiId[devId],
                            DEI_LINK_CMD_PRINT_STATISTICS,
                            NULL, 0, TRUE);
        }
    }

    dpline();
    for(devId=0; devId<MAX_SCLR_LINK; devId++)
    {
        if(gVcapModuleContext.sclrId[devId]!=SYSTEM_LINK_ID_INVALID)
        {
             System_linkControl(gVcapModuleContext.sclrId[devId],
                                SCLR_LINK_CMD_PRINT_STATISTICS,
                                NULL, 0, TRUE);
        }
    }

    dpline();
    if(gVdisModuleContext.mpSclrId != SYSTEM_LINK_ID_INVALID)
    {
        System_linkControl(SYSTEM_LINK_ID_MP_SCLR_INST_0,
                                MP_SCLR_LINK_CMD_PRINT_STATISTICS,
                                NULL, 0, TRUE);
    }

    dpline();
    if (gVcapModuleContext.capSwMsId != SYSTEM_LINK_ID_INVALID)
    {
        System_linkControl(gVcapModuleContext.capSwMsId,
                        SYSTEM_SW_MS_LINK_CMD_PRINT_STATISTICS,
                        NULL, 0, TRUE);
    }

    dpline();
    for(devId=0; devId<VDIS_DEV_MAX; devId++)
    {
        if(gVdisModuleContext.swMsId[devId]!=SYSTEM_LINK_ID_INVALID)
        {
            System_linkControl(gVdisModuleContext.swMsId[devId],
                            SYSTEM_SW_MS_LINK_CMD_PRINT_STATISTICS,
                            NULL, 0, TRUE);
        }
    }

    dpline();
    for(devId=0; devId<VDIS_DEV_MAX; devId++)
    {
        if(gVdisModuleContext.displayId[devId]!=SYSTEM_LINK_ID_INVALID)
        {
            System_linkControl(gVdisModuleContext.displayId[devId],
                            DISPLAY_LINK_CMD_PRINT_STATISTICS,
                            NULL, 0, TRUE);
        }
    }

    dpline();
    for(devId=0; devId<MAX_IPC_FRAMES_LINK; devId++)
    {
        if(gVcapModuleContext.ipcFramesOutVpssId[devId]!=SYSTEM_LINK_ID_INVALID)
        {
            System_linkControl(gVcapModuleContext.ipcFramesOutVpssId[devId],
                            IPCFRAMESOUTRTOS_LINK_CMD_PRINT_STATISTICS,
                            NULL, 0, TRUE);
        }
    }

    dpline();
    OSA_waitMsecs(4000); // allow for print to complete

    for(devId=0; devId<MAX_ALG_LINK; devId++)
    {
        if(gVcapModuleContext.dspAlgId[devId]!=SYSTEM_LINK_ID_INVALID)
        {
            System_linkControl(gVcapModuleContext.dspAlgId[devId],
                            ALG_LINK_SCD_CMD_PRINT_STATISTICS,
                            NULL, 0, TRUE);
        }
    }

    dpline();
    OSA_waitMsecs(2000); // allow for print to complete

    if(gVencModuleContext.encId != SYSTEM_LINK_ID_INVALID)
    {
        System_linkControl(gVencModuleContext.encId,
                            ENC_LINK_CMD_PRINT_IVAHD_STATISTICS,
                            NULL, 0, TRUE);
    }
    else if (gVdecModuleContext.decId !=SYSTEM_LINK_ID_INVALID)
    {
        System_linkControl(gVdecModuleContext.decId,
                            DEC_LINK_CMD_PRINT_IVAHD_STATISTICS,
                            NULL, 0, TRUE);
     }

    dpline();
    if(gVencModuleContext.encId != SYSTEM_LINK_ID_INVALID)
    {
        System_linkControl(gVencModuleContext.encId,
                            ENC_LINK_CMD_PRINT_STATISTICS,
                            NULL, 0, TRUE);
    }
    dpline();
    if(gVdecModuleContext.decId != SYSTEM_LINK_ID_INVALID)
    {
        System_linkControl(gVdecModuleContext.decId,
                            DEC_LINK_CMD_PRINT_STATISTICS,
                            NULL, 0, TRUE);
    }

    dpline();
    OSA_waitMsecs(3000); // allow for print to complete

    MultiCh_prfLoadPrint(TRUE,TRUE);
    dpline();
    return ERROR_NONE;
}


extern VdecVdis_Config      gVdecVdis_config;
extern void SwmsLayoutInit();
void MultiCh_createHdDvr1();
extern void *test_audio_out_thread(void *arg);
extern void *test_audio_volume_thread(void *arg);
extern void switch_display(int id);
int main_encode_decode()
{
	__attribute__((unused)) int i;
	__attribute__((unused)) int j;
	pthread_t tid = 0;

#if 1
	//test_set_osd_content();		// osd usercase
	//EncodeSetGetBufferCallback(My_EncodeGetBuffer_Callback);		// rtmp
	EncodeSetGetBufferCallback(My_EncodeGetBuffer_Callback2);		// 存文件
	ENCODE_FORMAT fmt;
	fmt.BitRateControl	= VENC_RATE_CTRL_VBR_;
	fmt.GOP			= 30;
	fmt.FrameRate		= 20;
	fmt.BitRate		= 3000;
	fmt.algType		= ETAH_VENC_CHN_BASELINE_PROFILE;
	fmt.width		= 1920;
	fmt.height		= 1080;

	printf("======g_dev.vi_channel_num: %d, g_dev.swms_encode_num: %d\n", g_dev.vi_channel_num, g_dev.swms_encode_num);
	for(i = 0; i < (g_dev.vi_channel_num + g_dev.swms_encode_num); i++) {
	//for(i = 0; i < 2; i++) {
		printf("encode channel start: %d\n", i);
		EncodeInit(i, STREAM_TYPE_PRIMARY);
		EncodeInit(i, STREAM_TYPE_SECOND);
#if 0
		fmt.width		= 1280;
		fmt.height		= 720;
		EncodeSetFormat(i, STREAM_TYPE_PRIMARY, &fmt);
		fmt.width		= 704;
		fmt.height		= 480;
		EncodeSetFormat(i, STREAM_TYPE_SECOND, &fmt);
#endif
		EncodeStart(i, STREAM_TYPE_PRIMARY);
		EncodeStart(i, STREAM_TYPE_SECOND);
		printf("encode channel leave: %d\n", i);
	}
#endif

#if 0
	// 变化拼接窗口
	SplitInit();
	//sleep(3);
	//pthread_create(&tid, NULL, test_system_debug_thread, NULL);
	DEVICE_CAPABILITY cap;
	SystemGetProductCapability(&cap);
	printf("\n\n\n\n\n\n");
	printf( "\t\tvi_channel_num: %d, decode_channel_num: %d, swms_encode_num: %d, stream_type: %d, \n"
		"\t\tai_channel_num: %d, ao_channel_num: %d, decode_max_width: %d, decode_max_height: %d, \n"
		"\t\tosd_win_max_width: %d, osd_win_max_height: %d\n"
		, cap.vi_channel_num
		, cap.decode_channel_num
		, cap.swms_encode_num
		, cap.stream_type

		, cap.ai_channel_num
		, cap.ao_channel_num
		, cap.decode_max_width
		, cap.decode_max_height

		, cap.osd_win_max_width
		, cap.osd_win_max_height
	);
#endif

#if 0
	sleep(5);
	printf("ao demo entry.\n");
	AudioOutInit(0);
	dpline();

	pthread_create(&tid, NULL, test_audio_out_thread, NULL);
#endif


	VIDEO_RECT rect[SWMS_MAX_WIN_PER_LINK];
	int ETAH_SCREEN_WIDTH;
	int ETAH_SCREEN_HEIGHT;
	//sleep(5);
	printf("\n\n\n\n\n\nwaiting for debug......\n");
	while(1) {
		//sleep(999999);
		int input = getchar();

		if('o' == input) {
			switch_display(0);
		}

		if('p' == input) {
			switch_display(1);
		}

		if('a' == input) {
			// 1个大窗口
			rect[0].channelNum = 0;
			rect[0].startX = 0;
			rect[0].startY = 0;
			rect[0].width =  50;
			rect[0].height = 50;
			rect[1].channelNum = 1;
			rect[1].startX = 50;
			rect[1].startY = 50;
			rect[1].width =  50;
			rect[1].height = 50;
			SplitSetCompositorMosaic(ETAH_SWMS_ID_HDMI,   2, rect);

			/*
			for(i = 0; i < 2; i++) {
				for(j = 0; j < 2; j++) {
					if((i * 2 + j) >= 4)
						continue;

					rect[i * 2 + j].channelNum	= (i *  2 + j);		// 第几个通道和对应通道的位置
					rect[i * 2 + j].startX		= 100 / 2 * j;
					rect[i * 2 + j].startY		= 100 / 2 * i;
					rect[i * 2 + j].width		= 100 / 2;
					rect[i * 2 + j].height		= 100 / 2;
				}
			}
			SplitSetCompositorMosaic(ETAH_SWMS_ID_HDMI,   4, rect);
			*/

			//SplitSetCompositorMosaic(ETAH_SWMS_ID_HDMI, 1, rect);
			SplitSetDisplayResolution(ETAH_SWMS_ID_HDMI,   ETAH_DISPLAY_STD_1080P_60);
		}

		if('b' == input) {
#if 1
			rect[0].channelNum = 0;
			rect[0].startX = 0;
			rect[0].startY = 0;
			rect[0].width =  50;
			rect[0].height = 50;
			rect[1].channelNum = 1;
			rect[1].startX = 50;
			rect[1].startY = 50;
			rect[1].width =  50;
			rect[1].height = 50;
			SplitSetCompositorMosaic(ETAH_SWMS_ID_MOSAIC,   2, rect);
#else
			for(i = 0; i < 2; i++) {
				for(j = 0; j < 2; j++) {
					if((i * 2 + j) >= 4)
						continue;

					rect[i * 2 + j].channelNum	= (i *  2 + j);		// 第几个通道和对应通道的位置
					rect[i * 2 + j].startX		= 100 / 2 * j;
					rect[i * 2 + j].startY		= 100 / 2 * i;
					rect[i * 2 + j].width		= 100 / 2;
					rect[i * 2 + j].height		= 100 / 2;
				}
			}
			SplitSetCompositorMosaic(ETAH_SWMS_ID_MOSAIC,   4, rect);
#endif
			//SplitSetCompositorMosaic(ETAH_SWMS_ID_HDMI, 1, rect);
			//SplitSetCompositorMosaic(ETAH_SWMS_ID_MOSAIC,   4, rect);
			/*
			// 1个大窗口
			rect[0].channelNum = 0;
			rect[0].startX = 0;
			rect[0].startY = 0;
			rect[0].width =  100;
			rect[0].height = 100;
			rect[1].channelNum = 1;
			rect[1].startX = 50;
			rect[1].startY = 50;
			rect[1].width =  50;
			rect[1].height = 50;

			//SplitSetCompositorMosaic(ETAH_SWMS_ID_HDMI, 1, rect);
			SplitSetCompositorMosaic(ETAH_SWMS_ID_MOSAIC,   1, rect);
			*/
		}

		if('1' == input) {
			// 1个大窗口
			rect[0].channelNum = 0;
			rect[0].startX = 0;
			rect[0].startY = 0;
			rect[0].width =  100;
			rect[0].height = 100;

			SplitSetCompositorMosaic(ETAH_SWMS_ID_HDMI, 1, rect);
			//SplitSetCompositorMosaic(ETAH_SWMS_ID_SD,   1, rect);
			SplitSetDisplayResolution(ETAH_SWMS_ID_HDMI, ETAH_DISPLAY_STD_1080P_60);
			//SplitSetDisplayResolution(ETAH_SWMS_ID_SD,   0);
		}

		if('2' == input) {
			// 1个大窗口
			rect[0].channelNum = 1;
			rect[0].startX = 0;
			rect[0].startY = 0;
			rect[0].width =  100;
			rect[0].height = 100;

			SplitSetCompositorMosaic(0, 1, rect);
			SplitSetCompositorMosaic(1, 1, rect);
			SplitSetCompositorMosaic(2, 1, rect);
			SplitSetDisplayResolution(0, ETAH_DISPLAY_STD_1080P_60);
			SplitSetDisplayResolution(1, ETAH_DISPLAY_STD_1080P_60);
			SplitSetDisplayResolution(2, 0);
		}

		if('3' == input) {
			// 1个大窗口
			rect[0].channelNum = 2;
			rect[0].startX = 0;
			rect[0].startY = 0;
			rect[0].width =  100;
			rect[0].height = 100;

			SplitSetCompositorMosaic(0, 1, rect);
			SplitSetCompositorMosaic(1, 1, rect);
			SplitSetCompositorMosaic(2, 1, rect);
			SplitSetDisplayResolution(0, ETAH_DISPLAY_STD_1080P_60);
			SplitSetDisplayResolution(1, ETAH_DISPLAY_STD_1080P_60);
			SplitSetDisplayResolution(2, 0);
		}

		if('4' == input) {
			// 1个大窗口
			rect[0].channelNum = 0;
			rect[0].startX = 0;
			rect[0].startY = 0;
			rect[0].width =  100;
			rect[0].height = 100;

			SplitSetCompositorMosaic(0, 1, rect);
			//SplitSetCompositorMosaic(1, 1, rect);
			//SplitSetCompositorMosaic(2, 1, rect);
			//SplitSetDisplayResolution(0, ETAH_DISPLAY_STD_1080P_60);
			//SplitSetDisplayResolution(1, ETAH_DISPLAY_STD_1080P_60);
			//SplitSetDisplayResolution(2, 0);
		}

		if('5' == input) {
			printf("change to 3x3 window.\n");
			for(i = 0; i < 3; i++) {
				for(j = 0; j < 3; j++) {
					if((i * 3 + j) >= 9)
						continue;

					rect[i * 3 + j].channelNum	= (i * 3 + j);		// 第几个通道和对应通道的位置
					rect[i * 3 + j].startX		= 100   / 3 * j;
					rect[i * 3 + j].startY		= 100  / 3 * i;
					rect[i * 3 + j].width		= 100   / 3;
					rect[i * 3 + j].height		= 100  / 3;
				}
			}

			SplitSetCompositorMosaic(0, 8, rect);
			//SplitSetCompositorMosaic(1, 8, rect);
			//SplitSetCompositorMosaic(2, 8, rect);
			SplitSetDisplayResolution(0, ETAH_DISPLAY_STD_1080P_60);
			//SplitSetDisplayResolution(1, ETAH_DISPLAY_STD_1080P_60);
			//SplitSetDisplayResolution(2, 0);
		}

		if('6' == input) {
			printf("change to 2x2 window.\n");
			for(i = 0; i < 2; i++) {
				for(j = 0; j < 2; j++) {
					if((i * 2 + j) >= 4)
						continue;

					rect[i * 2 + j].channelNum	= (i * 2 + j);		// 第几个通道和对应通道的位置
					rect[i * 2 + j].startX		= 100   / 2 * j;
					rect[i * 2 + j].startY		= 100  / 2 * i;
					rect[i * 2 + j].width		= 100   / 2;
					rect[i * 2 + j].height		= 100  / 2;
				}
			}
			rect[1].startX -= 2;

			SplitSetCompositorMosaic(0, 4, rect);
			//SplitSetCompositorMosaic(1, 4, rect);
			//SplitSetCompositorMosaic(2, 4, rect);
			SplitSetDisplayResolution(0, ETAH_DISPLAY_STD_1080P_60);
			//SplitSetDisplayResolution(1, ETAH_DISPLAY_STD_1080P_60);
			//SplitSetDisplayResolution(2, 0);
		}

		if('7' == input) {
			printf("change to 2x2 window.\n");
			for(i = 0; i < 2; i++) {
				for(j = 0; j < 2; j++) {
					if((i * 2 + j) >= 4)
						continue;

					rect[i * 2 + j].coordinate_mode	= COORDINATE_MODE_PIXEL;
					rect[i * 2 + j].channelNum	= (i * 2 + j);		// 第几个通道和对应通道的位置
					rect[i * 2 + j].startX		= 1920  / 2 * j;
					rect[i * 2 + j].startY		= 1080  / 2 * i;
					rect[i * 2 + j].width		= 1920  / 2;
					rect[i * 2 + j].height		= 1080  / 2;
				}
			}
			rect[1].startX -= 38;

			SplitSetCompositorMosaic(0, 4, rect);
			//SplitSetCompositorMosaic(1, 4, rect);
			//SplitSetCompositorMosaic(2, 4, rect);
			SplitSetDisplayResolution(0, ETAH_DISPLAY_STD_1080P_60);
			//SplitSetDisplayResolution(1, ETAH_DISPLAY_STD_1080P_60);
			//SplitSetDisplayResolution(2, 0);
		}

		if('8' == input) {
			printf("change to 2x2 window.\n");
			for(i = 0; i < 2; i++) {
				for(j = 0; j < 2; j++) {
					if((i * 2 + j) >= 4)
						continue;

					rect[i * 2 + j].channelNum	= (i * 2 + j);		// 第几个通道和对应通道的位置
					rect[i * 2 + j].startX		= 100   / 2 * j;
					rect[i * 2 + j].startY		= 100  / 2 * i;
					rect[i * 2 + j].width		= 100   / 2;
					rect[i * 2 + j].height		= 100  / 2;
				}
			}

			SplitSetCompositorMosaic(1, 4, rect);
			//SplitSetCompositorMosaic(1, 4, rect);
			//SplitSetCompositorMosaic(2, 4, rect);
			//SplitSetDisplayResolution(0, ETAH_DISPLAY_STD_1080P_60);
			//SplitSetDisplayResolution(1, ETAH_DISPLAY_STD_1080P_60);
			//SplitSetDisplayResolution(2, 0);
		}

		if('9' == input) {
			// cvbs 1个大窗口
			rect[0].channelNum = 0;
			rect[0].startX = 0;
			rect[0].startY = 0;
			rect[0].width =  100;
			rect[0].height = 100;

			SplitSetCompositorMosaic(1, 1, rect);
			//SplitSetCompositorMosaic(1, 1, rect);
			//SplitSetCompositorMosaic(2, 1, rect);
			//SplitSetDisplayResolution(1, ETAH_DISPLAY_STD_1080P_60);
			//SplitSetDisplayResolution(1, ETAH_DISPLAY_STD_1080P_60);
			//SplitSetDisplayResolution(2, 0);
		}

#if 0
		if('a' == input) {
		}

		if('b' == input) {
		}

		if('c' == input) {
		}
#endif

#if 1
		if('d' == input) {
			//pthread_create(&tid, NULL, test_set_split_all_thread, NULL);
			//for(i = 0; i < g_dev.decode_channel_num; i++) {
			decode_flag = 1;
			for(i = 0; i < 1; i++) {
				DecodeInit(i);
				DecodeStart(i);
				pthread_create(&tid, NULL, test_write_decode_buffer_thread, (void *)i);
			}
		}

		if('f' == input) {
			//pthread_create(&tid, NULL, test_set_split_all_thread, NULL);
			//for(i = 0; i < g_dev.decode_channel_num; i++) {
			decode_flag = 1;
			for(i = 0; i < 2; i++) {
				DecodeInit(i);
				DecodeStart(i);
				pthread_create(&tid, NULL, test_write_decode_buffer_thread, (void *)i);
			}
		}
#endif

		if('q' == input) {
			decode_flag = 0;
		}

		if('i' == input) {
			SYSTEM_DEBUG_INFO info;
			SystemGetDebug(&info);

			printf( "%d/%d, %d/%d, %d/%d, %d/%d, %d/%d\n"
				"%lld/%lld, %lld/%lld, %lld/%lld, %lld/%lld, %lld/%lld\n"
				, info.video_encode_cnt[0][0]
				, info.video_encode_cnt[0][1]
				, info.video_encode_cnt[1][0]
				, info.video_encode_cnt[1][1]
				, info.video_encode_cnt[2][0]
				, info.video_encode_cnt[2][1]
				, info.video_encode_cnt[3][0]
				, info.video_encode_cnt[3][1]
				, info.video_encode_cnt[4][0]
				, info.video_encode_cnt[4][1]
				, info.video_encode_size[0][0]
				, info.video_encode_size[0][1]
				, info.video_encode_size[1][0]
				, info.video_encode_size[1][1]
				, info.video_encode_size[2][0]
				, info.video_encode_size[2][1]
				, info.video_encode_size[3][0]
				, info.video_encode_size[3][1]
				, info.video_encode_size[4][0]
				, info.video_encode_size[4][1]
				);
		}

		if('x' == input) {
			printf("Vsys_printDetailedStatistics2()=======================================\n");
			Vsys_printDetailedStatistics();
			printf("end.=================================================================\n");
		}
		if('y' == input) {
			printf("Vsys_printBufferStatistics()=========================================\n");
			Vsys_printBufferStatistics();
			printf("end.=================================================================\n");
		}
#if 1
		if('k'==input) {
			SystemOutputVpssVicpInfo(0);
		}
#endif
#if 0
		if('e' == input) {
			SystemUninit();
			exit(0);
		}
#endif
	}
	printf("how do you run the f**k code?\n");
}

