/*!*********************************************************************************************************************
@file user_app.c                                                                
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- void UserApp1Initialize(void)
- void UserApp1Run(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u8 G_u8UserAppFlags;                  /*!< @brief Global state flags */
volatile u8 G_u8UserAppTimePeriodHi;           /*!< @brief Global saved Timer1 high count for ISR */
volatile u8 G_u8UserAppTimePeriodLo;           /*!< @brief Global saved Timer1 low count for ISR */

u8 G_au8UserAppsinTable[] = 
{
0x80,0x83,0x86,0x89,0x8c,0x8f,0x92,0x95,0x98,0x9b,0x9e,0xa2,0xa5,0xa7,0xaa,0xad,
0xb0,0xb3,0xb6,0xb9,0xbc,0xbe,0xc1,0xc4,0xc6,0xc9,0xcb,0xce,0xd0,0xd3,0xd5,0xd7,
0xda,0xdc,0xde,0xe0,0xe2,0xe4,0xe6,0xe8,0xea,0xeb,0xed,0xee,0xf0,0xf1,0xf3,0xf4,
0xf5,0xf6,0xf8,0xf9,0xfa,0xfa,0xfb,0xfc,0xfd,0xfd,0xfe,0xfe,0xfe,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xfe,0xfe,0xfe,0xfd,0xfd,0xfc,0xfb,0xfa,0xfa,0xf9,0xf8,0xf6,
0xf5,0xf4,0xf3,0xf1,0xf0,0xee,0xed,0xeb,0xea,0xe8,0xe6,0xe4,0xe2,0xe0,0xde,0xdc,
0xda,0xd7,0xd5,0xd3,0xd0,0xce,0xcb,0xc9,0xc6,0xc4,0xc1,0xbe,0xbc,0xb9,0xb6,0xb3,
0xb0,0xad,0xaa,0xa7,0xa5,0xa2,0x9e,0x9b,0x98,0x95,0x92,0x8f,0x8c,0x89,0x86,0x83,
0x80,0x7c,0x79,0x76,0x73,0x70,0x6d,0x6a,0x67,0x64,0x61,0x5d,0x5a,0x58,0x55,0x52,
0x4f,0x4c,0x49,0x46,0x43,0x41,0x3e,0x3b,0x39,0x36,0x34,0x31,0x2f,0x2c,0x2a,0x28,
0x25,0x23,0x21,0x1f,0x1d,0x1b,0x19,0x17,0x15,0x14,0x12,0x11,0x0f,0x0e,0x0c,0x0b,
0x0a,0x09,0x07,0x06,0x05,0x05,0x04,0x03,0x02,0x02,0x01,0x01,0x01,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x02,0x02,0x03,0x04,0x05,0x05,0x06,0x07,0x09,
0x0a,0x0b,0x0c,0x0e,0x0f,0x11,0x12,0x14,0x15,0x17,0x19,0x1b,0x1d,0x1f,0x21,0x23,
0x25,0x28,0x2a,0x2c,0x2f,0x31,0x34,0x36,0x39,0x3b,0x3e,0x41,0x43,0x46,0x49,0x4c,
0x4f,0x52,0x55,0x58,0x5a,0x5d,0x61,0x64,0x67,0x6a,0x6d,0x70,0x73,0x76,0x79,0x7c   
};


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp_<type>" and be declared as static.
***********************************************************************************************************************/


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void TimeXus(u16 u16TimeXus_)

@brief
Sets up Timer0 to provide an interrupt at u16Microseconds_

Requires:
- Timer0 configured such that each timer tick is 1 microsecond

Promises:
- Pre-loads TMR0H:L to clock out desired period
- TMR0IF cleared
- Timer0 enabled

*/
void TimeXus(u16 u16TimeXus_)
{
  u16 u16Temp = 65535;

#if 0  
  /* Handle edge case */
  if(u16TimeXus_ == 0)
  {
      PIR3bits.TMR0IF = 1;
      return;
  }
#endif
  
  /* Disable the timer during config */
  T0CON0bits.EN = 0;
  
  /* Preload TMR0H and TMR0L based on u16TimeXus */
  u16Temp -= u16TimeXus_;
  TMR0H = (u8)( (u16Temp >> 8) & 0x00FF);
  TMR0L = (u8)( u16Temp & 0x00FF);
   
  /* Clear TMR0IF and enable Timer 0 */
  PIR3bits.TMR0IF = 0;
  T0CON0bits.EN = 1;
  
} /* end TimeXus() */


/*!--------------------------------------------------------------------------------------------------------------------
@fn void InterruptTimerXus(u16 u16TimeXus_, bool bContinuous_)

@brief
Maximum 32,767us.  Arguments higher than this will be capped back.
Sets up Timer1 to provide an interrupt every u16TimeXus_ microseconds.
This can be configured as a single event, or continuous.
Note: it would be much better to pass a call-back function
parameter to this function to register the call-back for the ISR
to use, but we'll hard-code it for now.

Requires:
- Timer1 configured such that each timer tick is 0.5 microseconds
- TMR1_ISR holds code to respond to the interrupt
- bContinuous_ is true if the timer should run continuously;
  false if it should run once and stop.

Promises:
- Pre-loads TMR1H:L to clock out desired period
- G_u8UserAppTimePeriodHi/Lo updated to save period
- _U8_CONTINUOUS updated per bContinuous_
- TMR1IF cleared and interrupt enabled
- Timer1 enabled

*/
void InterruptTimerXus(u16 u16TimeXus_, bool bContinuous_)
{
  u16 u16Temp;

  /* Disable the timer during config */
  T1CONbits.ON = 0;
  
  /* Correct the input parameter if it's too high */
  if(u16TimeXus_ > 32767)
  {
    u16TimeXus_ = 32767;
  }

  /* Double the time so it's in us not 0.5us*/
  u16Temp = u16TimeXus_ << 1;
   
  /* Calculate, save, and preload TMR1H and TMR1L based on u16TimeXus_ */
  u16Temp = 65535 - u16TimeXus_;
  G_u8UserAppTimePeriodHi = (u8)( (u16Temp >> 8) & 0x00FF);
  G_u8UserAppTimePeriodLo = (u8)( u16Temp & 0x00FF);
  TMR1H = G_u8UserAppTimePeriodHi;
  TMR1L = G_u8UserAppTimePeriodLo;
  
  /* Flag continuous mode if required */
  G_u8UserAppFlags &= ~_U8_CONTINUOUS;
  if(bContinuous_)
  {
    G_u8UserAppFlags |= _U8_CONTINUOUS;
  }
  
  /* Clear the interrupt flag, enable interrupt and enable Timer */
  PIR3bits.TMR1IF = 0;
  PIE3bits.TMR1IE = 1;
  T1CONbits.ON = 1;
  
} /* end InterruptTimerXus() */


/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void UserAppInitialize(void)

@brief
Initializes the application's variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- Start with RA6:0 low

*/
void UserAppInitialize(void)
{
    /* LED initialization */
    LATA &= 0xC0;
    
    /* Timer0 control register initialization to turn timer on, asynch mode, 16-bit
     * Fosc/4, 1:16 prescaler, 1:1 postscaler  */
    T0CON0 = 0x90; // b'10010000'
    T0CON1 = 0x54; // b'01010100'
    
    /* Timer1 initialization:
     * 1:8 prescale, synced, enabled */
    T1GCON = 0x00;
    T1CLK  = 0x01;  
    T1CON  = 0x31;  // b'00110001'
    
    
 
    
} /* end UserAppInitialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserAppRun(void)

@brief Application code that runs once per system loop

Requires:
- 

Promises:
- 

*/
void UserAppRun(void)
{
    static u8 u8CSetting = 0;
    static u16 u16Hours = 0;
    static u16 u16Minutes = 0;
    static u16 u16Seconds = 0;
    static u16 u16ClockMilS = 0;
    
    
    static u8 u8SongSwitch = 0;
    
    static u16 au16Restless[] = {Bb4, Bb4, Bb4, Bb4, NN, G4, G4, G4, Ab4, Ab4, G4, G4, G4, G4,G4, F4, Eb4, Eb4, F4};
    static u16 au16RestlessTime[] = {N16, N16, N16, N8, N16, N16, N16, N16, N8, ND8, N16, N16, N8, N16, N8, N16, N16, N16, N16};
    
    
    // I tried to get it to play the very end of "when we were young" but it didn't come out super well. I may have made a mistake in the note conversions or timing somewhere.
    static u16 au16MyTime[] = {N16, N8, N8, ND8, N16, N16, N16, N8, ND8, N16, N8, N8, //0 to 11
                               ND8, N16, N16, N16, N8, N8, N16, N16, N16, N16, N16, N16, N8, N8, N4, N8, N16, N16, NT8, NT8, NT8,  // 12 -> 32
                               ND4, N16, ND8, N16, ND16, ND8, N16, N2, N2};
    
    static u16 au16MyNotes[] = {Bb4, Bb4, Bb4, Bb4, G4, G4, G4, Ab4, Ab4, G4, G4, G4, // 0 -> 11
                                G4, F4, Eb4, F4, NN, NN, Eb4, Eb4, G4, F4, Eb4, F4, F4, C4, NN, NN, Eb4, Eb4, G4, F4, Eb4,  // 12 -> 32
                                F4, Eb4, D4, NN, Eb4, D4, Eb4, Eb4, NN};
    static u32 u32MSeconds = 0;
    
    static u16 u16ThatIndexer = 26;
    static u16 u16ThatOtherIndexer = 0;
    u8 u8Slur  = 0;
    
    if((((PORTB & 0b00000001) != 0)) && ((PORTB & 0b00000010) != 0))
    {
        u16Minutes = 9;
        u16Seconds = 0;
        u16ClockMilS = 0;
        u8SongSwitch = 0;
        u16ThatIndexer = 26;
        u16ThatOtherIndexer = 0;
        u32MSeconds = 0;
        InterruptTimerXus(0,0);
        _delay(1000000);
  
    }
    
    
    
    
    if(u16ClockMilS == 1000)
    {
        u16ClockMilS = 0;
        u16Seconds += 1;
    }
    
    if(u16Seconds == 60)
    {
        u16Seconds = 0;
        u16Minutes += 1;
    }
    
    if(u16Minutes == 60)
    {
        u16Minutes = 0;
        u16Hours += 1;
    }
    
    if(u16Hours == 15)
    {
        u16Hours = 0;
    }
    
    //if((PORTB & 0b00100000) == 0)
    //{
    LATC = u16Seconds + 64 * u16Minutes;
    LATA = u16Minutes / 4;
    //}
    //if((PORTB & 0b00100000) == 0)
    //{
      //  LATC = 0;
    //}
    
    
    u16ClockMilS += 1;
   
    
    
    
    if((PORTB & 0b00000010) != 0)
        {
            u8SongSwitch  = 2;
            InterruptTimerXus(0,0);
            u32MSeconds == 0;
        }
    
    if(u8SongSwitch == 2)
    {
    
    if((u16ThatOtherIndexer == 2) || (u16ThatOtherIndexer == 8) || (u16ThatOtherIndexer == 13) || (u16ThatOtherIndexer > 14 ))
    {
        u8Slur = 1;
    }
    
    
    if((u32MSeconds <= (au16RestlessTime[u16ThatOtherIndexer] - RT)) || (u8Slur == 1))
    {
        InterruptTimerXus(au16Restless[u16ThatOtherIndexer],1);
    }
        if((u32MSeconds >= (au16RestlessTime[u16ThatOtherIndexer] - RT)) && (u8Slur != 1))
    {
        InterruptTimerXus(0,1);
    }
    if(u32MSeconds >=  (au16RestlessTime[u16ThatOtherIndexer]))
    {
        u16ThatOtherIndexer += 1;
        u32MSeconds = 0;
    }
    if(u16ThatOtherIndexer == 20)
    {
        u16ThatOtherIndexer = 0;
        u8SongSwitch = 0;
        InterruptTimerXus(0,0);
    }

    u32MSeconds += 1;
    }
    
    
    
    
    
    
    
    
    
    if(1)
    {
    
    
        if((PORTB & 0b00000001) != 0)
        {
            u8SongSwitch = 1;
            
        }
        


        if((u8SongSwitch == 1)){
            // There are not many slurs so I added them like this.
            if((u16ThatIndexer == 7) || (u16ThatIndexer == 13) || (u16ThatIndexer == 14))
            {
                u8Slur = 1;
            }
            if((u16ThatIndexer == 23) || (u16ThatIndexer == 33) || (u16ThatIndexer == 34))
            {
                u8Slur = 1;
            }




            if((u32MSeconds <= (au16MyTime[u16ThatIndexer] - RT)) || (u8Slur == 1))
            {
                InterruptTimerXus(au16MyNotes[u16ThatIndexer],1);
            }
            if((u32MSeconds >= (au16MyTime[u16ThatIndexer] - RT)) && (u8Slur != 1))
            {
                InterruptTimerXus(0,1);
            }
            if(u32MSeconds >=  (au16MyTime[u16ThatIndexer]))
            {
                u16ThatIndexer += 1;
                u32MSeconds = 0;
            }
            if(u16ThatIndexer == 41)
            {
                u16ThatIndexer = 26;
                u8SongSwitch = 0;
                InterruptTimerXus(0,0);
            }

            u32MSeconds += 1;
        }

    }


  
} /* end UserAppRun() */



/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/





/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
