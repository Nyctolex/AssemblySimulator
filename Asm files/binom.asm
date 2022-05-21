lw a0, imm, zero, 0x100 #a0 = n
lw a1, imm, zero, 0x101 #a1 = k
jal ra, imm, zero, BINOM #goto BINOM
sw v0, imm, zero, 0x102
halt $zero, $zero, $zero, 0

BINOM:
beq imm, a1, zero, RETURN1 #if k==0 return 1;
beq imm, a0, a1, RETURN1 #if n==k return 1;

push ra
push a0
push a1
sub a0, a0, imm, 1 #a0 = n-1
sub a1, a1, imm, 1 #a1 = k-1
jal ra, imm, zero, BINOM
mov a2 v0 #a2 = binom(n-1, k-1)
pop a1 #a0 = n again
pop a0 #a1 = k again
pop ra

push a0
push a1
push ra

sub a0, a0, imm, 1 #a0 = n-1
jal ra, imm, zero, BINOM
add v0, v0, a2, 0 #v0 = binom(n-1, k-1)+binom(n-1, k)
pop a1
pop a0
pop ra
jal zero, ra, zero, 0 #return

RETURN1:
    add v0, zero, imm, 1
    jal zero, ra, zero, 0

.word 0x100 2
.word 0x101 1
