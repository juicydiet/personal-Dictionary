#include <bits/stdc++.h>
using namespace std;

// ======================= TRIE SECTION =======================

struct TrieNode {
    TrieNode* child[26];
    bool isEnd;
    string meaning;

    TrieNode() {
        isEnd = false;
        meaning = "";
        for (int i = 0; i < 26; i++) child[i] = nullptr;
    }
};

TrieNode* trieRoot = new TrieNode();

// convert word to lowercase and keep only a-z
string normalizeWord(const string &w) {
    string res;
    for (char c : w) {
        if (isalpha((unsigned char)c)) {
            res.push_back(tolower(c));
        }
    }
    return res;
}

void insertIntoTrie(const string &wordRaw, const string &meaning) {
    string word = normalizeWord(wordRaw);
    if (word.empty()) return;
    TrieNode* curr = trieRoot;
    for (char c : word) {
        int idx = c - 'a';
        if (!curr->child[idx]) curr->child[idx] = new TrieNode();
        curr = curr->child[idx];
    }
    curr->isEnd = true;
    curr->meaning = meaning;
}

TrieNode* searchInTrie(const string &wordRaw) {
    string word = normalizeWord(wordRaw);
    if (word.empty()) return nullptr;
    TrieNode* curr = trieRoot;
    for (char c : word) {
        int idx = c - 'a';
        if (!curr->child[idx]) return nullptr;
        curr = curr->child[idx];
    }
    return (curr->isEnd ? curr : nullptr);
}

// collect all (word, meaning) pairs from Trie
void collectTrieWords(TrieNode* node, string &current, vector<pair<string,string>> &out) {
    if (!node) return;
    if (node->isEnd) {
        out.push_back({current, node->meaning});
    }
    for (int i = 0; i < 26; i++) {
        if (node->child[i]) {
            current.push_back('a' + i);
            collectTrieWords(node->child[i], current, out);
            current.pop_back();
        }
    }
}

void clearTrie(TrieNode* node) {
    if (!node) return;
    for (int i = 0; i < 26; i++) {
        if (node->child[i]) clearTrie(node->child[i]);
    }
    delete node;
}

// ======================= HUFFMAN SECTION =======================

struct HuffmanNode {
    char ch;
    int freq;
    HuffmanNode *left;
    HuffmanNode *right;

    HuffmanNode(char c, int f) {
        ch = c;
        freq = f;
        left = right = nullptr;
    }
};

struct HuffmanCompare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->freq > b->freq; // min-heap by freq
    }
};

unordered_map<char,int> buildFrequencyTable(const string &text) {
    unordered_map<char,int> freq;
    for (char c : text) freq[c]++;
    return freq;
}

HuffmanNode* buildHuffmanTree(const unordered_map<char,int> &freq) {
    if (freq.empty()) return nullptr;
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, HuffmanCompare> pq;
    for (auto &p : freq) {
        pq.push(new HuffmanNode(p.first, p.second));
    }
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top();  pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();
        HuffmanNode* parent = new HuffmanNode('\0', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }
    return pq.top();
}

void buildCodes(HuffmanNode* root, const string &code, unordered_map<char,string> &codes) {
    if (!root) return;
    if (!root->left && !root->right) {
        codes[root->ch] = (code == "" ? "0" : code);
        return;
    }
    buildCodes(root->left, code + "0", codes);
    buildCodes(root->right, code + "1", codes);
}

// returns encoded bitstring, also gives root + freq table
string huffmanEncode(const string &text,
                     unordered_map<char,string> &codes,
                     HuffmanNode* &rootOut,
                     unordered_map<char,int> &freqOut) {
    if (text.empty()) {
        rootOut = nullptr;
        codes.clear();
        freqOut.clear();
        return "";
    }
    freqOut = buildFrequencyTable(text);
    HuffmanNode* root = buildHuffmanTree(freqOut);
    codes.clear();
    buildCodes(root, "", codes);

    string encoded;
    encoded.reserve(text.size() * 2);
    for (char c : text) encoded += codes[c];

    rootOut = root;
    return encoded;
}

string huffmanDecode(const string &encoded, HuffmanNode* root) {
    string decoded;
    if (!root) return decoded;
    HuffmanNode* curr = root;
    for (char bit : encoded) {
        curr = (bit == '0') ? curr->left : curr->right;
        if (!curr->left && !curr->right) {
            decoded.push_back(curr->ch);
            curr = root;
        }
    }
    return decoded;
}

void freeHuffmanTree(HuffmanNode* root) {
    if (!root) return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    delete root;
}

// ======================= INITIAL DATA (YOUR WORD LIST) =======================

string getInitialDictionaryText() {
    return
        "Apple - A fruit.\n"
        "Able - Having the power or skill to do something.\n"
        "About - Concerning or relating to.\n"
        "After - Following in time or order.\n"
        "Again - Once more.\n"
        "Always - At all times.\n"
        "Animal - A living organism that isn’t a plant.\n"
        "Answer - A reply to a question.\n"
        "Act - To do something.\n"
        "Angle - The space between two intersecting lines.\n"
        "Book - A set of written pages.\n"
        "Ball - A round object used in games.\n"
        "Back - The rear part.\n"
        "Big - Large in size.\n"
        "Buy - To obtain by paying money.\n"
        "Bring - To carry something with you.\n"
        "Brother - A male sibling.\n"
        "Better - Of higher quality.\n"
        "Body - The physical structure of a person or animal.\n"
        "Beautiful - Pleasing to the senses.\n"
        "Cat - A small domesticated animal.\n"
        "Car - A vehicle with wheels.\n"
        "Child - A young human being.\n"
        "Come - To move toward.\n"
        "Call - To shout or communicate.\n"
        "Cold - Having low temperature.\n"
        "Change - To make different.\n"
        "City - A large human settlement.\n"
        "Chair - A seat for one person.\n"
        "Color - The appearance of something due to light reflection.\n"
        "Day - 24 hours.\n"
        "Door - A movable barrier to enter/exit a room.\n"
        "Dog - A domesticated animal.\n"
        "Dance - To move rhythmically to music.\n"
        "Draw - To make a picture.\n"
        "Drive - To operate a vehicle.\n"
        "Dark - Absence of light.\n"
        "Dream - Thoughts during sleep.\n"
        "Different - Not the same.\n"
        "Dinner - The main meal of the day.\n"
        "Eat - To consume food.\n"
        "Eye - The organ of sight.\n"
        "End - The final part.\n"
        "Earth - Our planet.\n"
        "Enjoy - To take pleasure in.\n"
        "Enter - To go in.\n"
        "Example - Something illustrating a point.\n"
        "Early - Before the expected time.\n"
        "Event - Something that happens.\n"
        "Easy - Not difficult.\n"
        "Face - The front part of the head.\n"
        "Family - A group of related people.\n"
        "Find - To discover something.\n"
        "Food - What people or animals eat.\n"
        "Friend - A person you like and trust.\n"
        "Fast - Moving quickly.\n"
        "Feel - Experience an emotion or sensation.\n"
        "Fall - To drop downward.\n"
        "Full - Containing as much as possible.\n"
        "Far - A long distance away.\n"
        "Go - To move from one place to another.\n"
        "Good - Having desirable qualities.\n"
        "Girl - A young female person.\n"
        "Give - To hand something to someone.\n"
        "Great - Very large or important.\n"
        "Get - To receive something.\n"
        "Game - An activity for fun or competition.\n"
        "Group - A number of things or people together.\n"
        "Green - A color.\n"
        "Grow - To increase in size.\n"
        "Hand - The end part of the arm.\n"
        "House - A building for living.\n"
        "Help - To assist.\n"
        "Happy - Feeling joy.\n"
        "Hear - To perceive sound.\n"
        "Head - The uppermost part of the body.\n"
        "Home - Place where one lives.\n"
        "High - A great height.\n"
        "Hope - Expectation for something good.\n"
        "Hot - Having high temperature.\n"
        "I - Referring to oneself.\n"
        "Ice - Frozen water.\n"
        "Idea - A thought or plan.\n"
        "Important - Of great significance.\n"
        "Inside - The inner part.\n"
        "Involve - Include as part of something.\n"
        "Interest - Wanting to know or learn.\n"
        "Image - A picture or visual representation.\n"
        "Improve - Make better.\n"
        "Insect - A small animal with six legs.\n"
        "Job - Work done for pay.\n"
        "Jump - Push yourself off the ground.\n"
        "Key - A tool to open locks.\n"
        "King - A male ruler.\n"
        "Keep - To hold or maintain.\n"
        "Know - To have knowledge of something.\n"
        "Kind - Being helpful or caring.\n"
        "Kid - A child.\n"
        "Kill - To cause death.\n"
        "Kick - Strike with the foot.\n"
        "Kitchen - Room for cooking food.\n"
        "Kite - A flying object tied to a string.\n"
        "Love - Deep affection.\n"
        "Life - The existence of living things.\n"
        "Long - A large length.\n"
        "Look - To see or direct your eyes.\n"
        "Leave - To go away from.\n"
        "Light - Brightness; or not heavy.\n"
        "Learn - Gain knowledge.\n"
        "Little - Small in size.\n"
        "Laugh - Express joy through sound.\n"
        "Land - Solid ground.\n"
        "Joy - Great happiness.\n"
        "Join - Connect or become part of something.\n"
        "Journey - Traveling from one place to another.\n"
        "Just - Fair or morally right.\n"
        "Juice - Liquid from fruits/vegetables.\n"
        "Joke - Something said to make people laugh.\n"
        "Jeans - Denim trousers.\n"
        "Journal - A daily written record.\n"
        "Man - An adult male.\n"
        "Make - To create something.\n"
        "Mother - A female parent.\n"
        "More - A greater amount.\n"
        "Move - Change position.\n"
        "Money - Medium of exchange to buy things.\n"
        "Month - One-twelfth of a year.\n"
        "Meet - Come into the presence of.\n"
        "Mind - The part that thinks and feels.\n"
        "Morning - The early part of the day.\n"
        "Mommy - Mother of a child.\n"
        "Name - A word identifying a person or thing.\n"
        "Night - Time of darkness.\n"
        "New - Recently created or discovered.\n"
        "Near - Close by.\n"
        "Need - Require something.\n"
        "Never - At no time.\n"
        "Number - A value used for counting.\n"
        "Nice - Pleasant or good.\n"
        "Nothing - No thing.\n"
        "Nature - The physical world and living things.\n"
        "Question - Something asked to get information.\n"
        "Quick - Fast.\n"
        "Run - Move fast on foot.\n"
        "Read - Look at and understand written words.\n"
        "Right - Correct; or direction opposite left.\n"
        "Room - A space in a building.\n"
        "Road - A path for vehicles.\n"
        "Red - A color.\n"
        "Reach - Stretch to touch or get.\n"
        "Reason - Explanation for something.\n"
        "River - Flowing water body.\n"
        "Rain - Water falling from clouds.\n"
        "See - TO look at.\n"
        "Say - Speak words.\n"
        "Small - Not large.\n"
        "School - Educational institution.\n"
        "Start - Begin something.\n"
        "Story - Description of events.\n"
        "Strong - Having great strength.\n"
        "Short - Small in length or height.\n"
        "Smile - Expression of happiness.\n"
        "Same - Not different.\n"
        "Time - Ongoing sequence of events.\n"
        "Thing - An object.\n"
        "Take - To get or receive.\n"
        "Think - Use your mind.\n"
        "Talk - Speak words.\n"
        "Try - Attempt something.\n"
        "Turn - Rotate or change direction.\n"
        "Team - Group working together.\n"
        "Table - Furniture with a flat top.\n"
        "True - Not false.\n"
        "Up - Toward a higher place.\n"
        "Use - To apply something for a purpose.\n"
        "Under - Below something.\n"
        "Until - Up to a time.\n"
        "Unit - A single thing.\n"
        "Usual - Commonly happening.\n"
        "Urban - Related to city areas.\n"
        "Universe - All space and matter.\n"
        "Useful - Helpful or beneficial.\n"
        "Uncle - Brother of your parent.\n"
        "Very - Extremely.\n"
        "Voice - Sound produced when speaking.\n"
        "Village - A small community.\n"
        "Value - Worth of something.\n"
        "Visit - To go see a person/place.\n"
        "View - A sight or opinion.\n"
        "Victory - Winning.\n"
        "Vehicle - Means of transportation.\n"
        "Video - Moving visual recording.\n"
        "Variety - Different elements or types.\n"
        "Water - A clear liquid essential for life.\n"
        "Work - Activity to achieve a result.\n"
        "World - Earth or human society.\n"
        "Way - A path or method.\n"
        "Write - Mark letters or words.\n"
        "Walk - Move on foot.\n"
        "Word - A unit of language.\n"
        "Window - Opening in a wall with glass.\n"
        "Watch - To look at; or a timepiece.\n"
        "Warm - Moderately hot.\n"
        "You - The person being spoken to.\n"
        "Young - Not old.\n"
        "Yes - A positive response.\n"
        "Year - 12 months.\n"
        "Yellow - A color.\n"
        "Yard - Open area around a building.\n"
        "Youth - The time of being young.\n"
        "Yawn - To open your mouth wide when tired.\n"
        "Yogurt - Fermented milk product.\n"
        "Yield - Produce or give way.\n"
        "Zoo - A place where animals are kept.\n"
        "Zero - Number 0.\n"
        "Zone - An area with boundaries.\n"
        "Zebra - A striped African animal.\n"
        "Zip - Fasten with a zipper.\n"
        "Zoom - Move quickly or magnify digitally.\n"
        "Zinc - A chemical element.\n"
        "Zest - Great enthusiasm.\n"
        "Zombie - Fictional undead creature.\n"
        "Zeal - Passion or great energy.\n"
        "Quality - How good something is.\n"
        "Quiet - Making little noise.\n"
        "Queen - Female ruler.\n"
        "Quit - To stop doing something.\n"
        "Quarter - One-fourth.\n"
        "Quote - Repeat someone’s words.\n"
        "Quest - A long search or mission.\n"
        "Queue - A line of people waiting.\n";
}

// Parse "Word - Meaning." style line
bool parseLine(const string &line, string &word, string &meaning) {
    int pos = line.find('-');
    if (pos == string::npos) return false;
    word = line.substr(0, pos);
    meaning = line.substr(pos + 1);
    // trim spaces
    auto trim = [](string &s) {
        while (!s.empty() && isspace((unsigned char)s.front())) s.erase(s.begin());
        while (!s.empty() && isspace((unsigned char)s.back())) s.pop_back();
    };
    trim(word);
    trim(meaning);
    return !word.empty();
}

// build Trie from initial big text
void loadInitialDataIntoTrie() {
    string text = getInitialDictionaryText();
    stringstream ss(text);
    string line;
    while (getline(ss, line)) {
        if (line.empty()) continue;
        string word, meaning;
        if (parseLine(line, word, meaning)) {
            insertIntoTrie(word, meaning);
        }
    }
}

// ======================= FILE HANDLING + HUFFMAN INTEGRATION =======================
// File format: dictionary.huff
// Line 1: number_of_distinct_characters (N)
// Next N lines: token frequency
//    token is: single char OR "\n" for newline OR "\s" for space
// Next token: the word "DATA"
// Next token: encoded bitstring (0/1)

const string HUFF_FILE = "dictionary.huff";

void saveCompressedDictionaryToFile() {
    // 1) Get all words from Trie and convert to text
    vector<pair<string,string>> entries;
    string current;
    collectTrieWords(trieRoot, current, entries);

    string text;
    for (auto &p : entries) {
        text += p.first + " - " + p.second + "\n";
    }

    if (text.empty()) {
        cout << "Nothing to save.\n";
        return;
    }

    // 2) Huffman encode
    unordered_map<char,string> codes;
    unordered_map<char,int> freq;
    HuffmanNode* root = nullptr;
    string encoded = huffmanEncode(text, codes, root, freq);

    // 3) Write freq table + encoded string to file
    ofstream fout(HUFF_FILE);
    if (!fout) {
        cout << "Error opening file for write.\n";
        freeHuffmanTree(root);
        return;
    }

    fout << freq.size() << "\n";
    for (auto &p : freq) {
        char c = p.first;
        string token;
        if (c == '\n') token = "\\n";
        else if (c == ' ') token = "\\s";
        else token = string(1, c);
        fout << token << " " << p.second << "\n";
    }
    fout << "DATA\n";
    fout << encoded << "\n";

    fout.close();
    freeHuffmanTree(root);
    cout << "Dictionary compressed and saved to " << HUFF_FILE << "\n";
}

void loadCompressedDictionaryFromFile() {
    ifstream fin(HUFF_FILE);
    if (!fin) {
        cout << "No compressed file found (" << HUFF_FILE << ").\n";
        return;
    }

    int n;
    if (!(fin >> n)) {
        cout << "Corrupted file.\n";
        return;
    }

    unordered_map<char,int> freq;
    for (int i = 0; i < n; i++) {
        string token;
        int f;
        fin >> token >> f;
        char c;
        if (token == "\\n") c = '\n';
        else if (token == "\\s") c = ' ';
        else c = token[0];
        freq[c] = f;
    }

    string marker;
    fin >> marker;
    if (marker != "DATA") {
        cout << "Invalid file format.\n";
        return;
    }

    string encoded;
    fin >> encoded;
    fin.close();

    // rebuild Huffman tree
    HuffmanNode* root = buildHuffmanTree(freq);

    // decode back to full text
    string decoded = huffmanDecode(encoded, root);
    freeHuffmanTree(root);

    // clear old Trie and rebuild from decoded text
    clearTrie(trieRoot);
    trieRoot = new TrieNode();
    stringstream ss(decoded);
    string line;
    while (getline(ss, line)) {
        if (line.empty()) continue;
        string word, meaning;
        if (parseLine(line, word, meaning)) {
            insertIntoTrie(word, meaning);
        }
    }

    cout << "Dictionary loaded and decoded back into Trie from " << HUFF_FILE << "\n";
}

// ======================= SIMPLE MENU =======================

void menu() {
    while (true) {
        cout << "\n--- DICTIONARY + TRIE + HUFFMAN ---\n";
        cout << "1. Search word\n";
        cout << "2. Insert new word\n";
        cout << "3. Save compressed dictionary to file\n";
        cout << "4. Load compressed dictionary from file\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        int ch;
        if (!(cin >> ch)) break;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (ch == 1) {
            cout << "Enter word: ";
            string w;
            getline(cin, w);
            TrieNode* node = searchInTrie(w);
            if (node) {
                cout << "Meaning: " << node->meaning << "\n";
            } else {
                cout << "Word not found.\n";
            }
        } else if (ch == 2) {
            cout << "Enter word: ";
            string w;
            getline(cin, w);
            cout << "Enter meaning: ";
            string m;
            getline(cin, m);
            insertIntoTrie(w, m);
            cout << "Inserted.\n";
        } else if (ch == 3) {
            saveCompressedDictionaryToFile();
        } else if (ch == 4) {
            loadCompressedDictionaryFromFile();
        } else if (ch == 5) {
            cout << "Exiting.\n";
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }
}

int main() {
    loadInitialDataIntoTrie();   // load your full word list into Trie at start
    menu();
    clearTrie(trieRoot);
    return 0;
}
