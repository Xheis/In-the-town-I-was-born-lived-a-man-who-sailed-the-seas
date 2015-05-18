#ifndef Methods
#define Methods

//--------------------------------------------------------------------------------------------------------------------
//                              Function prototypes
//--------------------------------------------------------------------------------------------------------------------

void set_Tone(unsigned short, unsigned char);
void theta_Manager(void);
unsigned short octave_Adjust(unsigned char, unsigned char);
char	combined_Sine(void);
void PORT1_TO_PLAY_TONE();
void UpdateLEDS();
void DAC_Init();
void Timer_Init();
void Voltage_Reference_Init();
void DAC_Sine_Wave();
void Oscillator_Init(void);
void Interrupts_Init(void);


void delay(unsigned short);


void reset_Timer_0(void);
void update_millis(void);
unsigned short millis(void);
void millis_RESET(void);

void blink(void);
unsigned short micros(void);
void update_micros(void);
void reset_Timer_1(void);
void delay_micro(unsigned short);

void InitialiseUART();
void SendUltrasonicTimes(unsigned short);
void PulseGeneration(void);

#endif