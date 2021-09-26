# TruthTable
A bonus hw

`play.cpp` is a too-long cpp code, while `ttable.nb` provides a neat mathematica implementation. However, the latter is not expected to be accepted by the teacher.

Compile `play.cpp` using `g++ play.cpp -o play -std=c++11`. Then type expressions like `(pvq>r^p)=(q=r)`. You'll figure out what's going on.

To use the Mathematica code, call the function using `TruthTable[(p&&q)~Implies~(r||p)==(q==r), {p, q, r}]`.

My cpp is good because I tried to do OOP (at an unprofessional level)! And the code presents the truth in **prefix notation** format, which is flat but never plain!
