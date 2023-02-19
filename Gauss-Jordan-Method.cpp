
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

void printResult(long long*** matrix, int n, int m, long long **basis) {
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
				basis[j][0] = matrix[i][m - 1][0];
				basis[j][1] = matrix[i][m - 1][1];
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
		if (firstOne) {
			cout << endl;
			
		}
	}

	cout << endl << "Базисное решение: (";

	for (int i = 0; i < m - 1; i++) {
		if (basis[i][1] != 1)
			cout << basis[i][0] << "/" << basis[i][1];
		else
			cout << basis[i][0];
		if(i != m - 2)
			cout << "; ";
	}
	cout << ")" << endl;
}

void deleteZeroRows(long long ***matrix, int *n, int m) {
	for (int i = 0; i <	*n; i++) {
		bool isZero = true;
		for (int j = 0; j < m; j++) {
			if (matrix[i][j][0] != 0) {
				isZero = false;
				break;
			}
		}
	
		if (isZero) {
			for (int k = i; k < *n - 1; k++) {
				for (int j = 0; j < m; j++) {
					swap(matrix[k][j][0], matrix[k + 1][j][0]);
					swap(matrix[k][j][1], matrix[k + 1][j][1]);
				}
			}
			(*n)--;
		}
	}
}

long long factorial(long long num) {
	if (num == 0) return 1;
	else return num * factorial(num - 1);
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
	deleteZeroRows(matrix, &n, m);
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

			if (current[0] != temp1) {
				for (int j = 0; j < m; j++) {
					swap(matrix[current[0]][j][0], matrix[temp1][j][0]);
					swap(matrix[current[0]][j][1], matrix[temp1][j][1]);
				}

			}

			current[1] = temp2;
			deleteZeroRows(matrix, &n, m);
			printMatrix(matrix, n, m);

		}

		if (maxNumCol(matrix, current[0], current[1], n, m))
			printMatrix(matrix, n, m);

		if (matrix[current[0]][current[1]][0] != 1 || matrix[current[0]][current[1]][1] != 1) {
			divRow(matrix, current[0], current[1], matrix[current[0]][current[1]][0], matrix[current[0]][current[1]][1], n, m);
			printMatrix(matrix, n, m);
		}

		if (nullify(matrix, current[0], current[1], n, m)) {
			deleteZeroRows(matrix, &n, m);
			printMatrix(matrix, n, m);
		}

		current[0]++;
		current[1]++;
	}

	if (valid(matrix, n, m)) {
		long long** basis = new long long*[m - 1];
		for (int i = 0; i < m - 1; i++)
			basis[i] = new long long[2] {0, 1};
		printResult(matrix, n, m, basis);
		
		cout << endl << "Ранг матрицы = " << n << endl;
		cout << endl << "Количество различных базисных решений:\n" << "C(" << m - 1 << ", " << n << ") = " << m - 1 << "! / " << n << "! * (" << m - 1 << "! - " << n << "!) = ";
		long long countBasis = factorial(m - 1) / (factorial(n) * factorial(m - 1 - n));
		cout << countBasis << endl;


		long long*** bases = new long long** [countBasis];
		for (long long i = 0; i < countBasis; i++) {
			bases[i] = new long long* [m];
			bases[i][0] = new long long[n]; // базисные индексы
			for (long long j = 1; j < m; j++) {
				bases[i][j] = new long long[2] {0, 1};
			}
		}

		for (long long i = 0; i < n; i++) {
			bases[0][0][i] = i;
		}

		long long p = n - 1;

		for (long long i = 1; i < countBasis; i++) {
			for (long long j = 0; j < n; j++) {
				bases[i][0][j] = bases[i - 1][0][j];
			}
			bases[i][0][p]++;
			if (bases[i][0][p] == m - 1 || bases[i][0][p] == bases[i][0][p + 1]) {
				bases[i][0][p]--;
				p--;
				while (bases[i][0][p] == bases[i][0][p + 1] - 1 && p > 0)
					p--;
				if (bases[i][0][p] == bases[i][0][p + 1] - 1)
					break;
				bases[i][0][p]++;
				for (long long j = p + 1; j < n; j++) {
					bases[i][0][j] = bases[i][0][j - 1] + 1;
				}
				p = n - 1;
			}
			
		}


		for (long long c = 0; c < countBasis; c++) {

			cout << endl << c + 1 << ")";
			for (long long q = 0; q < n; q++) {
				cout << " X" << bases[c][0][q] + 1;
			}
			cout << endl;

			long long*** matrix2 = new long long** [n];
			for (int i = 0; i < n; i++) {
				matrix2[i] = new long long* [m];
				for (int j = 0; j < m; j++) {
					matrix2[i][j] = new long long[2] {1, 1};
				}
			}

			for (int i = 0; i < n; i++) {
				for (int j = 0; j < m; j++) {
					matrix2[i][j][0] = matrix[i][j][0];
					matrix2[i][j][1] = matrix[i][j][1];
				}
			}
			bool error = false;
			bool* busyRows = new bool[n] {};
			for (long long j = 0; j < n; j++) {
				bool isFound = false;
				for (long long i = 0; i < n; i++) {
					if (busyRows[i] == 1) continue;
					if (matrix2[i][bases[c][0][j]][0] != 0) {
						isFound = true;
						busyRows[i] = 1;
						if (matrix2[i][bases[c][0][j]][0] != 1 || matrix2[i][bases[c][0][j]][1] != 1) {
							divRow(matrix2, i, bases[c][0][j], matrix2[i][bases[c][0][j]][0], matrix2[i][bases[c][0][j]][1], n, m);
						}
						nullify(matrix2, i, bases[c][0][j], n, m);
						break;
					}
				}
				if (!isFound) {
					error = true;
					cout << "Столбцы линейно зависимы" << endl;
					break;
				}

			}

			printMatrix(matrix2, n, m);
			cout << endl << endl;

			for (long long j = 0; j < n; j++) {
				for (long long i = 0; i < n; i++) {
					if (matrix2[i][bases[c][0][j]][0] == 1) {
						bases[c][bases[c][0][j] + 1][0] = matrix2[i][m - 1][0];
						bases[c][bases[c][0][j] + 1][1] = matrix2[i][m - 1][1];
					}
				}
			}


			if (!error) {
				cout << endl << "Базис: (";

				for (int i = 1; i < m; i++) {
					if (bases[c][i][1] != 1)
						cout << bases[c][i][0] << "/" << bases[c][i][1];
					else
						cout << bases[c][i][0];
					if (i != m - 1)
						cout << "; ";
				}
				cout << ")" << endl;

			}
		}

	}
	else
		return 0;

}

/*
бригада № 6

4 6
12 -16 4 3 -7 -56
-18 15 26 34 27 77
-17 24 11 20 17 79
-9 25 15 32 10 48

*/