.data

message: 	.string "The Force will be with you. Always."
				   //Ymj Ktwhj bnqq gj bnym dtz. Fqbfdx.
key:		.byte 5
len:		.long 0

.text
.global main

main:		ldr r0, =key
			ldr r1, =message
			mov r2, #80   // 0x50
			movt r2,#8192 // 0x2000
			// Codé se trouve à l'adresse 0x20000050

			ldr r3, [r0]
			mov r0, #0
loop:		add r0, r0, #1
			ldrb r4, [r1]// Chargement lettre

			cmp r4, #00
			beq quitter

			add r4, r4, r3
			str r4,[r2]
			add r1,r1,#1
			add r2,r2,#1

			b loop
quitter:
			str r4,[r2]
			ldr r1,=len
			sub r0, r0, #1
			str r0,[r1]

			bx lr
.end
