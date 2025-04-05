#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <iomanip>

using namespace std;

struct Task {
    string title;
    int priority; // 1 = High, 2 = Medium, 3 = Low
    string dueDate; // format: YYYY-MM-DD
    bool completed;

    Task(string t, int p, string d, bool c = false)
        : title(t), priority(p), dueDate(d), completed(c) {}
};

class TaskManager {
private:
    vector<Task> tasks;

public:
    void loadFromFile(const string& filename);
    void saveToFile(const string& filename);
    void addTask();
    void listTasks();
    void listByPriority();
    void deleteTask();
    void markCompleted();
};

void showMenu() {
    cout << "\n========== Smart Task Scheduler ==========\n";
    cout << "1. Add New Task\n";
    cout << "2. View All Tasks\n";
    cout << "3. View Tasks by Priority\n";
    cout << "4. Mark Task as Completed\n";
    cout << "5. Delete a Task\n";
    cout << "6. Exit and Save\n";
    cout << "=========================================\n";
    cout << "Enter your choice: ";
}

int main() {
    TaskManager tm;
    string filename = "tasks.txt";
    tm.loadFromFile(filename);

    int choice = 0;
    while (true) {
        showMenu();
        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            cin.clear(); // clear error flag
            cin.ignore(10000, '\n'); // discard bad input
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        cin.ignore(); // flush newline

        switch (choice) {
            case 1: tm.addTask(); break;
            case 2: tm.listTasks(); break;
            case 3: tm.listByPriority(); break;
            case 4: tm.markCompleted(); break;
            case 5: tm.deleteTask(); break;
            case 6: tm.saveToFile(filename); return 0;
            default: cout << "Invalid choice. Please try again.\n";
        }
    }
}


void TaskManager::addTask() {
    string title, dueDate;
    int priority;

    cout << "Enter task title: ";
    getline(cin, title);
    cout << "Enter due date (YYYY-MM-DD): ";
    getline(cin, dueDate);
    cout << "Enter priority (1 = High, 2 = Medium, 3 = Low): ";
    cin >> priority;
    cin.ignore();

    tasks.emplace_back(title, priority, dueDate);
    cout << "Task added!\n";
}

void TaskManager::listTasks() {
    if (tasks.empty()) {
        cout << "No tasks to display.\n";
        return;
    }

    cout << left << setw(5) << "No" << setw(25) << "Title" << setw(12)
         << "Due Date" << setw(10) << "Priority" << setw(10) << "Status" << endl;
    cout << string(65, '-') << endl;

    for (size_t i = 0; i < tasks.size(); ++i) {
        string pr;
        if (tasks[i].priority == 1) pr = "High";
        else if (tasks[i].priority == 2) pr = "Medium";
        else pr = "Low";

        cout << setw(5) << i+1
             << setw(25) << tasks[i].title
             << setw(12) << tasks[i].dueDate
             << setw(10) << pr
             << setw(10) << (tasks[i].completed ? "Done" : "Pending")
             << endl;
    }
}

void TaskManager::listByPriority() {
    if (tasks.empty()) {
        cout << "No tasks to display.\n";
        return;
    }

    vector<Task> sorted = tasks;
    sort(sorted.begin(), sorted.end(), [](const Task& a, const Task& b) {
        return a.priority < b.priority;
    });

    cout << "Tasks sorted by priority:\n";
    for (size_t i = 0; i < sorted.size(); ++i) {
        string pr = (sorted[i].priority == 1 ? "High" : (sorted[i].priority == 2 ? "Medium" : "Low"));
        cout << i+1 << ". " << sorted[i].title << " | Due: " << sorted[i].dueDate
             << " | Priority: " << pr << " | Status: "
             << (sorted[i].completed ? "Done" : "Pending") << "\n";
    }
}

void TaskManager::markCompleted() {
    int index;
    listTasks();
    cout << "Enter task number to mark as completed: ";
    cin >> index;
    cin.ignore();

    if (index < 1 || index > (int)tasks.size()) {
        cout << "Invalid task number.\n";
        return;
    }

    tasks[index - 1].completed = true;
    cout << "Marked as completed!\n";
}

void TaskManager::deleteTask() {
    int index;
    listTasks();
    cout << "Enter task number to delete: ";
    cin >> index;
    cin.ignore();

    if (index < 1 || index > (int)tasks.size()) {
        cout << "Invalid task number.\n";
        return;
    }

    tasks.erase(tasks.begin() + index - 1);
    cout << "Task deleted.\n";
}

void TaskManager::saveToFile(const string& filename) {
    ofstream out(filename);
    for (const auto& t : tasks) {
        out << t.title << ";" << t.priority << ";" << t.dueDate << ";" << t.completed << "\n";
    }
    out.close();
    cout << "Tasks saved successfully.\n";
}

void TaskManager::loadFromFile(const string& filename) {
    ifstream in(filename);
    if (!in) return;

    string line;
    while (getline(in, line)) {
        size_t pos1 = line.find(';');
        size_t pos2 = line.find(';', pos1 + 1);
        size_t pos3 = line.find(';', pos2 + 1);

        string title = line.substr(0, pos1);
        int priority = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
        string dueDate = line.substr(pos2 + 1, pos3 - pos2 - 1);
        bool completed = stoi(line.substr(pos3 + 1));

        tasks.emplace_back(title, priority, dueDate, completed);
    }

    in.close();
}

