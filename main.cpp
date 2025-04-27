#include <iostream>
using namespace std;

template <typename T>
class CircularDoublyList {
private:
    struct Node {
        T val;
        Node* next;
        Node* prev;
        Node(T x) : val(x), next(this), prev(this) {}
    };

    Node* head;        // puntero al primer nodo
    size_t n;          // tamaño lógico

    // Devuelve puntero al nodo en posición idx 
    Node* at(int idx) const {
        if (idx < 0 || idx >= static_cast<int>(n)) {
            cout << "Índice fuera de rango." << endl;
            return nullptr;
        }
        Node* cur;
        // Elegimos dirección de recorrido para optimizar
        if (idx <= static_cast<int>(n) / 2) {
            cur = head;
            for (int i = 0; i < idx; ++i) cur = cur->next;
        } else {
            cur = head->prev;
            for (int i = n - 1; i > idx; --i) cur = cur->prev;
        }
        return cur;
    }

public:
    /* ---------- constructor / destructor ---------- */
    CircularDoublyList() : head(nullptr), n(0) {}
    ~CircularDoublyList() { clear(); }

    /* ---------- acceso ---------- */
    T front() const {
        if (!head) { cout << "La lista está vacía." << endl; return T(); }
        return head->val;
    }

    T back() const {
        if (!head) { cout << "La lista está vacía." << endl; return T(); }
        return head->prev->val;
    }

    T operator[](int idx) {
        Node* node = at(idx);
        return node ? node->val : T();
    }

    /* ---------- inserciones ---------- */
    void push_front(T x) {
        Node* newNode = new Node(x);
        if (!head) head = newNode;
        else {
            Node* tail = head->prev;
            // enlazamos: tail <-> newNode <-> head
            tail->next = newNode;
            newNode->prev = tail;
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        ++n;
    }

    void push_back(T x) {
        Node* newNode = new Node(x);
        if (!head) head = newNode;
        else {
            Node* tail = head->prev;
            tail->next = newNode;
            newNode->prev = tail;
            newNode->next = head;
            head->prev = newNode;
        }
        ++n;
    }

    // Inserta en posición idx (0 = principio, n = final)
    void insert(T x, int idx) {
        if (idx < 0 || idx > static_cast<int>(n)) {
            cout << "Índice fuera de rango." << endl; return;
        }
        if (idx == 0) { push_front(x); return; }
        if (idx == static_cast<int>(n)) { push_back(x); return; }

        Node* pos = at(idx);
        Node* newNode = new Node(x);

        Node* prev = pos->prev;
        prev->next = newNode;
        newNode->prev = prev;
        newNode->next = pos;
        pos->prev = newNode;
        ++n;
    }

    /* ---------- eliminaciones ---------- */
    void pop_front() {
        if (!head) return;
        if (n == 1) { delete head; head = nullptr; n = 0; return; }
        Node* tail = head->prev;
        Node* old = head;
        head = head->next;
        tail->next = head;
        head->prev = tail;
        delete old;
        --n;
    }

    void pop_back() {
        if (!head) return;
        if (n == 1) { delete head; head = nullptr; n = 0; return; }
        Node* tail = head->prev;
        Node* newTail = tail->prev;
        newTail->next = head;
        head->prev = newTail;
        delete tail;
        --n;
    }

    void remove(int idx) {
        if (idx < 0 || idx >= static_cast<int>(n)) {
            cout << "Índice fuera de rango." << endl; return;
        }
        if (idx == 0) { pop_front(); return; }
        if (idx == static_cast<int>(n) - 1) { pop_back(); return; }

        Node* target = at(idx);
        target->prev->next = target->next;
        target->next->prev = target->prev;
        delete target;
        --n;
    }

    /* ---------- utilidades ---------- */
    bool empty() const { return n == 0; }
    int  size()  const { return static_cast<int>(n); }

    void clear() {
        while (!empty()) pop_back();
    }

    void reverse() {
        if (n < 2) return;
        Node* cur = head;
        for (size_t i = 0; i < n; ++i) {
            swap(cur->next, cur->prev);
            cur = cur->prev; 
        }
        head = head->next; 
    }

    void print_forward() const {
        if (!head) { cout << "(vacía)" << endl; return; }
        Node* cur = head;
        for (size_t i = 0; i < n; ++i) {
            cout << cur->val << " ";
            cur = cur->next;
        }
        cout << endl;
    }

    void print_backward() const {
        if (!head) { cout << "(vacía)" << endl; return; }
        Node* cur = head->prev;
        for (size_t i = 0; i < n; ++i) {
            cout << cur->val << " ";
            cur = cur->prev;
        }
        cout << endl;
    }
};


int main() {
    CircularDoublyList<int> list;

    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    list.push_back(40);

    cout << "Lista inicial: ";
    list.print_forward();     // 10 20 30 40

    list.push_front(5);
    cout << "Después de push_front(5): ";
    list.print_forward();     // 5 10 20 30 40

    list.insert(25, 3);
    cout << "Insertar 25 en idx 3: ";
    list.print_forward();     // 5 10 20 25 30 40

    cout << "front(): " << list.front() << endl; // 5
    cout << "back():  " << list.back()  << endl; // 40
    cout << "list[4]: " << list[4]      << endl; // 30

    list.pop_front();
    list.pop_back();
    cout << "Tras pop_front + pop_back: ";
    list.print_forward();     // 10 20 25 30

    list.reverse();
    cout << "Reverse: ";
    list.print_forward();     // 30 25 20 10
    cout << "Reverse (sentido contrario): ";
    list.print_backward();    // 10 20 25 30

    cout << "Size: " << list.size() << endl;     // 4
    cout << "empty?: " << list.empty() << endl;  // 0 (false)

    list.clear();
    cout << "Después de clear(), empty?: " << list.empty() << endl; // 1 (true)

    return 0;
}

