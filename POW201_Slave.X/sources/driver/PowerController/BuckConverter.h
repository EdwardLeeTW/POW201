
#ifdef __BuckConverter
#define EXTERN  
#else
#define EXTERN extern
#endif

// Reference for Voltage Loop Compensator
#define VCOMP_VREF          2245

// Compensator Clamp Limits
#define VCOMP_MIN_CLAMP     0x0010
#define VCOMP_MAX_CLAMP     0x3839

EXTERN uint16_t Buck_Vref;
volatile uint16_t VCOMP_ControlObject_Initialize(void);
void Buck_Softstart(void);

