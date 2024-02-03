#include <stdio.h>
#include <sys/utsname.h>


static void prologue(int argc, char* argv[]);
static void epilogue(void);

int main(int argc, char* argv[])
{
	prologue(argc, argv);

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
	puts("Program done!");
}

