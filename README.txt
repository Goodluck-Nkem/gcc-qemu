To run another architecture, we need to install qemu-user.
If we want to debug then we also need to install gdb-multiarch.

If the compiler is not native, then we must setup the locations of the runtime libs.
Move: 		sudo mv /usr/lib/<compiler>/* /usr/<compiler>/lib
Delete:		sudo rmdir /usr/lib/<compiler>
Symlink: 	sudo ln -s /usr/<compiler>/lib /usr/lib/<compiler>

Again If the compiler is not native, we must also setup its interp .so file
Expected location: 	
	readelf -a <bin> | grep interpreter
Depending on result, interpreter may want it lib or lib64: 
	sudo ln -s /usr/<compiler>/lib/<ld_so> /lib/
	sudo ln -s /usr/<compiler>/lib64/<ld_so> /lib64/

The following shows how to run/debug.
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
