#include "RC632.h"
#include <memory.h>

// 未在相应的头文件中声明，并且仅在本文件中有使用
uint8 RC632_DataFieldAdd(uint8 *data, uint8 len);

uint8 RC632_DataFieldAdd(uint8 *data, uint8 len) {
    if(len == 0) {
        return 0;
    }
    uint8 i;
    uint8 numOfSopl;
    for(i = 0, numOfSopl = 0; i < len; i++) {
        if(data[i] == RC632_FRAME_SOPL) {
            numOfSopl++;  // the num of sopl
        }
    }
    uint8 *p = data + len - 1;
    uint8 *pTail = p + numOfSopl;
    for(i = 0; i < len; i++) {
        if(*p == RC632_FRAME_SOPL) {
            *(pTail--) = 0x00;
        }
        *(pTail--) = *(p--);
    }
    return numOfSopl;
}

uint8 RC632_CalcFrameFCS(uint8 *msg_ptr, uint8 len) {
    uint8 x;
    uint8 xorResult;

    xorResult = 0;

    for ( x = 0; x < len; x++, msg_ptr++ ) {
        xorResult = xorResult ^ *msg_ptr;
    }
    return ( xorResult );
}

uint8 RC632_AnalysisFrame(uint8 *frame) {
    uint16 sof = BUILD_UINT16(frame[RC632_RPC_SOP], frame[RC632_RPC_SOP + 1]);
    uint16 len = BUILD_UINT16(frame[RC632_RPC_LEN], frame[RC632_RPC_LEN + 1]);
    if(sof == RC632_FRAME_SOP) {
        uint8 fcs = RC632_CalcFrameFCS(&frame[RC632_RPC_LEN+1], len);
        if(fcs == frame[RC632_RPC_LEN + len + 1]) {
            return frame[RC632_RPC_DAT];
        }
    }
    return RC632_FAULT255;  // frame fcs error
}

uint16 RC632_BuildReqFrame(uint8 *pRet, uint16 cmd, uint8 *data, uint16 dataLen) {
    uint16 frameLen = dataLen + 9;   // 9 = sop(2)+len(2)+nc(2)+cmd(2)+fcs(1)
    pRet[0] = LO_UINT16(RC632_FRAME_SOP);
    pRet[1] = HI_UINT16(RC632_FRAME_SOP);
    pRet[2] = LO_UINT16(frameLen - 4);  // 4 = sop(2)+len(2)
    pRet[3] = HI_UINT16(frameLen - 4);  // 4 = sop(2)+len(2)
    pRet[4] = 0;  // NC
    pRet[5] = 0;  // NC
    pRet[6] = LO_UINT16(cmd);
    pRet[7] = HI_UINT16(cmd);
    memcpy(pRet + 8, data, dataLen);
    uint8 nsopl = RC632_DataFieldAdd(pRet + 8, dataLen);
    frameLen += nsopl;
    pRet[8+dataLen+nsopl] = RC632_CalcFrameFCS(pRet + 3, frameLen - 4);
    return frameLen;
}
