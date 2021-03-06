
/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

    Module Name:
    dfs.h

    Abstract:
    Support DFS function.

    Revision History:
    Who       When            What
    --------  ----------      ----------------------------------------------
    Fonchi    03-12-2007      created
*/

#define RADAR_PULSE 1
#define RADAR_WIDTH 2

#define WIDTH_RD_IDLE 0
#define WIDTH_RD_CHECK 1

#ifdef CONFIG_AP_SUPPORT
#ifdef CARRIER_DETECTION_SUPPORT
#define CARRIER_DETECT_HIGH_BOUNDARY_1	100
#define CARRIER_DETECT_HIGH_BOUNDARY_2	110
#define CARRIER_DETECT_LOW_BOUNDARY		40
#endif // CARRIER_DETECTION_SUPPORT //
#endif // CONFIG_AP_SUPPORT //

VOID BbpRadarDetectionStart(
	IN PRTMP_ADAPTER pAd);

VOID BbpRadarDetectionStop(
	IN PRTMP_ADAPTER pAd);

VOID RadarDetectionStart(
	IN PRTMP_ADAPTER pAd,
	IN BOOLEAN CTS_Protect,
	IN UINT8 CTSPeriod);

VOID RadarDetectionStop(
	IN PRTMP_ADAPTER	pAd);

VOID RadarDetectPeriodic(
	IN PRTMP_ADAPTER	pAd);
	
#ifdef CONFIG_AP_SUPPORT
VOID ApRadarDetectPeriodic(
	IN PRTMP_ADAPTER pAd);

VOID RadarSMDetect(
	IN PRTMP_ADAPTER pAd,
	IN BOOLEAN RadarType);
#endif // CONFIG_AP_SUPPORT //

BOOLEAN RadarChannelCheck(
	IN PRTMP_ADAPTER	pAd,
	IN UCHAR			Ch);

ULONG JapRadarType(
	IN PRTMP_ADAPTER pAd);

ULONG RTMPBbpReadRadarDuration(
	IN PRTMP_ADAPTER	pAd);

ULONG RTMPReadRadarDuration(
	IN PRTMP_ADAPTER	pAd);

VOID RTMPCleanRadarDuration(
	IN PRTMP_ADAPTER	pAd);

VOID RTMPPrepareRDCTSFrame(
	IN	PRTMP_ADAPTER	pAd,
	IN	PUCHAR			pDA,
	IN	ULONG			Duration,
	IN  UCHAR           RTSRate,
	IN  ULONG           CTSBaseAddr,
	IN  UCHAR			FrameGap);

VOID RTMPPrepareRadarDetectParams(
	IN PRTMP_ADAPTER	pAd);

#ifdef CONFIG_AP_SUPPORT
VOID AdaptRadarDetection(
	IN PRTMP_ADAPTER pAd);

VOID DFSStartTrigger(
	IN PRTMP_ADAPTER pAd);

INT Set_FastDfs_Proc(
	IN PRTMP_ADAPTER pAd, 
	IN PUCHAR arg);
#endif // CONFIG_AP_SUPPORT //

INT Set_ChMovingTime_Proc(
	IN PRTMP_ADAPTER pAd, 
	IN PUCHAR arg);

INT Set_LongPulseRadarTh_Proc(
	IN PRTMP_ADAPTER pAd, 
	IN PUCHAR arg);

#ifdef CONFIG_AP_SUPPORT
#ifdef CARRIER_DETECTION_SUPPORT
static inline INT isCarrierDetectExist(
	IN PRTMP_ADAPTER pAd)
{
	if (pAd->CommonCfg.CarrierDetect.CD_State != CD_NORMAL)
		return TRUE;
	else
		return FALSE;
}		

static inline INT CarrierDetectReset(
	IN PRTMP_ADAPTER pAd)
{
	pAd->CommonCfg.CarrierDetect.CD_State = CD_NORMAL;
	return 0;
}

static inline VOID CarrierDetectionStart(
	IN PRTMP_ADAPTER pAd,
	IN BOOLEAN CTS_Protect)
{
	UINT8 CTSSending = 0x01;
	UINT8 ActiveTime;
	UINT8 Period;
	UINT8 Band;

	//pAd->CommonCfg.CarrierDetect.CDPeriod = 250;
	pAd->CommonCfg.CarrierDetect.CDPeriod = 10; /* 10 * 100ms = 1 sec. */
	pAd->CommonCfg.CarrierDetect.CDSessionTime = 25;

	/* CTSSending = 1 mean start CS without CTS protection,
	** CTSSending = 2 mean start CS with one CTS protection.
	** CTSSending = 3 mean start CS with two CTS protection.
	*/
	CTSSending = (CTS_Protect != 0 ? 0x02 : 0x01);

	/* N[4:0] indicates radar/carrier detection period in unit of 1ms.
	** N[6:5] indicates CTS protect, 0: stop detection,	1: detect with one CTS protect, 3: detect with two CTS protect.
	** N[7] indicates A/G band, 0: A Band, 1: G Band.
	** M[7:0] indicates radar detection period.	
	*/
	Period = pAd->CommonCfg.CarrierDetect.CDPeriod;
	ActiveTime = pAd->CommonCfg.CarrierDetect.CDSessionTime & 0x1f;
	Band = (pAd->CommonCfg.Channel > 14) ? 0 : 1;
	//AsicSendCommandToMcu(pAd, 0x61, 0xff, Period, ActiveTime | (CTSSending << 5) | (Band << 7));
	AsicSendCommandToMcu(pAd, 0x63, 0xff, Period, ActiveTime | (CTSSending << 5) | (Band << 7)); // perform Carrier-Sense very 1 sec.

	mdelay(5);

	return;
}	

static inline VOID CarrierDetectionStop(
    IN PRTMP_ADAPTER	pAd)
{
	CarrierDetectReset(pAd);

	//AsicSendCommandToMcu(pAd, 0x61, 0xff, 0x00, 0x00);
	AsicSendCommandToMcu(pAd, 0x63, 0xff, 0x00, 0x00);
	mdelay(5);

	return;
}

VOID CarrierDetectionFsm(
	IN PRTMP_ADAPTER pAd,
	IN UINT32 CurFalseCCA);

VOID CarrierDetectionCheck(
	IN PRTMP_ADAPTER pAd);

VOID CarrierDetectStartTrigger(
	IN PRTMP_ADAPTER pAd);

INT Set_CarrierDetect_Proc(IN PRTMP_ADAPTER pAd, IN PUCHAR arg);
#endif // CARRIER_DETECTION_SUPPORT //
#endif // CONFIG_AP_SUPPORT //

