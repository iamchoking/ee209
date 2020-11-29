# # # --------------------------------------------------------------------
# # # mydc.s
# # #
# # # Desk Calculator (dc)
# # # --------------------------------------------------------------------
# #define ARRAYSIZE 20
# #define EOF -1
# #define NULL 0 //not needed in assembly language. (becuase stack comparison happens differently)

	.equ   ARRAYSIZE, 20
	.equ   EOF, -1
	
	# macros for powerfunc stack offset
	.equ   BASE, 8
	.equ   EXPONENT, 12

	# macro for random
	.equ   MAX_RANDOM, 1024

	# macro for isPrime
	.equ   TRUE,  1
	.equ   FALSE, 0
	.equ   NUMBER,8
.section ".rodata"

# static char* scanfFormat = "%s";
# static char* printFormat = "%d\n";
# static char* errEmpty = "dc: stack empty\n";
# static char* errInvalid = "dc: invalid command/input\n";

scanfFormat:
	.asciz "%s"

printFormat:
	.asciz "%d\n"

errEmpty:
	.asciz "dc: stack empty\n"

errInvalid:
	.asciz "dc: invalid command/input\n"

# # # --------------------------------------------------------------------

.section ".data"
# used for random
rand_max:
	.long 1023

# # # --------------------------------------------------------------------

.section ".bss"
# static char buffer[ARRAYSIZE]; //used to catch scanf data
buffer: # catches scanf values
        .skip  ARRAYSIZE

# used in printall (address cursor)
printall_current_address:
		.skip 4
# used in prime (number cursor)
prime_current_int:
		.skip 4

# # # --------------------------------------------------------------------

.section ".text"

	# # -------------------------------------------------------------
	# # int main(void)
	# # Runs desk calculator program.  Returns 0.
	# # -------------------------------------------------------------

	.globl  main
	.type   main,@function

main: # START OF MAIN #
	# inialize / prolog
	pushl   %ebp
	movl    %esp, %ebp

	# # srand(time(NULL)); // call srand at the beginning of the main (for random)
	# # first, call time (saved to %eax)

	# pushl $0
	# call time
	# # call time
	# addl $4, %esp
	# # now, call srand
	# pushl %eax
	# call srand@PLT
	# # call srand
	# addl $4, %esp

	# input:
	start:
		# # dc number stack initialized. %esp = %ebp
		
	# ; scanf(scanfFormat, buffer);
	pushl	$buffer
	pushl	$scanfFormat
	call    scanf
	addl    $8, %esp

	# # check if user input EOF
	# ; if (buffer == EOF){goto quit;}
	cmp	$EOF, %eax
	je	quit		


	movzx buffer, %eax # moves first char of buffer to %eax
	# ; if (buffer[0] == '_') { goto digitStart; }
	cmp $'_', %eax
		je digitStart
	
	# ; if (!isdigit(buffer[0])) {goto nonDigitStart;}
	cmp $'0', %eax
  		jl nonDigitStart
	cmp $'9', %eax
		jg nonDigitStart

	# ; goto digitStart;
	jmp digitStart


	nonDigitStart: # branching point for commands
		# //TODO: check if command is only one char

		# ;if (buffer[0] == '+') { goto plus;}
		cmp $'+', %eax
			je plus

		# # if (buffer[0] == '-') { goto minus;}
		cmp $'-', %eax
			je minus

		# # if (buffer[0] == '*') { goto times;}
		cmp $'*', %eax
			je times

		# # if (buffer[0] == '/') { goto divide;}
		cmp $'/', %eax
			je divide

		# # if (buffer[0] == '%') { goto mod;}
		cmp $'%', %eax
			je mod
		
		# ;if (buffer[0] == '^') { goto power;}
		cmp $'^', %eax
			je power
		# //this one calls a function

		# ;if (buffer[0] == 'p') { goto print; }
		cmp $'p', %eax
			je print

		# ;if (buffer[0] == 'q') { goto quit;}
		cmp $'q', %eax
			je quit
		
		# //PART 1 CAPABILITIES COMPLETE
		# ;if (buffer[0] == 'f') { goto printall;}
		cmp $'f', %eax
			je printall

		# ;if (buffer[0] == 'c') { goto clear;}
		cmp $'c', %eax
			je clear

		# ;if (buffer[0] == 'd') { goto boil;}
		cmp $'d', %eax
			je boil

		# ;if (buffer[0] == 'r') { goto rev;}
		cmp $'r', %eax
			je rev

		# ;if (buffer[0] == 'x') { goto random;}
		cmp $'x', %eax
			je random

		# ;if (buffer[0] == 'y') { goto prime;}
		cmp $'y', %eax
			je prime

	end_nonDigitStart:

	# //DONE: probably need to say something like "invalid command" here
	
	# goto invalid; // end of all cmd case branching.
	jmp invalid


	digitStart: # branching point for number entries
		# /* data validation (are these all digits?) */
				
		# a = 1; // first digit already checked.
		movl $1, %eax
		digitCheck_loop:
			# ;if(buffer[a] == '\0'){goto digitCheck_loopend;}
			movzx buffer(,%eax,1), %edx

			cmp $0, %edx
				je digitCheck_loopend

			# ;if (!(isdigit(buffer[a]))){goto invalid;}
			cmp $'0', %edx
				jl invalid
			cmp $'9', %edx
				jg invalid
			# same logic as branching

			# ++a;
			inc %eax
			# goto digitCheck_loop;
			jmp digitCheck_loop
		digitCheck_loopend:

		# ; if (buffer[0] == '_') { goto digitStart; }
		cmp $'_', %eax
			je digitStart
	

		# ;if (buffer[0] == '_') { goto digitStart; }
		movzx buffer, %eax
		cmp $'_', %eax
			je digitStart_negative
		# a = atoi(buffer);

		movl $0, %eax
		pushl $buffer
		call atoi
		addl $4, %esp

		# stack.push(a);	/* pushl some register value */
		pushl %eax

		# goto start;
		jmp start
	end_digitStart:

	# goto start; // end of all number case branching.
	jmp start

	# // (in function) implementation of all cases #####

	digitStart_negative:
		# /* need data validation (are these all digits?) */
		# buffer[0] = '-';
		movb $'-', buffer # b used to change only 1 word

		# a = atoi(buffer)
		pushl $buffer
		call atoi
		addl $4, %esp

		# stack.push(a)
		pushl %eax

		# goto start;
		jmp start
	end_digitStart_negative:

	print:
		# ;if (stack.peek() == NULL) {goto stackEmpty;}
		cmp %esp, %ebp
			je stackEmpty

		# printf(printFormat, (int)stack.top()); /* value is already pushed in the stack */
		pushl (%esp)
		pushl $printFormat
		call  printf
		addl  $8, %esp

		# goto start;
		jmp	start
	end_print:

	plus:
		#  if (stack.peek() == NULL) {goto stackEmpty;}
		cmp %esp, %ebp
			je stackEmpty
		#  a = (int)stack.pop();
		popl %eax

		#  if (stack.peek() == NULL) {goto stackShort;}
		cmp %esp, %ebp
			je stackShort
		#  b = (int)stack.pop(); /* popl to some register */
		popl %ebx

		#  a = a + b;
		addl %ebx, %eax

		#  stack.push(a);
		pushl %eax

		#  goto start;
		jmp start
	end_plus:

	minus:
		# if (stack.peek() == NULL) {goto stackEmpty;}
		cmp %esp, %ebp
			je stackEmpty
		#  a = (int)stack.pop();
		popl %eax

		#  if (stack.peek() == NULL) {goto stackShort;}
		cmp %esp, %ebp
			je stackShort
		#  b = (int)stack.pop(); /* popl to some register */
		popl %ebx

		#  a = a - b;
		subl %eax, %ebx # ebx - eax and stores it to ebx

		#  stack.push(a);
		pushl %ebx

		#  goto start;
		jmp start
	end_minus:

	times:
		# if (stack.peek() == NULL) {goto stackEmpty;}
		cmp %esp, %ebp
			je stackEmpty
		#  a = (int)stack.pop();
		popl %eax

		#  if (stack.peek() == NULL) {goto stackShort;}
		cmp %esp, %ebp
			je stackShort
		#  b = (int)stack.pop(); /* popl to some register */
		popl %ebx

		#  a = a * b;
		imull %eax, %ebx # ebx * eax and stores it to ebx

		#  stack.push(a);
		pushl %ebx

		#  goto start;
		jmp start
	end_times: # its the end of the times...

	divide:
		# ;if (stack.peek() == NULL) {goto stackEmpty;}
		cmp %esp, %ebp
			je stackEmpty
		#  a = (int)stack.pop();
		popl %eax

		# ;if (stack.peek() == NULL) {goto stackShort;}
		cmp %esp, %ebp
			je stackShort
		#  b = (int)stack.pop(); /* popl to some register */
		popl %ebx


		#  a = a / b;
		movl %eax,%ecx
		movl %ebx,%eax
		movl %ecx,%ebx # switch eax and ebx
		movl $0, %edx  # set edx to zero just in case
		idiv %ebx # (edx:eax)/ebx and stores it to eax

		#  stack.push(a);
		pushl %eax

		#  goto start;
		jmp start
	end_divide: # end the divide...

	mod:
		# ;if (stack.peek() == NULL) {goto stackEmpty;}
		cmp %esp, %ebp
			je stackEmpty
		#  a = (int)stack.pop();
		popl %eax

		# ;if (stack.peek() == NULL) {goto stackShort;}
		cmp %esp, %ebp
			je stackShort
		#  b = (int)stack.pop(); /* popl to some register */
		popl %ebx

		# d = a % b;
		movl %eax,%ecx
		movl %ebx,%eax
		movl %ecx,%ebx # switch eax and ebx
		movl $0, %edx  # set edx to zero just in case
		idiv %ebx # (edx:eax)/ebx and stores it to eax

		#  stack.push(a);
		pushl %edx

		#  goto start;
		jmp start
	end_mod:

	power: # here, call a function
		# ;if (stack.peek() == NULL) {goto stackEmpty;}
		cmp %esp, %ebp
			je stackEmpty
		#  a = (int)stack.pop();
		popl %eax

		# ;if (stack.peek() == NULL) {goto stackShort;}
		cmp %esp, %ebp
			je stackShort
		#  b = (int)stack.pop(); /* popl to some register */
		popl %ebx
		# we are looking for ebx^eax (b is base)
		
		# a = powerfunc(b,a);
		# ; push for parameter passing (reverse order)
		pushl %eax
		pushl %ebx
		call powerfunc
		addl $8,%esp # restore stack pointer
		# stack.push(a);
		pushl %eax

		# goto start;
		jmp start
	end_power:
	# ############## PART 1 COMPLETE ############## #

	printall:
		# ;if (stack.peek() == NULL) {goto stackEmpty;}
		cmp %esp, %ebp
			je printall_finish # printall does not generate error messages
		# // (better implemnted in assembly language)
		# // duplicate current esp to X
		movl %esp, printall_current_address # X is edx
		print_one:
		# //print (using printf) current X
			# movl $0, %eax
			movl printall_current_address, %eax
			pushl (%eax)
			pushl $printFormat
			call  printf
			addl  $8, %esp
		end_print_one:

		# //increment X (by 4 bytes)
		movl printall_current_address, %eax
		addl $4, %eax
		movl %eax, printall_current_address

		# //if X equals ebp, goto finish. if not, goto print_one:
		cmp printall_current_address, %ebp
			je printall_finish
		jmp print_one
		printall_finish:
			# goto start;
			jmp start
	end_printall:

	clear:
		# // simply move esp to ebp
		mov %ebp, %esp
		jmp start
	end_clear:

	boil:
		# ;if (stack.peep() == NULL) {goto stackEmpty;}
		cmp %esp, %ebp
			je stackEmpty

		# stack.push(stack.peep());
		pushl (%esp)
		# goto start;
		jmp start
	end_boil:
	
	rev:
		# ;if (stack.peek() == NULL) {goto stackEmpty;}
		cmp %esp, %ebp
			je stackEmpty
		#  a = (int)stack.pop();
		popl %eax

		# ;if (stack.peek() == NULL) {goto stackShort;}
		cmp %esp, %ebp
			je stackShort
		#  b = (int)stack.pop(); /* popl to some register */
		popl %ebx

		# stack.push(a);
		pushl %eax
		# stack.push(b);
		pushl %ebx
		# goto start;
		jmp start
	end_rev:
	
	random:
		# // implemented entirely different in assembly langauge.
		# // has a differnt funtion called rdrand
		rdrand %eax
		andl rand_max, %eax
		pushl %eax
		jmp start;
		# goto start
	end_random:

	prime:
		# ;if (stack.peek() == NULL) {goto stackEmpty;}
		cmp %esp, %ebp
			je invalid # 'y' command does not output errors
		# a = (int)stack.pop();
		popl %eax
		pushl %eax
		# ;if(a - 1 <= 0){goto start;}
		cmpl $1, %eax # no primes less than or equal to 1
			jle start
		
		# prime_current_int = a;
		movl %eax, prime_current_int

		prime_loop:
		# ;if (prime_current_int - 1<= 0){goto prime_loopend_fail;}
		cmpl $1, prime_current_int
			jle prime_loopend_fail

		# a = isprime(prime_current_int);
		pushl prime_current_int
		call isPrime
		addl $4, %esp

		# ;if(a == TRUE){goto prime_loopend_success;}
		cmpl $TRUE, %eax
			je prime_loopend_success

		# --prime_current_int;
		movl prime_current_int, %eax
		decl %eax
		movl %eax, prime_current_int

		# goto prime_loop;
		jmp prime_loop

		prime_loopend_success: # succedded. push.
			# stack.push(prime_current_int);
			pushl prime_current_int
		prime_loopend_fail: # failed. do nothing.
			# goto start;
			jmp start
	end_prime:
	# ############## PART 2 COMPLETE ############## #

	#  //some error cases
	stackEmpty:
		# ; printf(errEmpty);
		pushl $errEmpty
		call printf
		addl $4, %esp

		# ; goto start;
		jmp start
	end_stackEmpty:

	stackShort: # //when only one element is in stack (where 2 is needed).
		# ; stack.push(a);
		pushl %eax

		# ; goto stackEmpty;
		jmp stackEmpty
	end_stackShort:

	invalid: # jumps to here in case of invalid input or command.
		# ; printf(errInvalid);
		# uncomment these 3 lines to output error message (used for debugging)
		# pushl $errInvalid
		# call printf
		# addl $4, %esp
		# ; goto start;
		jmp start
	end_invalid:

	quit:
		# ; return 0
		# epilog
		movl    $0, %eax
		movl    %ebp, %esp
		popl    %ebp
		#return
		ret
end_main: # this place should not be reached. (use in gdb)
# END OF MAIN #

# function POWERFUNC #
powerfunc:
	# int powerfunc(int iBase,int iExp){
	# inialize / prolog
	pushl %ebp
	movl %esp, %ebp

	# here, the stack frame must be:
	# old ebp <- ebp  <- esp
	# old eip						 4(%ebp)
	# (paramter: base) iBase		 8(%ebp) == BASE(%ebp)
	# (parameter:exponent) iExp		12(%ebp) == EXPONENT(%ebp)

	# 	d = iExp;  //edx used as exponent 
	# movl BASE(%ebp), %edx  # rather than having edx. just
	# directly reference 8(%ebp)
	movl $0, %edx

	# movl 12(%ebp), %ecx # rather than having ecx. just
	# directly reference 12(%ebp)
	movl $0, %ecx # clear ecx since it is used in imull

	# 	//computing without for loop (flattened)
	# b = 1; //ebx used as iIndex (current index) [int iIndex = 1;]
	# (ebx = exponent cursor)
	movl $1, %ebx
	# 	a = 1; //eax used as iPower [int iPower = 1;]
	movl $1, %eax

	loop1:
	# 	if (b > d){goto loopend1;}
		cmp EXPONENT(%ebp), %ebx
		jg loopend1 # jg: ebx-EXP > 0 -> ebx > EXP 
		# -> current exponent cursor larger than total exponent

	# 	a *= c;
	imull BASE(%ebp) # use 1 operand version

	# 	b ++;
	incl %ebx

	# 	goto loop1;
	jmp loop1

	loopend1:
	# return a
	# epilog
	movl %ebp, %esp
	popl %ebp
	ret
	# }
end_powerfunc: # this place should not be reached. (use in gdb)
# END OF POWERFUNC #

# function ISPRIME #
isPrime:
	# int isprime(int number){
	# inialize / prolog
	pushl %ebp
	movl %esp, %ebp

	cmp $2, NUMBER(%ebp)
		je isprime_TRUE

	# 	c = number/2+1; //ecx: number top
	movl NUMBER(%ebp), %ecx
	sarl %ecx
	inc %ecx
	# idiv %ebx # (edx:eax)/ebx and stores it to eax

	# a = 2;
	movl $2, %eax # a used as cursor

	isprime_loop:
		# ;if(a-c>0){goto isprime_loopend;}
		cmpl %ecx, %eax
			jg isprime_loopend
		# d = 0;
		movl $0, %edx

		# d = number%a;
		movl %eax, %ebx
		movl NUMBER(%ebp), %eax
		idiv %ebx
		movl %ebx, %eax

		# ;if(d == 0){goto isprime_FALSE;}
		cmpl $0, %edx
			je isprime_FALSE		
		# goto isprime_loop;
		# a++;
		inc %eax
		jmp isprime_loop
	isprime_loopend:
	isprime_TRUE: # IS A PRIME
		# 		return TRUE;
		# epilog
		movl $TRUE, %eax
		movl %ebp, %esp
		popl %ebp
		ret	
	
	isprime_FALSE: # NOT A PRIME
		# 		return FALSE;
		# epilog
		movl $FALSE, %eax
		movl %ebp, %esp
		popl %ebp
		ret
		# }
	
end_isPrime: # this place should not be reached. (use in gdb)
# END OF ISPRIME #

