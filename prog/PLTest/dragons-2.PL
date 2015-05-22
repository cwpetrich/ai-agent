# Physics for 2 dragons
tell ((RatA & RedA) | (PredA & GrayA)) <=> (!TTA)
tell ((PredA & RedA) | (RatA & GrayA)) <=> (TTA)
tell RedA <=> (!GrayA)
tell RedA | GrayA
tell PredA <=> (!RatA)
tell PredA | RatA
tell ((RatB & RedB) | (PredB & GrayB)) <=> (!TTB)
tell ((PredB & RedB) | (RatB & GrayB)) <=> (TTB)
tell RedB <=> (!GrayB)
tell RedB | GrayB
tell PredB <=> (!RatB)
tell PredB | RatB
# Instance for problem 2
tell TTA <=> (RatB & RatA)
tell TTA <=> (!TTB)
tell TTB <=> (PredA & PredB)
tell TTB <=> (RedB)

# Questions
ask PredA
ask PredB
dpll PredA
dpll PredB





