// addresses of the registers are declared as constants
.equ RCC_AHBENR,	0x40021014
.equ GPIOE_MODER, 	0x48001000
.equ GPIOE_ODR,	  	0x48001014
.equ NVIC_ISER,		0x01000040

.text
.global LED_init
.global Led_on
.global wait

LED_init: 		mov r4,lr
 			push {r4}

			// enable the clock of the peripheral
			movw r0, #:lower16:RCC_AHBENR	// the content of the register is stored
			movt r0, #:upper16:RCC_AHBENR	// into the register r1
			ldr r1, [r0]
			movw r2, #:lower16:0x00200000	// the mask is written on the register r2
			movt r2, #:upper16:0x00200000
			orr r1, r1, r2 					// the mask is applied
			str r1, [r0]   					// the new value of the register is written on the peripheral

			// set bits from 8 to 15 of the port in output mode
			movw r0, #:lower16:GPIOE_MODER
			movt r0, #:upper16:GPIOE_MODER
			movw r1, #:lower16:0x55550000	// the new register value is directly written on the peripheral
			movt r1, #:upper16:0x55550000
			str r1, [r0]

			pop {r4}
			mov lr,r4
			bx lr

Led_on:			mov r4,lr
 			push {r4}

 			// write 1 on the pin associated to the LED we want to turn on
			movw r3, #:lower16:GPIOE_ODR
			movt r3, #:upper16:GPIOE_ODR
//			ldr r2, [r3]

//			movw r2, #:lower16:0x00008000
//			movt r2, #:upper16:0x000080001
//			orr r1, r1, r2 // ORR = OU
			str r0, [r3]

 			pop {r4}
			mov lr,r4
			bx lr

wait:			mov r4,lr
 			push {r4}
 			mov r2, #0

loop:			add r2, r2, #1
			cmp r2, r0
			ble loop

			pop {r4}
			mov lr,r4
			bx lr

Button_init:		mov r4,lr
 			push {r4}

 			movw r0, #:lower16:NVIC_ISER
			movt r0, #:upper16:NVIC_ISER




			pop {r4}
			mov lr,r4
			bx lr
