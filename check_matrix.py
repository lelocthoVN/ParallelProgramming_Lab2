import numpy as np

FOLDER = "C:\\Users\\ASUS ZENBOOK\\source\\repos\\lab2_ПаралельноеПрограммирование\\"


def load_matrix(filename):
    with open(filename, 'r') as file:
        size_mat = int(file.readline())
        matrix = []
        for _ in range(size_mat):
            row = list(map(int, file.readline().split()))
            matrix.append(row)
        return np.array(matrix)


def verify_result(mat1, mat2, result):
    saved_result = load_matrix(result)
    calculated_result = np.dot(mat1, mat2)
    if np.array_equal(saved_result, calculated_result):
        return True;
    else:
        return False;


if __name__ == "__main__":
    num_threads_values = [2, 4, 8, 16]
    for num_threads in num_threads_values:
        for size in range(10, 110, 10):
            matrix1 = load_matrix(FOLDER + f"matrix\\First_Matrix\\matrix1_{size}.txt")
            matrix2 = load_matrix(FOLDER + f"matrix\\Second_Matrix\\matrix2_{size}.txt")
            result_filename = FOLDER + f"matrix\\Result_Matrix\\result_{num_threads}_{size}.txt"
            if verify_result(matrix1, matrix2, result_filename):
                print(f"result_{num_threads}_{size} is match!")
            else:
                print(f"result_{num_threads}_{size} is not match!")
