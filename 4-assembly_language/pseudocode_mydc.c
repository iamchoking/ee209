/*PSEUDO-CODE
 * In this pseudo-code we are assuming that no local variables are created
 * in the _main_ process stack In case you want to allocate space for local
 * variables, please remember to update logic for 'empty dc stack' condition
 * (lines 6, 15 and 20) accordingly.
*/
// #include<stdio.h>
// #include <stdlib.h>
// #include <time.h>
// takes care of EOF


#define ARRAYSIZE 20
#define EOF -1
#define NULL 0 //not needed in assembly language. (becuase stack comparison happens differently)
#define TRUE 1
#define FALSE 0

//define in rodata
static char* scanfFormat = "%s";
static char* printFormat = "%d\n";
static char* errEmpty = "dc: stack empty\n";
static char* errInvalid = "dc: invalid command/input\n";

//defined in bss
static char buffer[ARRAYSIZE]; //used to catch scanf data
// pointer for printall
static int prime_current_int; //used for prime

static int a, b, c, d; //these will be treated as registers %eax - %edx

// functions here because i don't like red underlines.
int stackpush(int a){return 0;} // pushes into stack
int stackpeek(){return 0;} // looks at top of stack

int main() {
	srand(time(NULL)); // call srand at the beginning of the main (for random)

	start:
	
	scanf(scanfFormat, buffer);
	
	if (buffer == EOF){goto quit;}

	if (buffer[0] == '_') { goto digitStart; }

	if (!isdigit(buffer[0])) {goto nonDigitStart;}

	goto digitStart;

	nonDigitStart:
		if (buffer[0] == '+') { goto plus;}
		if (buffer[0] == '-') { goto minus;}
		if (buffer[0] == '*') { goto times;}
		if (buffer[0] == '/') { goto divide;}
		if (buffer[0] == '%') { goto mod;}

		if (buffer[0] == '^') { goto power;}
		//this one calls a function
		
		if (buffer[0] == 'p') { goto print; }
		if (buffer[0] == 'q') { goto quit;}

		//PART 1 CAPABILITIES COMPLETE
		
		if (buffer[0] == 'f') { goto printall;}
		if (buffer[0] == 'c') { goto clear;}
		if (buffer[0] == 'd') { goto boil;}
		if (buffer[0] == 'r') { goto rev;}
		if (buffer[0] == 'x') { goto random;}
		if (buffer[0] == 'y') { goto prime;}

		//TODO: probably need to say something like "invalid command" here
	
	goto start; // end of all case branching.

	// (in function) implementation of all cases

	digitStart: /* the first no. is a digit */ //push this.
		/* data validation (are these all digits?) */
		a = 1; // # first digit already checked.
		digitCheck_loop:
		if(buffer[a] == '\0'){goto digitCheck_loopend;}
		if (!(isdigit(buffer[a]))){goto invalid;}
		++a;
		goto digitCheck_loop;
		digitCheck_loopend:

		a = buffer[0];
		if (buffer[0] == '_') { goto digitStart; }
		a = atoi(buffer);
		stackpush(a);	/* pushl some register value */
	goto start;

	digitStart_negative:
		/* need data validation (are these all digits?) */
		buffer[0] = '-';
		a = atoi(buffer);
		stackpush(a);
	goto start;

	//case functions

	print:
	if (stackpeek() == NULL) {goto stackEmpty;}
	printf(printFormat, (int)stacktop()); /* value is already pushed in the stack */
	goto start;
	end_print:

	plus:
	if (stackpeek() == NULL) {goto stackEmpty;}
	a = (int)stackpop();
	if (stackpeek() == NULL) {goto stackShort;}
	b = (int)stackpop(); /* popl to some register */
	a = a + b;
	stackpush(a);
	goto start;
	end_plus:

	minus:
	if (stackpeek() == NULL) {goto stackEmpty;}
	a = (int)stackpop();
	if (stackpeek() == NULL) {goto stackShort;}
	b = (int)stackpop(); /* popl to some register */
	a = a - b;
	stackpush(a);
	goto start;
	end_minus:

	times:
	if (stackpeek() == NULL) {goto stackEmpty;}
	a = (int)stackpop();
	if (stackpeek() == NULL) {goto stackShort;}
	b = (int)stackpop(); /* popl to some register */
	a = a * b;
	stackpush(a);
	goto start;
	end_times:

	divide:
	if (stackpeek() == NULL) {goto stackEmpty;}
	a = (int)stackpop();
	if (stackpeek() == NULL) {goto stackShort;}
	b = (int)stackpop(); /* popl to some register */
	a = a * b;
	stackpush(a);
	goto start;
	end_divide:

	mod:
	if (stackpeek() == NULL) {goto stackEmpty;}
	a = (int)stackpop();
	if (stackpeek() == NULL) {goto stackShort;}
	b = (int)stackpop(); /* popl to some register */
	a = a * b;
	stackpush(a);
	goto start;
	end_mod:

	power:
	/* probably use a function*/
	if (stackpeek() == NULL) {goto stackEmpty;}
	a = (int)stackpop();
	if (stackpeek() == NULL) {goto stackShort;}
	b = (int)stackpop(); /* popl to some register */
	a = powerfunc(b,a);
	stackpush(a);
	goto start;
	end_power:
	// 	# ############## PART 1 COMPLETE ############## #

	printall:
	if (stackpeep() == NULL) {goto stackEmpty;}
	// (better implemnted in assembly language)
	// duplicate current esp to X
	print_one:
	//print (using printf) current X
	end_print_one:
	//increment X (by 4 bytes)
	//if X equals ebp, goto finish. if not, goto print_one:
	printall_finish:
		goto start;
	end_printall:
	
	clear:
	// also better implemented in assembly language
	// simply move esp to ebp
	goto start;
	end_clear:
	
	boil:
	if (stackpeep() == NULL) {goto stackEmpty;}
	stackpush(stackpeep());
	goto start;
	end_boil:
	
	rev:
	if (stackpeek() == NULL) {goto stackEmpty;}
	a = (int)stackpop();
	if (stackpeek() == NULL) {goto stackShort;}
	b = (int)stackpop(); /* popl to some register */
	stackpush(a);
	stackpush(b);
	goto start;
	end_rev:
	
	random:
	// implemented entirely different in assembly langauge.
	// has a differnt funtion called rdrand
	goto start;
	end_random:

	prime:
	if (stackpeek() == NULL) {goto stackEmpty;}
	a = (int)stackpop();
	if(a - 1 <= 0){goto start;}

	prime_current_int = a;
	prime_loop:
	if (prime_current_int <= 1){goto prime_loopend_fail;}

	a = isprime(prime_current_int);

	if(a == TRUE){goto prime_loopend_success;}

	--prime_current_int;
	goto prime_loop;
	prime_loopend_success:
	stackpush(prime_current_int);
	prime_loopend_fail:
	goto start;
	end_prime:

	//some error cases
	stackEmpty:
	printf(errEmpty);
	goto start;
	end_stackEmpty:

	stackShort: //when only one element is in stack (where 2 is needed).
	stackpush(a);
	goto stackEmpty;
	end_stackShort:

	invalid:
	printf(errInvalid);
	goto start;
	end_invalid:

	quit:
	return 0;
} //MAIN FUNCTION END

int powerfunc(int iBase,int iExp){
	d = iExp;  //edx used as exponent
	c = iBase; //ecx used as base (want to compute c^d)

	//computing without for loop (flattened)
	b = 1; //ebx used as iIndex (current index) [int iIndex = 1;]
	a = 1; //eax used as iPower [int iPower = 1;]

	loop1:
	if (b > d){goto loopend1;}

	a *= c;
	b ++;

	goto loop1;
	loopend1:
	return a;
}

int isprime(int number){
	c = number/2+1; //ecx: number top
	a = 2;
	isprime_loop:
	if(a-c>0){goto isprime_loopend;}
	d = 0;
	d = number%a;
	if(d == 0){goto isprime_FALSE;}
	a++;
	goto isprime_loop;
	isprime_loopend:
	isprime_TRUE:
		return TRUE;
	isprime_FALSE:
		return FALSE;
}