.equ RCC_AHBENR,	0x40021014
.equ GPIOE_MODER, 	0x48001000
.equ GPIOE_ODR,	  	0x48001014

.data
result:		.byte 0
sentence:	.string "Ta bete te bat"


.text
.global main

main:		mov r0, #0			// Compteur dans un sens + SECURITE
			mov r1, #0			// Compteur dans l'autre sens
			mov r2, #0			// Compteur de la nouvelle chaine
			mov r3, #0			// Charge la lettre ICI
			ldr r4, =sentence
			mov r5, #80			// 0x50
			movt r5,#8192		// 0x2000

compter:	ldrb r3, [r4, r0] 	// Chargement lettre
			cmp r3, #00
			beq postcompter
// Transformation de la chaine
			add r0, r0, #1

			cmp r3, #32			// 32 = espace
			beq compter

			cmp r3, #90 		// 90 = z donc si supérieur, majuscule
			blt a
			sub r3, r3, #32		// A-a = 32
a:			strb r3, [r5, r2]
			add r2, r2, #1
			b compter


postcompter:sub r0, r2, #1
comparer:	cmp r1, r2
			bgt pass
			ldrb r4, [r5, r0]
			ldrb r6, [r5, r1]
			cmp r4, r6
			bne led_fail

			sub r2, r2, #1
			sub r0, r0, #1
			add r1, r1, #1


			cmp r2, r1
			bne comparer

LED_init: 	// enable the clock of the peripheral
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

			b led_pass

pass:
			ldr r1, =result
			mov r0, #1
			b end
fail:		bl led_fail
			ldr r1, =result
			mov r0, #0

end:		strb r0, [r1]
			bx lr


led_pass:
 			// write 1 on the pin associated to the LED we want to turn on
			movw r0, #:lower16:GPIOE_ODR
			movt r0, #:upper16:GPIOE_ODR
			ldr r1, [r0]

			movw r2, #:lower16:0x00008000
			movt r2, #:upper16:0x000080001
			orr r1, r1, r2 // ORR = OU
			str r1, [r0]

			b pass

led_fail:	// write 1 on the pin associated to the LED we want to turn on
			movw r3, #:lower16:GPIOE_ODR
			movt r3, #:upper16:GPIOE_ODR
//			ldr r2, [r3]

//			movw r2, #:lower16:0x00008000
//			movt r2, #:upper16:0x000080001
//			orr r1, r1, r2 // ORR = OU
			str r0, [r3]
