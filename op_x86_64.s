.intel_syntax noprefix		# use intel syntax

# extern uint64_t operation_override(uint64_t a, uint64_t b);
.global operation_override	# make function visibility global

#; Linux x64 parameter passing

#; syscall number: rax
#; system call:  rdi, rsi, rdx, r10, r8, r9
#; sys return: rax

#; user normal call: rdi, rsi, rdx, rcx, r8, r9
#; user normal return: rax(lo), rdx(hi)
#; user float call: xmm0 - xmm7
#; user float return: xmm0(lo), xmm1(hi)

#; Saved: rbx, rbp, rsp, r12, r13, r14, r15
#; volatile: args, return, r10, r11, xmm

.text

.p2align 4					# align section to 16 bytes
.section .text.operation_override
operation_override:
	add rdi, rsi			# add both inputs
	not rdi					# invert sum
	mov rax, rdi			# copy output to rax
	ret						# return

.section .note.GNU-stack	# gcc seems to need this section defined

