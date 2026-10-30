// AI Study Buddy - IT23304 Object Oriented Programming (Team 17)
// A console learning assistant: subjects, quizzes, study sessions and reports.
// Build: g++ -std=c++11 -Wall -o study_buddy main.cpp
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

// ---------- Ex 1: basic commands (cout/cin, getline, loops, try-catch) ----------
void printLine(int length = 70) {
    cout << string(length, '-') << '\n';
}

string readString(const string& prompt) {
    string value;
    cout << prompt;
    if (!getline(cin, value)) {
        throw runtime_error("Input ended.");
    }
    return value;
}

int readInt(const string& prompt) {
    while (true) {
        string input = readString(prompt);
        try {
            size_t used = 0;
            int value = stoi(input, &used);
            if (input.find_first_not_of(" \t", used) == string::npos) {
                return value;
            }
        } catch (const exception&) {
            // Fall through and ask again.
        }
        cout << "Please enter a valid whole number.\n";
    }
}

string toLower(string s) {
    for (size_t i = 0; i < s.size(); ++i) {
        s[i] = static_cast<char>(tolower(static_cast<unsigned char>(s[i])));
    }
    return s;
}

vector<string> splitFields(const string& line) {
    vector<string> fields;
    string part;
    stringstream ss(line);
    while (getline(ss, part, '|')) {
        fields.push_back(part);
    }
    if (!line.empty() && line[line.size() - 1] == '|') {
        fields.push_back("");
    }
    return fields;
}

string cleanText(string s) {
    replace(s.begin(), s.end(), '|', '/');
    return s;
}

void checkDate(const string& d) {
    bool ok = d.size() == 10 && d[2] == '-' && d[5] == '-';
    for (size_t i = 0; ok && i < d.size(); ++i) {
        if (i != 2 && i != 5 && !isdigit(static_cast<unsigned char>(d[i]))) {
            ok = false;
        }
    }
    if (!ok) {
        throw invalid_argument("Date must be in dd-mm-yyyy format.");
    }
}

// ---------- Ex 7: function template, inline function, class template ----------
template <typename T>
void printCell(const T& value, int width) {
    cout << left << setw(width) << value;
}

inline bool isLowProgress(int done, int total) {
    return total > 0 && done * 2 < total;
}

template <typename T>
class RecordBox {
private:
    vector<T> data;
public:
    void add(const T& value) { data.push_back(value); }
    int size() const { return static_cast<int>(data.size()); }
    T largest() const {
        if (data.empty()) throw runtime_error("RecordBox is empty.");
        T best = data[0];
        for (size_t i = 1; i < data.size(); ++i) {
            if (data[i] > best) best = data[i];
        }
        return best;
    }
    double average() const {
        double sum = 0;
        for (size_t i = 0; i < data.size(); ++i) sum += data[i];
        return data.empty() ? 0.0 : sum / data.size();
    }
};

// ---------- Ex 2, 4, 5: class with private data, operator==, friends ----------
class Tutor;  // forward declaration for the friend class

class Subject {
private:
    int id;
    string name;
    string category;
    int total;
    int done;
    string level;

    static void requireText(const string& v, const string& field) {
        if (v.empty()) {
            throw invalid_argument(field + " cannot be empty.");
        }
        if (v.find('|') != string::npos) {
            throw invalid_argument(field + " cannot contain the '|' character.");
        }
    }

public:
    Subject(int subjectId, const string& n, const string& c,
            int chapters, int chaptersDone, const string& lv)
        : id(subjectId), total(0), done(0), level("Medium") {
        if (subjectId <= 0) {
            throw invalid_argument("Subject ID must be a positive number.");
        }
        setName(n);
        setCategory(c);
        setLevel(lv);
        setTotal(chapters);
        setDone(chaptersDone);
    }
    virtual ~Subject() {}

    int getId() const { return id; }
    string getName() const { return name; }
    string getCategory() const { return category; }
    int getTotal() const { return total; }
    int getDone() const { return done; }
    string getLevel() const { return level; }

    void setName(const string& v) { requireText(v, "Name"); name = v; }
    void setCategory(const string& v) { requireText(v, "Category"); category = v; }
    void setLevel(const string& v) {
        string l = toLower(v);
        if (l == "easy") level = "Easy";
        else if (l == "medium") level = "Medium";
        else if (l == "hard") level = "Hard";
        else throw invalid_argument("Difficulty must be Easy, Medium or Hard.");
    }
    void setTotal(int v) {
        if (v < 0) throw invalid_argument("Total chapters cannot be negative.");
        if (v < done) throw invalid_argument("Total chapters is below completed chapters.");
        total = v;
    }
    void setDone(int v) {
        if (v < 0) throw invalid_argument("Completed chapters cannot be negative.");
        if (v > total) throw invalid_argument("Completed chapters exceed total chapters.");
        done = v;
    }

    // Operator overloading: two subjects are equal when their IDs match.
    bool operator==(const Subject& other) const { return id == other.id; }

    // Harder subjects with more chapters left get a bigger share of study time.
    int weight() const {
        int factor = (level == "Easy") ? 1 : (level == "Medium") ? 2 : 3;
        return (total - done) * factor;
    }

    // Ex 8: virtual functions (pure virtual and overridable)
    virtual string getType() const = 0;
    virtual string getExtra() const = 0;
    virtual void setExtra(const string& value) = 0;
    virtual string extraLabel() const = 0;
    virtual string status() const {
        if (done == 0) return "Not Started";
        if (done >= total) return "Completed";
        return "In Progress";
    }
    virtual void displayDetails() const {
        printCell(id, 6);
        printCell(name.size() > 22 ? name.substr(0, 21) + "." : name, 24);
        printCell(category, 14);
        printCell(to_string(done) + "/" + to_string(total), 8);
        printCell(level, 8);
        printCell(status(), 13);
        cout << '\n';
        cout << "    " << extraLabel() << ": " << getExtra() << '\n';
    }

    static void printHeader() {
        printLine(73);
        printCell("ID", 6);
        printCell("Subject", 24);
        printCell("Category", 14);
        printCell("Done", 8);
        printCell("Level", 8);
        printCell("Status", 13);
        cout << '\n';
        printLine(73);
    }

    string toFileString() const {
        return getType() + "|" + to_string(id) + "|" + name + "|" + category + "|" +
               to_string(total) + "|" + to_string(done) + "|" + level + "|" + getExtra();
    }

    // Friend function and friend class can reach the private data.
    friend void progressReport(const Subject& s);
    friend class Tutor;
};

void progressReport(const Subject& s) {
    double percent = s.total > 0 ? s.done * 100.0 / s.total : 0.0;
    cout << "Progress Report -> ID " << s.id << ", " << s.name << ": "
         << s.done << " of " << s.total << " chapters done ("
         << fixed << setprecision(1) << percent << "%)\n";
}

class Tutor {
public:
    void addChapters(Subject& s, int extra) {
        if (extra < 0) throw invalid_argument("Extra chapters cannot be negative.");
        if (s.done + extra > s.total) throw invalid_argument("Completed chapters exceed total chapters.");
        s.done += extra;
        cout << "Tutor added " << extra << " chapter(s) to " << s.name
             << ". New total done: " << s.done << '\n';
    }
};

// ---------- Ex 6: inheritance ----------
class CoreSubject : public Subject {
private:
    string textbook;
public:
    CoreSubject(int i, const string& n, const string& c, int t, int d,
                const string& lv, const string& tb)
        : Subject(i, n, c, t, d, lv), textbook(tb) {}
    string getType() const override { return "CORE"; }
    string getExtra() const override { return textbook; }
    void setExtra(const string& v) override { textbook = cleanText(v); }
    string extraLabel() const override { return "Textbook"; }
};

class PracticalSubject : public Subject {
private:
    string tools;
public:
    PracticalSubject(int i, const string& n, const string& c, int t, int d,
                     const string& lv, const string& tl)
        : Subject(i, n, c, t, d, lv), tools(tl) {}
    string getType() const override { return "PRACTICAL"; }
    string getExtra() const override { return tools; }
    void setExtra(const string& v) override { tools = cleanText(v); }
    string extraLabel() const override { return "Tools"; }
    string status() const override {
        string s = Subject::status();
        return s == "Completed" ? "Lab Ready" : s;
    }
};

class ElectiveSubject : public Subject {
private:
    string focus;
public:
    ElectiveSubject(int i, const string& n, const string& c, int t, int d,
                    const string& lv, const string& f)
        : Subject(i, n, c, t, d, lv), focus(f) {}
    string getType() const override { return "ELECTIVE"; }
    string getExtra() const override { return focus; }
    void setExtra(const string& v) override { focus = cleanText(v); }
    string extraLabel() const override { return "Focus area"; }
    string status() const override {
        string s = Subject::status();
        return s == "Not Started" ? "Optional" : s;
    }
};

Subject* makeSubject(const string& type, int id, const string& name, const string& category,
                     int total, int done, const string& level, const string& extra) {
    string e = cleanText(extra);
    if (type == "CORE") return new CoreSubject(id, name, category, total, done, level, e);
    if (type == "PRACTICAL") return new PracticalSubject(id, name, category, total, done, level, e);
    if (type == "ELECTIVE") return new ElectiveSubject(id, name, category, total, done, level, e);
    throw invalid_argument("Choose a valid subject type.");
}

// ---------- Records stored in STL containers ----------
class QuizRecord {
public:
    int subjectId;
    string subjectName;
    int score;
    int total;
    string before, after, date, remark;

    QuizRecord() : subjectId(0), score(0), total(1) {}

    void display() const {
        cout << "ID: " << subjectId << " | " << subjectName << " | Score: " << score << "/" << total
             << " | Level: " << before << " -> " << after << " | " << date << '\n'
             << "    Feedback: " << remark << '\n';
    }
    string toFileString() const {
        return to_string(subjectId) + "|" + subjectName + "|" + to_string(score) + "|" +
               to_string(total) + "|" + before + "|" + after + "|" + date + "|" + remark;
    }
    void fromFileString(const string& line) {
        vector<string> f = splitFields(line);
        if (f.size() != 8) throw runtime_error("Wrong number of fields.");
        subjectId = stoi(f[0]);
        score = stoi(f[2]);
        total = stoi(f[3]);
        if (subjectId <= 0 || score < 0 || total <= 0 || score > total) {
            throw runtime_error("Invalid quiz values.");
        }
        subjectName = f[1]; before = f[4]; after = f[5]; date = f[6]; remark = f[7];
    }
};

class StudySession {
public:
    int subjectId;
    string subjectName;
    int chapters;
    int minutes;
    int focus;
    string date, remarks;

    StudySession() : subjectId(0), chapters(0), minutes(0), focus(0) {}

    void display() const {
        cout << "ID: " << subjectId << " | " << subjectName << " | Covered: " << chapters
             << " | Time: " << minutes << " mins | Focus: " << focus << "% | " << date
             << " | " << remarks << '\n';
    }
    string toFileString() const {
        return to_string(subjectId) + "|" + subjectName + "|" + to_string(chapters) + "|" +
               to_string(minutes) + "|" + to_string(focus) + "|" + date + "|" + remarks;
    }
    void fromFileString(const string& line) {
        vector<string> f = splitFields(line);
        if (f.size() != 7) throw runtime_error("Wrong number of fields.");
        subjectId = stoi(f[0]);
        chapters = stoi(f[2]);
        minutes = stoi(f[3]);
        focus = stoi(f[4]);
        if (subjectId <= 0 || chapters < 0 || minutes <= 0 || focus < 0 || focus > 100) {
            throw runtime_error("Invalid session values.");
        }
        subjectName = f[1]; date = f[5]; remarks = f[6];
    }
};

// ---------- Quiz questions ----------
struct Question {
    string text;
    string options[3];
    int answer;
};

vector<Question> questionsFor(int id) {
    vector<Question> q;
    if (id == 101) {
        q.push_back(Question{"Which feature lets a derived class reuse base members?",
                             {"Encapsulation", "Inheritance", "Overloading"}, 2});
        q.push_back(Question{"Which keyword enables run-time polymorphism?",
                             {"virtual", "static", "friend"}, 1});
        q.push_back(Question{"Which operator creates an object on the heap?",
                             {"malloc", "new", "alloc"}, 2});
    } else if (id == 102) {
        q.push_back(Question{"Which structure is LIFO?", {"Stack", "Queue", "Tree"}, 1});
        q.push_back(Question{"Which structure is FIFO?", {"Stack", "Queue", "Graph"}, 2});
        q.push_back(Question{"Which traversal visits the root first?",
                             {"Inorder", "Preorder", "Postorder"}, 2});
    } else if (id == 103) {
        q.push_back(Question{"Which law says NOT(p AND q) = (NOT p) OR (NOT q)?",
                             {"De Morgan", "Distributive", "Absorption"}, 1});
        q.push_back(Question{"Reflexive + symmetric + transitive means:",
                             {"Partial order", "Equivalence relation", "Function"}, 2});
        q.push_back(Question{"How many subsets does an n-element set have?",
                             {"n^2", "2^n", "n!"}, 2});
    } else {
        q.push_back(Question{"Which is the best revision technique?",
                             {"Re-reading only", "Active recall", "Highlighting only"}, 2});
        q.push_back(Question{"What is an ideal study block?",
                             {"25-50 min with breaks", "6 hours straight", "5 min"}, 1});
        q.push_back(Question{"When should you revise a new topic?",
                             {"Never", "Within 24 hours", "After exams"}, 2});
    }
    return q;
}

// ---------- Ex 9: file handling with a template ----------
template <typename Container, typename Record>
void loadRecords(const string& fileName, Container& records, const string& what) {
    ifstream file(fileName.c_str());
    string line;
    while (file && getline(file, line)) {
        if (line.empty()) continue;
        try {
            Record r;
            r.fromFileString(line);
            records.push_back(r);
        } catch (const exception&) {
            cout << "Skipped one invalid " << what << " record.\n";
        }
    }
}

template <typename Container>
void saveRecords(const string& fileName, const Container& records, const string& what) {
    ofstream file(fileName.c_str());
    if (!file) {
        cout << "Could not save " << what << " file.\n";
        return;
    }
    for (typename Container::const_iterator it = records.begin(); it != records.end(); ++it) {
        file << it->toFileString() << '\n';
    }
}

// ---------- Ex 10: STL containers (map, vector, list) ----------
class StudyBuddy {
private:
    map<int, Subject*> subjects;      // sorted by subject ID
    vector<QuizRecord> quizzes;       // quiz history in order
    list<StudySession> sessions;      // study session history
    string subjectFile, quizFile, sessionFile;

    StudyBuddy(const StudyBuddy&);
    StudyBuddy& operator=(const StudyBuddy&);

    Subject* find(int id) const {
        map<int, Subject*>::const_iterator it = subjects.find(id);
        return it == subjects.end() ? 0 : it->second;
    }

    void seedSubjects() {
        subjects[101] = makeSubject("CORE", 101, "Object Oriented Programming", "Programming",
                                    12, 8, "Medium", "Classes, Inheritance, Templates");
        subjects[102] = makeSubject("CORE", 102, "Data Structures", "Programming",
                                    10, 7, "Hard", "Stacks, Queues, Trees, Graphs");
        subjects[103] = makeSubject("ELECTIVE", 103, "Discrete Mathematics", "Mathematics",
                                    8, 3, "Medium", "Logic, Sets, Relations, Functions");
    }

public:
    StudyBuddy()
        : subjectFile("subjects.txt"), quizFile("quizzes.txt"), sessionFile("sessions.txt") {}

    ~StudyBuddy() {
        for (map<int, Subject*>::iterator it = subjects.begin(); it != subjects.end(); ++it) {
            delete it->second;
        }
    }

    void load() {
        ifstream file(subjectFile.c_str());
        if (!file) {
            seedSubjects();
        } else {
            string line;
            while (getline(file, line)) {
                if (line.empty()) continue;
                try {
                    vector<string> f = splitFields(line);
                    if (f.size() != 8) throw runtime_error("Wrong number of fields.");
                    Subject* s = makeSubject(f[0], stoi(f[1]), f[2], f[3],
                                             stoi(f[4]), stoi(f[5]), f[6], f[7]);
                    map<int, Subject*>::iterator old = subjects.find(s->getId());
                    if (old != subjects.end()) {
                        delete old->second;
                    }
                    subjects[s->getId()] = s;
                } catch (const exception&) {
                    cout << "Skipped one invalid subject record.\n";
                }
            }
        }
        loadRecords<vector<QuizRecord>, QuizRecord>(quizFile, quizzes, "quiz");
        loadRecords<list<StudySession>, StudySession>(sessionFile, sessions, "session");
    }

    void save() const {
        ofstream file(subjectFile.c_str());
        if (!file) {
            cout << "Could not save subject file.\n";
        } else {
            for (map<int, Subject*>::const_iterator it = subjects.begin(); it != subjects.end(); ++it) {
                file << it->second->toFileString() << '\n';
            }
        }
        saveRecords(quizFile, quizzes, "quiz");
        saveRecords(sessionFile, sessions, "session");
    }

    // ----- subject management -----
    void addSubject() {
        int id = readInt("Subject ID: ");
        cout << "1. Core Subject\n2. Practical Subject\n3. Elective Subject\n";
        int t = readInt("Choose type: ");
        string type = t == 1 ? "CORE" : t == 2 ? "PRACTICAL" : t == 3 ? "ELECTIVE" : "";
        string name = readString("Name: ");
        string category = readString("Category: ");
        int total = readInt("Total chapters: ");
        int done = readInt("Chapters completed: ");
        string level = readString("Difficulty (Easy/Medium/Hard): ");
        string extra = readString("Detail (textbook / tools / focus area): ");
        try {
            Subject* s = makeSubject(type, id, name, category, total, done, level, extra);
            // Use the overloaded == operator to compare subject IDs.
            for (map<int, Subject*>::const_iterator it = subjects.begin(); it != subjects.end(); ++it) {
                if (*it->second == *s) {
                    cout << "This subject ID already exists.\n";
                    delete s;
                    return;
                }
            }
            subjects[id] = s;
            cout << "Subject added.\n";
        } catch (const exception& error) {
            cout << error.what() << '\n';
        }
    }

    void updateSubject() {
        Subject* s = find(readInt("Subject ID to update: "));
        if (s == 0) {
            cout << "Subject not found.\n";
            return;
        }
        cout << "1. Name\n2. Category\n3. Chapters completed\n4. Difficulty\n5. "
             << s->extraLabel() << '\n';
        int choice = readInt("Choose field: ");
        try {
            if (choice == 1) s->setName(readString("New name: "));
            else if (choice == 2) s->setCategory(readString("New category: "));
            else if (choice == 3) s->setDone(readInt("New chapters completed: "));
            else if (choice == 4) s->setLevel(readString("New difficulty: "));
            else if (choice == 5) s->setExtra(readString("New value: "));
            else {
                cout << "Invalid choice.\n";
                return;
            }
        } catch (const exception& error) {
            cout << error.what() << '\n';
            return;
        }
        cout << "Subject updated. Status: " << s->status() << '\n';
    }

    void deleteSubject() {
        int id = readInt("Subject ID to delete: ");
        map<int, Subject*>::iterator it = subjects.find(id);
        if (it == subjects.end()) {
            cout << "Subject not found.\n";
            return;
        }
        delete it->second;
        subjects.erase(it);
        cout << "Subject deleted.\n";
    }

    // ----- Ex 3: function overloading - three ways to search -----
    void showSubject(int id) const {
        Subject* s = find(id);
        if (s == 0) {
            cout << "[By ID] Subject not found.\n";
            return;
        }
        Subject::printHeader();
        s->displayDetails();
    }

    void showSubject(const string& name) const {
        bool found = false;
        string key = toLower(name);
        for (map<int, Subject*>::const_iterator it = subjects.begin(); it != subjects.end(); ++it) {
            if (toLower(it->second->getName()).find(key) != string::npos) {
                if (!found) Subject::printHeader();
                it->second->displayDetails();
                found = true;
            }
        }
        if (!found) cout << "[By Name] No matching subject.\n";
    }

    void showSubject(const string& category, int minRemaining) const {
        bool found = false;
        string key = toLower(category);
        for (map<int, Subject*>::const_iterator it = subjects.begin(); it != subjects.end(); ++it) {
            const Subject* s = it->second;
            if (toLower(s->getCategory()) == key && s->getTotal() - s->getDone() >= minRemaining) {
                if (!found) Subject::printHeader();
                s->displayDetails();
                found = true;
            }
        }
        if (!found) cout << "[By Category] No subject with at least " << minRemaining << " chapters left.\n";
    }

    void searchMenu() const {
        cout << "1. Search by ID\n2. Search by name\n3. Search by category and chapters left\n";
        int choice = readInt("Choose search: ");
        if (choice == 1) {
            showSubject(readInt("Subject ID: "));
        } else if (choice == 2) {
            showSubject(readString("Search text: "));
        } else if (choice == 3) {
            string category = readString("Category: ");
            showSubject(category, readInt("Minimum chapters left: "));
        } else {
            cout << "Invalid choice.\n";
        }
    }

    void detailedProgress() const {
        Subject* s = find(readInt("Subject ID: "));
        if (s == 0) cout << "Subject not found.\n";
        else progressReport(*s);  // friend function
    }

    void tutorBoost() {
        Subject* s = find(readInt("Subject ID: "));
        if (s == 0) {
            cout << "Subject not found.\n";
            return;
        }
        int extra = readInt("Chapters for the tutor to add: ");
        try {
            Tutor tutor;  // friend class
            tutor.addChapters(*s, extra);
        } catch (const exception& error) {
            cout << error.what() << '\n';
        }
    }

    void subjectMenu() {
        while (true) {
            cout << "\nSubject Management\n1. Add subject\n2. Update subject\n3. Delete subject\n"
                 << "4. Search subject\n5. Detailed progress\n6. Tutor adds chapters\n0. Back\n";
            int choice = readInt("Choice: ");
            if (choice == 1) addSubject();
            else if (choice == 2) updateSubject();
            else if (choice == 3) deleteSubject();
            else if (choice == 4) searchMenu();
            else if (choice == 5) detailedProgress();
            else if (choice == 6) tutorBoost();
            else if (choice == 0) return;
            else cout << "Invalid choice.\n";
        }
    }

    // ----- quiz -----
    void takeQuiz() {
        Subject* s = find(readInt("Subject ID: "));
        if (s == 0) {
            cout << "Subject not found.\n";
            return;
        }
        vector<Question> qs = questionsFor(s->getId());
        int score = 0;
        cout << "\nQuiz: " << s->getName() << " (" << qs.size() << " questions)\n";
        for (size_t i = 0; i < qs.size(); ++i) {
            cout << "\nQ" << i + 1 << ". " << qs[i].text << '\n';
            for (int k = 0; k < 3; ++k) {
                cout << "  " << k + 1 << ". " << qs[i].options[k] << '\n';
            }
            int answer = 0;
            while (answer < 1 || answer > 3) {
                answer = readInt("Your answer (1-3): ");
                if (answer < 1 || answer > 3) cout << "Choose 1, 2 or 3.\n";
            }
            if (answer == qs[i].answer) {
                ++score;
                cout << "Correct!\n";
            } else {
                cout << "Wrong. Answer: " << qs[i].answer << ". "
                     << qs[i].options[qs[i].answer - 1] << '\n';
            }
        }
        int total = static_cast<int>(qs.size());
        int percent = score * 100 / total;
        string levels[3] = {"Easy", "Medium", "Hard"};
        QuizRecord r;
        r.subjectId = s->getId();
        r.subjectName = s->getName();
        r.score = score;
        r.total = total;
        r.before = s->getLevel();
        int k = r.before == "Easy" ? 0 : r.before == "Medium" ? 1 : 2;
        if (percent >= 80 && k < 2) ++k;
        else if (percent < 40 && k > 0) --k;
        s->setLevel(levels[k]);
        r.after = s->getLevel();
        r.remark = percent >= 80 ? "Great work, ready for harder topics"
                 : percent >= 40 ? "Keep practising" : "Revise " + s->getName();
        while (true) {
            try {
                r.date = readString("Date (dd-mm-yyyy): ");
                checkDate(r.date);
                break;
            } catch (const invalid_argument& error) {
                cout << error.what() << '\n';
            }
        }
        quizzes.push_back(r);
        cout << "\nScore: " << score << " / " << total << " (" << percent << "%)\n"
             << "Level before: " << r.before << "\nLevel after: " << r.after
             << "\nFeedback: " << r.remark << '\n';
    }

    // ----- study sessions -----
    void pendingChapters() const {
        for (map<int, Subject*>::const_iterator it = subjects.begin(); it != subjects.end(); ++it) {
            cout << "ID: " << it->first << " | " << it->second->getName()
                 << " | Pending chapters: " << it->second->getTotal() - it->second->getDone() << '\n';
        }
    }

    void logSession() {
        Subject* s = find(readInt("Subject ID: "));
        if (s == 0) {
            cout << "Subject not found.\n";
            return;
        }
        StudySession x;
        x.subjectId = s->getId();
        x.subjectName = s->getName();
        x.chapters = readInt("Chapters completed: ");
        x.minutes = readInt("Duration (mins): ");
        x.focus = readInt("Focus score (0-100): ");
        x.date = readString("Study date (dd-mm-yyyy): ");
        x.remarks = cleanText(readString("Remarks: "));
        try {
            if (x.chapters < 0) throw invalid_argument("Chapters completed cannot be negative.");
            if (x.minutes <= 0) throw invalid_argument("Duration must be a positive number of minutes.");
            if (x.focus < 0 || x.focus > 100) throw invalid_argument("Focus score must be between 0 and 100.");
            checkDate(x.date);
            s->setDone(s->getDone() + x.chapters);
            sessions.push_back(x);
            cout << "Session saved. Progress updated.\n";
        } catch (const exception& error) {
            cout << error.what() << '\n';
        }
    }

    void sessionHistory() const {
        if (sessions.empty()) {
            cout << "No study sessions yet.\n";
            return;
        }
        for (list<StudySession>::const_iterator it = sessions.begin(); it != sessions.end(); ++it) {
            it->display();
        }
    }

    void quizHistory() const {
        if (quizzes.empty()) {
            cout << "No quiz records yet.\n";
            return;
        }
        for (vector<QuizRecord>::const_iterator it = quizzes.begin(); it != quizzes.end(); ++it) {
            it->display();
        }
    }

    void sessionMenu() {
        while (true) {
            cout << "\nStudy Session Menu\n1. View pending chapters\n2. Log study session\n"
                 << "3. Session history\n0. Back\n";
            int choice = readInt("Choice: ");
            if (choice == 1) pendingChapters();
            else if (choice == 2) logSession();
            else if (choice == 3) sessionHistory();
            else if (choice == 0) return;
            else cout << "Invalid choice.\n";
        }
    }

    // ----- report -----
    void report() const {
        cout << "\nSubject Report\n";
        Subject::printHeader();
        int total = 0, done = 0;
        for (map<int, Subject*>::const_iterator it = subjects.begin(); it != subjects.end(); ++it) {
            it->second->displayDetails();
            if (isLowProgress(it->second->getDone(), it->second->getTotal())) {
                cout << "    ! Low progress - revise soon\n";
            }
            total += it->second->getTotal();
            done += it->second->getDone();
        }
        printLine(73);
        cout << "Subjects: " << subjects.size() << '\n'
             << "Total chapters: " << total << '\n'
             << "Completed chapters: " << done << '\n';

        RecordBox<int> scores;
        for (vector<QuizRecord>::const_iterator it = quizzes.begin(); it != quizzes.end(); ++it) {
            scores.add(it->score * 100 / it->total);
        }
        cout << "Quiz attempts: " << scores.size() << '\n';
        if (scores.size() > 0) {
            cout << "Average score: " << fixed << setprecision(2) << scores.average() << "%\n"
                 << "Best score: " << scores.largest() << "%\n";
        }
        int minutes = 0, focus = 0;
        for (list<StudySession>::const_iterator it = sessions.begin(); it != sessions.end(); ++it) {
            minutes += it->minutes;
            focus += it->focus;
        }
        cout << "Study sessions: " << sessions.size() << '\n'
             << "Total study time: " << minutes << " mins\n";
        if (!sessions.empty()) {
            cout << "Average focus: " << fixed << setprecision(2)
                 << static_cast<double>(focus) / sessions.size() << "%\n";
        }
        cout << "\nStudy buddy suggests: ";
        const Subject* next = 0;
        for (map<int, Subject*>::const_iterator it = subjects.begin(); it != subjects.end(); ++it) {
            if (it->second->getDone() < it->second->getTotal() &&
                (next == 0 || it->second->weight() > next->weight())) {
                next = it->second;
            }
        }
        if (next) cout << next->getName() << " (" << next->getTotal() - next->getDone()
                       << " chapters left, " << next->getLevel() << ")\n";
        else cout << "all subjects are complete.\n";
    }

    void analytics() const {
        report();
        cout << "\nQuiz history\n";
        quizHistory();
        cout << "\nSession history\n";
        sessionHistory();
    }

    void mainMenu() {
        const string bar(50, '=');
        while (true) {
            cout << '\n' << bar << "\n AI STUDY BUDDY - MAIN MENU (User: student)\n" << bar << '\n'
                 << " [1] Manage Academic Subjects & Progress\n"
                 << " [2] Take Subject Assessment Quiz\n"
                 << " [3] View Performance Analytics & History\n"
                 << " [4] Manage Study Sessions\n"
                 << " [0] Save, Logout & Exit\n" << bar << '\n';
            int choice = readInt("Select an option (0-4): ");
            if (choice == 1) subjectMenu();
            else if (choice == 2) takeQuiz();
            else if (choice == 3) analytics();
            else if (choice == 4) sessionMenu();
            else if (choice == 0) {
                save();
                cout << ">> Data saved. Logged out. Goodbye.\n";
                return;
            } else cout << "Invalid choice.\n";
        }
    }
};

// ---------- Ex 1: login ----------
class Student {
private:
    string username;
    string password;
public:
    Student() : username("student"), password("study123") {}
    bool login() const {
        for (int attempt = 1; attempt <= 3; ++attempt) {
            string user = readString("Username: ");
            string pass = readString("Password: ");
            if (user == username && pass == password) {
                cout << "Login successful.\n";
                return true;
            }
            cout << "Incorrect login (" << attempt << "/3).\n";
        }
        return false;
    }
};

// ---------- Ex 8: exceptions in main ----------
int main() {
    try {
        cout << "AI STUDY BUDDY - LOGIN\n";
        Student student;
        if (!student.login()) {
            cout << "Too many wrong attempts.\n";
            return 0;
        }
        StudyBuddy app;
        app.load();
        app.mainMenu();
    } catch (const exception& error) {
        cerr << "Program stopped: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
