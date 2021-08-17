/*В-20. 
Порядок: по возрастанию элементов. Методы: шейкера, сортировка прямым слиянием, 
сортировка естественным слиянием, поразрядная сортировка. N1=50000, N2=90000, N3=120000, 
N4=150000. Критерий – количество присваиваний.*/

#include <iostream>
#include <fstream> 
#include <ctime>

using namespace std;

struct testAlgorithm {
	int* mass;
	int* upMass;
	int* z;
	int n;
	float startTime;
	float endTime;
	float timeShaker;
	float timeMergeNonRecurs;
	float timeNaturalMerge;
	float timeCounting;
	long long int flagShaker;
	long long int flagMergeNonRecurs;
	long long int flagNaturalMerge;
	long long int flagCounting;
	long long int otherChanges;
	long int needMemory;

	void upToDown() {
		z = new int[n];

		for (int i = 0; i < n; i++)
			z[i] = upMass[i];
	}

	void differentToDown() {
		for (int i = 0; i < n; i++)
			z[i] = mass[i];
	}

	void choice() {
		cout << "Выберите количество цифр для сортировки:" << endl;
		cout << "1. 50.000" << endl;
		cout << "2. 90.000" << endl;
		cout << "3. 120.000" << endl;
		cout << "4. 150.000" << endl;
		int change;
		cin >> change;
		switch (change) {
		case 1: {
			cout << "Вы выбрали пункт '1. 50.000'" << endl;
			n = 12;
			mass = new int[n];
			loadFile();
			break;
		}
		case 2: {
			cout << "Вы выбрали пункт '2. 90.000'" << endl;
			n = 90.000;
			mass = new int[n];
			loadFile();
			break;
		}
		case 3: {
			cout << "Вы выбрали пункт '3. 120.000'" << endl;
			n = 120000;
			mass = new int[n];
			loadFile();
			break;
		}
		case 4: {
			cout << "Вы выбрали пункт '4. 150.000'" << endl;
			n = 150000;
			mass = new int[n];
			loadFile();
			break;
		}
		}
	}

	void loadFile() {
		ifstream file("test1.txt");
		if (!file) {
			cout << "Ошибка открытия файла" << endl;
			exit(0);
		}
		for (int i = 0; i < n; i++)
			file >> mass[i];

		file.close();
	}

	void Shell() {

		upMass = new int[n];

		for (int i = 0; i < n; i++)
			upMass[i] = mass[i];

		int m, x, j, i, k;
		const int t = log(n - 1) / log(3);
		int* h = new int[t];

		h[t - 1] = 1;

		for (m = t - 2; m >= 0; m--)
			h[m] = h[m + 1] * 3 + 2;

		for (m = 0; m < t; m++) {
			k = h[m];
			for (i = k; i < n; i++) {
				x = upMass[i];
				j = i - k;
				while (j >= 0 && x < upMass[j]) {
					upMass[j + k] = upMass[j];
					j -= k;
				}
				upMass[j + k] = x;
			}
		}
		/*for (int i = 0; i < n; i++)
			cout << "Shell[" << i << "] " << z[i] << endl;*/
		delete[] h;
		system("Pause");
	}

	/// --------------------------------->
	/// ---------------------------------> Шейкерная сортировка
	/// --------------------------------->

	void Swap(int* Mas, int i)
	{
		int temp;
		temp = Mas[i];
		Mas[i] = Mas[i - 1];
		Mas[i - 1] = temp;
		flagShaker += 3;
	}

	void ShakerSort() {
		int Left, Right, i;
		Left = 1;
		Right = n - 1;
		int t = 0;
		int g = 0;
		int flagReturn = 0;
		flagShaker = 0;
		otherChanges = 0;
		needMemory = 0;

		startTime = clock();
		while (Left <= Right)
		{
			for (i = Right; i >= Left; i--) {
				if (z[i - 1] < z[i]) {
					
					//cout << "Ключ " << z[i] << " повторяется" << endl;
					Swap(z, i);
					otherChanges++;
				}
				for (int j = 0; j < t; j++) {
					if (z[j] == z[i])
						flagReturn++;
				}
			}
			Left++;
			t++;
			
			for (i = Left; i <= Right; i++) {
					//cout << "Ключ " << z[i] << " повторяется" << endl;
				if (z[i - 1] < z[i]) {

					Swap(z, i);
					otherChanges++;
				}
				for (int j = 0; j < g; j++) {
					if (z[j] == z[i])
						//j = g;
						flagReturn++;
				}
			}
			Right--;
			g++;
		}
		endTime = clock();
		timeShaker = (endTime - startTime) / 1000;

		needMemory = sizeof(Left) + sizeof(Right) + sizeof(i);

		for (int i = 0; i < 5; i++)
			cout << "shaker[" << i << "] " << z[i] << endl;

		cout << "Количество времени, которое потребовалось для обработки массива шейкером: " << timeShaker << " секунд" << endl;
		cout << "Количество присваиваний равно: " << flagShaker << endl;
		cout << "Количество вспомогательных изменений равно: " << otherChanges << endl;
		cout << "Количество использования дополнительно памяти " << needMemory << " байт" << endl;
		cout << "Количество повторяющихся ключей: " << flagReturn << endl;
		system("Pause");
	}

	/// --------------------------------->
	/// ---------------------------------> Прямым слиянием
	/// --------------------------------->

	void mergeSortNonRecursive()
	{
		int* temp = new int[n];	/* вспомогательный массив */
		int* p1, 	/* адрес элемента в одной серии */
			* p2, 	/* адрес элемента в другой серии */
			* p3, 	/* адрес элемента в формируемом массиве */
			length = 1, 		/* длина серии */
			len1 = 0, len2 = 0, 	/* текущие длины серий */
			* end_a,				/* адрес конца массива a */
			* end_temp;			/* адрес конца массива temp */
		char flag = 0;			/* признак того, откуда куда идет перезапись: 0 - из сортируемого массива во вспомогательный, 1 - из вспомогательного в исходный */
		flagMergeNonRecurs = 0;
		otherChanges = 0;
		needMemory = 0;
		int flagReturn = 0;

		startTime = clock();
		while (length < n)	/* пока длина серии меньше длины массива */
		{
			p1 = z;
			p2 = z + length;
			p3 = temp;
			end_a = z + n;
			end_temp = temp + n; /* указатели ставим на стартовые позиции */
			otherChanges += 5;

			while (p3 < end_temp)			/* пока переписаны не все элементы массива */
			{
				len1 = 0;			/* в первой серии всегда будут элементы, устанавливаем счетчик в 0 */
				if (p2 < end_a) 	/* если на вторую серию элементов в массиве хватает, */
					len2 = 0;		/* то счетчик тоже устанавливаем в 0 */
				else
					len2 = length;	/* а если элементов не хватает, то серия не будет принимать участие в переписывании */

				while (len1 < length && len2 < length && p2 < end_a)   /* пока в обеих сериях есть элементы */
				{
					if (*p1 > *p2) {						/* если значение в первой серии не больше, чем во второй, */
						*p3++ = *p1++, len1++;
						otherChanges++;
					}										/* то переписываем его в другой массив, увеличиваем счетчик изъятых элементов */
					else if(*p1 == *p2){						/* если значение в первой серии не больше, чем во второй, */
							*p3++ = *p1++, len1++;
							flagReturn++;
							otherChanges++;
						}
					else {
						*p3++ = *p2++, len2++;			/* иначе переписываем значение из второй серии */
						otherChanges++;
					}
				}
				while (len1 < length && p3 < end_temp) { 	/* пока первая серия не пуста */
					*p3++ = *p1++, len1++;					/* переписываем ее до конца во второй массив */
					otherChanges++;
				}
				while (len2 < length && p3 < end_temp) {   	/* пока вторая серия не пуста */
					*p3++ = *p2++, len2++;					/* переписываем ее до конца во второй массив */
					otherChanges++;
				}
				p1 += length; 			/* переставляем указатели на первые элементы следующих серий */
				p2 += length;
				flagMergeNonRecurs += 2;
			}
			length *= 2;			/* увеличиваем длину серии вдвое */
			p3 = z;			/* меняем местами входной и выходной массивы */
			z = temp;
			temp = p3;
			flag = !flag; 	/* меняем значение флага */
			otherChanges += 5;
		}
		endTime = clock();

		timeMergeNonRecurs = (endTime - startTime) / 1000;
		needMemory = sizeof(temp) + sizeof(p1) + sizeof(p2) + sizeof(p3) + sizeof(length) + sizeof(len1) + sizeof(len2) + sizeof(end_a) + sizeof(end_temp) + sizeof(flag);

		for (int i = 0; i < 5; i++)
			cout << "z[" << i << "] " << z[i] << endl;

		cout << "Количество времени, которое потребовалось для обработки массива прямым слиянием: " << timeMergeNonRecurs << " секунд" << endl;
		cout << "Количество присваиваний равно: " << flagMergeNonRecurs << endl;
		cout << "Количество вспомогательных изменений равно: " << otherChanges << endl;
		cout << "Количество использования дополнительно памяти " << needMemory << " байт" << endl;
		cout << "Количество повторяющихся ключей: " << flagReturn << endl;

		if (flag)	/* если флаг установлен, то упорядоченная последовательность находится во вспомогательном массиве */
		{
			for (p1 = temp, p2 = z; p1 < temp + n; *p1++ = *p2++);	/* переписываем ее в исходный */
																	/* освобождаем память от вспомогательного массива */
		}
		else
			delete[] temp;	/* иначе на вспомогательный массив указыает temp, освобождаем память от вспомогательного массива */
		system("Pause");
	}

	/// --------------------------------->
	/// ---------------------------------> Естественным слиянием
	/// --------------------------------->

	void NaturalMergeSort()
	{
		int split;                   /* индекс, по которому делим массив */
		int last, end, i, * p = z, * tmp;
		char flag = 0, sorted = 0;
		int pos1, pos2, pos3;
		tmp = new int[n];
		flagNaturalMerge = 0;
		otherChanges = 0;
		needMemory = 0;
		int flagReturn = 0;
		int t = 0;

		startTime = clock();
		do {                /* если есть более 1 элемента */
			end = n;
			pos2 = pos3 = 0;
			do
			{
				p += pos2; end = n - pos3;
				for (split = 1; split < end && p[split - 1] >= p[split]; split++); /*первая серия*/
				if (split == n) { sorted = 1; break; }
				pos1 = 0; pos2 = split;
				while (pos1 < split && pos2 < end) 	/*идет слияние, пока есть хоть один элемент в каждой серии*/
				{
					if (p[pos1] > p[pos2]) {
						tmp[pos3++] = p[pos1++];
						otherChanges++;
					}
					else
					{
						tmp[pos3++] = p[pos2++];		/* одна последовательность закончилась - копировать остаток другой в конец буфера */
						otherChanges++;
						if (p[pos2] > p[pos2 - 1])
							break;

						for (int i = 0; i < t; i++) {
							if (p[pos1] == p[i])
								flagReturn++;
						}
						t++;
					}
				}

				while (pos2 < end && tmp[pos3 - 1] >= p[pos2]) {  			 /* пока вторая последовательность не пуста */
					tmp[pos3++] = p[pos2++];
					flagNaturalMerge++;
				}
				while (pos1 < split) {  		/* пока первая последовательность не пуста */
					tmp[pos3++] = p[pos1++];
					flagNaturalMerge++;
				}
			} while (pos3 < n);
			
			if (sorted) 
				break;
			p = tmp;
			tmp = z;
			z = p;
			flag = !flag;
			otherChanges += 4;
		} while (split < n);
		endTime = clock();

		needMemory = sizeof(split) + sizeof(last) + sizeof(end) + sizeof(i) + sizeof(p) + sizeof(p) + sizeof(tmp) + sizeof(flag) + sizeof(sorted) + sizeof(pos1) + sizeof(pos2) + sizeof(pos3) + sizeof(tmp);
		for (int i = 0; i < 5; i++)
			cout << "z[" << i << "] " << z[i] << endl;
		delete[] tmp;

		timeNaturalMerge = (endTime - startTime) / 1000;
		cout << "Количество времени, которое потребовалось для обработки массива естественным слиянием: " << timeNaturalMerge << " секунд" << endl;
		cout << "Количество присваиваний равно: " << flagNaturalMerge << endl;
		cout << "Количество вспомогательных изменений равно: " << otherChanges << endl;
		cout << "Количество использования дополнительно памяти " << needMemory << " байт" << endl;
		cout << "Количество повторяющихся ключей: " << flagReturn << endl;

		system("Pause");

	}

	/// --------------------------------->
	/// ---------------------------------> Подсчётом
	/// --------------------------------->

	void CountingSort()
	{
		int max = INT_MIN;
		int min = INT_MAX;
		int L;
		int k;
		flagCounting = 0;
		otherChanges = 0;
		needMemory = 0;

		startTime = clock();
		for (int i = 0; i < n; i++)
		{
			if (z[i] > max) {
				max = z[i];
				otherChanges++;
			}
			if (z[i] < min) {
				min = z[i];
				otherChanges++;
			}
		}
		L = max - min + 1;
		int* B = new int[L];

		for (int i = 0; i < L; i++)
		{
			B[i] = 0;
			otherChanges++;
		}

		for (int i = 0; i < n; i++)
		{
			B[z[i] - min]++;
			otherChanges++;
		}

		k = 0;
		for (int i = 0; i < L; i++)
		{
			for (int j = 0; j < B[i]; j++)
			{
				z[k++] = (i - max) * -1;
				flagCounting++;
			}
		}
		endTime = clock();
		timeCounting = (endTime - startTime) / 1000;
		needMemory = sizeof(min) + sizeof(max) + sizeof(L) + sizeof(k);

		for (int i = 0; i < 5; i++)
			cout << "z[" << i << "] " << z[i] << endl;

		cout << "Количество времени, которое потребовалось для обработки массива подсчётом: " << timeCounting << " секунд" << endl;
		cout << "Количество присваиваний равно: " << flagCounting << endl;
		cout << "Количество вспомогательных изменений равно: " << otherChanges << endl;
		cout << "Количество использования дополнительно памяти " << needMemory << " байт" << endl;

		system("Pause");

		delete[] B;
	}

	~testAlgorithm() {
		delete[] mass;
		delete[] z;
		cout << "Вызван деструктор" << endl;
	}
};

int main()
{
	setlocale(LC_ALL, "ru");

	testAlgorithm a;

	int change;

	a.choice();
	a.Shell();

	do {
		cout << "1. Сортировка методом шейкера" << endl;
		cout << "2. Сортировка методом прямого слияния" << endl;
		cout << "3. Сортировка методом естественного слияния" << endl;
		cout << "4. Сортировка методом подсчёта" << endl;
		cout << "5. Изменение количества цифр" << endl;
		cout << "0. Выход" << endl;

		cin >> change;

		switch (change) {
		case 1: {
			cout << endl << "Сортировка методом шейкера" << endl;
			cout << endl << "Сортировка массива, упорядоченного по возрастанию, по убыванию" << endl;
			a.upToDown();
			a.ShakerSort();
			cout << endl << "Сортировка неупорядоченного массива по убыванию" << endl;
			a.differentToDown();
			a.ShakerSort();
			cout << endl << "Сортировка массива, упорядоченного по убыванию, по убыванию" << endl;
			a.ShakerSort();
			cout << endl << endl << "Запомните результаты, после нажания любой клавиши очистится консоль!" << endl;
			system("Pause");
			system("CLS");
			break;
		}
		case 2: {
			cout << endl << "Сортировка методом прямого слияния" << endl;
			cout << endl << "Сортировка массива, упорядоченного по возрастанию, по убыванию" << endl;
			a.upToDown();
			a.mergeSortNonRecursive();
			cout << endl << "Сортировка неупорядоченного массива по убыванию" << endl;
			a.differentToDown();
			a.mergeSortNonRecursive();
			cout << endl << "Сортировка массива, упорядоченного по убыванию, по убыванию" << endl;
			a.mergeSortNonRecursive();
			cout << endl << endl << "Запомните результаты, после нажания любой клавиши очистится консоль!" << endl;
			system("Pause");
			system("CLS");
			break;
		}
		case 3: {
			cout << endl << "Сортировка методом естественного слияния" << endl;
			cout << endl << "Сортировка массива, упорядоченного по возрастанию, по убыванию" << endl;
			a.upToDown();
			a.NaturalMergeSort();
			cout << endl << "Сортировка неупорядоченного массива по убыванию" << endl;
			a.differentToDown();
			a.NaturalMergeSort();
			cout << endl << "Сортировка массива, упорядоченного по убыванию, по убыванию" << endl;
			a.NaturalMergeSort();
			cout << endl << endl << "Запомните результаты, после нажания любой клавиши очистится консоль!" << endl;
			system("Pause");
			system("CLS");
			break;
		}
		case 4: {
			cout << endl << "Сортировка методом подсчёта" << endl;
			cout << endl << "Сортировка массива, упорядоченного по возрастанию, по убыванию" << endl;
			a.upToDown();
			a.CountingSort();
			cout << endl << "Сортировка неупорядоченного массива по убыванию" << endl;
			a.differentToDown();
			a.CountingSort();
			cout << endl << "Сортировка массива, упорядоченного по убыванию, по убыванию" << endl;
			a.CountingSort();
			cout << endl << endl << "Запомните результаты, после нажания любой клавиши очистится консоль!" << endl;
			system("Pause");
			system("CLS");
			break;
		}
		case 5: {
			a.choice();
			a.Shell();
			break;
		}
		}
	} while (change != 0);

	return 0;
}
