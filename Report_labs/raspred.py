import random
import math
import matplotlib.pyplot as plt
import numpy as np

# Параметры распределения (как в C++ коде)
DEFAULT_CHAMP_MU = 1.0
DEFAULT_CHAMP_ALPHA = 1.57

def generate_champernaun_continuous(mu, alpha):
    """
    Генерирует случайное число по непрерывному распределению Чампернауна.
    Формула: mu + (1/alpha) * log(tan(pi * r / 2))
    r — равномерно распределённая величина на (0,1).
    """
    while True:
        r = random.random()          # r в (0,1)
        if 0.0 < r < 1.0:
            break
    return mu + (1.0 / alpha) * math.log(math.tan(math.pi * r / 2.0))

def generate_random_degree_champernaun(mu, alpha):
    """
    Генерирует целую степень (degree) по распределению Чампернауна.
    Округляет непрерывное значение до ближайшего целого и отсекает отрицательные в 0.
    """
    x = generate_champernaun_continuous(mu, alpha)
    deg = int(round(x))
    if deg < 0:
        deg = 0
    return deg

# Симуляция 1000 значений
num_samples = 1000
degrees = [generate_random_degree_champernaun(DEFAULT_CHAMP_MU, DEFAULT_CHAMP_ALPHA) for _ in range(num_samples)]

# Построение гистограммы
plt.figure(figsize=(10, 6))
# Гистограмма с целыми бинами (каждый бин соответствует одному целому числу)
bins = np.arange(min(degrees), max(degrees) + 2) - 0.5
plt.hist(degrees, bins=bins, edgecolor='black', alpha=0.7, rwidth=0.8)
plt.xlabel('Значение степени (degree)')
plt.ylabel('Количество (частота)')
plt.title(f'Гистограмма распределения Чампернауна\nμ = {DEFAULT_CHAMP_MU}, α = {DEFAULT_CHAMP_ALPHA}, n = {num_samples}')
plt.grid(axis='y', linestyle='--', alpha=0.7)

# Отображение подписей на оси X (целые значения)
plt.xticks(np.arange(min(degrees), max(degrees) + 1, step=1))

plt.show()