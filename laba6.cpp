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
        : name(n), surname(s), patronymic(p) {}

    bool operator==(const fio& other) const { 
        return name == other.name &&
               surname == other.surname &&
               patronymic == other.patronymic;
    }

    friend istream& operator>>(istream& in, fio& f) {
        cout << "Введите имя: ";
        in >> f.name;
        cout << "Введите фамилию: ";
        in >> f.surname;
        cout << "Введите отчество: ";
        in >> f.patronymic;

        if (f.name.empty() || f.surname.empty()) //Выброс ошибки, класса, созданного выше
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
    shipment() : day(1), month(1), year(2000) {}

    shipment& operator=(const shipment& other) {
        day = other.day;
        month = other.month;
        year = other.year;
        return *this;
    }

    friend istream& operator>>(istream& in, shipment& s) {
        cout << "Введите дату продажи (день месяц год): ";
        in >> s.day >> s.month >> s.year;

        if (s.day < 1 || s.day > 31 ||
            s.month < 1 || s.month > 12 ||
            s.year < 2000)
            throw MyException("Некорректная дата");

        return in;
    }

    bool operator==(const shipment& other) const {
        return day == other.day &&
               month == other.month &&
               year == other.year;
    }

    void print() const {
        cout << day << "." << month << "." << year;
    }
};

// ===== Класс Supplier (поставщик) =====
class supplier {
private:
    fio person;
    shipment saleDate;

public:
    supplier() {}

    supplier& operator=(const supplier& other) {
        person = other.person;
        saleDate = other.saleDate;
        return *this;
    }

    friend istream& operator>>(istream& in, supplier& s) {
        in >> s.person;
        in >> s.saleDate;
        return in;
    }

    bool matchFio(const fio& f) const {
        return person == f;
    }

    bool matchDate(const shipment& d) const {
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

        supplier* resultFio = new supplier[N]; //создание динамического массива объектов с искомым именем
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

        supplier* resultDate = new supplier[N];//создание динамического массива объектов с искомой датой
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
    catch (MyException& e) { //Отлов ошибок класса MyException(мы выбрасывали их внутри создаваемых классов)
        cout << "Ошибка: " << e.msg << endl;
    }
    catch (...) { //Отлов других лшиблк
        cout << "Неизвестная ошибка" << endl;
    }

    return 0;
}
