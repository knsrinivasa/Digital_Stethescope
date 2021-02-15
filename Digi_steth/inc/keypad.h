
#ifndef __KEYPAD_H__
#define __KEYPAD_H__

/* [QAC-Supress] Identifier not used */
//PRQA S 3205 EOF
/* [QAC-Supress] Identifier declared not used */
//PRQA S 3210 EOF
/* [QAC-Supress] Identifier not used */
//PRQA S 3408 EOF
/******************************************** DEFINES ***********************************************/
/** @PARAM
* Debounce time for key press (in ms)
*/
#define KEYPAD_DEBTIME_BTNPRESS     20

/** @PARAM
* Debounce time for key release (in ms)
*/
#define KEYPAD_DEBTIME_BTNRLS       30

/******************************************** Type Definitions ***********************************************/
/// Structure type definition for keypad data collector interface
typedef struct
{
  bool   u8_statBtn1;             ///< Button 1 status
  bool   u8_statBtn2;             ///< Button 2 status
  bool   u8_statBtn3;             ///< Button 3 status
  bool   u8_statBtn4;             ///< Button 4 status
  bool   u8_stat_pump_swt;             ///< pump swt status
  bool   u8_stat_flush_swt;             ///< flush swt status
  bool   u8_multipleKeyPrsd;      ///< Status of simultaneous key-press of specific keys
  bool   u8_ASD_Detect;
  uint32 u32_tmSinceBtnPrsd_ms;   ///< Time since last button pressed
}DC_KeyPad_Intfce_Struct;

extern uint8 u8_BtnPresd;  

void  KeyPad_ProcIni (void);
void  KeyPad_Proc (void);
void  KeyPad_ResetTmr(void);

void  DC_KeyPad_1_Wr (void *Dest, void *Src);

#endif
