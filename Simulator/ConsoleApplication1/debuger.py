import tkinter as tk
from tkinter import Tk, Label, Entry, Button, StringVar, IntVar
import json
regs = ["zero", "imm", "v0", "a0", "a1", "a2", "a3",
        "t0", "t1", "t2", "s0", "s1", "s2", "gp", "sp", "ra"]


with open('test.asm', 'r') as f:
    allCode = f.read()

with open('debug.json', 'r') as f:
    debug = json.load(f)
print(debug)
global loc

allCode = ""
root = tk.Tk()

loc = IntVar(root)

canvas = tk.Canvas(root, width=600, height=300)
canvas.grid(columnspan=3)

codeTitle = tk.Label(root, text="code")
codeTitle.grid(columnspan=3, column=0, row=0)

statusTitle = tk.Label(root, text="status")
statusTitle.grid(columnspan=3, column=1, row=0)


currentCode = tk.StringVar()
currentCode.set(allCode)
codeLabel = tk.Label(root, textvariable=currentCode)
codeLabel.grid(columnspan=3, column=0, row=1)

currentStatusValue = tk.StringVar()
currentStatusValue.set("")
currentStatus = tk.Label(root, textvariable=currentStatusValue)
currentStatus.grid(columnspan=3, column=1, row=1)


def next_line():
    global loc
    old_loc = loc.get()
    loc.set(loc.get()+1)
    stat = debug[loc.get()]
    status_srt = ""
    for reg in regs:
        status_srt = status_srt+reg+":"+str(stat[reg])+"\n"
    currentStatusValue.set(status_srt)


nextBtn = tk.Button(root, text="next", command=lambda: next_line())
nextBtn.grid(column=0, row=2)
root.mainloop()
