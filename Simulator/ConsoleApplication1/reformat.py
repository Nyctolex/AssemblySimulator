error_accured = False
arg_error = lambda line_number: print_error_messgae("error in compile: not enogh arguments at line "+str(line_number))
regs = ["zero", "imm", "v0", "a0", "a1", "a2", "a3",
        "t0", "t1", "t2", "s0", "s1", "s2", "gp", "sp", "ra"]

def remove_double_chars(txt, char):
    while char+char in txt:
        txt = txt.replace(char+char,char)
    return txt


def print_error_messgae(msg):
    global error_accured
    error_accured = True
    print("\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
    print(msg)
    print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n")


with open('test.asm', 'r') as f:
    code = f.read()
if not 'halt' in code:
    print_error_messgae("no halt in code")
old_code = (code + '.')[:-1] #coping code string
code = remove_double_chars(code, ' ')
code = remove_double_chars(code, '#')
lines = code.split('\n')
new_code = ''


for i, line in enumerate(lines):
    if '#' in line:
        commented_line = True
        line, comment = line.split('#')
        raw_comment = (comment + '.')[:-1]
        comment = ' # ' + comment + '\n'
    else:
        commented_line = False
        comment = '\n'
    line = line.rstrip()
    #print(line)
    if line == '' or line == ' ' or ".word" in line:
        new_code += line+'\n'
        continue
    if 'set' in line:
        if len(line.split(' ')) == 3:
            op, rd, num = line.split(' ')
            new_code += 'add {rd}, imm, zero, {num} '.format(rd=rd, num=num)
        else:
            arg_error(i+1)
            break
    elif 'mov' in line:
        if len(line.split(' ')) == 3:
            op, rd, rs = line.split(' ')
            new_code += 'add {rd}, {rs}, zero, 0 '.format(rd=rd, rs=rs)
        else:
            arg_error(i+1)
            break
    elif 'push' in line:
        if len(line.split(' ')) == 2:
            op, rs = line.split(' ')
            comment = ""
            new_code += 'add sp, imm, sp, -4 #allocating memory in the stack\n'
            new_code += 'sw {rs}, zero, sp, 0 #pushing {rs} to stack ,{comment}'.format(rs=rs,comment=raw_comment)
            comment = "\n"
        else:
            arg_error(i+1)
            break
    elif 'pop' in line:
        if len(line.split(' ')) == 2:
            op, rs = line.split(' ')
            new_code += 'lw {rs}, zero, sp, 0 #poping {rs} from stack, {comment}\n'.format(rs=rs, comment=raw_comment)
            new_code += 'add sp, imm, sp, 4 #freeing memory in stack'
            comment = "\n"
        else:
            arg_error(i+1)
            break
    elif len(line.split(',')) != 4 and not ":" in line:
        arg_error(i+1)
        break
    else:
        new_code += line
    
    new_code += comment

code = new_code
for reg in regs:
    code = code.replace(reg, "$"+reg)
code = code.replace("$$", "$")

if error_accured:
    code = old_code
with open('test.asm', 'w') as f:
    f.write(code)
