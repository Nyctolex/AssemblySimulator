
# n is in a0
#k is in a1
beq a1, zero, RETURN1, 0 #if k==0 return 1;
beq a0, a1, RETURN1, 0 #if n==k return 1;



RETURN1:
    add v0, zero, imm, 0
    reti zero, zero, zero, 0
