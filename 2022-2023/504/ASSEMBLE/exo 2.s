.data
result:		.byte 0
sentence:	.string "Ta bete te bat"


.text
.global main

main:
			mov r0, #0			// Compteur dans un sens + SECURITE
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
			bne fail

			sub r2, r2, #1
			sub r0, r0, #1
			add r1, r1, #1


			cmp r2, r1
			bne comparer

			b pass

pass:		ldr r1, =result
			mov r0, #1
			b end
fail:		ldr r1, =result
			mov r0, #0

end:		strb r0, [r1]
			bx lr
