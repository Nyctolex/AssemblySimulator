with open('test.asm', 'r') as f:
    code = f.read()
regs = ["zero", "imm", "v0", "a0", "a1", "a2", "a3",
        "t0", "t1", "t2", "s0", "s1", "s2", "gp", "sp", "ra"]

for reg in regs:
    code = code.replace(reg, "$"+reg)
code = code.replace("$$", "$")

with open('test.asm', 'w') as f:
    f.write(code)
