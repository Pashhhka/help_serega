#include <iostream>
#include <string>

using namespace std;

// ===== Класс исключения =====
class MyException {
public:
    string msg;
    MyException(string m) : msg(m) {}
};

// ===== Класс ФИО =====
class fio {
private:
    string name;
    string surname;
    string patronymic;

public:
    fio() {}

    fio(string n, string s, string p)
        : name(n), surname(s), patronymic(p) {} //инициализация полей класса

    bool operator==(const fio& other) const { //реализация оператора сравнения через сравнение соответсвующих полей
        return name == other.name &&
               surname == other.surname &&
               patronymic == other.patronymic;
    }

    friend istream& operator>>(istream& in, fio& f) { //реализация ввода значений полей с клавиатуры
        cout << "Введите имя: ";
        in >> f.name;
        cout << "Введите фамилию: ";
        in >> f.surname;
        cout << "Введите отчество: ";
        in >> f.patronymic;

        if (f.name.empty() || f.surname.empty())
            throw MyException("Ошибка ввода ФИО");

        return in;
    }

    void print() const {
        cout << surname << " " << name << " " << patronymic;
    }
};

// ===== Класс Shipment (поставка) =====
class shipment {
private:
    int day, month, year;

public:
    shipment() : day(1), month(1), year(2000) {} //у полей есть значения по умолчанию при созданиии объекта через пустой конструктор 

    shipment& operator=(const shipment& other) { //реализация операора присвоения через копирование полей
        day = other.day;
        month = other.month;
        year = other.year;
        return *this;
    }

    friend istream& operator>>(istream& in, shipment& s) { //реализация ввода значений полей с клавиатуры
        cout << "Введите дату продажи (день месяц год): ";
        in >> s.day >> s.month >> s.year;

        if (s.day < 1 || s.day > 31 ||
            s.month < 1 || s.month > 12 ||
            s.year < 2000)
            throw MyException("Некорректная дата"); //выброс исключения при неправильном вводе данных

        return in;
    }

    bool operator==(const shipment& other) const { //реализация оператора сравнения через сравнение соответсвующих полей
        return day == other.day &&
               month == other.month &&
               year == other.year;
    }

    void print() const {
        cout << day << "." << month << "." << year;
    }
};

// ===== Класс Supplier (поставщик) =====
//Данный класс демонстрирует работу с объектом класса как с полем другого класса
class supplier {
private:
    fio person;
    shipment saleDate;

public:
    supplier() {}

    supplier& operator=(const supplier& other) {//реализация операора присвоения через копирование полей
        person = other.person;
        saleDate = other.saleDate;
        return *this;
    }

    friend istream& operator>>(istream& in, supplier& s) {  //реализация ввода значений полей с клавиатуры
        in >> s.person;
        in >> s.saleDate;
        return in;
    }

    bool matchFio(const fio& f) const { //проверка на тезку
        return person == f;
    }

    bool matchDate(const shipment& d) const { //день рождение в один день
        return saleDate == d;
    }

    void print() const {
        person.print();
        cout << " | Дата продажи: ";
        saleDate.print();
        cout << endl;
    }
};

// ===== Главная функция =====
int main() {
    try {
        const int N = 3;
        supplier suppliers[N];

        cout << "=== Ввод поставщиков ===" << endl;
        for (int i = 0; i < N; i++) {
            cout << "\nПоставщик #" << i + 1 << endl;
            cin >> suppliers[i];
        }

        // ---- Поиск по ФИО ----
        fio searchFio;
        cout << "\n=== Поиск по ФИО ===" << endl;
        cin >> searchFio;

        supplier* resultFio = new supplier[N];
        int countFio = 0;

        for (int i = 0; i < N; i++) {
            if (suppliers[i].matchFio(searchFio)) {
                resultFio[countFio++] = suppliers[i];
            }
        }

        cout << "\nНайдено: " << countFio << endl;
        for (int i = 0; i < countFio; i++)
            resultFio[i].print();

        delete[] resultFio;

        // ---- Поиск по дате ----
        shipment searchDate;
        cout << "\n=== Поиск по дате продажи ===" << endl;
        cin >> searchDate;

        supplier* resultDate = new supplier[N];
        int countDate = 0;

        for (int i = 0; i < N; i++) {
            if (suppliers[i].matchDate(searchDate)) {
                resultDate[countDate++] = suppliers[i];
            }
        }

        cout << "\nНайдено: " << countDate << endl;
        for (int i = 0; i < countDate; i++)
            resultDate[i].print();

        delete[] resultDate;
    }
    catch (MyException& e) {
        cout << "Ошибка: " << e.msg << endl;
    }
    catch (...) {
        cout << "Неизвестная ошибка" << endl;
    }

    return 0;
}
