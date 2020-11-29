# ################## RODATA SECTION ################## #	
	.section ".rodata"
cGreeting: # later referenced by main
	.asciz "hello, world! \n" # directive to store ascii string folled by null (zero terminated)

# ################## DATA   SECTION ################## #	
# not needed here

# ################## BSS    SECTION ################## #
# not needed here

# ################## TEXT   SECTION ################## #
	.section ".text"
	.global main
	.type main,@function

main:
	# inialize / prolog
	pushl %ebp
	movl %esp, %ebp

	# call printf
	pushl $cGreeting # push parameter
	call printf
	addl $4, %esp    # pop parameter

	# main: epilog
	movl $0, %eax # corresponds to return 0;
	movl %ebp, %esp
	popl %ebp
	
	# return
	ret

