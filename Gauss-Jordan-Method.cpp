
/*
example matrix 3x5

3 5
1 1 1 1 1
0 0 0 1 1
0 0 1 1 1
*/

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

void printMatrix(long long*** matrix, int n, int m) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (matrix[i][j][1] == 1)
				cout << "\t\t" << matrix[i][j][0];
			else
				cout << "\t\t" << matrix[i][j][0] << "/" << matrix[i][j][1];
		}
		cout << endl;
	}
	cout << endl;
}

long long nodFraction(long long x, long long y) {
	if (y == 0) return x;
	return nodFraction(y, x % y);
}

void reduceFraction(long long* numeration, long long* denumeration) {
	long long x = max(abs(*numeration), abs(*denumeration));
	long long y = min(abs(*numeration), abs(*denumeration));
	int sign = 1;
	if (*numeration < 0 && *denumeration >= 0 || *denumeration < 0 && *numeration >= 0)
		sign = -1;
	long long nod = nodFraction(x, y);
	*numeration = sign * abs(*numeration / nod);
	*denumeration = abs(*denumeration / nod);
}

void reduceFraction(long long*** matrix, int n, int m) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			reduceFraction(&matrix[i][j][0], &matrix[i][j][1]);
		}
	}
}

bool valid(long long*** matrix, int n, int m) {
	for (int i = 0; i < n; i++) {
		bool allZero = true;
		for (int j = 0; j < m - 1; j++) {
			if (matrix[i][j][0] != 0) {
				allZero = false;
				break;
			}
		}
		if (allZero && matrix[i][m - 1][0] != 0) {
			cout << endl << "Решение отсутствует!" << endl;
			return false;
		}
	}
	return true;
}

void divRow(long long*** matrix, int row, int col, long long numeration, long long denumeration, int n, int m) {
	for (int j = col; j < m; j++) {
		matrix[row][j][0] *= denumeration;
		matrix[row][j][1] *= numeration;
		reduceFraction(&matrix[row][j][0], &matrix[row][j][1]);
	}
}

bool nullify(long long*** matrix, int exceptionRow, int startCol, int n, int m) {
	bool zeroed = false;
	for (int i = 0; i < n; i++) {
		if (i == exceptionRow) continue;
		if (matrix[i][startCol][0] == 0) continue;
		zeroed = true;
		long long subtractorNumeration = -matrix[i][startCol][0];
		long long subtractorDenumeration = matrix[i][startCol][1];
		for (int j = startCol; j < m; j++) {
			matrix[i][j][0] = matrix[i][j][0] * (matrix[exceptionRow][j][1] * subtractorDenumeration) + matrix[exceptionRow][j][0] * subtractorNumeration * matrix[i][j][1];
			matrix[i][j][1] *= subtractorDenumeration * matrix[exceptionRow][j][1];
			reduceFraction(&matrix[i][j][0], &matrix[i][j][1]);
		}
	}
	return zeroed;
}

bool maxNumCol(long long*** matrix, int row, int col, int n, int m) {
	double maxNum = (double)matrix[row][col][0] / (double)matrix[row][col][1];
	int maxNumRow = -1;
	for (int i = row + 1; i < n; i++) {
		if ((double)matrix[i][col][0] / (double)matrix[i][col][1] > maxNum) {
			maxNum = (double)matrix[i][col][0] / (double)matrix[i][col][1];
			maxNumRow = i;
		}
	}
	if (maxNumRow != -1) {
		for (int j = col; j < m; j++) {
			swap(matrix[row][j][0], matrix[maxNumRow][j][0]);
			swap(matrix[row][j][1], matrix[maxNumRow][j][1]);
		}
		return true;
	}
	return false;
}

void printResult(long long*** matrix, int n, int m) {
	cout << "Ответ:" << endl;
	for (int i = 0; i < n; i++) {
		bool firstOne = false;
		for (int j = 0; j < m - 1; j++) {
			if (!firstOne && matrix[i][j][0] == 1 && matrix[i][j][1] == 1) {
				firstOne = true;
				cout << "X" << j + 1 << " = ";
				if (matrix[i][m - 1][1] == 1)
					cout << matrix[i][m - 1][0] << " ";
				else
					cout << matrix[i][m - 1][0] << "/" << matrix[i][m - 1][1] << " ";
			}
			else if (matrix[i][j][0] != 0) {
				if (matrix[i][j][0] > 0) {
					cout << "- ";
					if (matrix[i][j][0] != 1 || matrix[i][j][1] != 1) {
						if (matrix[i][j][1] == 1)
							cout << matrix[i][j][0];
						else
							cout << matrix[i][j][0] << "/" << matrix[i][j][1];
					}
					cout << "X" << j + 1 << " ";
				}
				else {
					cout << "+ ";
					if (matrix[i][j][0] != -1 || matrix[i][j][1] != 1) {
						if (matrix[i][j][1] == 1)
							cout << abs(matrix[i][j][0]);
						else
							cout << abs(matrix[i][j][0]) << "/" << matrix[i][j][1];
					}
					cout << "X" << j + 1 << " ";
				}
			}
		}
		if (firstOne)
			cout << endl;
	}
}

int main() {
	setlocale(LC_ALL, "ru");
	int n, m;
	cin >> n >> m;
	long long*** matrix = new long long** [n];
	for (int i = 0; i < n; i++) {
		matrix[i] = new long long* [m];
		for (int j = 0; j < m; j++) {
			matrix[i][j] = new long long[2] {1, 1};
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			string temp;
			cin >> temp;
			size_t pos = temp.find("/");
			if (pos != string::npos) {
				matrix[i][j][0] = stoi(temp.substr(0, pos));
				matrix[i][j][1] = stoi(temp.substr(pos + 1));
			}
			else
				matrix[i][j][0] = stoi(temp);
		}
	}
	reduceFraction(matrix, n, m);
	printMatrix(matrix, n, m);

	int current[2]{ 0, 0 };

	while (current[0] < n && current[1] < m - 1) {

		if (matrix[current[0]][current[1]][0] == 0) {
			int temp1 = current[0], temp2 = current[1];
			bool move = false;
			for (int k = temp1; k < n && temp2 < m - 1; k++) {
				if (matrix[k][temp2][0] != 0) {
					temp1 = k;
					move = true;
					break;
				}
				if (k == n - 1) {
					temp2++;
					k = temp1 - 1;
				}
			}

			if (!move)
				break;

			if (current[1] != temp1) {
				for (int j = 0; j < m; j++) {
					swap(matrix[current[0]][j][0], matrix[temp1][j][0]);
					swap(matrix[current[0]][j][1], matrix[temp1][j][1]);
				}

			}

			current[1] = temp2;

		}

		if (maxNumCol(matrix, current[0], current[1], n, m))
			printMatrix(matrix, n, m);

		if (matrix[current[0]][current[1]][0] != 1 || matrix[current[0]][current[1]][1] != 1) {
			divRow(matrix, current[0], current[1], matrix[current[0]][current[1]][0], matrix[current[0]][current[1]][1], n, m);
			printMatrix(matrix, n, m);
		}

		if (nullify(matrix, current[0], current[1], n, m))
			printMatrix(matrix, n, m);

		current[0]++;
		current[1]++;
	}

	if (valid(matrix, n, m))
		printResult(matrix, n, m);

	return 0;
}