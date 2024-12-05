#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

class Product {
private:
    string name;
    double price;
    int quantity;

public:
    Product(string name, double price, int quantity)
        : name(name), price(price), quantity(quantity) {}

    void display() const {
        cout << "Товар: " << name << ", Цена: " << price << ", Количество: " << quantity << endl;
    }

    bool operator==(const string& other_name) const {
        return name == other_name;
    }

    bool operator==(const Product& other) const {
        return name == other.name;
    }

    void change_quantity(int new_quantity) {
        quantity = new_quantity;
    }

    int get_quantity() const {
        return quantity;
    }

    double get_price() const {
        return price;
    }
};

class Customer {
private:
    string name;
    double balance;

public:
    Customer(string name, double balance)
        : name(name), balance(balance) {}

    void add_balance(double amount) {
        balance += amount;
    }

    double get_balance() const {
        return balance;
    }

    bool operator==(const string& other_name) const {
        return name == other_name;
    }

    bool buy(Product& product) {
        if (product.get_quantity() > 0 && balance >= product.get_price()) {
            balance -= product.get_price();
            product.change_quantity(product.get_quantity() - 1);
            cout << name << " купил товар за " << product.get_price() << " рублей." << endl;
            return true;
        }
        cout << "Недостаточно средств или товара на складе." << endl;
        return false;
    }

    friend void display_customer_info(const Customer& customer);
};

void display_customer_info(const Customer& customer) {
    cout << "Имя клиента: " << customer.name << ", Баланс: " << customer.balance << endl;
}

class Store {
private:
    vector<Product> products;
    vector<Customer> customers;
    static int product_count;

public:
    void add_product(const Product& product) {
        products.push_back(product);
        product_count++;
    }

    void add_customer(const Customer& customer) {
        customers.push_back(customer);
    }

    template <typename T>
    void display_info() const {
        if constexpr (is_same<T, Product>::value) {
            for (const auto& product : products) {
                product.display();
            }
        }
        else if constexpr (is_same<T, Customer>::value) {
            for (const auto& customer : customers) {
                display_customer_info(customer);
            }
        }
    }

    void purchase_product(Customer& customer, Product& product) {
        if (!customer.buy(product)) {
            cout << "Покупка не удалась." << endl;
        }
    }

    void operator+=(const Product& product) {
        for (auto& p : products) {
            if (p == product) {
                p.change_quantity(p.get_quantity() + 1);
                break;
            }
        }
    }

    void operator-=(const Product& product) {
        for (auto& p : products) {
            if (p == product && p.get_quantity() > 0) {
                p.change_quantity(p.get_quantity() - 1);
                break;
            }
        }
    }

    static int get_product_count() {
        return product_count;
    }

    const vector<Product>& get_products() const {
        return products;
    }

    const vector<Customer>& get_customers() const {
        return customers;
    }
};

int Store::product_count = 0;

template <typename T>
class EmployeeAccount {
private:
    T name;
    double salary;
    string status;

public:
    EmployeeAccount(T name, double salary, string status)
        : name(name), salary(salary), status(status) {}

    void display_info() const {
        cout << "Имя сотрудника: " << name << ", Зарплата: " << salary << ", Статус: " << status << endl;
    }
};

int main() {
    setlocale(LC_CTYPE, "rus");
    Store store;

    int num_products;
    cout << "Введите количество товаров в магазине: ";
    cin >> num_products;

    for (int i = 0; i < num_products; ++i) {
        string product_name;
        double product_price;
        int product_quantity;

        cout << "Введите название товара #" << i + 1 << ": ";
        cin >> product_name;
        cout << "Введите цену товара: ";
        cin >> product_price;
        cout << "Введите количество товара на складе: ";
        cin >> product_quantity;

        store.add_product(Product(product_name, product_price, product_quantity));
    }

    int num_customers;
    cout << "\nВведите количество клиентов: ";
    cin >> num_customers;

    for (int i = 0; i < num_customers; ++i) {
        string customer_name;
        double customer_balance;

        cout << "Введите имя клиента #" << i + 1 << ": ";
        cin >> customer_name;
        cout << "Введите баланс клиента: ";
        cin >> customer_balance;

        store.add_customer(Customer(customer_name, customer_balance));
    }

    cout << "\nИнформация о товарах:\n";
    store.display_info<Product>();

    cout << "\nИнформация о клиентах:\n";
    store.display_info<Customer>();

    string buy_name;
    cout << "\nВведите имя клиента, который хочет сделать покупку: ";
    cin >> buy_name;

    string buy_product_name;
    cout << "Введите название товара для покупки: ";
    cin >> buy_product_name;

    bool customer_found = false;
    bool product_found = false;
    Customer* customer = nullptr;
    Product* product = nullptr;

    for (auto& c : store.get_customers()) {
        if (c == buy_name) {
            customer = const_cast<Customer*>(&c);
            customer_found = true;
            break;
        }
    }

    for (auto& p : store.get_products()) {
        if (p == buy_product_name) {
            product = const_cast<Product*>(&p);
            product_found = true;
            break;
        }
    }

    if (customer_found && product_found) {
        store.purchase_product(*customer, *product);
    }
    else {
        cout << "Ошибка при поиске клиента или товара." << endl;
    }

    return 0;
}
