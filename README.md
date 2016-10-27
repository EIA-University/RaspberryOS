Sistema operativo de raspberry para controlar los pines del GPIO

INSTRUCCIONES OPCION 1:
Paso a Paso: Construir un sistema operativo en C para raspberry

1.El primer ejemplo sencillo que se maneja es el siguiente (se usa para probar un SO sencillo y que los linkers y builders estén haciendo bien las cosas):
int main(void)
{
    while(1)
    {

    }

    return 0;
}
2.Hacer los build scripts y el cross compiler (para cada versión de raspberry es diferente):
•	Comando para los build scripts, para el raspberry pi es:
arm-none-eabi-gcc -O2 -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s arm-test.c
•	Comando para los build scripts, para el raspberry pi B+ y V2 es:
arm-none-eabi-gcc -O2 -mfpu=vfp -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 arm-test.c

Al intentar buildear este código saldrá un error en cuanto a los linkers (sé que no deberíamos poner errores en este trabajo, pero es para que se entienda completamente que pasa, y porque nuestro primer intento de un SO no usara un hello world como siempre hemos hecho con todo lo otro):
.../arm-none-eabi/lib/fpu\libc.a(lib_a-exit.o): In function `exit':
exit.c:(.text.exit+0x2c): undefined reference to `_exit'
collect2.exe: error: ld returned 1 exit status

Después ejecutamos:
arm-none-eabi-gcc -O2 -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s -nostartfiles armc-2.c -o kernel.elf

y por ultimo:
arm-none-eabi-objcopy kernel.elf -O binary kernel.img

para generar el kernel.img que es el archivo que buscamos generar
Este error sucede porque hace falta una línea _exit que hace referencia a una llamada al sistema (como las que vimos en el parcial y el libro) normalmente esta seria llamada cuando un programa termine o se necesite, pero como en este caso nosotros somos nuestro propio OS y es lo único que está corriendo estas llamadas pueden estar en blanco, pero se dejan para que el linker no ocasione errores.por esta razón el código que teníamos ya debe quedar asi (de igual manera los archivos del SO y sus versiones finales estarán adjuntadas):
int main(void)
{
    while(1)
    {

    }

    return 0;
}

void exit(int code)
{
    while(1)
        ;
}

La razón por la cual no haremos un hello world es porque estos en C usan el printf,que a su vez es una forma elevada de un write(),por lo cual printf escribe en el archivo de stdout que comúnmente cuando hay un SO produce una salida visible a pantalla,pero sin un SO stdout escribe a una UART,entonces en este caso el helloworld común no seria un ejemplo tan sencillo.

Procesador de raspberry: El BCM2385 contiene dos procesadores uno videocore que es el GPU que es el que hace que raspberry sea muy buen centro de multimedia,y otro que es el ARM (que lo hemos visto en clase en ejemplos anteriores como arm-gnulinux-eabi y sus variaciones) este procesador es el que corre el sistema operativo.
El orden en el que raspberry hace las cosas es el siguiente,para entender mejor que es lo que se ejecuta y como lo hace:
1.	El GPU toma la primera porción de la FAT presente en la SD y la carga en el bus MMC.
2.	Este busca y carga un archivo llamado bootcode.bin y lo ejecuta,el bootcode.bin busca en la SD un archivo llamado start.elf y un archivo de texto de configuración (Config.txt) en este se definen las configuraciones de los kernell.
3.	Se busca por un archivo .img según el config.txt en nuestro caso el kernell.img este se carga en una dirección especifica (0x8000)
Señales de vida: Hasta el momento solo hemos logrado que el procesador ejecute un ciclo infinito,pero esto no tenemos como apreciarlo para saber si si nos esta funcionando o no,por lo cual vamos a prender el act LED,para esto se necesita saber las direcciones de la GPIO que son desde la 0x7E200000 hasta la 0x7EFFFFF y estas corresponden a unas direcciones físicas del ARM que van desde 0x20200000 hasta 0x202FFFFF(esto en el raspberry 1 y en el dos o superior 0x3F200000 hasta 0x3F2FFFFF.
Como queremos prender el LED act del raspberry debemos dirigirnos al pin 16 que es el que controla este LED.
Definimos el GPIO base según el modelo que estemos usando:

#ifdef RPI2 //si se esta usando el raspberry 2
    #define GPIO_BASE 0x3F200000UL
#else //para raspberry 1
    #define GPIO_BASE 0x20200000UL
#endif

volatile unsigned int* gpio_fs1 = (unsigned int*)(GPIO_BASE+0x04);
volatile unsigned int* gpio_fs4 = (unsigned int*)(GPIO_BASE+0x10);

Ahora que definimos el GPIO base debemos definir el pin del gpio 16 como salida para poder prender el LED lo hacemos así:

#if defined( RPIPLUS ) || defined ( RPI2 ) //se va definir según el modelo del raspberry
    *gpio_fs4 |= (1<<21); //se genera un apuntador al registro para escribir en el valor, dependiendo del modelo se usa la función 4 o la 1 (fs4 o fs1)
#else
    *gpio_fs1 |= (1<<18);
#endif

Estos son los pedazos del código que debían ser explicados por aparte para comprender mejor, en el código adjunto nosotros declaramos todos los pines y salidas del GPIO para tener el control de cualquiera que quisiéramos, estas debían ser definidas en la parte superior como si fueran un include.

INSTRUCCIONES OPCION 2:

Pasos:
1-Nombrar el código a compilar main.s
2-Entrar a la terminal
3 -Dirigirse al directorio donde se encuentra el código y el makefile entregado
4-Ejecutar make
5-copiar el kernel.img generado en la sd del raspberry que se va utilizar

INSTRUCCIONES OPCION 3:

1-Se descarga el cross compiler con el comando:
sudo apt-get install gcc-arm-none-eabi

2-se entra a la terminal
3-Nos dirigimos a la carpeta donde este el código y el make file(deben estar en la misma)
4-ejecutamos el make,esto generara una serie de .o y un .elf
5-Hacemos el comando:
arm-none-eabi-objcopy nombredelarchivoquecadaunotenga.elf -O binary kernel.img





