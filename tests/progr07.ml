(= n 10)
(= Q (linspace 1 n n))
(= On (ones n n))
(= W (./ On (- (+ (* (transpose Q) (ones 1 n)) (* (ones n 1) Q)) On)))
(= R (inv W))
(disp R)

