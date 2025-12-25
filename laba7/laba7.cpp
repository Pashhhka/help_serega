#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>

using namespace std;

// ======================= КЛАСС =======================
class Product {
private:
    string name;
    int quantity;
    double price;

public:
    Product() : name(""), quantity(0), price(0.0) {}
    Product(string n, int q, double p)
        : name(n), quantity(q), price(p) {}

    string getName() const { return name; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }

    void increasePrice(double percent) {
        price += price * percent / 100.0;
    }

    // Ввод из файла / cin
    friend istream& operator>>(istream& in, Product& p) {
        in >> p.name >> p.quantity >> p.price;
        return in;
    }

    // Вывод в файл / cout
    friend ostream& operator<<(ostream& out, const Product& p) {
        out << left << setw(15) << p.name
            << setw(10) << p.quantity
            << setw(10) << fixed << setprecision(2) << p.price;
        return out;
    }
};

// ======================= ФУНКЦИИ STL =======================

// Печать (for_each)
void printProduct(const Product& p) {
    cout << p << endl;
}

// Предикат (find_if)
bool isLowQuantity(const Product& p) {
    return p.getQuantity() < 5;
}

// Преобразование (transform)
Product raisePrice(Product p) {
    p.increasePrice(10);
    return p;
}

// ======================= ФАЙЛ =======================

void loadFromFile(vector<Product>& v) {
    ifstream fin("data.txt");
    if (!fin) return;

    Product p;
    while (fin >> p) {
        v.push_back(p);
    }
}

void saveToFile(const vector<Product>& v) {
    ofstream fout("data.txt");
    for (const auto& p : v) {
        fout << p.getName() << " "
             << p.getQuantity() << " "
             << p.getPrice() << endl;
    }
}

// ======================= МЕНЮ =======================

void printTable(const vector<Product>& v) {
    cout << left << setw(15) << "Название"
         << setw(10) << "Кол-во"
         << setw(10) << "Цена" << endl;
    cout << "----------------------------------\n";

    for_each(v.begin(), v.end(), printProduct);
}

void addProduct(vector<Product>& v) {
    string name;
    int q;
    double p;

    cout << "Название: "; cin >> name;
    cout << "Количество: "; cin >> q;
    cout << "Цена: "; cin >> p;

    v.emplace_back(name, q, p);
}

void findLowQuantity(const vector<Product>& v) {
    auto it = find_if(v.begin(), v.end(), isLowQuantity);
    if (it != v.end()) {
        cout << "Найден товар:\n" << *it << endl;
    } else {
        cout << "Товар не найден\n";
    }
}

void increaseAllPrices(vector<Product>& v) {
    transform(v.begin(), v.end(), v.begin(), raisePrice);
    cout << "Цены увеличены на 10%\n";
}

// ======================= MAIN =======================

int main() {
    vector<Product> warehouse;

    // Загрузка данных
    loadFromFile(warehouse);

    int choice;
    do {
        cout << "\n===== МЕНЮ =====\n";
        cout << "1. Показать все товары\n";
        cout << "2. Добавить товар\n";
        cout << "3. Найти товар с малым количеством\n";
        cout << "4. Увеличить цены на 10%\n";
        cout << "0. Выход (с сохранением)\n";
        cout << "Ваш выбор: ";
        cin >> choice;

        switch (choice) {
        case 1: printTable(warehouse); break;
        case 2: addProduct(warehouse); break;
        case 3: findLowQuantity(warehouse); break;
        case 4: increaseAllPrices(warehouse); break;
        case 0:
            saveToFile(warehouse);
            cout << "Данные сохранены\n";
            break;
        default:
            cout << "Неверный пункт меню\n";
        }
    } while (choice != 0);

    return 0;
}
