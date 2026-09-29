/**
 * @file  messages.h
 * @brief MDH message definition
 *
**/
#ifndef MESSAGES_H
#define MESSAGES_H

#define Msg_Null                   0    /* No message will be transmitted            */

/* Input messages, bit 0 - activate (0) or deactivate (1), bits 1-3 (>> 1) input number */
#define Msg_Activate_1             2    /* Button 1 pressed  or input 1 activated    */
#define Msg_Deactivate_1           3    /* Button 1 released or input 1 de-activated */
#define Msg_Activate_2             4    /* Button 2 pressed  or input 2 activated    */
#define Msg_Deactivate_2           5    /* Button 2 released or input 2 de-activated */
#define Msg_Activate_3             6    /* Button 3 pressed  or input 3 activated    */
#define Msg_Deactivate_3           7    /* Button 3 released or input 3 de-activated */
#define Msg_Activate_4             8    /* Button 4 pressed  or input 4 activated    */
#define Msg_Deactivate_4           9    /* Button 4 released or input 4 de-activated */
#define Msg_Activate_5            10    /* Button 5 pressed  or input 5 activated    */
#define Msg_Deactivate_5          11    /* Button 5 released or input 5 de-activated */
#define Msg_Activate_6            12    /* Button 6 pressed  or input 6 activated    */
#define Msg_Deactivate_6          13    /* Button 6 released or input 6 de-activated */
#define Msg_Activate_7            14    /* Button 7 pressed  or input 7 activated    */
#define Msg_Deactivate_7          15    /* Button 7 released or input 7 de-activated */
#define Msg_Activate_8            16    /* Button 8 pressed  or input 8 activated    */
#define Msg_Deactivate_8          17    /* Button 8 released or input 8 de-activated */


/* Tilt Messages */
#define Msg_TiltActivated         18    /* Unit has tilted past 60 degrees (Resend every ??s while active) */
#define Msg_TiltDeactivated       19    /* Unit has returned to an upright position                        */
#define Msg_TiltFalseAlarm        20    /* User has indicated a false alarm                                */

/* Still alive message */
#define Msg_StillAlive            23    /* Unit is still alive          */

/* Power messages */
#define Msg_PowerLoss             24    /* Unit input power has failed (Resend every ??s while active)        */
#define Msg_PowerReturn           25    /* Unit input power has been re-applied                               */
#define Msg_BatteryOK             26    /* Battery OK, can be used as still alive signal                      */
#define Msg_BatteryLow            27    /* Battery should be replaced soon, can be used as still alive signal */
#define Msg_BatteryCritical       28    /* Battery needs replacing now, can be used as still alive signal     */

/* Sounder messages */
#define Msg_SounderActivate       32    /* Activate all remote sounders */
#define Msg_SounderReset          33    /* Reset all remote sounders    */
#define Msg_SounderActivate00     34    /* Activate remote sounder 0    */
#define Msg_SounderReset00        35    /* Reset remote sounder 0       */
#define Msg_SounderActivate01     36    /* Activate remote sounder 1    */
#define Msg_SounderReset01        37    /* Reset remote sounder 1       */
#define Msg_SounderActivate02     38    /* Activate remote sounder 2    */
#define Msg_SounderReset02        39    /* Reset remote sounder 2       */
#define Msg_SounderActivate03     40    /* Activate remote sounder 3    */
#define Msg_SounderReset03        41    /* Reset remote sounder 3       */
#define Msg_SounderActivate04     42    /* Activate remote sounder 4    */
#define Msg_SounderReset04        43    /* Reset remote sounder 4       */
#define Msg_SounderActivate05     44    /* Activate remote sounder 5    */
#define Msg_SounderReset05        45    /* Reset remote sounder 5       */
#define Msg_SounderActivate06     46    /* Activate remote sounder 6    */
#define Msg_SounderReset06        47    /* Reset remote sounder 6       */
#define Msg_SounderActivate07     48    /* Activate remote sounder 7    */
#define Msg_SounderReset07        49    /* Reset remote sounder 7       */
#define Msg_SounderActivate08     50    /* Activate remote sounder 8    */
#define Msg_SounderReset08        51    /* Reset remote sounder 8       */
#define Msg_SounderActivate09     52    /* Activate remote sounder 9    */
#define Msg_SounderReset09        53    /* Reset remote sounder 9       */
#define Msg_SounderActivate10     54    /* Activate remote sounder 10   */
#define Msg_SounderReset10        55    /* Reset remote sounder 10      */
#define Msg_SounderActivate11     56    /* Activate remote sounder 11   */
#define Msg_SounderReset11        57    /* Reset remote sounder 11      */
#define Msg_SounderActivate12     58    /* Activate remote sounder 12   */
#define Msg_SounderReset12        59    /* Reset remote sounder 12      */
#define Msg_SounderActivate13     60    /* Activate remote sounder 13   */
#define Msg_SounderReset13        61    /* Reset remote sounder 13      */
#define Msg_SounderActivate14     62    /* Activate remote sounder 14   */
#define Msg_SounderReset14        63    /* Reset remote sounder 14      */
#define Msg_SounderActivate15     64    /* Activate remote sounder 15   */
#define Msg_SounderReset15        65    /* Reset remote sounder 15      */

#define MESSAGE_UNKNOWN           255

int   message_lookup_name(char *name);
char *message_lookup_number(int number);
void  message_print(void);
int message_is_valid(int message_number);

void message_send(int message_type);

#endif
