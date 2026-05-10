#ifndef COLORING_H
#define COLORING_H

// Реализация алгоритма DSATUR для раскраски неориентированного графа.
// int* dsatur_coloring(int** adj, int N, int& colors_used)
//   - adj: матрица смежности (симметричная), 0/1.
//   - N: число вершин.
//   - colors_used: выходной параметр — число использованных цветов.
// Возвращает: массив цветов длины N (цвета нумеруются с 1), или nullptr при ошибке.
// Память для возвращаемого массива выделяется внутри функции; вызывающий обязан delete[].
int* dsatur_coloring(int** adj, int N, int& colors_used);

#endif
