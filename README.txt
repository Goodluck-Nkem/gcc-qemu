ABOUT QEMU-USER
---------------
To run another architecture, we need to install qemu-user.
If we want to debug then we also need to install gdb-multiarch.
We must specify library path when running qemu using the -L flag, "init-qemu.sh" helps with this by aliasing qemu executables with -L flags. 

The following shows how to run/debug.
Run: init-qemu.sh 
Run: qemu-<abi> <bin>
Debug: If we want to debug instead, we should use two windows, it's cleaner.
	Window1:
		qemu-<abi> -g <PORT> <bin>
	Window2:
		gdb-multiarch -tui <bin> 
		target remote localhost:<PORT> 
		b main 
		continue

The last 3 commands can be shorthanded by defining a new command in the ~/.gdbinit file.

PS: 
In Android, no user process is allowed to access /lib or /lib64, also chroot is denied.
So If using Termux, we need to run under proot to pretend access to such directories.
See https://github.com/termux/proot-distro on how to install proot and a minimal distro.


ABOUT INLINING
--------------
I tested assembly inlining in the revbit64 function.
See gcc online manual for complete help, I'll explain just a little of operand naming.
There are 2 ways to specify inline assembly operands.
	- assembly symbolic naming.
	- zero based position naming.
For symbolic naming, choose any valid variable name, enclosed in [] .
For zero-based, output starts from 0, input starts after the last output position.
my __arm__ and __aarch64__ option in revbit64 illustrates the zero-based method.
while my __x86_64__ option illustrates the symbolic naming method.
Other architectures use the C implementation for revbit64.
 
