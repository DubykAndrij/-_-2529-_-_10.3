#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdio>
#include <windows.h>

using namespace std;

struct Student {
    string surname;
    string initials;
    int grades[5];
};

void CreateFile(char* fname);
void PrintFile(char* fname);
void AppendFile(char* fname);
void ShowByCondition(char* fname, int conditionType);
void RemoveStudentsWithBadGrades(char* fname);
bool HasTwoOrMoreTwos(const Student& s);
int Menu();

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    char fname[100];
    cout << "Введіть ім'я файлу: ";
    cin >> fname;

    while (true) {
        switch (Menu()) {
        case 1: CreateFile(fname); break;
        case 2: PrintFile(fname); break;
        case 3: AppendFile(fname); break;
        case 4: ShowByCondition(fname, 5); break; 
        case 5: ShowByCondition(fname, 3); break; 
        case 6: ShowByCondition(fname, 2); break; 
        case 7: ShowByCondition(fname, 22); break; 
        case 8: RemoveStudentsWithBadGrades(fname); break;
        case 0: return 0;
        default: cout << "Помилка вводу!" << endl;
        }
    }
}

int Menu() {
    int ch;
    cout << "\n--- МЕНЮ ---\n";
    cout << "1. Створити список (новий файл)\n";
    cout << "2. Переглянути весь список\n";
    cout << "3. Поповнити список\n";
    cout << "4. Студенти з оцінками тільки '5'\n";
    cout << "5. Студенти, що мають трійки\n";
    cout << "6. Студенти з хоча б однією двійкою\n";
    cout << "7. Студенти з більш ніж однією двійкою\n";
    cout << "8. Вилучити студентів з >1 двійкою\n";
    cout << "0. Вихід\n";
    cout << "Ваш вибір: ";

    while (!(cin >> ch)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Введіть число!: ";
    }
    return ch;
}

void WriteStudent(ofstream& fout) {
    Student s;
    cout << "Прізвище: "; cin >> s.surname;
    cout << "Ініціали: "; cin >> s.initials;
    cout << "Оцінки (5 предметів): ";
    for (int i = 0; i < 5; i++) {
        while (!(cin >> s.grades[i]) || s.grades[i] < 2 || s.grades[i] > 5) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Оцінка має бути від 2 до 5: ";
        }
    }
    fout << s.surname << " " << s.initials;
    for (int i = 0; i < 5; i++) fout << " " << s.grades[i];
    fout << endl;
}

void CreateFile(char* fname) {
    ofstream fout(fname);
    char ch;
    do {
        WriteStudent(fout);
        cout << "Продовжити? (y/n): "; cin >> ch;
    } while (ch == 'y' || ch == 'Y');
    fout.close();
}

void AppendFile(char* fname) {
    ofstream fout(fname, ios::app);
    WriteStudent(fout);
    fout.close();
}

void PrintFile(char* fname) {
    ifstream fin(fname);
    if (!fin) { cout << "Файл не знайдено!\n"; return; }

    Student s;
    cout << "\n" << left << setw(15) << "Прізвище" << setw(10) << "Ініціали" << "Оцінки" << endl;
    while (fin >> s.surname >> s.initials) {
        cout << left << setw(15) << s.surname << setw(10) << s.initials;
        for (int i = 0; i < 5; i++) {
            fin >> s.grades[i];
            cout << s.grades[i] << " ";
        }
        cout << endl;
    }
    fin.close();
}

void ShowByCondition(char* fname, int conditionType) {
    ifstream fin(fname);
    Student s;
    bool found = false;

    while (fin >> s.surname >> s.initials) {
        int twos = 0, threes = 0, fives = 0;
        for (int i = 0; i < 5; i++) {
            fin >> s.grades[i];
            if (s.grades[i] == 2) twos++;
            if (s.grades[i] == 3) threes++;
            if (s.grades[i] == 5) fives++;
        }

        bool match = false;
        if (conditionType == 5 && fives == 5) match = true;
        else if (conditionType == 3 && threes > 0) match = true;
        else if (conditionType == 2 && twos > 0) match = true;
        else if (conditionType == 22 && twos > 1) match = true;

        if (match) {
            cout << s.surname << " " << s.initials << endl;
            found = true;
        }
    }
    if (!found) cout << "Студентів не знайдено.\n";
    fin.close();
}

bool HasTwoOrMoreTwos(const Student& s) {
    int count = 0;
    for (int i = 0; i < 5; i++) {
        if (s.grades[i] == 2) count++;
    }
    return count > 1;
}

void RemoveStudentsWithBadGrades(char* fname) {
    ifstream fin(fname);
    ofstream temp("temp.txt");
    Student s;

    while (fin >> s.surname >> s.initials) {
        for (int i = 0; i < 5; i++) fin >> s.grades[i];

        if (!HasTwoOrMoreTwos(s)) {
            temp << s.surname << " " << s.initials;
            for (int i = 0; i < 5; i++) temp << " " << s.grades[i];
            temp << endl;
        }
    }

    fin.close();
    temp.close();

    remove(fname);
    rename("temp.txt", fname);
    cout << "Дані оновлено.\n";
}