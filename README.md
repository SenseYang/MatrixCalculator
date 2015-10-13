# MatrixCalculator
A MATLAB-style command line matrix calculator which supported matrix operations including: 

(1) In-time matrix definition.Including functions of eye(row, col), rand(row, col), zeros(row, col) and direct bracket [] definition.

(2) Matrix operations: arithmetic(+-*/%), power(^) and determinant calculations(det), transpose(M'), round(round(M)), sum up(sum(M)).

(3) Submatrix retrieve: e.g. A(1:2, :)

(4) Vector operations: dot product(dot(A, B)), norm (norm(A)) and inner angle between two vectors (angle(A, B)). 

(5) Function and matrix name matching: e.g. rand(2, 2) will call random creator; A will search for stored matrix named "A" and print its values.
