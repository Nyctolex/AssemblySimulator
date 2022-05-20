

with open('test.asm', 'r') as f:
    code = f.read()
regs = ["zero", "imm", "v0", "a0", "a1", "a2", "a3",
        "t0", "t1", "t2", "s0", "s1", "s2", "gp", "sp", "ra"]

lines = code.split('\n')
for i, line in enumerate(lines):
    if len(line.split(',')) != 4 and not ":" in line:
        print("\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
        print("     error in compile: not enogh arguments at line", i+1)
        print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n")

for reg in regs:
    code = code.replace(reg, "$"+reg)
code = code.replace("$$", "$")

with open('test.asm', 'w') as f:
    f.write(code)
