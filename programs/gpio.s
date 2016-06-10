
ldr r0,=0x20200000
mov r1,#1
lsl r1,#18

str r1,[r0,#4]

mov r1,#1
lsl r1,#16

loop:
str r1,[r0,#40]

mov r2,#0xFF0000
wait1:
sub r2,r2,#1
cmp r2,#0
bne wait1

str r1,[r0,#28]

mov r2,#0xFF0000
wait:
sub r2,r2,#1
cmp r2,#0
bne wait
b loop

andeq r0,r0,r0
