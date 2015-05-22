#
# This problem comes from "Logic Puzzles" by Fernandez, Summers, and Willis
#
# Sailors row to shore, except it is actually a big fish.
# The didn't get swallowed up by the big fish.  Was it
# morning or evening?  Why did they survive?  Was the
# fish full, was the fish too slow, or were they too small?
#
# Clues:
# 1- If the fish was full, then it was evening.
# 2- If it was morning, they were too small.
# 3- The fish was too slow if it was evening.
# 4- It was morning if the fish was too slow.
#

# FF == fish full
# FS == fish slow
# TS == too small
# E == evening
# M == morning

#
# Physics
#
# It was morning or evening, but not both
tell M | E
tell M => (!E)
tell E => (!M)

# The reason was fish full, too small, or fish slow,
# but only one.
tell FF | TS | FS
tell FF => ((!TS) & (!FS))
tell TS => ((!FF) & (!FS))
tell FS => ((!FF) & (!TS))

# Instance
# If fish full then evening
tell FF => E
# If morning then too small
tell M => TS
# Fish slow if evening
tell E => FS
# Morning if fish slow
tell FS => M

# Questions
ask FF
ask !FF
ask TS
ask !TS
ask FS
ask !FS
ask M
ask !M
ask E
ask !E

dpll FF
dpll !FF
dpll TS
dpll !TS
dpll FS
dpll !FS
dpll M
dpll !M
dpll E
dpll !E
