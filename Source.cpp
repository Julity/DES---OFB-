#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>  
#include <iostream>
#include <bitset>
#include <vector>
#include <random>
#include <ctime>

using namespace std;
const int S[8][4][16] = { {{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7}
,{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8}
,{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0}
,{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}},

{{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10}
,{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5}
,{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15}
,{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}},

{{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8}
,{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1}
,{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7}
,{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}},

{{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15}
,{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9}
,{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4}
,{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}},

{{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9}
,{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6}
,{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14}
,{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},

{{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11}
,{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8}
,{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6}
,{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}},

{{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1}
,{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6}
,{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2}
,{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}},

{{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7}
,{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2}
,{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8}
,{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}} };

string charToBinary(char ch) {
	return bitset<8>(static_cast<unsigned char>(ch)).to_string();
}
string textToBinary(const string& text) {
	stringstream result;
	for (char ch : text) {
		result << charToBinary(ch);
	}
	return result.str();
}
// Функция binaryToText
string binaryToText(const string& binaryText) {
	stringstream result;

	// Проходим по каждым 8 битам и конвертируем их в символы
	for (size_t i = 0; i < binaryText.length(); i += 8) {
		string byte = binaryText.substr(i, 8);
		char character = static_cast<char>(bitset<8>(byte).to_ulong());
		result << character;
	}

	return result.str();
}

//Деление н блоки по 64 бита
void splitInto64BitBlocks(const string& binaryText, vector<bitset<64>>& blocks) {
	size_t length = binaryText.length();
	size_t start = 0;

	while (start < length) {
		blocks.push_back(bitset<64>(binaryText.substr(start, 64)));
		start += 64;
	}
}
//формирование случайных строк
string generateRandomKeyIV() {
	const  string russianAlphabet = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
	const int alphabetSize = russianAlphabet.size();

	mt19937 generator(static_cast<unsigned int>(time(nullptr)));
	uniform_int_distribution<int> distribution(0, alphabetSize - 1);

	string keyIV;
	for (int i = 0; i < 8; ++i) {
		keyIV += russianAlphabet[distribution(generator)];
	}

	return keyIV;
}
//Перестановка
string Transformation(string key, int table[], int len) {
	string rez;
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < key.length(); j++) {
			if (j + 1 == table[i]) {
				rez += key[j];
				break;
			}
		}
	}

	return rez;
}
string Sdvig(string T, int n) {
	//n-величина сдвига
	string tmp, temp;
	for (int j = 0; j < n; j++) {
		tmp = T[0];
		for (int i = 1; i < T.length(); i++)
		{
			temp += T[i];

		}
		T = temp + tmp;
	}
	return T;
}
//получение 16 шт ключей по 48 бит
string* GetKey48(string temp) {
	string* keyround = new string[16];
	string key;
	string C, D;
	int table[] = { 57,49,41,33,25,17,9,1,58,50,42,34,26,18,
				   10,2,59,51,43,35,27,19,11,3,60,52,44,36,
				   63,55,47,39,31,23,15,7,62,54,46,38,30,22,
				   14,6,61,53,45,37,29,21,13,5,28,20,12,4 };
	//Цикл по количесву блоков =16

		//Перестановка, сжатие
	key = Transformation(temp, table, 56);
	//cout << "Перестановка и сжатие ключа до 56 битов: " << key << endl;

	C = key.substr(0, key.length() / 2);
	D = key.substr(key.length() / 2);
	for (int i = 0; i < 16; i++)
	{
		//Сдвиг
		if (i + 1 == 1 || i + 1 == 2 || i + 1 == 9 || i + 1 == 16)
		{
			C = Sdvig(C, 1);
			D = Sdvig(D, 1);
		}
		else {
			C = Sdvig(C, 2);
			D = Sdvig(D, 2);
		}
		//Объединение
		key = C + D;

		//P-бокс
		int table1[] = { 14,17,11,24,1,5,3,28,
			15,6,21,10,23,19,12,4,
			26,8,16,7,27,20,13,2,
			41,52,31,37,47,55,30,40,
			51,45,33,48,44,49,39,56,
			34,53,46,42,50,36,29,32 };

		key = Transformation(key, table1, 48);
		keyround[i] = key;
		//cout << "Ключ " << i << "го раунда: " << keyround[i] << endl;
	}
	return keyround;
}

string XOR_f(string IVR, string keyround) {

	string rez = "";
	// В функции XOR_f
	if (IVR.length() != keyround.length()) {
		cout << "Ошибка: длины строк не совпадают!" << endl;
		return ""; // или как-то обработать эту ошибку
	}

	for (int i = 0; i < IVR.length(); i++) {
		if (IVR[i] == keyround[i]) {
			rez += '0';
		}
		else {
			rez += '1';
		}
	}
	return rez;
}

vector<string> DivideString(const string& input) {
	vector< string> result;

	// Проверяем, что длина строки равна 48
	if (input.length() == 48) {
		// Разбиваем строку на 8 подстрок по 6 символов
		for (int i = 0; i < 48; i += 6) {
			result.push_back(input.substr(i, 6));
		}
	}
	else {
		cerr << "Ошибка: неверная длина строки, ожидается 48 символов." << endl;
	}

	return result;
}

//2->10
int binaryToDecimal(const string& binaryNumber) {
	int decimalNumber = 0;

	// Проходим по каждому символу в обратном порядке
	for (int i = binaryNumber.length() - 1; i >= 0; --i) {
		// Преобразуем символ в целочисленное значение ('0' -> 0, '1' -> 1)
		int bit = binaryNumber[i] - '0';

		// Добавляем в десятичное число вес очередного бита
		decimalNumber += bit * static_cast<int>(pow(2, binaryNumber.length() - 1 - i));
	}

	return decimalNumber;
}

string S_blocks(string IVR) {
	string rez = "";
	string k = "", n = "";
	//string* S = new string[8];
	//Делим на 8 блоков по 6 символов
	vector<string> block = DivideString(IVR);
	for (int i = 0; i < block.size(); ++i) {//8
		k = n = "";
		cout << "Подстрока " << i + 1 << ": " << block[i] << endl;
		for (int j = 1; j < 5; j++) k += block[i][j];
		n += block[i][0]; n += block[i][5];
		block[i] = bitset<4>(S[i][binaryToDecimal(n)][binaryToDecimal(k)]).to_string();
		cout << block[i] << endl;
		rez += block[i];
	}
	return rez;
}

//Шифрование IV DES
string DES(string key, string IV) {
	string* keyround = GetKey48(key);
	for (int i = 0; i < 16; i++)
		cout << "Ключ " << i << "го раунда: " << keyround[i] << endl;
	//Начальная перестановка
	int table[] = { 58,50,42,34,26,18,10,2,
		60,52,44,36,28,20,12,4,
		62,54,46,38,30,22,14,6,
		64,56,48,40,32,24,16,8,
		57,49,41,33,25,17,9,1,
		59,51,43,35,27,19,11,3,
		61,53,45,37,29,21,13,5,
		63,55,47,39,31,23,15,7 };
	IV = Transformation(IV, table, 64);
	cout << "\nНачальая престановка " << IV << endl;
	//Разбиение IV на 2 части: IVL, IVR - 32bit
	string IVL = IV.substr(0, IV.length() / 2);
	string IVR = IV.substr(IV.length() / 2);


	int table1[] = { 32,1,2,3,4,5,
		4,5,6,7,8,9,
		8,9,10,11,12,13
		,12,13,14,15,16,17,
		16,17,18,19,20,21,
		20,21,22,23,24,25,
		24,25,26,27,28,29,
		28,29,30,31,32,1 };
	for (int i = 0; i < 16; i++) {
		cout << "\nРаунд " << i << endl;
		//Расширение правой половины- 48bit
		IVR = Transformation(IVR, table1, 48);
		cout << "Расширение правой половины: " << IVR << endl;
		//XOR
		cout << "IVR: " << IVR.length() << "  Key: " << keyround[i].length() << endl;
		IVR = XOR_f(IVR, keyround[i]);
		cout << "XOR: " << IVR << endl;
		// S-блоки.
		IVR = S_blocks(IVR);//32bit
		//P-бокс
		int table2[] = { 16,7,20,21,29,12,28,17,
			1,15,23,26,5,18,31,10,
			2,8,24,14,32,27,3,9,
			19,13,30,6,22,11,4,25 };
		IVR = Transformation(IVR, table2, 32);
		//XOR с левой половиной.
		IVL = XOR_f(IVR, IVL);
		//Обмен левой и правой половин.
		IV = IVR + IVL;
	}
	//Конечная перестановка
	int table3[] = { 40,8,48,16,56,24,64,32,
		39,7,47,15,55,23,63,31,
		38,6,46,14,54,22,62,30,
		37,5,45,13,53,21,61,29,
		36,4,44,12,52,20,60,28,
		35,3,43,11,51,19,59,27,
		34,2,42,10,50,18,58,26,
		33,1,41,9,49,17,57,25 };
	IV = Transformation(IV, table3, 64);
	cout << "\nЗашифрованный IV : " << IV << endl << endl;
	return IV;
}

string newIV(string iv, int r) {
	string rez = "", temp = iv.substr(0, r);
	rez = iv.substr(r) + temp;
	return rez;
}

//Разбиение бинарной строки на под блоки по r
vector<string> SplitBinaryString(const string & binaryString, int r) {
	vector<string> substrings;

	for (size_t i = 0; i < binaryString.length(); i += r) {
		substrings.push_back(binaryString.substr(i, r));
	}
	return substrings;
}
int main() {
	string IVsh;
	setlocale(LC_ALL, "Russian");
	// Загружаем ключ и IV из файла
	ifstream keyIVFile("D:\\8сем\\ТЗИ\\TZI2lab\\TZI2lab\\keyIV.txt");
	if (!keyIVFile.is_open()) {
		cerr << "Unable to open file: keyIV.txt" << endl;
		return 1;
	}

	string key, iv;
	keyIVFile >> key >> iv;
	keyIVFile.close();

	// Переводим ключ и IV в двоичное представление
	string binaryKey = textToBinary(key);
	string binaryIV = textToBinary(iv);

	// Выводим результат
	cout << "\n\nКлюч из файла: " << key << "\n";
	cout << "Двоичное представление ключа: " << binaryKey << "\n\n";

	cout << "IV из файла: " << iv << "\n";
	cout << "Двоичное представление IV: " << binaryIV << "\n";

	// Загружаем зашифрованный текст из файла
	ifstream encryptedFile("D:\\8сем\\ТЗИ\\TZI2lab\\TZI2lab\\rez.txt");
	if (!encryptedFile.is_open()) {
		cerr << "Unable to open file: rez.txt" << endl;
		return 1;
	}

	stringstream encryptedBuffer;
	encryptedBuffer << encryptedFile.rdbuf();
	string encryptedText = encryptedBuffer.str();
	encryptedFile.close();

	// Выводим зашифрованный текст
	cout << "\n\nЗашифрованный текст из файла:\n" << encryptedText << "\n";

	size_t remainder = encryptedText.size() % 64;
	if (remainder != 0) {
		size_t padding = 64 - remainder;
		encryptedText.insert(0, padding, '0');
		cout << "Текст после добавления нулей для кратности 64:\n" << encryptedText << endl;
	}
	// Разбиваем двоичный код на блоки по 64 бита
	vector<bitset<64>> blocks;
	splitInto64BitBlocks(encryptedText, blocks);

	// Выводим блоки на экран
	cout << "Блоки по 64 бита:\n";

	for (size_t i = 0; i < blocks.size(); ++i) {
		cout << "Блок " << i + 1 << ": " << blocks[i] << "\n";


	}
	string RSHtext;// расшифрованный текст

	int n = blocks.size() / 8;//количество подблоков
	string IVshR; int r = 8;
	vector<string> RSHtextR;
	//цикл по kоличеству блоков
	for (int i = 0; i < blocks.size(); i++) {

		cout << "=========================================" << endl;

		RSHtextR = SplitBinaryString(blocks[i].to_string(), r);//блоки по  r  символов
		for (int t = 0; t < RSHtextR.size(); t++) {//цикл по  r  блокам
			//r=8
			cout << i << " блок " << t << " подблок: " << RSHtextR[t] << "\n_________________________________________________________" << endl;
			//Шифруем IV
			IVsh = DES(binaryKey, binaryIV);

			IVshR = IVsh.substr(0, r);
			cout << "Первые 8 бит IV:" << IVshR << endl;
			cout << "XOR куска блока текста и  IV=8бит: " << XOR_f(RSHtextR[t], IVshR) << endl;


			RSHtext += XOR_f(RSHtextR[t], IVshR);
			binaryIV = newIV(IVsh, r);
			cout << " Новый IV: " << binaryIV << endl;
		}
	}
	cout << "\n\nРАСШИФРОВАННЫЙ ТЕКСТ: " << RSHtext <<"\n"<< binaryToText(RSHtext) << endl;
	ofstream outputFile("rez.txt");
	// Проверяем, успешно ли открыт файл
	if (outputFile.is_open()) {
		// Записываем текст в файл
		outputFile << binaryToText(RSHtext);
		// Закрываем файл
		outputFile.close();

		cout << "Текст успешно записан в файл.\n";
	}
	else {
		cerr << "Ошибка открытия файла для записи.\n";
	}
	return 0;
}
