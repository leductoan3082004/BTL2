#include <iostream>
using namespace std;

class Node {
public:
    int data;
    Node *next;

    Node(int val) {
        data = val;
        next = nullptr;
    }
};

Node *head = nullptr;

void insert(int val) {
    Node *newNode = new Node(val);
    newNode->next = head;
    head = newNode;
}

void remove_k(int k) {
    if (!head)
        return;

    Node *curr = head;
    Node *prev = nullptr;
    while (curr != nullptr) {
        if (curr->data == k) {
            if (curr == head) {
                delete head;
                cur = curr->next;
                head = curr;
            } else {
                Node *new_head = head->next;
                Node *tmp = head;
                tmp->next = curr->next;
                prev->next = 
            }
        }
        prev = curr;
        curr = curr->next;
    }
}

void display() {
    Node *temp = head;
    while (temp) {
        cout << temp->data << " ";
        temp = temp->next;
    }
    cout << endl;
}

int main() {
    insert(5);
    insert(4);
    insert(3);
    insert(2);
    insert(1);

    cout << "Original linked list: ";
    display();

    remove_k(3);

    cout << "Linked list after removing 3: ";
    display();

    return 0;
}