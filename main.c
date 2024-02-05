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
static uint64_t revbyte64(uint64_t in);

/* default implementation for revbit64_func */
static uint64_t operation_default(uint64_t a, uint64_t b);

/* {weak function to be overridden} */
__attribute__ ((weak, alias("operation_default")))
extern uint64_t operation_override(uint64_t a, uint64_t b);

/* operation() function revealed as a pointer */
/* applies revbyte then revbit to each bit, add reapply rev, return result */
static uint64_t (*const operation)(uint64_t a, uint64_t b) = operation_override;

int main(int argc, char* argv[])
{
	prologue(argc, argv);

	puts("\nMain tests... ");

	uint64_t in = (0xf3Efull << 48) | 0x35dfull, out;
	out = revbit64(in);
	printf("revbit64 of " FMT_HEX_64 " is " FMT_HEX_64 "\n", in, out);

	out = operation(0xFFull << 16, 0xFFull << 32);
	printf("Result of operation is " FMT_HEX_64 "\n", out);

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
	unsigned nbits = 64;

#if defined(__aarch64__)
	asm volatile ("rbit %0, %1" : "=r" (output) : "r" (in) );
#elif defined(__arm__)
	/* reverse input hi32 into output lo32 */
	/* reverse input lo32 into output hi32 */
	asm volatile (	"rbit %0, %2;" 
					"rbit %1, %3;"
		   			: "=r" (*(((uint32_t*)&output)+0)), "=r" (*(((uint32_t*)&output)+1))
					: "r" (*(((uint32_t*)&in)+1)), "r" (*(((uint32_t*)&in)+0)));
#elif defined(__x86_64__)
	asm volatile(	".intel_syntax noprefix;"
					"1: ;"
					"ror %[in], 1;"			// rotate input right into carry
					"rcl %[output], 1;"		// rotate output left through carry
					"dec %[nbits];"			// decrement counter
					"jnz 1b;"				// not done?, continue operation
					".att_syntax;"
					: [output] "+r" (output)
					: [in] "r" (in), [nbits] "r" (nbits));
#else
	output = 0;
	while(nbits--)
	{
		output = (output << 1) | (in & 1);		/* left shift the bit into output */
		in >>= 1; 								/* right shift input into lsb */
	}
#endif

	return output;
}

static uint64_t operation_default(uint64_t a, uint64_t b)
{
	return a + b;
}

static uint64_t revbyte64(uint64_t in)
{
	uint64_t output;
	return 0;
}

