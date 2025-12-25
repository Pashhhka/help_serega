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

    // Метод для ручного ввода данных поставщика
    void input() {
        cout << "Введите данные поставщика:\n";
        cin >> *this;
    }
};

// ===== Функции для работы с меню =====
void printMenu() {
    cout << "\n=== МЕНЮ ===" << endl;
    cout << "1. Ввод данных поставщиков" << endl;
    cout << "2. Поиск поставщиков по ФИО" << endl;
    cout << "3. Поиск поставщиков по дате продажи" << endl;
    cout << "4. Просмотр всех поставщиков" << endl;
    cout << "5. Выход" << endl;
    cout << "Ваш выбор: ";
}

void inputSuppliers(supplier suppliers[], int& count) {
    cout << "\n=== Ввод поставщиков ===" << endl;
    cout << "Сколько поставщиков вы хотите ввести? (максимум 100): ";
    cin >> count;

    if (count <= 0 || count > 100) {
        throw MyException("Некорректное количество поставщиков");
    }

    for (int i = 0; i < count; i++) {
        cout << "\nПоставщик #" << i + 1 << endl;
        suppliers[i].input();
    }
    cout << "Данные успешно сохранены!" << endl;
}

void searchByFio(supplier suppliers[], int count) {
    if (count == 0) {
        cout << "Сначала введите данные поставщиков!" << endl;
        return;
    }

    fio searchFio;
    cout << "\n=== Поиск по ФИО ===" << endl;
    cin >> searchFio;

    supplier* resultFio = new supplier[count];
    int countFio = 0;

    for (int i = 0; i < count; i++) {
        if (suppliers[i].matchFio(searchFio)) {
            resultFio[countFio++] = suppliers[i];
        }
    }

    cout << "\nНайдено поставщиков: " << countFio << endl;
    if (countFio > 0) {
        for (int i = 0; i < countFio; i++) {
            cout << i + 1 << ". ";
            resultFio[i].print();
        }
    } else {
        cout << "Поставщики с такими ФИО не найдены." << endl;
    }

    delete[] resultFio;
}

void searchByDate(supplier suppliers[], int count) {
    if (count == 0) {
        cout << "Сначала введите данные поставщиков!" << endl;
        return;
    }

    shipment searchDate;
    cout << "\n=== Поиск по дате продажи ===" << endl;
    cin >> searchDate;

    supplier* resultDate = new supplier[count];
    int countDate = 0;

    for (int i = 0; i < count; i++) {
        if (suppliers[i].matchDate(searchDate)) {
            resultDate[countDate++] = suppliers[i];
        }
    }

    cout << "\nНайдено поставщиков: " << countDate << endl;
    if (countDate > 0) {
        for (int i = 0; i < countDate; i++) {
            cout << i + 1 << ". ";
            resultDate[i].print();
        }
    } else {
        cout << "Поставщики с такой датой продажи не найдены." << endl;
    }

    delete[] resultDate;
}

void viewAllSuppliers(supplier suppliers[], int count) {
    if (count == 0) {
        cout << "Нет данных о поставщиках. Сначала введите данные!" << endl;
        return;
    }

    cout << "\n=== Все поставщики ===" << endl;
    cout << "Всего поставщиков: " << count << endl;
    for (int i = 0; i < count; i++) {
        cout << i + 1 << ". ";
        suppliers[i].print();
    }
}

// ===== Главная функция =====
int main() {
    try {
        const int MAX_SUPPLIERS = 100;
        supplier suppliers[MAX_SUPPLIERS];
        int supplierCount = 0;
        int choice;

        do {
            printMenu();
            cin >> choice;

            switch (choice) {
                case 1:
                    inputSuppliers(suppliers, supplierCount);
                    break;

                case 2:
                    searchByFio(suppliers, supplierCount);
                    break;

                case 3:
                    searchByDate(suppliers, supplierCount);
                    break;

                case 4:
                    viewAllSuppliers(suppliers, supplierCount);
                    break;

                case 5:
                    cout << "Выход из программы..." << endl;
                    break;

                default:
                    cout << "Некорректный выбор! Попробуйте снова." << endl;
            }

        } while (choice != 5);
    }
    catch (MyException& e) {
        cout << "Ошибка: " << e.msg << endl;
    }
    catch (...) {
        cout << "Неизвестная ошибка" << endl;
    }

    return 0;
}