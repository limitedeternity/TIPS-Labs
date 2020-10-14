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

template<class T> string vecToString(vector<T> vec, char const * delim = "") {
    ostringstream oss;
    if (!vec.empty()) {
        copy(vec.begin(), vec.end() - 1, ostream_iterator<T>(oss, delim));
        oss << vec.back();
    }

    return oss.str();
}

template <class A, class B> multimap<B, A> flip_map(map<A, B>& src) {
    multimap<B, A> dst;
    for (typename map<A, B>::const_iterator it = src.begin(); it != src.end(); it++) {
        dst.insert(pair<B, A>(it->second, it->first));
    }

    return dst;
}

pair<vector<char>, vector<char> > shennonSplit(vector<double>& probabilities, vector<char>& chars) {
    // Здесь будет точка разделения
    int j = 1;
    double smallestSumDiff = 2.0;

    for (;;j++) {
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

    vector<char> left = vector<char>(chars.begin(), chars.begin() + j);
    vector<char> right = vector<char>(chars.begin() + j, chars.end());
    return pair<vector<char>, vector<char> >(left, right);
}

BSTNode* buildTree(vector<char> leftVec, vector<char> rightVec) {
    // Это корень дерева. Есть левое и правое ребро.
    BSTNode* tree = new BSTNode(NULL);
    BSTNode* currentPtr = tree;

    // Если в левом векторе только одно значение,
    // то только его и вешаем на левый лист
    if (leftVec.size() == 1) {
        currentPtr->getLeft()->setValue(leftVec[0]);
    }
    // В противном случае:
    else {
        // Создаем дерево на левом листе. Переключаемся на него
        currentPtr = currentPtr->getLeft()->attachNewTree();
        for (int i = 0; i < leftVec.size() - 1; i++) {
            // Вешаем значение на левый лист
            currentPtr->getLeft()->setValue(leftVec[i]);

            // Добавляем последнее значение на правый лист, если в векторе (осталось) два значения
            if (leftVec.size() - 2 == i) {
                currentPtr->getRight()->setValue(leftVec[i + 1]);
            }

            // Если осталось не два значения, а больше, то делаем новое дерево на правом листе
            // и переключаемся на него
            else {
                currentPtr = currentPtr->getRight()->attachNewTree();
            }
        }
    }

    // Делаем то же самое с правым вектором.
    // Переключаемся на корень дерева.
    currentPtr = tree;

    if (rightVec.size() == 1) {
        currentPtr->getRight()->setValue(rightVec[0]);
    }
    else {
        currentPtr = currentPtr->getRight()->attachNewTree();
        for (int i = 0; i < rightVec.size() - 1; i++) {
            currentPtr->getLeft()->setValue(rightVec[i]);

            if (rightVec.size() - 2 == i) {
                currentPtr->getRight()->setValue(rightVec[i + 1]);
            }
            else {
                currentPtr = currentPtr->getRight()->attachNewTree();
            }
        }
    }

    return tree;
}

void replaceAll(string& target, const string& search, const string& replacement) {
    int pos = 0;
    while ((pos = target.find(search, pos)) != string::npos) {
        target.replace(pos, search.size(), replacement);
        pos += replacement.size();
    }
}

int main(void) {
    string input;
    cout << "Input: ";
    getline(cin, input);
    cout << endl;

    // Частотный анализ
    map<char, int> freqTable;
    for (int i = 0; i < input.size(); i++) {
        if (freqTable.find(input[i]) == freqTable.end()) {
            freqTable[input[i]] = 1;
        } else {
            freqTable[input[i]] += 1;
        }
    }

    // Вхождения сортируются (по возрастанию) по ключам автоматически
    multimap<int, char> flipFreq = flip_map(freqTable);

    // Построение уже отсортированных векторов с символами и соотв. вероятностями
    vector<char> chars;
    vector<double> probabilities;
    for (map<int, char>::const_reverse_iterator it = flipFreq.rbegin(); it != flipFreq.rend(); it++) {
        chars.push_back(it->second);
        probabilities.push_back((double)it->first / (double)input.size());
    }

    // Поделить вектор с символами надвое относительно вектора с вероятностями
    pair<vector<char>, vector<char> > split = shennonSplit(probabilities, chars);
    // Построить дерево
    BSTNode* tree = buildTree(split.first, split.second);

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

    // map {код->символ}
    multimap<string, char> decodingTable = flip_map(encodingTable);

    // Копии для проведения кодирования/декодирования
    string encode = input;
    string decode = input;

    for (map<char, string>::const_iterator it = encodingTable.begin(); it != encodingTable.end(); it++) {
        replaceAll(encode, string(1, it->first), it->second);
    }

    cout << "Encoded: " << encode << endl;
    // А теперь – обратно

    delete tree;
    return 0;
}
