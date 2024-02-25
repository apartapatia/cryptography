import matplotlib.pyplot as plt

# Данные
bits = [8, 10, 12, 14, 16]
average_collision_complexity = [265.26, 981.34, 1376.05, 1841.07, 2013.68]
average_second_preimage_complexity = [537.642, 1962.68, 2652.1, 3682.14, 4027.36]

# Построение графика
plt.plot(bits, average_second_preimage_complexity, marker='o', label='Average Second Pre-image Complexity')
plt.plot(bits, average_collision_complexity, marker='o', label='Average Collision Complexity')

# Добавление заголовка и меток осей
plt.title('Complexity vs. Bits')
plt.xlabel('Bits')
plt.ylabel('Complexity')

# Добавление легенды
plt.legend()

# Отображение графика
plt.grid(True)
plt.show()
