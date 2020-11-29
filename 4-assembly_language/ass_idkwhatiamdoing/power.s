# ################## RODATA SECTION ################## #	
	.section ".rodata"
cPromptBase:
	.asciz "Enter the Base: "

cPromptExponent:
	.asciz "Enter the Exponent: "

cScanfFormat:
	.asciz "%d"

cResult:
	.asciz "%d to the power of %d is %d \n"

# ################## DATA   SECTION ################## #	
	.section ".data"
iPower:
	.long 1

# ################## BSS    SECTION ################## #
	.section ".bss"
iBase:
	.skip 4
iExp:
	.skip 4
iIndex:
	.skip 4

# ################## TEXT   SECTION ################## #
	.section ".text"
	.global main
	.type main,@function

main:
	# inialize / prolog
	pushl %ebp
	movl %esp, %ebp
	#	printf("Enter the Base");
	pushl $cPromptBase
	call printf
	addl $4, %esp

	# 	scanf("%d", &iBase);
	pushl $iBase
	pushl $cScanfFormat
	call scanf
	addl $8, %esp

	#	printf("Enter the Exponent");
	pushl $cPromptExponent
	call printf
	addl $4, %esp


	# 	scanf("%d", &iExp);
	pushl $iExp
	pushl $cScanfFormat
	call scanf
	addl $8, %esp

 	# computing without for loop (flattened)
	# iIndex = 1; # defined in data section
	movl $1, iIndex
	
	loop1:
	# 	if (iIndex > iExp){goto loopend1;}
		movl iIndex, %eax
		cmpl iExp, %eax
		jg loopend1 # jg: comparison is greater than 0 -> iIndex - iExp > 0 -> iIndex > iExp (terminate condition)

	# 	iPower *= iBase;
		movl iPower, %eax
		imull iBase # multiplies with %eax and stores it to %edx:%eax (%edx: top 32 bits, %eax: bottom 32 bits)
		movl %eax, iPower # stores result of mult to iPower
	
	# 	iIndex ++;
		incl iIndex
	# 	goto loop1;
		jmp loop1

	loopend1:

	# 	printf("%d to the power of %d is %d.\n", iBase, iExp, iPower);
	pushl iPower
	pushl iExp
	pushl iBase
	pushl $cResult
	call printf
	addl $16, %esp


 	# return 0;
	# main: epilog
	movl $0, %eax # corresponds to return 0;
	movl %ebp, %esp
	popl %ebp
	# return
	ret
