#include "main.h"

#define MAX_TREE_HT 10000
using namespace std;

int MAXSIZE;
class imp_res {
private:
    class min_heap {
    private:
        vector<int> stt;

        bool Big_Than(int a, int b) {
            for (size_t i = 0; i < stt.size(); i++) {
                if (stt[i] == a) {
                    return false;
                }
                if (stt[i] == b) {
                    return  true;
                }
            }
            return false;
        }

        void minHeapify(size_t i) {
            size_t smallest = i;
            size_t left = 2 * i + 1;
            size_t right = 2 * i + 2;

            if (left < h.size() && h[left].size <= h[smallest].size) {
                if ((h[left].size == h[smallest].size && Big_Than(h[left].nameRoom, h[smallest].nameRoom)) || h[left].size < h[smallest].size)
                    smallest = left;
            }
            if (right < h.size() && h[right].size <= h[smallest].size) {
                if ((h[right].size == h[smallest].size && Big_Than(h[right].nameRoom, h[smallest].nameRoom)) || h[right].size < h[smallest].size)
                    smallest = right;
            }

            if (smallest != i) {
                swap(h[i], h[smallest]);
                minHeapify(smallest);
            }
        }


        class Node {
        public:
            std::shared_ptr<Node> next = nullptr;
            std::weak_ptr<Node> prev; // Use weak_ptr to avoid cyclic dependencies
            const int data = 0;

        public:
            Node(const int value) : next(nullptr), data(value) {}
            Node() : next(nullptr), data(0) {} // Default constructor
        };




        template<typename A, typename B, typename C, typename D, typename E>
        class pairr {
        public:
            A nameRoom;
            B size;
            shared_ptr<Node> head;
            shared_ptr<Node> tail;
            E stt;
            bool operator==(const pairr& other) const {
                return (nameRoom == other.nameRoom) &&
                    (size == other.size) &&
                    (head == other.head) &&
                    (tail == other.tail) &&
                    (stt == other.stt);
            }
            ~pairr() {
                head.reset(); // Ensure head is reset before tail to avoid cyclic dependencies
                tail.reset();
            }
        };

        vector<pairr<int, int, shared_ptr<Node>, shared_ptr<Node>, int>> h;
        bool thu = true;

    public:
        void buildHeap(int nameRoom, int data) {
            bool test = true;
            int I = 0;
            for (size_t i = 0; i < h.size(); i++) {
                if (h[i].nameRoom == nameRoom) {
                    I = i;
                    test = false;
                }
            }
            if (test == false) {
                if (h[I].head == nullptr) {
                    h[I].head = make_shared<Node>(data);
                    h[I].tail = h[I].head;
                }
                else {
                    shared_ptr<Node> newNode = make_shared<Node>(data);
                    newNode->prev = h[I].tail;
                    h[I].tail->next = newNode;
                    h[I].tail = newNode;
                }
                h[I].size++;
                stt.erase(remove(stt.begin(), stt.end(), nameRoom), stt.end());
                stt.insert(stt.begin(), nameRoom);
            }
            else {
                if (h.size() >= MAXSIZE) return;
                pairr<int, int, shared_ptr<Node>, shared_ptr<Node>, int> newPair;
                newPair.nameRoom = nameRoom;
                newPair.head = make_shared<Node>(data);
                newPair.tail = newPair.head;
                newPair.size = 1;
                newPair.stt = h.size() + 1;
                h.push_back(newPair);
                if (thu) {
                    thu = false;
                    stt.push_back(nameRoom);
                }
                else
                    stt.insert(stt.begin(), nameRoom);
            }
            for (int i = h.size() / 2 - 1; i >= 0; i--) {
                minHeapify(i);
            }
        }

        shared_ptr<Node> deleteNode(shared_ptr<Node>& head) {
            if (!head) {
                return head;
            }

            shared_ptr<Node> temp = head;
            head = head->next;
            if (head) {
                head->prev.reset(); // Reset prev to break cyclic dependency
            }
            temp.reset();
            return head;
        }

        void deleteAll(shared_ptr<Node>& head, int id) {
            while (head != nullptr) {
                cout << head->data << " " << id << endl;
                head = deleteNode(head);
            }
            thu = true;
        }

        void KEITEIKEN(int num) {
            if (h.size() == 0 || num == 0) return;

            int n = min(num, static_cast<int>(h.size()));

            vector<pairr<int, int, shared_ptr<Node>, shared_ptr<Node>, int>> new_heap = h;
            vector<pair<int, pairr<int, int, shared_ptr<Node>, shared_ptr<Node>, int>>> temp;
            vector<bool> Visited(h.size(), false);

            while (n--) {
                int minI = 0;
                pairr<int, int, shared_ptr<Node>, shared_ptr<Node>, int> minData;
                bool test = false;

                for (size_t i = 0; i < new_heap.size(); i++) {
                    if (!Visited[i] && !test) {
                        test = true;
                        minData = new_heap[i];
                        minI = i;
                        continue;
                    }
                    if (new_heap[i].head && minData.size <= new_heap[i].size) {
                        if (minData.size == new_heap[i].size) {
                            if (Big_Than(new_heap[i].nameRoom, minData.nameRoom)) {
                                minData = new_heap[i];
                                minI = i;
                            }
                        }
                        else {
                            minData = new_heap[i];
                            minI = i;
                        }
                    }
                }
                Visited[minI] = true;
                temp.push_back({ minI, minData });
                // Do not call minData destructor here
            }

            for (auto x : temp) {
                pairr<int, int, shared_ptr<Node>, shared_ptr<Node>, int> minData = x.second;
                int minI = x.first;
                int tmp = num;

                if (h[minI].head && num >= h[minI].size) {
                    deleteAll(h[minI].head, h[minI].nameRoom);
                    stt.erase(remove(stt.begin(), stt.end(), h[minI].nameRoom), stt.end());
                    h[minI].size = 0;
                }
                else if (h[minI].head && num < h[minI].size) {
                    int dem = 0;

                    while (tmp-- && h[minI].head) {
                        cout << h[minI].head->data << " " << h[minI].nameRoom << endl;
                        h[minI].head = deleteNode(h[minI].head);
                        dem++;
                    }

                    stt.erase(remove(stt.begin(), stt.end(), h[minI].nameRoom), stt.end());
                    stt.insert(stt.begin(), h[minI].nameRoom);
                    h[minI].size -= dem;
                }
            }

            vector<pairr<int, int, shared_ptr<Node>, shared_ptr<Node>, int>> New;

            for (size_t i = 0; i < h.size(); i++) {
                if (h[i].size != 0) {
                    New.push_back(h[i]);
                }
            }

            h.clear();

            h = New;
        }

        void CLEAVE(size_t index, const int num) {
            if (index >= h.size()) {
                return;
            }
            if (h[index].size != 0) {
                int x = num;
                shared_ptr<Node> node = h[index].tail;
                while (node && x--) {
                    cout << h[index].nameRoom << "-" << node->data << endl;
                    node = node->prev.lock();
                }
            }

            CLEAVE(2 * index + 1, num);
            CLEAVE(2 * index + 2, num);
        }

    };

    class BinarySearchTree {
    public:
        class Node {
        public:
            long long int data;
            shared_ptr<Node> left;
            shared_ptr<Node> right;
            long long int time;
            long long int size;
            Node(long long int value, long long int tg) : data(value), left(nullptr), right(nullptr), time(tg), size(0) {}
            Node() {};
        };
        shared_ptr<Node> Root = NULL;
        BinarySearchTree() : Root(nullptr) {}
        long long int size = 0;
    private:
        long long int tg = 0;
        long long int LeftOf(const long long int value, const shared_ptr<Node> root)
        {
            // Nếu bạn muốn cây BST cho phép giá trị trùng lặp, hãy sử dụng dòng code thứ 2
            return value < root->data;
            //    return value <= root->data;
        }

        long long int RightOf(const long long int value, const shared_ptr<Node> root)
        {
            return value >= root->data;
        }


        long long int gt(long long int n) {
            long long int s = 1;
            for (long long int i = 1; i <= n; i++)
                s *= i;
            return s;
        }
        long long int C(long long int k, long long int n) {
            return  gt(n) / (gt(k) * gt(n - k));
        }

        shared_ptr<Node> Insert(shared_ptr<Node> root, const long long int value)
        {
            if (root == NULL)
            {
                tg++;
                shared_ptr<Node> node = make_shared<Node>(value, tg);
                return node;
            }
            if (RightOf(value, root))
                root->right = Insert(root->right, value);
            else if (LeftOf(value, root))
                root->left = Insert(root->left, value);

            root->size++;
            return root;
        }

        shared_ptr<Node> deleteNode(shared_ptr<Node> root, long long int data, long long int time) {
            if (root == nullptr) {
                return nullptr;
            }

            if (root->data > data) {
                root->left = deleteNode(root->left, data, time);
            }
            else if (root->data < data) {
                root->right = deleteNode(root->right, data, time);
            }
            else {
                if (root->time != time) {
                    // Tìm node cần xóa dựa trên thời gian nếu dữ liệu bằng nhau
                    queue<shared_ptr<Node>> Q;
                    Q.push(root);
                    while (!Q.empty()) {
                        shared_ptr<Node> temp = Q.front();
                        Q.pop();
                        if (temp->left) {
                            Q.push(temp->left);
                        }
                        if (temp->right) {
                            Q.push(temp->right);
                        }
                        if (temp->data == data && temp->time == time) {
                            root = temp;
                            break;
                        }
                    }
                }

                // Xóa node
                if (root->right == nullptr) {
                    if (root->left == nullptr) {
                        root.reset();
                        return nullptr;
                    }
                    return root->left;
                }
                else {
                    shared_ptr<Node> succParent = root;
                    shared_ptr<Node> succ = root->right;

                    while (succ->left != nullptr) {
                        succParent = succ;
                        succ = succ->left;
                    }

                    if (succParent != root) {
                        succParent->left = succ->right;
                    }
                    else {
                        succParent->right = succ->right;
                    }
                    root->data = succ->data;
                    root->time = succ->time;

                    //root->right = deleteNode(root->right, root->data, root->time);
                }
            }

            return root;
        }

        vector<long long int> BFS(shared_ptr<Node> root) {
            vector<long long int> result;
            if (root == nullptr) {
                return result;
            }

            std::queue<shared_ptr<Node>> q;
            q.push(root);

            while (!q.empty()) {
                shared_ptr<Node> current = q.front();
                q.pop();

                result.push_back(current->data);

                if (current->left != nullptr) {
                    q.push(current->left);
                }
                if (current->right != nullptr) {
                    q.push(current->right);
                }
            }

            return result;
        }

        long long int numOfWays(vector<long long int>& nums) {
            long long int m = nums.size();
            table.resize(m + 1);
            for (int i = 0; i < m + 1; ++i) {
                table[i] = vector<long long int>(i + 1, 1);
                for (int j = 1; j < i; ++j) {
                    table[i][j] = (table[i - 1][j - 1] + table[i - 1][j]) % mod;
                }
            }

            return (dfs(nums) - 1) % mod;
        }

        vector<vector<long long int>> table;
        long long int mod = 1e9 + 7;

        long long int dfs(vector<long long int>& nums) {
            long long int m = nums.size();
            if (m < 3) {
                return 1;
            }

            vector<long long int> leftNodes, rightNodes;
            for (int i = 1; i < m; ++i) {
                if (nums[i] < nums[0]) {
                    leftNodes.push_back(nums[i]);
                }
                else {
                    rightNodes.push_back(nums[i]);
                }
            }

            long long int leftWays = dfs(leftNodes) % mod;
            long long int rightWays = dfs(rightNodes) % mod;

            return (((leftWays * rightWays) % mod) * table[m - 1][leftNodes.size()]) % mod;
        }

        void deleteTree(shared_ptr<Node> node) {
            if (node == NULL) {
                return;
            }
            tg = 0;
            deleteTree(node->left);
            deleteTree(node->right);
            node = nullptr;
        }

        void GiamTime() {
            if (Root == NULL) return;
            queue<shared_ptr<Node>>Q;
            Q.push(Root);
            while (!Q.empty()) {
                shared_ptr<Node> node = Q.front();
                Q.pop();
                node->time--;
                if (node->left) {
                    Q.push(node->left);
                }
                if (node->right) {
                    Q.push(node->right);
                }
            }
        }

        void inOrderTraversal(shared_ptr<Node> root) {
            if (root == nullptr) {
                return;
            }
            inOrderTraversal(root->left);
            cout << root->data << endl;
            inOrderTraversal(root->right);
        }
    public:
        ~BinarySearchTree() {
            deleteTree(Root);
        }

        void insert(long long int data) {
            size++;
            Root = Insert(Root, data);
        }

        void remove() {
            if (Root == NULL) return;
            queue<shared_ptr<Node>>q;
            q.push(Root);
            shared_ptr<Node> answer = nullptr;
            while (!q.empty()) {
                shared_ptr<Node> node = q.front();
                q.pop();
                if (node->time == 1) {
                    answer = node;
                    break;
                }
                if (node->left != nullptr) {
                    q.push(node->left);
                }
                if (node->right != nullptr) {
                    q.push(node->right);
                }
            }
            if (answer != nullptr) {
                Root = deleteNode(Root, answer->data, answer->time);
                tg--;
                GiamTime();
                size--;
            }
        }

        long long int getHoanVi() {
            vector<long long int> bfs = BFS(Root);
            return numOfWays(bfs) + 1;
        }

        void deleteTree() {
            if (Root == NULL) return;
            deleteTree(Root);
            Root = NULL;
            tg = 0;
            size = 0;
        }

        bool isEmpty() {
            return (Root == nullptr);
        }

        void in_oder() {
            inOrderTraversal(Root);
        }
    };
    class HashMap {
    private:
        vector<BinarySearchTree> hash;
        bool test = false;
    public:
        void insert(int id, int result) {
            if (!test) {
                test = true;
                hash.resize(MAXSIZE + 1);
                for (size_t i = 0; i < MAXSIZE + 1; i++) {
                    hash[i].Root = nullptr;
                }
            }
            hash[id].insert(result);
        }

        void KOKUSEN() {
            for (size_t i = 0; i < hash.size(); i++) {
                if (hash[i].size == 0)continue;
                size_t n = hash[i].getHoanVi() % MAXSIZE;
                if (n >= MAXSIZE) {
                    hash[i].deleteTree();
                }
                else {
                    while (n--)
                    {
                        hash[i].remove();
                        if (hash[i].Root)break;
                    }
                }
            }
        }

        void LIMITLESS(size_t num) {
            if (hash.size() == 0)return;
            if (num > MAXSIZE || num > hash.size() || hash[num].isEmpty() || hash[num].size == 0)return;
            hash[num].in_oder();
        }

    };

    class HuffMan {
    private:
        class HuffNode {
        public:
            virtual ~HuffNode() {}
            virtual int weight() const = 0;
            virtual bool isLeaf() const = 0;
            virtual char val() const = 0;
            virtual int time() const = 0;
        };

        class Node : public HuffNode {
        public:
            shared_ptr<Node> left;
            shared_ptr<Node> right;
            int wgt;
            int _time;
            int dem;

            Node() : left(nullptr), right(nullptr), wgt(0), _time(0) {}
            Node(shared_ptr<Node> l, shared_ptr<Node> r, int thoigian, int count) : left(l), right(r), wgt(l->weight() + r->weight()), _time(thoigian), dem(count) {}

            int weight() const override { return wgt; }
            bool isLeaf() const override { return false; }
            char val() const override { return '\0'; }
            int time() const override { return _time; }
        };

        class LeafNode : public Node {
        private:
            char it;
            int wgt;
            int _time;

        public:
            LeafNode(const char& val, int freq, int time) : it(val), wgt(freq), _time(time) {}

            int weight() const override { return wgt; }
            char val() const override { return it; }
            bool isLeaf() const override { return true; }
            int time() const override { return _time; }
        };

        shared_ptr<Node> root;

        int getHeightRec(shared_ptr<Node> node) const {
            if (node == nullptr) {
                return 0;
            }
            if (!node->isLeaf()) {
                int lh = getHeightRec(node->left);
                int rh = getHeightRec(node->right);
                return max(lh, rh) + 1;
            }
            return 0;
        }

        shared_ptr<Node> rotateRight(shared_ptr<Node> y) const {
            shared_ptr<Node> x = y->left;
            shared_ptr<Node> T2 = x->right;

            x->right = y;
            y->left = T2;

            return x;
        }

        shared_ptr<Node> rotateLeft(shared_ptr<Node> x) const {
            shared_ptr<Node> y = x->right;
            shared_ptr<Node> T2 = y->left;

            y->left = x;
            x->right = T2;

            return y;
        }


        int getBalance(shared_ptr<Node> node) const {
            if (!node) {
                return 0;
            }
            return getHeightRec(node->left) - getHeightRec(node->right);
        }

        void getHCodes(shared_ptr<Node> node, int arr[], int top, vector<pair<string, string>>& temp, string s) const {
            if (node == nullptr) {
                return;
            }

            if (node->isLeaf()) {
                shared_ptr<LeafNode> leafNode = dynamic_pointer_cast<LeafNode>(node);
                int i;
                string str;
                for (i = 0; i < top; ++i) {
                    str += to_string(arr[i]);
                }
                temp.push_back({ s, str });
            }
            else {
                if (node->left) {
                    arr[top] = 0;
                    getHCodes(node->left, arr, top + 1, temp, "left");
                }
                if (node->right) {
                    arr[top] = 1;
                    getHCodes(node->right, arr, top + 1, temp, "right");
                }
            }
        }

        void xoayAvl(shared_ptr<Node>& node, int& count) const {
            int balance = getBalance(node);
            if (balance >= -1 && balance <= 1) {
                return;
            }
            else {
                int balanLeft = 0;
                int balanRight = 0;
                count++;
                if (count > 3) return;
                if (balance > 1) {
                    balanLeft = getBalance(node->left);
                    if (balanLeft >= 0) {
                        node = rotateRight(node);
                    }
                    else if (balanLeft < 0) {
                        node->left = rotateLeft(node->left);
                        node = rotateRight(node);
                    }
                }
                else if (balance < -1) {
                    balanRight = getBalance(node->right);
                    if (balanRight > 0) {
                        node->right = rotateRight(node->right);
                        node = rotateLeft(node);
                    }
                    else if (balanRight <= 0) {
                        node = rotateLeft(node);
                    }
                }
            }
            shared_ptr<Node> temp = node;
            xoayAvl(temp->left, count);
            xoayAvl(temp->right, count);
            if (abs(getBalance(node)) > 1)xoayAvl(node, count);
        }

        void getBinaryCodes(shared_ptr<Node> root, const string& codeSoFar, unordered_map<char, string>& codes) const {
            if (root == nullptr) {
                return;
            }

            if (root->isLeaf()) {
                shared_ptr<Node> leaf = root;
                codes[leaf->val()] = codeSoFar;
                return;
            }

            getBinaryCodes(root->left, codeSoFar + "0", codes);
            getBinaryCodes(root->right, codeSoFar + "1", codes);
        }

        class HuffTree {
        public:
            shared_ptr<Node> Root;

            HuffTree(char val, int freq, int& thoiGian) : Root(std::make_shared<LeafNode>(val, freq, thoiGian)) {
                thoiGian++;
            }

            HuffTree(shared_ptr<HuffTree> l, shared_ptr<HuffTree> r, int& thoiGian) : Root(std::make_shared<Node>(l->Root, r->Root, thoiGian++, 0)) {}

            shared_ptr<Node> root() const { return Root; }

            int weight() const { return Root->weight(); }

            char val() const { return Root->val(); }
        };


        struct MinTreeComp {
            bool operator()(const shared_ptr<HuffTree> tree1, const shared_ptr<HuffTree> tree2) const {
                if (tree1->root()->weight() == tree2->root()->weight()) {
                    return tree1->root()->time() > tree2->root()->time();
                }
                return tree1->root()->weight() > tree2->root()->weight();
            }
        };

        template <typename E, typename Comp>
        class PriorityQueue {
        private:
            priority_queue<E, vector<E>, Comp> pq;

        public:
            int size() const {
                return pq.size();
            }

            bool empty() const {
                return pq.empty();
            }

            void insert(const E& it) {
                pq.push(it);
            }

            E removeFirst() {
                if (pq.empty()) {
                    exit(1);
                }
                E top = pq.top();
                pq.pop();
                return top;
            }
        };


        vector<shared_ptr<HuffTree>> treeArray;

        shared_ptr<HuffTree> buildHuff(vector<shared_ptr<HuffTree>> treeArray, int count, int tg) const {
            PriorityQueue<std::shared_ptr<HuffTree>, MinTreeComp> forest;
            std::shared_ptr<HuffTree> temp1, temp2, temp3 = nullptr;

            for (int i = 0; i < count; ++i) {
                forest.insert(treeArray[i]);
            }

            int dem = 0;
            while (forest.size() > 1) {
                temp1 = forest.removeFirst();
                temp2 = forest.removeFirst();
                temp3 = std::make_shared<HuffTree>(temp1, temp2, tg);
                dem = 0;
                xoayAvl(temp3->Root, dem);
                forest.insert(temp3);
            }

            temp3 = forest.removeFirst();
            return temp3;
        }

    public:

        unordered_map<char, string> getHuffmanCodes(const shared_ptr<HuffTree> huffmanTree) const {
            unordered_map<char, string> codes;
            getBinaryCodes(huffmanTree->root(), "", codes);
            return codes;
        }

        unordered_map<char, string> HuffmanCodes(const vector<pair<char, int>>& s) {
            int thoiGian = 1;
            treeArray.clear();
            for (const auto& entry : s) {
                char character = entry.first;
                int frequency = entry.second;
                treeArray.push_back(std::make_shared<HuffMan::HuffTree>(character, frequency, thoiGian));
            }
            shared_ptr<HuffTree> huffmanTree = HuffMan().buildHuff(treeArray, treeArray.size(), thoiGian);
            unordered_map<char, string> huffmanCodes = HuffMan().getHuffmanCodes(huffmanTree);
            root = huffmanTree->root();

            return huffmanCodes;
        }

        void BFS() const {
            if (root == nullptr) {
                return;
            }
            queue<shared_ptr<Node>> q;
            q.push(root);
            while (!q.empty()) {
                shared_ptr<Node>  node = q.front();
                q.pop();

                if (node->isLeaf()) {
                    shared_ptr<LeafNode> leaf = dynamic_pointer_cast<LeafNode>(node);
                    cout << leaf->val() << endl;
                }
                else {
                    cout << node->weight() << endl;

                    if (node->left != nullptr) {
                        q.push(node->left);
                    }

                    if (node->right != nullptr) {
                        q.push(node->right);
                    }
                }
            }
        }
    };

private:

    HuffMan* huffman = new HuffMan();
    HashMap* hashmap = new HashMap();
    min_heap* minHeap = new min_heap();

    static bool comprater(pair<char, int>a, pair<char, int>b) {
        if (a.second == b.second) {
            if (islower(a.first) && isupper(b.first))
                return true;
            else if (islower(b.first) && isupper(a.first))
                return false;
            else {
                return a.first < b.first;
            }
        }
        return a.second < b.second;
    }

    vector<pair<char, int>> Caesar(unordered_map<char, int>s, string name) {
        vector<pair<char, int>> temp;
        for (size_t i = 0; i < name.size(); i++) {
            if (65 <= name[i] && name[i] <= 90) {
                int x = name[i] + s[name[i]];
                if (x > 90) {
                    x = (x - 'A') % 26 + 'A';
                }
                temp.push_back({ x, s[name[i]] });
            }
            else if (97 <= name[i] && name[i] <= 122) {
                int x = name[i] + s[name[i]];
                if (x > 122) {
                    x = (x - 'a') % 26 + 'a';
                }
                temp.push_back({ x, s[name[i]] });
            }
        }
        return temp;
    }

    int binaryToDecimal(const std::string& binary) {
        int decimal = 0;
        int power = 0;

        // Start the loop from the last index (binary.length() - 1)
        for (int i = static_cast<int>(binary.length()) - 1; i >= 0; --i) {
            if (binary[i] == '1') {
                decimal += static_cast<int>(std::pow(2, power));
            }
            power++;
        }

        return decimal;
    }
public:
    void LAPSE(string name) {
        // unordered_map<char, int>Map;
        // for (size_t i = 0; i < name.size(); i++) {
        //     Map[name[i]]++;
        // }
        // vector<pair<char, int>>caesar = Caesar(Map, name);
        // unordered_map<char, int> newMap;
        // string result = "";
        // for (auto x : caesar) {
        //     newMap[x.first]++;
        //     result += x.first;
        // }
        // vector<pair<char, int>> XapXep(newMap.begin(), newMap.end());
        // sort(XapXep.begin(), XapXep.end(), comprater);
        // if (Map.size() < 3 || XapXep.size() <= 1)return;
        // unordered_map<char, string>tmpp = huffman->HuffmanCodes(XapXep);
        // string Result;
        // for (auto x : result) {
        //     Result += tmpp[x];
        // }
        // reverse(Result.begin(), Result.end());
        // Result = Result.substr(0, 10);

        // int num = binaryToDecimal(Result);
        int num = stoi(name);
        int id = num % MAXSIZE + 1;
        if (num % 2 != 0) {
            //cout << "HASHMAP " << id << " " << num << endl;
            hashmap->insert(id, num);
        }
        else {
            //cout << "MINHEAP " << id << " " << num << endl;
            minHeap->buildHeap(id, num);
        }
    }
    void KOKUSEN() {
        hashmap->KOKUSEN();
    }
    void KEITEIKEN(int num) {
        minHeap->KEITEIKEN(num);
    }
    void HAND() {
        huffman->BFS();
    }
    void LIMITLESS(int num) {
        hashmap->LIMITLESS(num);
    }
    void CLEAVE(int num) {
        minHeap->CLEAVE(0, num);
    }
    ~imp_res() {
        delete hashmap;
        delete minHeap;
        delete huffman;
    }
};
imp_res* r = new imp_res();
void simulate(string filename)
{
    ifstream ss(filename);
    string str, maxsize, name, energy, num;
    while (ss >> str)
    {
        if (str == "MAXSIZE")
        {
            ss >> maxsize;
            MAXSIZE = stoi(maxsize);
        }
        else if (str == "LAPSE") // RED <NAME> <ENERGY>
        {
            ss >> name;
            r->LAPSE(name);
        }
        else if (str == "KOKUSEN") // BLUE <NUM>
        {
            r->KOKUSEN();
        }
        else if (str == "KEITEIKEN") // PURPLE
        {
            ss >> num;
            r->KEITEIKEN(stoi(num));
        }
        else if (str == "HAND") // REVERSAL
        {
            r->HAND();
        }
        else if (str == "LIMITLESS") // UNLIMITED_VOID
        {
            ss >> num;
            r->LIMITLESS(stoi(num));
        }
        else if (str == "CLEAVE") // DOMAIN_EXPANSION
        {
            ss >> num;
            r->CLEAVE(stoi(num));
        }
    }
    //delete r;
}

