/*
 * Программа для кодирования методом Шеннона-Фано.
 *
 * Автор: Беспалов В. (3 курс, ИС)
 * Эффективное время написания: ~28 часов.
 * Компилятор: Apple LLVM version 10.0.0 (clang-1000.10.44.4)
 */

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <numeric> // для accumulate
#include <cmath> // для fabs
#include <sstream> // для ostringstream
#include <iterator> // для ostream_iterator

using namespace std;

// Совокупность этих двух классов – бинарное дерево.
// BSTNode – вершина.
// BSTConnector – ребро.
class BSTNode {
public:
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
            if (this->hasNextTree()) throw "IllegalAttachment";

            this->Value = value;
            this->HasValue = true;
        }

        BSTNode* attachNewTree() {
            if (this->HasValue) throw "IllegalAttachment";
            if (this->hasNextTree()) throw "TreeExists";

            this->NextTree = new BSTNode(this);
            return this->NextTree;
        }
        
        BSTNode* attachPrebuiltTree(BSTNode* tree) {
            if (this->HasValue) throw "IllegalAttachment";
            if (this->hasNextTree()) throw "TreeExists";

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

pair<
    pair<vector<double>, vector<char> >, 
    pair<vector<double>, vector<char> > 
> 
shennonSplit(vector<double>& probabilities, vector<char>& chars) {
    // Здесь будет точка разделения
    int j = 1;
    double smallestSumDiff = 2.0;

    for (; probabilities.size() > 1; j++) {
        vector<double> head = vector<double>(probabilities.begin(), probabilities.begin() + j);
        vector<double> tail = vector<double>(probabilities.begin() + j, probabilities.end());
        double sumDiff = fabs(accumulate(head.begin(), head.end(), 0.0) - accumulate(tail.begin(), tail.end(), 0.0));

        if (sumDiff >= smallestSumDiff) {
            j--;
            break;
        } else {
            smallestSumDiff = sumDiff;
        }
    }

    vector<double> probLeft = vector<double>(probabilities.begin(), probabilities.begin() + j);
    vector<char> chrLeft = vector<char>(chars.begin(), chars.begin() + j);
    pair<vector<double>, vector<char> > pairLeft = pair<vector<double>, vector<char> >(probLeft, chrLeft);

    vector<double> probRight = vector<double>(probabilities.begin() + j, probabilities.end());
    vector<char> chrRight = vector<char>(chars.begin() + j, chars.end());
    pair<vector<double>, vector<char> > pairRight = pair<vector<double>, vector<char> >(probRight, chrRight);

    return pair<pair<vector<double>, vector<char> >, pair<vector<double>, vector<char> > >(pairLeft, pairRight);
}

// Функция для построения дерева Шеннона 
// (выполнение условия префиксности гарантируется)
BSTNode* buildTreeEx(vector<double>& probabilities, vector<char>& chars) {
    BSTNode* tree = new BSTNode(NULL);
    pair<pair<vector<double>, vector<char> >, pair<vector<double>, vector<char> > > split = shennonSplit(probabilities, chars);
    
    pair<vector<double>, vector<char> > pairLeft = split.first;
    vector<double> probLeft = pairLeft.first;
    vector<char> chrLeft = pairLeft.second;

    pair<vector<double>, vector<char> > pairRight = split.second;
    vector<double> probRight = pairRight.first;
    vector<char> chrRight = pairRight.second;

    // Построение левой части
    if (chrLeft.size() == 1) {
        tree->getLeft()->setValue(chrLeft[0]);
    }
    else if (chrLeft.size() == 2) {
        BSTNode* newTree = tree->getLeft()->attachNewTree();
        newTree->getLeft()->setValue(chrLeft[0]);
        newTree->getRight()->setValue(chrLeft[1]);
    }
    else if (chrLeft.size() > 2) {
        tree->getLeft()->attachPrebuiltTree(buildTreeEx(probLeft, chrLeft));
    }

    // Построение правой части
    if (chrRight.size() == 1) {
        tree->getRight()->setValue(chrRight[0]);
    }
    else if (chrRight.size() == 2) {
        BSTNode* newTree = tree->getRight()->attachNewTree();
        newTree->getLeft()->setValue(chrRight[0]);
        newTree->getRight()->setValue(chrRight[1]);
    } else if (chrRight.size() > 2) {
        tree->getRight()->attachPrebuiltTree(buildTreeEx(probRight, chrRight));
    }

    return tree;
}

int main(void) {
    cout << "Input: ";

    string input;
    getline(cin, input);
    if (input.empty()) return 0;

    cout << endl;

    // Частотный анализ
    map<char, int> freqTable;
    for (int i = 0; i < input.size(); i++) {
        freqTable[input[i]] += 1;
    }

    // Вхождения сортируются (по возрастанию) по ключам автоматически
    multimap<int, char> flipFreq = flip_map(freqTable);

    // Построение уже отсортированных (по убыванию) векторов с символами и соотв. вероятностями
    vector<char> chars;
    vector<double> probabilities;
    for (map<int, char>::const_reverse_iterator it = flipFreq.rbegin(); it != flipFreq.rend(); it++) {
        chars.push_back(it->second);
        probabilities.push_back((double)it->first / (double)input.size());
    }

    // Построение дерева
    BSTNode* tree = buildTreeEx(probabilities, chars);

    // Строим map {символ->код}
    map<char, string> encodingTable;
    for (int i = 0; i < chars.size(); i++) {
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
    for (int i = 0; i < input.size(); i++) {
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
    for (int i = 0; i < encode.size(); i++) {
        selection += encode[i];
        map<string, char>::const_iterator search = decodingTable.find(selection);

        if (search != decodingTable.end()) {
            decode.replace(selectionStart, selection.size(), string(1, search->second));
            selectionStart += 1;
            selection = "";
        }
    }

    cout << "Decoded: " << decode << endl;

    delete tree;
    return 0;
}
