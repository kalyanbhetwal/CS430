
import argparse
import numpy as np
 
def generate_matrix(size, filename, matrix, vector=False):
    matrix_array = ["%%MatrixMarket matrix array real general\n"]
    m_row = size
    m_cols = 1 if vector else size
    matrix_array.append(f" {m_row} {m_cols}\n")
    for row in matrix:
        for elem in row:
            matrix_array.append(f" {elem}\n")
    with open(filename,"w") as fprt:
        fprt.writelines(matrix_array)
 
 
 
 
def main(mat1_filename, mat2_filename,  out_filename, size, is_vector):
    mat1 = np.random.randint(-5,5,size = (size,size))
    if is_vector:
        mat2 = np.random.randint(-5,5,size = (size,1))
    else:
        mat2 = np.random.randint(-5,5,size = (size,size))
    out = np.matmul(mat1, mat2)
    generate_matrix(size, mat1_filename, mat1)
    generate_matrix(size, mat2_filename, mat2, is_vector)
    generate_matrix(size, out_filename, out, is_vector)
 
 
# Using the special variable 
# __name__
if __name__=="__main__":
    parser = argparse.ArgumentParser(description = 'Matrix Generator')
    parser.add_argument("-m1", type=str, required=True, help="Filename for matrix 1")
    parser.add_argument("-m2", type=str, required=True, help="Filename for matrix 2")
    parser.add_argument("-o", type=str, required=True, help="Filename for output matrix")
    parser.add_argument("-size", type=int, required=True, help="Dimension of matrix")
    parser.add_argument('-v', action='store_true', help="Option for second one to be matrix or vector")
    args = parser.parse_args()
    size = args.size
    is_vector = args.v
    mat1_filename = args.m1
    mat2_filename = args.m2
    out_filename = args.o
    main(mat1_filename, mat2_filename,  out_filename, size, is_vector)
