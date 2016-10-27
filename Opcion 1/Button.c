
#ifdef RPI2
    #define GPIO_BASE       0x3F200000UL
#else
    #define GPIO_BASE       0x20200000UL
#endif
//Se usan solo por el momento la 16 y 15 que son el gpio24 y el controlador del led activador
#if defined( RPIBPLUS ) || defined( RPI2 )
    #define LED_GPFSEL      GPIO_GPFSEL4
    #define LED_GPFBIT      21
    #define LED_GPSET       GPIO_GPSET1
    #define LED_GPCLR       GPIO_GPCLR1
    #define LED_GPIO_BIT    15
#else
    #define LED_GPFSEL      GPIO_GPFSEL1
    #define LED_GPFBIT      12
    #define LED_GPSET       GPIO_GPSET0
    #define LED_GPCLR       GPIO_GPCLR0
    #define LED_GPIO_BIT    14

    #define LED_GPFSEL2     GPIO_GPFSEL0
    #define LED_GPFBIT2     15
    #define LED_GPSET2       GPIO_GPSET0
    #define LED_GPCLR2       GPIO_GPCLR0
    #define LED_GPIO_BIT2    15
#endif
//se define toda la seccion del gpio y la UART para tener acceso a ellas
#define GPIO_GPFSEL0    0
#define GPIO_GPFSEL1    1
#define GPIO_GPFSEL2    2
#define GPIO_GPFSEL3    3
#define GPIO_GPFSEL4    4
#define GPIO_GPFSEL5    5

#define GPIO_GPSET0     7
#define GPIO_GPSET1     8

#define GPIO_GPCLR0     10
#define GPIO_GPCLR1     11

#define GPIO_GPLEV0     13
#define GPIO_GPLEV1     14

#define GPIO_GPEDS0     16
#define GPIO_GPEDS1     17

#define GPIO_GPREN0     19
#define GPIO_GPREN1     20

#define GPIO_GPFEN0     22
#define GPIO_GPFEN1     23

#define GPIO_GPHEN0     25
#define GPIO_GPHEN1     26

#define GPIO_GPLEN0     28
#define GPIO_GPLEN1     29

#define GPIO_GPAREN0    31
#define GPIO_GPAREN1    32

#define GPIO_GPAFEN0    34
#define GPIO_GPAFEN1    35

#define GPIO_GPPUD      37
#define GPIO_GPPUDCLK0  38
#define GPIO_GPPUDCLK1  39

/** set de resgistros del GPIO */
volatile unsigned int* gpio;

/** Simple loop variable */
volatile unsigned int tim;

/** Main */
int main(void)
{
    /* se le asigna la direccion del gpio base usando la direccion fisica del arm*/
    gpio = (unsigned int*)GPIO_BASE;

    /* Se escribe un uno en el GPIO16 para inicializar el nible en la funcion select 1 y poder poner el GPIO16 como salida*/
    gpio[LED_GPFSEL] |= 000;

	gpio[LED_GPFSEL2] |= (1 << LED_GPFBIT2);
        gpio[LED_GPFSEL] = (1 << LED_GPIO_BIT2);


    /* Never exit as there is no OS to exit to! */
    while(1)
    {
       
    gpio[LED_GPCLR2] = (1 << LED_GPIO_BIT2);
    

 }
}
