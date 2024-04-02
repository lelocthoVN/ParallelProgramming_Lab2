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
        print("Verification successful: Results match!")
    else:
        print("Verification failed: Results do not match.")


if __name__ == "__main__":
    folder_result = FOLDER + "matrix\\Result_Matrix\\"
    for size in range(10, 110, 10):
        matrix1 = load_matrix(FOLDER + f"matrix\\First_Matrix\\matrix1_{size}.txt")
        matrix2 = load_matrix(FOLDER + f"matrix\\Second_Matrix\\matrix2_{size}.txt")
        result_filename = f"{folder_result}{size}.txt"
        verify_result(matrix1, matrix2, result_filename)
