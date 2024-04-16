import matplotlib.pyplot as plt
import re

FOLDER = "C:\\Users\\ASUS ZENBOOK\\source\\repos\\lab2_ПаралельноеПрограммирование\\"


def extract_size_and_time(line):
    matches = re.findall(r'\d+(?:\.\d+)?', line)
    size = int(matches[0])
    time = float(matches[-1])
    return size, time


def read_data(data):
    sizes_mat = []
    times_dot = []
    with open(data, 'r') as file:
        for line in file:
            print(line)
            size_mat, time_dot = extract_size_and_time(line)
            sizes_mat.append(size_mat)
            times_dot.append(time_dot)
    return sizes_mat, times_dot


def plot_dependency(sizes_mat, times_dot, name_figure):
    plt.figure(figsize=(7, 10))
    plt.plot(sizes_mat, times_dot, marker='o', linestyle='-')
    plt.title(f'Зависимость размера матрицы и времени выполнения для потока {name_figure}')
    plt.xlabel('Размер матрицы')
    plt.ylabel('Время выполнения (секунды)')

    plt.xticks(sizes_mat)
    plt.yticks(times_dot)
    plt.tick_params(axis='y', labelleft=False)
    for size, time in zip(sizes_mat, times_dot):
        plt.text(size, time, f'{time:.6f}', fontsize=8, ha='left', va='bottom')

    plt.show()


if __name__ == "__main__":
    num_threads_values = [2, 4, 8, 16]
    for num_threads in num_threads_values:
        filename = FOLDER + f"matrix\\Working_Time\\Time_{num_threads}.txt"
        sizes, times = read_data(filename)
        plot_dependency(sizes, times, num_threads)
