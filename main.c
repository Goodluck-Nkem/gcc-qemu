#include <stdio.h>
#include <stdint.h>
#include <sys/utsname.h>
#include <limits.h>

#if __WORDSIZE == 64
#define FMT_HEX_64 "0x%lX"
#else
#define FMT_HEX_64 "0x%llX"
#endif

static void prologue(int argc, char* argv[]);
static void epilogue(void);
static uint64_t revbit64(uint64_t in);

/* default implementation for revbit64_func */
static uint64_t operation_default(uint64_t a, uint64_t b);

/* {weak function to be overridden} */
__attribute__ ((weak, alias("operation_default")))
extern uint64_t operation_override(uint64_t a, uint64_t b);

/* Function pointer, to be called */
/* revbyte, revbit, add, return */
static uint64_t (*const operation)(uint64_t a, uint64_t b) = operation_override;

int main(int argc, char* argv[])
{
	prologue(argc, argv);

	puts("\nMain tests... ");

	uint64_t in = (0xf3Efull << 48) | 0x35dfull, out;
	out = revbit64(in);
	printf("revbit64 of " FMT_HEX_64 " is " FMT_HEX_64 "\n", in, out);

	operation(5, 15);

	epilogue();

	return 0;
}

static void prologue(int argc, char* argv[])
{
	unsigned u = 0x12345678;
	unsigned char uchdr = *(char*)&u; /* deref to get byte0 */
	struct utsname ust;
	uname(&ust);
	printf("Hello world (%s)\n", ust.machine);
	printf("Number of args: %i\n", argc);
	puts("First byte of 0x12345678 (L.E = 0x78, B.E = 0x12)");
	printf("We got 0x%02x! ", uchdr);
	if(0x78 == uchdr)
		puts("Hence, little-endian (L.E)");
	else
		puts("Hence, big-endian (B.E)");
}

static void epilogue(void)
{
	puts("\nProgram done!");
}

__attribute__ ((always_inline))
static inline uint64_t revbit64(uint64_t in)
{
	uint64_t output;

#if defined(__aarch64__)
	asm volatile ("rbit %0, %1" : "=r" (output) : "r" (in) );
#elif defined(__arm__)
	/* reverse input hi32 into output lo32 */
	/* reverse input lo32 into output hi32 */
	asm volatile ("rbit %0, %1" : "=r" (output) : "r" (in>>32) );
	asm volatile ("rbit %0, %1" : "=r" (*(((uint32_t*)&output)+1)) : "r" (in) );
#else
	output = 0;
	for(uint64_t i = 0; i < 64; i++)
	{
		if(in & (1ull << i))
			output |= 1ull << (63 - i);
	}
#endif
	return output;
}

static uint64_t operation_default(uint64_t a, uint64_t b)
{
	return a + b;
}

