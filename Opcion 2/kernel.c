#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>
 ////////////////////////////EL CODIGO ESTA MONTADO SOBRE EL CODIGO DE SANDRA Y PIPE PERO CON LAS DIRECCIONES CORREGIDAS,NO QUISIMOS TOCAR LAS PARTES QUE NO ENTENDIAMOS PORQUE ASI FUNCIONABA,EN ESTOS MOMENTOS ESTAMOS AVERIGUANDO SOBRE LA UART Y TODOS SUS COMPONENTES Y DIRECCIONES
static inline void mmio_write(uint32_t reg, uint32_t data)
{
	*(volatile uint32_t *)reg = data;
}
 
static inline uint32_t mmio_read(uint32_t reg)
{
	return *(volatile uint32_t *)reg;
}
 
/* Loop <delay> times in a way that the compiler won't optimize away. */
static inline void delay(int32_t count)
{
     // asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n": : [count]"r"(count) : "cc");
}
 
size_t strlen(const char* str)
{
	size_t ret = 0;
	while ( str[ret] != 0 )
		ret++;
	return ret;
}
 
enum
{
    // The GPIO registers base address.
    GPIO_BASE = 0x20200000,
 
    // The offsets for reach register.
 
    // Controls actuation of pull up/down to ALL GPIO pins.
    GPPUD = (GPIO_BASE + 0x94),
 
    // Controls actuation of pull up/down for specific GPIO pin.
    GPPUDCLK0 = (GPIO_BASE + 0x98),
 
    // The base address for UART.
    GPIO_BASE = 0x20201000,
 
    // The offsets for reach register for the UART.
    GPIO_DR     = (GPIO_BASE + 0x00),
    GPIO_RSRECR = (GPIO_BASE + 0x04),
    GPIO_FR     = (GPIO_BASE + 0x18),
    GPIO_ILPR   = (GPIO_BASE + 0x20),
    GPIO_IBRD   = (GPIO_BASE + 0x24),
    GPIO_FBRD   = (GPIO_BASE + 0x28),
    GPIO_LCRH   = (GPIO_BASE + 0x2C),
    GPIO_CR     = (GPIO_BASE + 0x30),
    GPIO_IFLS   = (GPIO_BASE + 0x34),
    GPIO_IMSC   = (GPIO_BASE + 0x38),
    GPIO_RIS    = (GPIO_BASE + 0x3C),
    GPIO_MIS    = (GPIO_BASE + 0x40),
    GPIO_ICR    = (GPIO_BASE + 0x44),
    GPIO_DMACR  = (GPIO_BASE + 0x48),
    GPIO_ITCR   = (GPIO_BASE + 0x80),
    GPIO_ITIP   = (GPIO_BASE + 0x84),
    GPIO_ITOP   = (GPIO_BASE + 0x88),
    GPIO_TDR    = (GPIO_BASE + 0x8C),
};
 
void uart_init()
{
	// Disable UART0.
        mmio_write(GPIO_CR, 0x00000000);
	// Setup the GPIO pin 14 && 15.
 
	// Disable pull up/down for all GPIO pins & delay for 150 cycles.
	mmio_write(GPPUD, 0x00000000);
	delay(150);
 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////PARTE FUNDAMENTAL PARA ESTABLECER ENTRADA Y SALIDA///////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	mmio_write(GPPUDCLK0, (1 << 14) | (0 << 15));
	delay(150);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Write 0 to GPPUDCLK0 to make it take effect.
	mmio_write(GPPUDCLK0, 0x00000000);
 
	// Clear pending interrupts.
        mmio_write(GPIO_ICR, 0x7FF);
 
	// Set integer & fractional part of baud rate.
	// Divider = UART_CLOCK/(16 * Baud)
	// Fraction part register = (Fractional part * 64) + 0.5
	// UART_CLOCK = 3000000; Baud = 115200.
 
	// Divider = 3000000 / (16 * 115200) = 1.627 = ~1.
	// Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
        mmio_write(GPIO_IBRD, 1);
        mmio_write(GPIO_FBRD, 40);
 
	// Enable FIFO & 8 bit data transmissio (1 stop bit, no parity).
        mmio_write(GPIO_LCRH, (1 << 4) | (1 << 5) | (1 << 6));
 
	// Mask all interrupts.
        mmio_write(GPIO_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
	                       (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
 
	// Enable UART0, receive & transfer part of UART.
        mmio_write(GPIO_CR, (1 << 0) | (1 << 8) | (1 << 9));
}
 
void uart_putc(unsigned char byte)
{
	// Wait for UART to become ready to transmit.
        while ( mmio_read(GPIO_FR) & (1 << 5) ) { }
        mmio_write(GPIO_DR, byte);
}
 
unsigned char uart_getc()
{
    // Wait for UART to have recieved something.
    while ( mmio_read(UART0_FR) & (1 << 4) ) { }
    return mmio_read(GPIO_DR);
}
 
void uart_write(const unsigned char* buffer, size_t size)
{
	for ( size_t i = 0; i < size; i++ )
		uart_putc(buffer[i]);
}
 
void uart_puts(const char* str)
{
	uart_write((const unsigned char*) str, strlen(str));
}
 
#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
	(void) r0;
	(void) r1;
	(void) atags;
 
	uart_init();
	uart_puts(1);
 
	while ( true ){
	


		
///LO QUE SE OBTENGA DEL INPUT SALE POR EL OUTPUT
        uart_putc(GPIO_getc());
	

};
}
