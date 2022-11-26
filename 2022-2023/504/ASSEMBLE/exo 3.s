.equ RCC_AHBENR,	0x40021014
.equ GPIOE_MODER, 	0x48001000
.equ GPIOE_ODR,	  	0x48001014

.text
.global LED_init
.global Led_on
.global wait

LED_init:	mov r4,lr
            push {r4}

 			/*enter your code here */

            pop {r4}
			mov lr,r4
			bx lr



Led_on:		mov r4,lr
            push {r4}

 			/*enter your code here */

            pop {r4}
			mov lr,r4
			bx lr



wait:		mov r4,lr
            push {r4}

			/*enter your code here */

			pop {r4}
			mov lr,r4
			bx lr