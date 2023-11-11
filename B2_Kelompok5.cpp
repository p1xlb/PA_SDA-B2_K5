#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <limits>

using namespace std;
using std::istringstream;

//memilih os yang dipakai
#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

// Deklarasi struct untuk User
struct User {
    string username;
    string password;
    string phoneNumber;
    string email;
    string address;
    bool isAdmin;
    User* next;

    User(const string& u, const string& p, const string& phone, const string& mail, const string& addr, bool admin)
        : username(u), password(p), phoneNumber(phone), email(mail), address(addr), isAdmin(admin), next(nullptr) {}
};

// Deklarasi Class untuk fungsi fungsi User
class UserList {
public:
    UserList() : head(nullptr) {}

    // Fungsi Untuk menambahkan User(Register)
    void addUser(const string& username, const string& password, const string& phoneNumber, const string& email, const string& address, bool isAdmin) {
        if (findUser(username) == nullptr) {  // Pengecekan apakah username sudah ada atau belum
            User* newUser = new User(username, password, phoneNumber, email, address, isAdmin);
            if (!head) {
                head = newUser;
            } else {
                User* current = head;
                while (current->next) {
                    current = current->next;
                }
                current->next = newUser;
            }
            cout << "User registered successfully!\n";
        } else {
            cout << "Username already exists. Registration failed.\n";
            cin.get();
        }
    }


    // Funsgi untuk mencari data suatu User
    User* findUser(const string& username) {
        User* current = head;
        while (current) {
            if (current->username == username) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    // Fungsi untuk menampilkan data User selain admin
    void listNonAdminUsers() {
        cout << "List of Users (Non-Admin):" << endl;
        cout << "====================================\n";

        User* current = head;

        while (current != nullptr) {
            if (!current->isAdmin) {
                cout << "Username: " << current->username << endl;
                cout << "Phone Number: " << current->phoneNumber << endl;
                cout << "Email: " << current->email << endl;
                cout << "Address: " << current->address << endl;
                cout << "====================================\n";
            }
            current = current->next;
        }
    }

    ~UserList() { 
        while (head) {
            User* temp = head;
            head = head->next;
            delete temp;
        }
    }

private:
    User* head;
};

// Deklarasi struct untuk Furniture
struct FurnitureItem {
    int cat_id;
    string prod_name;
    string f_type;
    string size;
    double price;
    FurnitureItem* next_item;

    FurnitureItem(int id, const string& name, const string& type, const string& s, double p) {
        cat_id = id;
        prod_name = name;
        f_type = type;
        size = s;
        price = p;
        next_item = nullptr;
    }
};

// Deklarasi Class untuk fungsi fungsi Furniture
class Catalog {
public:
    FurnitureItem* head;

    Catalog() {
        head = nullptr;
    }

    // FUngsi Untuk menambahkan Furniture
    void createItem(int cat_id, const string& prod_name, const string& f_type, const string& size, double price) {
        FurnitureItem* new_item = new FurnitureItem(cat_id, prod_name, f_type, size, price);
        if (head == nullptr) {
            head = new_item;
        } else {
            FurnitureItem* current = head;
            while (current->next_item != nullptr) {
                current = current->next_item;
            }
            current->next_item = new_item;
        }
    }

    // fungsi untuk menampilkan data Furniture
    void readItems() {
        FurnitureItem* current = head;
        while (current != nullptr) {
            cout << "Catalog ID: " << current->cat_id << endl;
            cout << "Product Name: " << current->prod_name << endl;
            cout << "Type: " << current->f_type << endl;
            cout << "Size: " << current->size << endl;
            cout << "Price: " << current->price << endl;
            cout << "------------" << endl;
            current = current->next_item;
        }
    }
    
    // Fungsi Untuk Mencari Furniture berdasarkan ID nya
    FurnitureItem* findItemByID(int itemID) {
        FurnitureItem* current = head;
        while (current != nullptr) {
            if (current->cat_id == itemID) {
                return current;
            }
            current = current->next_item;
        }
        return nullptr;
    }

     // Fungsi untuk mengupdate data Furniture berdasarkan ID nya
    void updateItem(int cat_id, const string& prod_name, const string& f_type, const string& size, double price) {
        FurnitureItem* current = head;
        while (current != nullptr) {
            if (current->cat_id == cat_id) {
                current->prod_name = prod_name;
                current->f_type = f_type;
                current->size = size;
                current->price = price;
                system(CLEAR);
                cout << "Item with Catalog ID " << cat_id << " updated successfully." << endl;
                cin.get();
                return;
            }
            current = current->next_item;
        }
        system(CLEAR);
        cerr << "Item with Catalog ID " << cat_id << " not found." << endl;
        cin.get();
    }

    // Fungsi untuk menghapus data Furniture berdasarkan ID nya
    void deleteItem(int cat_id) {
        FurnitureItem* current = head;
        FurnitureItem* previous = nullptr;

        while (current != nullptr) {
            if (current->cat_id == cat_id) {
                if (previous == nullptr) {
                    head = current->next_item;
                } else {
                    previous->next_item = current->next_item;
                }
                delete current;
                system(CLEAR);
                cout << "Item with Catalog ID " << cat_id << " deleted successfully." << endl;
                cin.get();
                return;
            }
            previous = current;
            current = current->next_item;
        }
        system(CLEAR);
        cerr << "Item with Catalog ID " << cat_id << " not found." << endl;
        cin.get();
    }

    // Fungsi untuk mencari Furniture berdasarkan tipe nya dengan algoritma Boyer-Moore
    void searchByType(const string& type) {
        FurnitureItem* current = head;
        bool found = false;

        cout << "Furniture Items of Type: " << type << endl;
        cout << "====================================\n";

        int badChar[256]; 
        for (int i = 0; i < 256; i++) {
            badChar[i] = -1;
        }

        int typeLength = type.length();
        for (int i = 0; i < typeLength - 1; i++) {
            badChar[static_cast<int>(type[i])] = i;
        }

        while (current != nullptr) {
            const string& text = current->f_type;
            int textLength = text.length();
            int s = 0;

            while (s <= textLength - typeLength) {
                int j = typeLength - 1;

                while (j >= 0 && type[j] == text[s + j]) {
                    j--;
                }

                if (j < 0) { // Kecocokan ditemukan
                    cout << "Catalog ID: " << current->cat_id << endl;
                    cout << "Product Name: " << current->prod_name << endl;
                    cout << "Size: " << current->size << endl;
                    cout << "Price: " << current->price << endl;
                    cout << "------------" << endl;
                    found = true;

                    // Memindahkan s ke kemunculan berikutnya dari type
                    if (s + typeLength < textLength) {
                        s += typeLength - badChar[static_cast<int>(text[s + typeLength])];
                    } else {
                        s++;
                    }
                } else {
                    s += max(1, j - badChar[static_cast<int>(text[s + j])]);
                }
            }

            current = current->next_item;
        }

        if (!found) {
            system(CLEAR);
            cout << "No furniture items of type " << type << " found." << endl;
            cin.get();
        }
    }

    // Fungsi untuk menyimpan data Furniture ke dalam file CSV
    void saveToCSV(const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open the file for writing." << endl;
            return;
        }
        file << "Catalog ID,Product Name,Type,Size,Price" << endl;

        FurnitureItem* current = head;
        while (current != nullptr) {
            file << current->cat_id << "," << current->prod_name << "," << current->f_type << "," << current->size << "," << current->price << endl;
            current = current->next_item;
        }

        file.close();
        system(CLEAR);
        cout << "Catalog saved to " << filename << endl;
        cin.get();
    }

    // Fungsi untuk memuat data Furniture dari file CSV
    void loadFromCSV(Catalog& catalog, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open the file for reading." << endl;
        return;
    }

    string header;
    getline(file, header);

    while (!file.eof()) {
        int cat_id;
        string prod_name, f_type, size;
        double price;
        
        char comma;

        file >> cat_id >> comma;
        getline(file, prod_name, ',');
        getline(file, f_type, ',');
        getline(file, size, ',');
        file >> price;

        catalog.createItem(cat_id, prod_name, f_type, size, price);
    }

    file.close();
    cout << "Catalog loaded from " << filename << endl;
}
    // Fungsi untuk menampilkan data Furniture yang sudah diurutkan berdasarkan harga(Menaik)
    void sortByPriceAscending() {
        head = mergeSort(head, true);
    }

    // Fungsi untuk menampilkan data Furniture yang sudah diurutkan berdasarkan harga(Menurun)
    void sortByPriceDescending() {
        head = mergeSort(head, false);
    }

private:
     // Fungsi untuk membagi 2 list
    FurnitureItem* merge(FurnitureItem* left, FurnitureItem* right, bool ascending) {
        FurnitureItem* merged = nullptr;
        FurnitureItem* tail = nullptr;

        while (left != nullptr && right != nullptr) {
            bool condition = ascending ? (left->price < right->price) : (left->price > right->price);

            if (condition) {
                if (merged == nullptr) {
                    merged = left;
                    tail = left;
                    left = left->next_item;
                } else {
                    tail->next_item = left;
                    tail = left;
                    left = left->next_item;
                }
            } else {
                if (merged == nullptr) {
                    merged = right;
                    tail = right;
                    right = right->next_item;
                } else {
                    tail->next_item = right;
                    tail = right;
                    right = right->next_item;
                }
            }
        }

        if (left != nullptr) {
            tail->next_item = left;
        } else if (right != nullptr) {
            tail->next_item = right;
        }

        return merged;
    }
    // Fungsi Merge sort untuk mensortir Harga
    FurnitureItem* mergeSort(FurnitureItem* list, bool ascending) {
        if (list == nullptr || list->next_item == nullptr) {
            return list;
        }

        FurnitureItem* middle = getMiddle(list);
        FurnitureItem* nextToMiddle = middle->next_item;
        middle->next_item = nullptr;

        FurnitureItem* left = mergeSort(list, ascending);
        FurnitureItem* right = mergeSort(nextToMiddle, ascending);

        return merge(left, right, ascending);
    }

    // Fungsi untuk mencari nilai tengah dari list
    FurnitureItem* getMiddle(FurnitureItem* list) {
        if (list == nullptr) {
            return list;
        }

        FurnitureItem* slow = list;
        FurnitureItem* fast = list;

        while (fast->next_item != nullptr && fast->next_item->next_item != nullptr) {
            slow = slow->next_item;
            fast = fast->next_item->next_item;
        }

        return slow;
    }

};

// Deklarasi Struct untuk Order
struct Order {
    int order_id;
    string product_name;
    string name;
    string phone_number;
    string status;
    double total_price;
    Order* next_order;

    Order(int id, const string& prod_name, const string& n, const string& phone, const string& stat, double total) {
        order_id = id;
        product_name = prod_name;
        name = n;
        phone_number = phone;
        status = stat;
        total_price = total;
        next_order = nullptr;
    }
};

// Deklarasi Class untuk fungsi fungsi Order
class OrderManager {
public:
    Order* head;
    int last_order_id; // Untuk menyimpan ID terakhir yang digunakan

    OrderManager() {
        head = nullptr;
        last_order_id = 0;
    }

    // Fungsi untuk menambahkan Order
    void placeOrder(const string& product_name, const string& name, const string& phone_number, const string& status, double total_price) {
        int order_id = last_order_id + 1; // ID Auto Increment
        last_order_id = order_id; // Update ID terakhir yang digunakan
        Order* new_order = new Order(order_id, product_name, name, phone_number, status, total_price);
        if (head == nullptr) {
            head = new_order;
        } else {
            Order* current = head;
            while (current->next_order != nullptr) {
                current = current->next_order;
            }
            current->next_order = new_order;
        }
        system(CLEAR);
        cout << "Order placed successfully for " << product_name << " (Order ID: " << order_id << ")." << endl;
        cin.get();
    }

    // Fungsi untuk mengubah status menjadi Processed lalu memindahkan ke file CSV lain
    void processOrder(int order_id) {
    Order* current = head;
    Order* previous = nullptr;
    
    while (current != nullptr) {
        if (current->order_id == order_id) {
            if (current->status == "Pending") {
                current->status = "Processed";

                // Memindahtangankan order ke processed_orders.csv
                saveOrderToCSV("processed_orders.csv", current->order_id, current->product_name, current->name, current->phone_number, current->status, current->total_price);

                // Menghapus order dari order_history.csv
                removeOrderFromCSV("order_history.csv", order_id);

                return;
            } else {
                system(CLEAR);
                cerr << "Order " << order_id << " is not in the 'Pending' status and cannot be processed." << endl;
                cin.get();
                return;
            }
        }

        previous = current;
        current = current->next_order;
    }
    system(CLEAR);
    cerr << "Order " << order_id << " not found." << endl;
    cin.get();
}

// Fungsi untuk mengubah status menjadi Finished lalu memindahkan ke file CSV lain
void removeOrderFromCSV(const string& filename, int order_id) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        system(CLEAR);
        cerr << "Error: Could not open " << filename << " for reading." << endl;
        cin.get();
        return;
    }

    ofstream tempFile("temp.csv"); // Membuat file sementara untuk menulis data

    string line;
    bool orderFound = false;

    while (getline(inputFile, line)) {
        if (line.find(to_string(order_id) + ",") == 0) {
            orderFound = true;
        } else {
            tempFile << line << endl;
        }
    }

    inputFile.close();
    tempFile.close();

    if (orderFound) {
        // Menghapus file awal
        if (remove(filename.c_str()) != 0) {
            system(CLEAR);
            cerr << "Error: Failed to remove " << filename << endl;
            cin.get();
            return;
        }
        // Mengganti nama file sementara menjadi file awal
        if (rename("temp.csv", filename.c_str()) != 0) {
            system(CLEAR);
            cerr << "Error: Failed to rename temp.csv to " << filename << endl;
            cin.get();
            return;
        }
    } else {
        system(CLEAR);
        cerr << "Order " << order_id << " not found in " << filename << endl;
        cin.get();
    }
}
    // Fungsi untuk Menyimpan data Order ke dalam file CSV
    void saveOrdersToCSV(const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            system(CLEAR);
            cerr << "Error: Could not open the file for writing." << endl;
            cin.get();
            return;
        }

        Order* current = head;
        while (current != nullptr) {
            file << current->order_id << "," << current->product_name << "," << current->name << ","
                 << current->phone_number << "," << current->status << "," << current->total_price << endl;
            current = current->next_order;
        }

        file.close();
        system(CLEAR);
        cout << "Orders saved to " << filename << endl;
        cin.get();
    }

    // Fungsi untuk memuat data Order dari file CSV
    void loadOrdersFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        system(CLEAR);
        cerr << "Error: Could not open the file for reading." << endl;
        cin.get();
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        string orderData[6];
        int i = 0;

        while (getline(ss, token, ',')) {
            orderData[i++] = token;
        }

        if (i == 6) {
            try {
                int order_id = stoi(orderData[0]);
                string product_name = orderData[1];
                string name = orderData[2];
                string phone_number = orderData[3];
                string status = orderData[4];
                double total_price = stod(orderData[5]);

                int orderID = last_order_id + 1;
                last_order_id = orderID;
                Order* new_order = new Order(order_id, product_name, name, phone_number, status, total_price);
                new_order->next_order = head;
                head = new_order;
            } catch (const invalid_argument& e) {
                system(CLEAR);
                cerr << "Error: Invalid data in the CSV file - " << e.what() << endl;
                cin.get();
            }
        } else {
            system(CLEAR);
            cerr << "Invalid order data: " << line << endl;
            cin.get();
        }
    }

    file.close();
    system(CLEAR);
    cout << "Orders loaded from " << filename << endl;
}

// Fungsi untuk menampilkan data Order dari 3 file CSV
void showOrders() {
    showOrdersPending();
    showOrdersProcessed(); 
    showOrdersFinished();
}

void showOrdersPending() {
    cout << "===============================\n";
    cout << "Pending Orders (order_history.csv):" << endl;
    cout << "===============================\n";
    displayOrdersFromFile("order_history.csv", "Pending");
}

void showOrdersProcessed() {
    cout << "===============================\n";
    cout << "Processed Orders (processed_orders.csv):" << endl;
    cout << "===============================\n";
    displayOrdersFromFile("processed_orders.csv", "Processed");
}

void showOrdersFinished() {
    cout << "===============================\n";
    cout << "Finished Orders (finished_orders.csv):" << endl;
    cout << "===============================\n";
    displayOrdersFromFile("finished_orders.csv", "Finished");
}

// Fungsi untuk menampilkan data Order dari file CSV
void displayOrdersFromFile(const string& filename, const string& status) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        system(CLEAR);
        cerr << "Error: Could not open " << filename << " for reading." << endl;
        cin.get();
        return;
    }

    string line;
    while (getline(inputFile, line)) {
        istringstream iss(line);
        string token;
        getline(iss, token, ','); // Order ID
        int order_id = stoi(token);
        getline(iss, token, ','); // Product Name
        string product_name = token;
        getline(iss, token, ','); // Name
        string name = token;
        getline(iss, token, ','); // Phone Number
        string phone_number = token;
        getline(iss, token, ','); // Status
        string order_status = token;
        getline(iss, token, ','); // Total Price
        double total_price = stod(token);

        if (order_status == status) {
            displayOrder(order_id, product_name, name, phone_number, status, total_price);
        }
    }

    inputFile.close();
}
    // Menampilkan data individual Order
    void displayOrder(int order_id, const string& product_name, const string& name, const string& phone_number, const string& status, double total_price) {
    cout << "Order ID: " << order_id << endl;
    cout << "Product Name: " << product_name << endl;
    cout << "Name: " << name << endl;
    cout << "Phone Number: " << phone_number << endl;
    cout << "Status: " << status << endl;
    cout << "Total Price: " << total_price << endl;
    cout << "-----------------------\n";
}
    // fungsi untuk menyelesaikan order dan memindahkan datanya ke file CSV lain
    void finishOrder() {
    ifstream processedFile("processed_orders.csv");
    if (!processedFile.is_open()) {
        system(CLEAR);
        cerr << "Error: Could not open processed_orders.csv for reading." << endl;
        cin.get();
        return;
    }

    string firstLine;

    if (getline(processedFile, firstLine)) {
        istringstream iss(firstLine);
        string token;
        vector<string> tokens;
        while (getline(iss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() == 6) {
            int order_id = stoi(tokens[0]);
            string product_name = tokens[1];
            string name = tokens[2];
            string phone_number = tokens[3];
            string status = "Finished";
            double total_price = stod(tokens[5]);

            // Memindahkan order ke finished_orders.csv
            saveOrderToCSV("finished_orders.csv", order_id, product_name, name, phone_number, status, total_price);

            // Menghapus order dari processed_orders.csv
            deleteTransferredOrder(order_id);
            
        } else {
            system(CLEAR);
            cerr << "Error: Invalid data in processed_orders.csv." << endl;
            cin.get();
        }
    } else {
        system(CLEAR);
        cerr << "No processed orders available to finish." << endl;
        cin.get();
    }

    processedFile.close();
}

// Menghapus order berdasarkan ID (hanya untuk order yang ditransfer)
void deleteTransferredOrder(int order_id) {
    ifstream processedFile("processed_orders.csv");
    if (!processedFile.is_open()) {
        system(CLEAR);
        cerr << "Error: Could not open processed_orders.csv for reading." << endl;
        cin.get();
        return;
    }

    string tempFilename = "temp_processed_orders.csv";
    ofstream tempFile(tempFilename);

    string line;
    bool orderDeleted = false;

    while (getline(processedFile, line)) {
        istringstream iss(line);
        int currentOrderId;
        if (iss >> currentOrderId && currentOrderId == order_id) {
            orderDeleted = true;
        } else {
            tempFile << line << '\n';
        }
    }

    processedFile.close();
    tempFile.close();

    if (orderDeleted) {
        remove("processed_orders.csv"); // Menghapus file awal
        rename(tempFilename.c_str(), "processed_orders.csv"); // Merubah nama file sementara menjadi file awal
    } else {
        system(CLEAR);
        cerr << "Order " << order_id << " not found in processed_orders.csv." << endl;
        cin.get();
    }
}
    // Mneyimpan order individu ke file CSV
    void saveOrderToCSV(const string& filename, int order_id, const string& product_name, const string& name, const string& phone_number, const string& status, double total_price) {
        ofstream file(filename, ios::app); 

        if (file.is_open()) {
            file << order_id << "," << product_name << "," << name << "," << phone_number << "," << status << "," << total_price << "\n";
            file.close();
            system(CLEAR);
            cout << "Order saved to " << filename << endl;
            cin.get();
        } else {
            system(CLEAR);
            cerr << "Error: Could not open the file for writing." << endl;
            cin.get();
        }
    }

// Fungsi untuk user untuk menampilkan pesanannya
void displayUserOrders(const string& username) {
    cout << "Orders for User: " << username << endl;
    cout << "===============================\n";

    displayUserOrdersFromFile("order_history.csv", username, "Pending");
    displayUserOrdersFromFile("processed_orders.csv", username, "Processed");
    displayUserOrdersFromFile("finished_orders.csv", username, "Finished");
}

// Fungsi Pembaca file pesanan untuk menampilkan pesanan kepada user
void displayUserOrdersFromFile(const string& filename, const string& username, const string& status) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        system(CLEAR);
        cerr << "Error: Could not open " << filename << " for reading." << endl;
        cin.get();
        return;
    }

    string line;
    while (getline(inputFile, line)) {
        istringstream iss(line);
        string token;
        getline(iss, token, ','); // Order ID
        int order_id = stoi(token);
        getline(iss, token, ','); // Product Name
        string product_name = token;
        getline(iss, token, ','); // Name (username)
        string name = token;
        getline(iss, token, ','); // Phone Number
        string phone_number = token;
        getline(iss, token, ','); // Status
        string order_status = token;
        getline(iss, token, ','); // Total Price
        double total_price = stod(token);

        if (name == username && order_status == status) {
            displayOrder(order_id, product_name, name, phone_number, order_status, total_price);
        }
    }

    inputFile.close();
}

};

int main() {
    UserList userList; 
    Catalog catalog;
    OrderManager orderManager;

    string filename = "user_credentials.csv";
    catalog.loadFromCSV(catalog, "catalog.csv");
    orderManager.loadOrdersFromCSV("order_history.csv");

    // Mengisi data User dari file CSV
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            string username, password, phoneNumber, email, address;
            int isAdmin; // Membaca privilege akun (0 = Client, 1 = Admin)

            if (getline(iss, username, ',') &&
                getline(iss, password, ',') &&
                getline(iss, phoneNumber, ',') &&
                getline(iss, email, ',') &&
                getline(iss, address, ',') &&
                (iss >> isAdmin)) {

                bool isAdminBool = (isAdmin == 1);

                userList.addUser(username, password, phoneNumber, email, address, isAdminBool);
            } else {
                system(CLEAR);
                cerr << "Error: Invalid data in the file." << endl;
                cin.get();
            }
        }
        file.close();
    }

    User* loggedInUser = nullptr;

    do {
        while(true){
        system(CLEAR);
        int choice;
        cout << "1. Login\n";
        cout << "2. Register as Client\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        while(!(cin >> choice)){cin.clear();cin.ignore(numeric_limits<streamsize>::max(), '\n');cout<<"Enter your choice: ";}

        switch (choice) {
            case 1: { // Login
            system(CLEAR);
                string username, password;
                cout << "Enter username: ";
                while(!(cin >> username)){cin.clear();cin.ignore(numeric_limits<streamsize>::max(), '\n');cout << "Enter username: ";}
                cout << "Enter password: ";
                while(!(cin >> password)){cin.clear();cin.ignore(numeric_limits<streamsize>::max(), '\n');cout << "Enter password: ";}
                loggedInUser = userList.findUser(username);
                if (loggedInUser && loggedInUser->password == password) {
                    cout << "Login successful!\n";
                    system(CLEAR);
                    if (loggedInUser->isAdmin) { // Menu jika yang login admin (Menu Awal Admin)
                        do {
                            cout << "Admin Menu:\n";
                            cout << "1. Manajemen Katalog\n";
                            cout << "2. Manajemen Pesanan Toko\n";
                            cout << "3. Data User\n";
                            cout << "4. Logout\n";
                            cout << "Enter your choice: ";
                            int adminChoice;
                            while(!(cin >> adminChoice)){cin.clear();cin.ignore(numeric_limits<streamsize>::max(), '\n');cout<<"Enter your choice: ";}
                            switch (adminChoice) {
                                case 1:{
                                    bool loop1 = true;
                                    system(CLEAR);
                                    while (loop1 == true) { // Menu Manajemen Katalog Admin
                                    cout << "Menu Admin Katalog:\n";
                                    cout << "1. Tambah Produk\n";
                                    cout << "2. Tampilkan Produk\n";
                                    cout << "3. Update Produk\n";
                                    cout << "4. Hapus Produk\n";
                                    cout << "5. Simpan Katalog ke File\n";
                                    cout << "6. Kembali ke Menu Utama\n";
                                    cout << "Pilih aksi: ";
                                    string adminChoice;
                                    cin >> adminChoice;
                                        if (adminChoice == "1"){ // Form Tambah Produk
                                            int id;
                                            string nama, tipe, ukuran;
                                            int harga;
                                            system(CLEAR);
                                            cout << "ID Produk: ";
                                            while(!(cin >> id)){cin.clear();cin.ignore(numeric_limits<streamsize>::max(), '\n');cout << "Masukkan Hanya Angka\nID Produk: ";}
                                            cout << "Nama Produk: "; cin.ignore();
                                            getline(cin, nama);
                                            cout << "Tipe Produk: "; 
                                            getline(cin, tipe);
                                            cout << "Ukuran: "; 
                                            getline(cin, ukuran);
                                            cout << "Harga: "; 
                                            while(!(cin>>harga)){cin.clear();cin.ignore(numeric_limits<streamsize>::max(), '\n');cout << "Masukkan Hanya Angka\nHarga: ";}
                                            catalog.createItem(id, nama, tipe, ukuran, harga);
                                            catalog.saveToCSV("catalog.csv");
                                        }
                                        else if (adminChoice == "2"){ // Tampilkan Produk
                                            system(CLEAR);
                                            catalog.readItems();
                                            cin.get();
                                        }
                                        else if (adminChoice == "3"){ // Form Update Produk
                                            int id;
                                            string nama, tipe, ukuran;
                                            int harga;
                                            system(CLEAR);
                                            catalog.readItems();
                                            cout << "ID Produk yang ingin diupdate: ";
                                            while(!(cin>>id)){cin.clear();cin.ignore(numeric_limits<streamsize>::max(), '\n');cout << "Masukkan Hanya Angka\nID Produk yang ingin diupdate: ";}
                                            cout << "Nama Produk baru: ";  cin.ignore();
                                            getline(cin, nama);
                                            cout << "Tipe Produk baru: ";
                                            getline(cin, tipe); 
                                            cout << "Ukuran baru: "; 
                                            getline(cin, ukuran);
                                            cout << "Harga baru: ";
                                            while(!(cin>>harga)){cin.clear();cin.ignore(numeric_limits<streamsize>::max(), '\n');cout << "Masukkan Hanya Angka\nHarga baru: ";}
                                            catalog.updateItem(id, nama, tipe, ukuran, harga);
                                            catalog.saveToCSV("catalog.csv");
                                            cin.get();                                           
                                        }
                                        else if (adminChoice == "4"){ // Hapus Produk
                                            system(CLEAR);
                                            catalog.readItems();
                                            int id;
                                            cout << "ID Produk yang ingin dihapus: ";
                                            while(!(cin>>id)){cin.clear();cin.ignore(numeric_limits<streamsize>::max(), '\n');cout << "Masukkan Hanya Angka\nID Produk yang ingin dihapus: ";}
                                            string choice;
                                            cout << "Anda yakin ingin menghapus Produk? (Y/N):";
                                            cin >> choice;
                                            if(choice == "Y" || choice == "y"){
                                                catalog.deleteItem(id);
                                                catalog.saveToCSV("catalog.csv");
                                            }
                                            else if(choice == "N" || choice == "n"){
                                                cout << "Kembali ke Menu Utama.\n";
                                                
                                            }
                                            else{
                                                system(CLEAR);
                                                cout << "Pilihan tidak valid. Silakan coba lagi.\n";
                                                cin.get();
                                            }
                                            cin.get();
                                        }
                                        else if (adminChoice == "5"){ // Simpan Katalog ke File
                                            catalog.saveToCSV("catalog.csv");
                                        }
                                        else if (adminChoice == "6"){ // Kembali ke Menu Utama
                                            cout << "Kembali ke Menu Utama.\n";
                                            system(CLEAR);
                                            loop1 = false;
                                        }
                                        else {
                                            system(CLEAR);
                                            cout << "Pilihan tidak valid. Silakan coba lagi.\n";
                                            cin.get();

                                        }
                                    }       
                                    break;                                 
                                }
                                case 2:{
                                    system(CLEAR);
                                    bool loop1 = true;
                                    while (loop1 == true) { // Menu Manajemen Pesanan Admin
                                    cout << "Menu Admin Katalog:\n";
                                    cout << "1. Tampilkan Pesanan\n";
                                    cout << "2. Proses Pesanan\n";
                                    cout << "3. Selesaikan Pesanan\n";
                                    cout << "4. Kembali ke Menu Utama\n";
                                    cout << "Pilih aksi: ";
                                    string adminChoice2;
                                    cin >> adminChoice2;
                                    if(adminChoice2 == "1"){ // Menu Menampilkan Pesanan
                                        system(CLEAR);
                                        orderManager.showOrders();
                                        cin.get();
                                    }
                                    else if(adminChoice2 == "2"){ // Menu Proses Pesanan
                                        system(CLEAR);
                                        int id;
                                        string choice;
                                        orderManager.showOrdersPending();
                                        cout << "Masukkan Id Pesanan: ";
                                        while(!(cin>>id)){cin.clear();cin.ignore(numeric_limits<streamsize>::max(), '\n');cout << "Masukkan Hanya Angka\nMasukkan Id Pesanan: ";}
                                        cout << "Anda yakin ingin memproses Pesanan? (Y/N):";
                                        cin >> choice;
                                        if(choice == "Y" || choice == "y"){
                                            orderManager.processOrder(id);
                                        }
                                        else if(choice == "N" || choice == "n"){
                                            cout << "Kembali ke Menu Utama.\n";
                                            
                                        }
                                        else{
                                            system(CLEAR);
                                            cout << "Pilihan tidak valid. Silakan coba lagi.\n";
                                            cin.get();
                                    }
                                    }
                                    else if(adminChoice2 == "3"){ // Menu Selesaikan Pesanan
                                        string choice;
                                        orderManager.showOrdersProcessed();
                                        cout << "Anda yakin ingin menyelesaikan Pesanan paling atas? (Y/N):";
                                        cin >> choice;
                                        if(choice == "Y" || choice == "y"){
                                            orderManager.finishOrder();
                                        }
                                        else if(choice == "N" || choice == "n"){
                                            cout << "Kembali ke Menu Utama.\n";
                                            system(CLEAR);
                                        }
                                        else{
                                            system(CLEAR);
                                            cout << "Pilihan tidak valid. Silakan coba lagi.\n";
                                            cin.get();
                                    }
                                    }
                                    else if(adminChoice2 == "4"){ // Kembali ke Menu Utama
                                        cout << "Kembali ke Menu Utama.\n";
                                        system(CLEAR);
                                        loop1 = false;
                                        
                                    }
                                    else { // Error Handling Menu
                                        system(CLEAR);
                                        cout << "Pilihan tidak valid. Silakan coba lagi.\n";
                                        cin.get();
                                    }
                                    }
                                    break;
                                }
                                case 3: // Menu Data User
                                    system(CLEAR);
                                    userList.listNonAdminUsers();
                                    cin.get();
                                    break;                                    
                                case 4: // Logout
                                    system(CLEAR);
                                    cout << "Logged out as Admin.\n";
                                    cin.get();
                                    loggedInUser = nullptr;
                                    break;     
                                default: // Error Handling Menu
                                    system(CLEAR);
                                    cout << "Invalid choice. Please try again.\n";
                                    cin.get();
                                    break;
                            }
                        } while (loggedInUser && loggedInUser->isAdmin);
                    }
                    else {
                        // Menu Awal Pelanggan / User
                        system(CLEAR);
                        do {
                            cout << "Client Menu:\n";
                            cout << "1. Lihat Katalog\n";
                            cout << "2. Buat Pesanan\n";
                            cout << "3. Lihat Pesanan\n";
                            cout << "4. Logout\n";
                            cout << "Enter your choice: ";
                            int clientChoice;
                            while(!(cin >> clientChoice)){cin.clear();cin.ignore(numeric_limits<streamsize>::max(), '\n');cout << "Enter your choice: ";}
                            
                            switch (clientChoice) {
                                case 1:{ // Menu melihat katalog
                                    system(CLEAR);
                                    cout << "Menu Katalog:\n";
                                    cout << "1. Tampilkan Semua\n";
                                    cout << "2. Cari Berdasarkan Jenis\n";
                                    cout << "3. Urutkan Berdasarkan Harga\n";
                                    cout << "Pilih aksi: ";
                                    string userChoice2;
                                    cin >> userChoice2;
                                    if(userChoice2 == "1"){ // Menampilkan semua furnitur yang ada di katalog
                                        system(CLEAR);
                                        catalog.readItems();
                                        cin.get();
                                    }
                                    else if(userChoice2 == "2"){ // Mencari Furnitur berdasarkan jenisnya
                                        string type;
                                        system(CLEAR);
                                        cout << "Masukkan Jenis: "; cin.ignore();
                                        getline(cin, type);
                                        catalog.searchByType(type);
                                        cin.get();
                                    }
                                    else if(userChoice2 == "3"){  // Mengurutkan Furnitur berdasarkan harganya
                                        system(CLEAR);
                                        cout << "Menu Urutkan Berdasarkan Harga:\n";
                                        cout << "1. Urutkan Harga Terendah\n";
                                        cout << "2. Urutkan Harga Tertinggi\n";
                                        cout << "Pilih aksi: ";
                                        string userChoice3;
                                        while(!(cin >> userChoice3)){cin.clear();cin.ignore(numeric_limits<streamsize>::max(), '\n');cout << "Pilih aksi: ";}
                                        if(userChoice3 == "1"){ // Pengurutan harga ke atas
                                            system(CLEAR);
                                            catalog.sortByPriceAscending();
                                            catalog.readItems();
                                            cin.get();
                                        }
                                        else if(userChoice3 == "2"){ // Pengurutan harga ke bawah
                                            system(CLEAR);
                                            catalog.sortByPriceDescending();
                                            catalog.readItems();
                                            cin.get();
                                        }
                                        else{
                                            system(CLEAR);
                                            cout << "Pilihan tidak valid. Silakan coba lagi.\n";
                                            cin.get();
                                        }
                                    }
                                    else {
                                        system(CLEAR);
                                        cout << "Pilihan tidak valid. Silakan coba lagi.\n";
                                        cin.get();
                                    }
                                    break;
                                }
                                case 2:{ // Menu membuat pesanan
                                    system(CLEAR);
                                    catalog.readItems();
                                    int catalogId;
                                    cout << "Masukkan ID barang yang ingin dipesan: ";
                                    while(!(cin >> catalogId)){cin.clear();cin.ignore(numeric_limits<streamsize>::max(), '\n');cout << "Masukkan Hanya Angka\nMasukkan ID barang yang ingin dipesan: ";}
                                    FurnitureItem* selectedItem = catalog.findItemByID(catalogId);
                                    if (selectedItem) {
                                        if (loggedInUser) {
                                            string choiceconfirm;
                                            cout << "Anda yakin ingin memesan " << selectedItem->prod_name << "? (Y/N): ";
                                            cin >> choiceconfirm;
                                            if(choiceconfirm == "Y"){
                                                orderManager.placeOrder(selectedItem->prod_name, loggedInUser->username, loggedInUser->phoneNumber, "Pending", selectedItem->price);
                                                orderManager.saveOrdersToCSV("order_history.csv");
                                            }
                                            else if(choiceconfirm == "N"){
                                                cout << "Kembali ke Menu Utama.\n";
                                            }
                                            else{
                                                system(CLEAR);
                                                cout << "Pilihan tidak valid. Silakan coba lagi.\n";
                                                cin.get();
                                            }
                                        } else {
                                            system(CLEAR);
                                            cout << "You need to be logged in to place an order.\n";
                                            cin.get();
                                        }
                                    } else {
                                        system(CLEAR);
                                        cout << "Product not found in the catalog.\n";
                                        cin.get();
                                    }
                                    break;
                            }
                                case 3: // Menu menampilkan pesanan yang berlangsung dan selesai
                                    system(CLEAR);
                                    orderManager.displayUserOrders(loggedInUser->username);
                                    cin.get();
                                    break;
                                case 4: // Logout
                                    system(CLEAR);
                                    cout << "Logged out as Client.\n";
                                    cin.get();
                                    loggedInUser = nullptr;
                                    break;
                                default: // Error Handling Menu
                                    system(CLEAR);
                                    cout << "Invalid choice. Please try again.\n";
                                    cin.get();
                                    break;
                            }
                        } while (loggedInUser && !loggedInUser->isAdmin);
                    }

                } else { // Login Gagal
                    system(CLEAR);
                    cout << "Login failed. Invalid username or password.\n";
                    cin.get();
                }
                break;
            }
            case 2: { // Form Registrasi
                string username, password, phoneNumber, email, address;
                system(CLEAR);
                cout << "Enter username: ";cin.ignore();
                getline(cin, username);
                if (userList.findUser(username) == nullptr) {
                    cout << "Enter password: ";
                    getline(cin, password);
                    cout << "Enter phone number: ";
                    getline(cin, phoneNumber);
                    cout << "Enter email: ";
                    getline(cin, email);
                    cout << "Enter address: ";
                    getline(cin, address); 
                    userList.addUser(username, password, phoneNumber, email, address, false); // isAdmin nilai default False
                
                    // Menyimpan data registrasi User ke file CSV
                    ofstream outFile(filename, ios::app);
                    if (outFile.is_open()) {
                        outFile << username << "," << password << "," << phoneNumber << "," << email << "," << address << "," << false << endl;
                        outFile.close();
                        system(CLEAR);
                        cout << "User registered successfully!\n";
                        cin.get();
                        break;
                    } else {
                        system(CLEAR);
                        cout << "Unable to open the file for saving. Registration failed.\n";
                        cin.get();
                    }
                } else {
                    system(CLEAR);
                    cout << "Username already exists. Registration failed.\n";
                    break;
                    cin.get();
                }
            }
            case 3:{
                system(CLEAR);
                cout << "Goodbye!\n";
                cin.get();
                exit(0);
            }
            default:
                system(CLEAR);
                cout << "Invalid choice. Please try again.\n";
                cin.get();
                break;
        }
    }
     } while (loggedInUser == nullptr);

    return 0;
}
