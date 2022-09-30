#include <linSlaveTask.h>
#include <linStackTask.h>
#include <applicationTask.h>
#include <linsNodeCfgIdentify.h>
#include <softTimerTask.h>
#include <taskManager.h>
#include <ColorMixingTask.h>
#include <isrfuncs.h>
#include <measureTask.h>
#include <pdsTask.h>

#define DIAG_DATA_BUFF_SIZE     (128U)


#define FID_COLOR_CTRL_INDEX            (0)
#define FID_STATUS_FRAME_INDEX          (1)
#define USER_DATA_REPORT_INDEX          (2)
#define EVENT_TRIGGERED_INDEX           (3)

/* ******************internal function declarations****************************/
void DiagnosticSubscribedCmdsHandle(const DiagReqInfo_t * const frameInfo);
void DiagnosticSleepRequestHandle(SleepRequestType_t type);
void UnconditionalSubscribedCmdsHandle(LIN_Device_Frame_t const *frame);
void UnconditionalPublishedCmdsISR(LIN_Device_Frame_t *const frame);

uint8_t getPIDFromFID(uint8_t frameId);

/* *******************global variants declarations*****************************/
static TaskState_t        linsTaskState = TASK_STATE_INIT;
static uint8_t diagDataRspBuff[DIAG_DATA_BUFF_SIZE];
static uint8_t diagDataRecvBuff[DIAG_DATA_BUFF_SIZE];

/*  LIN command handle callback  declarations  */
static ls_LinsFramesCallback_t linsFramesCallback ={
  UnconditionalSubscribedCmdsHandle,
  UnconditionalPublishedCmdsISR,       /*  it's an interrupt function, please fill the data as fast as possible   */
  DiagnosticSubscribedCmdsHandle,
  NULL,
  DiagnosticSleepRequestHandle,
};

/* **FID table declarations****/
static LIN_Device_Frame_t UnconditionalCmdsTable[DEFAULT_LINS_FID_SIZE] = {
  [FID_COLOR_CTRL_INDEX] = {
      .frame_id = FID_COLOR_CTRL,
      .msg_type = LIN_MSG_TYPE_RX,
      .checksum = LIN_CHECKSUM_ENHANCED,
      .length   = LIN_BUFF_SIZE,
      .frameIsValid = TRUE,
  },
  [FID_STATUS_FRAME_INDEX] = {
      .frame_id = FID_STATUS_FRAME,   /* status management frame */
      .msg_type = LIN_MSG_TYPE_TX,
      .checksum = LIN_CHECKSUM_ENHANCED,
      .length   = LIN_BUFF_SIZE,
      .frameIsValid = TRUE,
  },
  [USER_DATA_REPORT_INDEX] = {
      .frame_id = USER_DATA_REPORT,   /* user data report */
      .msg_type = LIN_MSG_TYPE_TX,
      .checksum = LIN_CHECKSUM_ENHANCED,
      .length   = LIN_BUFF_SIZE,
      .frameIsValid = TRUE,
      .linkedEventTriggerFidTableIndex = EVENT_TRIGGERED_INDEX,
  },
  [EVENT_TRIGGERED_INDEX] = {
      .frame_id = EVENT_TRIGGER_DATA_REPORT,   /* event trigger data report */
      .msg_type = LIN_MSG_TYPE_TX_EVENT,       /*event trigger data request from sender for slave; */
      .checksum = LIN_CHECKSUM_ENHANCED,       
      .length   = LIN_BUFF_SIZE,
      .frameIsValid = TRUE,
      .eventTriggered = FALSE,
      .linkedEventTriggerFidTableIndex = USER_DATA_REPORT_INDEX,
  },
};


/* please don't do any changes for it would be used by lin stck */
LIN_Device_Frame_t *LINS_GetUcndFramesTable(uint8_t *tableItemsCount)
{
    *tableItemsCount = (uint8_t)(sizeof(UnconditionalCmdsTable)/sizeof(LIN_Device_Frame_t));
    return UnconditionalCmdsTable;
}



/* 
  Diagnostic subscribed frame received from LIN master
*/
void DiagnosticSubscribedCmdsHandle(const DiagReqInfo_t *const diagReq)
{
    DiagRspInfo_t diagRsp={
      .sid      = diagReq->sid,
      .type     = diagReq->type,
      .payload  = diagDataRspBuff,
    };
    if (diagReq->type == PDU_TYPE_SINGLE_PDU){
        if (diagReq->sid >= LIN_SID_ASSIGN_NAD && diagReq->sid <= LIN_SID_ASSIGN_FRAME_ID_RANGE){
            /* handle Node configuration and Identification commands*/
            diagRsp.type = PDU_TYPE_SINGLE_PDU;
            if (LNCI_HandleNodeCfgIdentifyRequest(diagReq,&diagRsp)== TRUE){
                ls_handle_diagnostic_response(&diagRsp);
            }
        }else{
            if (diagReq->sid == LIN_SID_APP_DIAG_LED_GET_CONFIG){
                /*  prepare diagnostic single pdu, user defined diagnostic frame, here is for LED param configuration*/
                for (uint16_t i = 0; i < diagReq->frameDataLength; i++){
                    diagDataRecvBuff[i+diagReq->packCurrIndex] = diagReq->payload[i];
                }
                if(APPL_PrepareLedParamRequest(diagReq->sid,diagDataRecvBuff,&diagRsp) == TRUE){
                    ls_handle_diagnostic_response(&diagRsp);
                }
            }else if (diagReq->sid == LIN_SID_APP_DIAG_SINGLE_PDU_RECV_MULTI_RSP){
                /*  handle diagnostic single pdu, multi response, user defined diagnostic frame here for example: */
                for (uint8_t i = 0U; i < DIAG_DATA_BUFF_SIZE; i++){
                    diagDataRspBuff[i] = i;
                }
                diagRsp.sid = diagReq->sid;
                diagRsp.packLength = DIAG_DATA_BUFF_SIZE;
                diagRsp.type = (diagRsp.packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
                ls_handle_diagnostic_response(&diagRsp);  
            }
        }
    }else{
        /*  handle diagnostic multi pdu received sigle pdu response, user defined diagnostic frame here   */
        if (diagReq->sid == LIN_SID_APP_DIAG_LED_SET_CONFIG){
            for (uint16_t i = 0; i < diagReq->frameDataLength; i++){
                diagDataRecvBuff[i+diagReq->packCurrIndex] = diagReq->payload[i];
            }
            /* Package received finished */
            if ((diagReq->frameDataLength + diagReq->packCurrIndex)>=diagReq->packLength){
                /* handle finished data here  */
                if ( APPL_HandleLedParamConfigRequest(diagReq->sid,diagDataRecvBuff,&diagRsp) == TRUE){
                    ls_handle_diagnostic_response(&diagRsp);  
                }
            }
        }else if (diagReq->sid == LIN_SID_APP_DIAG_LED_GET_CONFIG){
            /*  handle diagnostic multi pdu received multi pdu response, user defined diagnostic frame here   */
            for (uint16_t i = 0; i < diagReq->frameDataLength; i++){
                diagDataRecvBuff[i+diagReq->packCurrIndex] = diagReq->payload[i];
            }
            if ((diagReq->frameDataLength + diagReq->packCurrIndex)>=diagReq->packLength){
                /*  prepare diagnostic single pdu, user defined diagnostic frame, here is for LED param configuration*/
                if(APPL_PrepareLedParamRequest(diagReq->sid,diagDataRecvBuff,&diagRsp) == TRUE){
                    ls_handle_diagnostic_response(&diagRsp);
                }
            }
            
        }else if (diagReq->sid == LIN_SID_APP_DIAG_MULTI_PDU_RECV_SINGLE_RSP){
            /*  handle diagnostic multi pdu received multi pdu response, user defined diagnostic frame here   */
            for (uint16_t i = 0; i < diagReq->frameDataLength; i++){
                diagDataRecvBuff[i+diagReq->packCurrIndex] = diagReq->payload[i];
            }
            /* Package received finished */
            if ((diagReq->frameDataLength + diagReq->packCurrIndex)>=diagReq->packLength){
                /* handle finished data here  */
                /* handle response data here  */
                diagRsp.sid = diagReq->sid;
                diagRsp.packLength = 5;/* Valid data length not includes sid*/
                diagRsp.type = (diagRsp.packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
                ls_handle_diagnostic_response(&diagRsp);  
            }
        }else if (diagReq->sid == LIN_SID_APP_DIAG_MULTI_PDU_RECV_MULTI_RSP){
            /*  handle diagnostic multi pdu received multi pdu response, user defined diagnostic frame here   */
            for (uint16_t i = 0; i < diagReq->frameDataLength; i++){
                diagDataRecvBuff[i+diagReq->packCurrIndex] = diagReq->payload[i];
            }
            /* Package received finished */
            if ((diagReq->frameDataLength + diagReq->packCurrIndex)>=diagReq->packLength){
                /* handle finished data here  */
                /* handle response data here  */
                diagRsp.sid = diagReq->sid;
                diagRsp.packLength = DIAG_DATA_BUFF_SIZE; /* Valid data length not includes sid*/
                diagRsp.type = (diagRsp.packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
                ls_handle_diagnostic_response(&diagRsp);  
            }
        }else{
          
        }
    }
}

/* 
  Received sleep command from lin master or bus idle timeout occurs
*/
void DiagnosticSleepRequestHandle(SleepRequestType_t type)
{
    /* Set lin to sleep mode */
    /*LINS_SetSlaveModuleToSleep();*/
    /* Set system to hibranate mode*/
    /*PMU_EnterToDeepSleepMode();*/
}

uint8_t getPIDFromFID(uint8_t frameId)
{
    uint8_t id = frameId;
    uint8_t P0,P1;
    P0 = (uint8_t)(((id >> 0)&0x01U)^((id >> 1)&0x01U)^((id >> 2)&0x01U) ^ ((id>> 4)&0x01U));
    P1 = (uint8_t)(~(((id >> 1)&0x01U)^((id >> 3)&0x01U)^((id >> 4)&0x01U) ^ ((id>> 5)&0x01U))) & 0x01U;
    id = frameId | (P0 << 6) | (P1 << 7);
    return id;
}

/*  
  User defined published frame callback function which from LINS ISR
  please fill frame data[0-7] only, please don't do any changes on the left info except data[x]!!!!
*/
void UnconditionalPublishedCmdsISR(LIN_Device_Frame_t *const frame)
{
    /*   feed buffer to frame   */
    if (frame->frame_id == UnconditionalCmdsTable[FID_STATUS_FRAME_INDEX].frame_id){
        /* status management frame */
        uint16_t status = ls_read_sys_status();
        frame->data[0] = (uint8_t)status;
        frame->data[1] = getPIDFromFID((uint8_t)(status >> 8));
        frame->data[2] = ls_read_error_code();
        for (uint8_t i = 3U; i < LIN_BUFF_SIZE; i++){
            frame->data[i] = 0xFFU;
        }
    }else if (frame->frame_id == UnconditionalCmdsTable[USER_DATA_REPORT_INDEX].frame_id){
        /* user defined data report */
        for (uint8_t i = 1U; i < LIN_BUFF_SIZE; i++){
            frame->data[i] = 0xFFU;
        }
    }else if (frame->frame_id == UnconditionalCmdsTable[EVENT_TRIGGERED_INDEX].frame_id){
        /* event trigger data report */
      if (frame->eventTriggered == TRUE){
          frame->data[0] = getPIDFromFID(UnconditionalCmdsTable[UnconditionalCmdsTable[EVENT_TRIGGERED_INDEX].linkedEventTriggerFidTableIndex].frame_id);/* transfer FID to PID  */
          for (uint8_t i = 1U; i < LIN_BUFF_SIZE; i++){
              frame->data[i] = 0x55U;
          }
      }
    }else{
      
    }
}
/* 
  User defined subscribed frame received from LIN master
*/
void UnconditionalSubscribedCmdsHandle(LIN_Device_Frame_t const *frame)
{
    if (frame->frame_id == UnconditionalCmdsTable[FID_COLOR_CTRL_INDEX].frame_id){
        APPL_HandleColorControlCommands(frame);
    }
}

void LINS_TaskHandler(void)
{
    switch(linsTaskState){
    case TASK_STATE_ACTIVE:
      break;
    case TASK_STATE_INIT:
      (void)l_sys_init();
      ls_register_services(UnconditionalCmdsTable, (uint8_t)(sizeof(UnconditionalCmdsTable)/sizeof(LIN_Device_Frame_t)), DIAG_DATA_BUFF_SIZE,&linsFramesCallback);
      (void)ls_set_tp_timeout(N_AS, N_CR, 10U);
      linsTaskState = TASK_STATE_ACTIVE;
      break;
    default:
      break;
    }
}