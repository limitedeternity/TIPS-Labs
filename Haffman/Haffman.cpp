/*
 * Программа для кодирования методом Хаффмана.
 * 
 * Автор: Беспалов В. (3 курс, ИС)
 * Эффективное время написания: ~12 часов.
 * Компилятор: g++ (GCC) 10.2.0
 * Среда: Visual Studio 2019
 * Доп.флаги компиляции: -std=c++17
 */

#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <string>
#include <any>
#include <functional> // для типа function
#include <algorithm> // для sort
#include <sstream> // для ostringstream
#include <iterator> // для ostream_iterator

using namespace std;

// BSTNode – вершина.
// BSTConnector – ребро.
// BSTException – класс исключения.
class BSTNode {
public:
    class BSTException : public exception {
    public:
        BSTException(const char* err) : _err(err) {}

        virtual const char* what() const noexcept override {
            return _err;
        }

    private:
        const char* _err;
    };

    class BSTConnector {
    public:
        bool hasNextTree() {
            return this->NextTree != NULL;
        }

        bool hasValue() {
            return this->HasValue;
        }

        BSTNode* getNextTree() {
            return this->NextTree;
        }

        BSTNode* getPreviousTree() {
            return this->PreviousTree;
        }

        char getValue() {
            return this->Value;
        }

        int getSign() {
            return this->Sign;
        }

        void setValue(char value) {
            if (this->hasNextTree()) throw BSTException("IllegalAttachment");

            this->Value = value;
            this->HasValue = true;
        }

        BSTNode* attachNewTree() {
            if (this->HasValue) throw BSTException("IllegalAttachment");
            if (this->hasNextTree()) throw BSTException("TreeExists");

            this->NextTree = new BSTNode(this);
            return this->NextTree;
        }

        BSTNode* attachPrebuiltTree(BSTNode* tree) {
            if (this->HasValue) throw BSTException("IllegalAttachment");
            if (this->hasNextTree()) throw BSTException("TreeExists");

            tree->_setAttachedTo(this);
            this->NextTree = tree;
            return this->NextTree;
        }

        BSTConnector(BSTNode* previousTree, int sign) {
            this->Sign = sign;
            this->HasValue = false;
            this->Value = (char) 10;
            this->PreviousTree = previousTree;
            this->NextTree = NULL;
        }

        ~BSTConnector() {
            delete this->NextTree;
        }

    private:
        int Sign;
        bool HasValue;
        char Value;
        BSTNode* PreviousTree;
        BSTNode* NextTree;
    };

    bool isAttached() {
        return this->AttachedTo != NULL;
    }

    BSTConnector* getAttachedTo() {
        return this->AttachedTo;
    }

    BSTConnector* getLeft() {
        return this->Left;
    }

    BSTConnector* getRight() {
        return this->Right;
    }

    void _setAttachedTo(BSTConnector* attachedTo) {
        this->AttachedTo = attachedTo;
    }

    BSTConnector* search(char c) {
        stack<BSTConnector*> traversal;
        traversal.push(this->Right);
        traversal.push(this->Left);

        while (!traversal.empty()) {
            BSTConnector* temp = traversal.top();
            traversal.pop();

            if (temp->hasValue() && temp->getValue() == c) {
                return temp;
            }

            if (temp->hasNextTree()) {
                traversal.push(temp->getNextTree()->getRight());
                traversal.push(temp->getNextTree()->getLeft());
            }
        }

        return NULL;
    }

    string stringify() {
        return "(" + (this->Left->hasValue() ? string(1, this->Left->getValue()) : this->Left->hasNextTree() ? this->Left->getNextTree()->stringify() : "") + ")"
            + "(" + (this->Right->hasValue() ? string(1, this->Right->getValue()) : this->Right->hasNextTree() ? this->Right->getNextTree()->stringify() : "") + ")";
    }

    BSTNode(BSTConnector* attachedTo) {
        this->AttachedTo = attachedTo;
        this->Left = new BSTConnector(this, 0);
        this->Right = new BSTConnector(this, 1);
    }

    ~BSTNode() {
        delete this->Left;
        delete this->Right;
    }

private:
    BSTConnector* AttachedTo;
    BSTConnector* Left;
    BSTConnector* Right;
};

// Конвертация вектора в строку
template<class T> string vecToString(vector<T> vec, char const * delim = "") {
    ostringstream oss;
    if (!vec.empty()) {
        copy(vec.begin(), vec.end() - 1, ostream_iterator<T>(oss, delim));
        oss << vec.back();
    }

    return oss.str();
}

// "Переворачивание" ассоциативного контейнера map
template <class A, class B> multimap<B, A> flip_map(map<A, B>& src) {
    multimap<B, A> dst;
    for (typename map<A, B>::const_iterator it = src.begin(); it != src.end(); it++) {
        dst.insert(pair<B, A>(it->second, it->first));
    }

    return dst;
}

// Кастомная "очередь с приоритетом"
template <class T> class PQueue {
public:
    T top() {
        return this->storage[0];
    }

    bool empty() {
        return this->storage.empty();
    }

    int size() {
        return this->storage.size();
    }

    int push(T el) {
        size_t i = 0;
        while (i < this->storage.size() && !comparator(el, this->storage[i])) i++;
        this->storage.insert(this->storage.begin() + i, el);
        return i;
    }

    void pop() {
        this->storage.erase(this->storage.begin());
    }

    PQueue(function<bool(T, T)> cmp) {
        this->comparator = cmp;
    }

    PQueue(vector<T>& vec, function<bool(T, T)> cmp) {
        this->comparator = cmp;
        this->storage = vec;
        sort(this->storage.begin(), this->storage.end(), this->comparator);
    }

    ~PQueue() {
        this->storage.clear();
    }

private:
    function<bool(T, T)> comparator;
    vector<T> storage;
};

int main(void) {
    cout << "Input: ";

    string input;
    getline(cin, input);
    if (input.empty()) return 0;

    // Частотный анализ
    map<char, int> freqTable;
    for (size_t i = 0; i < input.size(); i++) {
        freqTable[input[i]] += 1;
    }

    // Вхождения сортируются (по возрастанию) по ключам автоматически
    multimap<int, char> flipFreq = flip_map(freqTable);

    // Построение уже отсортированных векторов с символами и соотв. вероятностями
    vector<char> chars;
    vector<double> probabilities;
    for (map<int, char>::const_iterator it = flipFreq.begin(); it != flipFreq.end(); it++) {
        chars.push_back(it->second);
        probabilities.push_back((double)it->first / (double)input.size());
    }

    PQueue<double>* q = new PQueue<double>(probabilities, [](double a, double b) -> bool { return a < b; });
    vector<any> buffer; // тип: char | BSTNode*
    for (size_t i = 0; i < chars.size(); i++) {
        buffer.push_back(chars[i]);
    }

    while (buffer.size() > 1) {
        double probSum = 0.0;
        probSum += q->top(); q->pop();
        probSum += q->top(); q->pop();
        int insertIndex = q->push(probSum);

        BSTNode* miniTree = new BSTNode(NULL);

        try { // оба char
            char fst = any_cast<char>(buffer[0]);
            char snd = any_cast<char>(buffer[1]);
            miniTree->getLeft()->setValue(fst);
            miniTree->getRight()->setValue(snd);
        }
        catch ([[maybe_unused]] const bad_any_cast& _) {
            try { // слева дерево, справа char
                BSTNode* fst = any_cast<BSTNode*>(buffer[0]);
                char snd = any_cast<char>(buffer[1]);
                miniTree->getLeft()->attachPrebuiltTree(fst);
                miniTree->getRight()->setValue(snd);
            }
            catch ([[maybe_unused]] const bad_any_cast& _) {
                try { // слева char, справа дерево
                    char fst = any_cast<char>(buffer[0]);
                    BSTNode* snd = any_cast<BSTNode*>(buffer[1]);
                    miniTree->getLeft()->setValue(fst);
                    miniTree->getRight()->attachPrebuiltTree(snd);
                }
                catch ([[maybe_unused]] const bad_any_cast& _) {
                    // оба деревья
                    BSTNode* fst = any_cast<BSTNode*>(buffer[0]);
                    BSTNode* snd = any_cast<BSTNode*>(buffer[1]);
                    miniTree->getLeft()->attachPrebuiltTree(fst);
                    miniTree->getRight()->attachPrebuiltTree(snd);
                }
            }
        }

        cout << "Size = " << buffer.size() << "; Position = " << insertIndex << ": " << miniTree->stringify() << endl;
        buffer.erase(buffer.begin(), buffer.begin() + 2);
        buffer.insert(buffer.begin() + insertIndex, miniTree);
    }

    BSTNode* tree = NULL;

    try {
        tree = any_cast<BSTNode*>(buffer[0]);
    } 
    catch ([[maybe_unused]] const bad_any_cast& _) {
        // если был введён только один символ, дерево в цикле не построилось
        tree = new BSTNode(NULL);
        tree->getLeft()->setValue(any_cast<char>(buffer[0]));
    }

    cout << "Tree: " << tree->stringify() << endl << endl;

    // Строим map {символ->код}
    map<char, string> encodingTable;
    for (size_t i = 0; i < chars.size(); i++) {
        vector<int> code;
        BSTNode::BSTConnector* currentEdge = tree->search(chars[i]);

        // Собираем код, поднимаясь от найденного ребра с символом к корню
        code.insert(code.begin(), currentEdge->getSign());
        while (currentEdge->getPreviousTree()->isAttached()) {
            currentEdge = currentEdge->getPreviousTree()->getAttachedTo();
            code.insert(code.begin(), currentEdge->getSign());
        }

        encodingTable[chars[i]] = vecToString(code);

        // Выводим "символ (вероятность) -> код"
        cout << chars[i] << " (" << probabilities[i] << ")" << " -> " << encodingTable[chars[i]] << endl;
    }

    cout << endl;

    // Копия для проведения кодирования
    string encode = input;

    int position = 0;
    for (size_t i = 0; i < input.size(); i++) {
        encode.replace(position, 1, encodingTable[input[i]]);
        position += encodingTable[input[i]].size();
    }

    cout << "Encoded: " << encode << endl;

    // map {код->символ}
    multimap<string, char> decodingTable = flip_map(encodingTable);

    // Копия для проведения декодирования
    string decode = encode;

    string selection;
    int selectionStart = 0;
    for (size_t i = 0; i < encode.size(); i++) {
        selection += encode[i];
        map<string, char>::const_iterator search = decodingTable.find(selection);

        if (search != decodingTable.end()) {
            decode.replace(selectionStart, selection.size(), string(1, search->second));
            selectionStart += 1;
            selection = "";
        }
    }

    cout << "Decoded: " << decode << endl;

    delete q;
    delete tree;
    return 0;
}
