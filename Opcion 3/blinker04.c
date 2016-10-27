
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );//NO TOCARRR
extern unsigned int GET32 ( unsigned int );// funcion externa presente en el archivo vector,NOOOO TOCAR
extern void dummy ( unsigned int );//funcion falsa para que el optimizador del compilador no elimine lineas de codigo

#define ARM_TIMER_LOD 0x2000B400//variables del manejo de los timers NOOOO TOCAR SANDRAAA
#define ARM_TIMER_VAL 0x2000B404
#define ARM_TIMER_CTL 0x2000B408
#define ARM_TIMER_CLI 0x2000B40C
#define ARM_TIMER_RIS 0x2000B410
#define ARM_TIMER_MIS 0x2000B414
#define ARM_TIMER_RLD 0x2000B418
#define ARM_TIMER_DIV 0x2000B41C
#define ARM_TIMER_CNT 0x2000B420

#define SYSTIMERCLO 0x20003004
#define GPFSEL1 0x20200004
#define GPSET0  0x2020001C// gpio set funcion que pone un 1 en el bit elegido
#define GPCLR0  0x20200028//gpio clear pone un 0 en un bit

//-------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;

    ra=GET32(GPFSEL1);
    ra&=~(7<<18);
    ra|=1<<18;
    PUT32(GPFSEL1,ra);

    PUT32(ARM_TIMER_CTL,0x003E0000);
    PUT32(ARM_TIMER_LOD,4000000-1);
    PUT32(ARM_TIMER_RLD,4000000-1);
    PUT32(ARM_TIMER_DIV,0x000000F9);
    PUT32(ARM_TIMER_CLI,0);
    PUT32(ARM_TIMER_CTL,0x003E0082);

    while(1)
    {
        PUT32(GPSET0,1<<16);//pin del gpio que se va a manejar
        while(1) if(GET32(ARM_TIMER_RIS)) break;
        PUT32(ARM_TIMER_CLI,0);
        PUT32(GPCLR0,1<<16);//se pone un cero(se apaga el led)
        while(1) if(GET32(ARM_TIMER_RIS)) break;
        PUT32(ARM_TIMER_CLI,0);
    }
    return(0);
}
