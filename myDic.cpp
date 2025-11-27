// ---------------- HUFFMAN CODING PART (STUDENT PROJECT FRIENDLY) ----------------
// You can paste this whole block in your project file.
// Then connect it with your Trie + file handling.

// NOTE: This block already includes ALL the words you sent as one big text,
// so you can test Huffman compression directly.

#include <bits/stdc++.h>
using namespace std;

// -------------------- HUFFMAN DATA STRUCTURES --------------------

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
        return a->freq > b->freq;  // smaller freq = higher priority
    }
};

// -------------------- HUFFMAN CORE FUNCTIONS --------------------

// Count frequency of each character.
unordered_map<char,int> buildFrequencyTable(const string &text) {
    unordered_map<char,int> freq;
    for (char c : text) freq[c]++;
    return freq;
}

// Build the Huffman tree.
HuffmanNode* buildHuffmanTree(const unordered_map<char,int> &freq) {
    if (freq.empty()) return nullptr;

    priority_queue<HuffmanNode*, vector<HuffmanNode*>, HuffmanCompare> pq;

    for (auto &p : freq) {
        pq.push(new HuffmanNode(p.first, p.second));
    }

    while (pq.size() > 1) {
        HuffmanNode* left  = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();

        HuffmanNode* parent = new HuffmanNode('\0', left->freq + right->freq);
        parent->left  = left;
        parent->right = right;
        pq.push(parent);
    }

    return pq.top();
}

// Generate Huffman codes (character → "0101…" string).
void buildCodes(HuffmanNode* root, const string &code,
                unordered_map<char,string> &codes) {
    if (!root) return;

    // Leaf node
    if (!root->left && !root->right) {
        codes[root->ch] = (code == "" ? "0" : code); // handle single-char case
        return;
    }

    buildCodes(root->left,  code + "0", codes);
    buildCodes(root->right, code + "1", codes);
}

// Encode any text to Huffman bitstring.
string huffmanEncode(const string &text,
                     unordered_map<char,string> &codes,
                     HuffmanNode* &rootOut) {
    if (text.empty()) {
        rootOut = nullptr;
        codes.clear();
        return "";
    }

    auto freq = buildFrequencyTable(text);
    HuffmanNode* root = buildHuffmanTree(freq);

    codes.clear();
    buildCodes(root, "", codes);

    string encoded;
    for (char c : text) encoded += codes[c];

    rootOut = root;
    return encoded;
}

// Decode Huffman bitstring back to original text.
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

// Free memory of the tree.
void freeHuffmanTree(HuffmanNode* root) {
    if (!root) return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    delete root;
}

// -------------------- YOUR DICTIONARY TEXT (ALL WORDS GIVEN) --------------------

// This function packs ALL your words + meanings into one big string.
// Later, you can replace this with:
//   - data exported from Trie
//   - or text read from file
string getDictionaryText() {
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
        "X-ray - Radiation used for imaging bones.\n"
        "Xylophone - A musical instrument.\n"
        "Xenon - A chemical element.\n"
        "Xerox - To photocopy.\n"
        "Xenial - Friendly or hospitable.\n"
        "Xylography - Wood engraving art.\n"
        "Xerosis - Abnormal dryness of skin.\n"
        "Xylose - A type of sugar.\n"
        "X-axis - Horizontal axis on a graph.\n"
        "Xenophobia - Fear of foreigners.\n"
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

// -------------------- READY-TO-USE HOOKS FOR YOUR PROJECT --------------------

// Use this in your project to get compressed data for the dictionary.
string getCompressedDictionary(unordered_map<char,string> &codes, HuffmanNode* &root) {
    string text = getDictionaryText();              // all words + meanings
    string encoded = huffmanEncode(text, codes, root);
    return encoded; // you can save this 'encoded' string into a file
}

// Use this to get back original dictionary text from encoded form.
string decodeDictionary(const string &encoded, HuffmanNode* root) {
    return huffmanDecode(encoded, root);
}

/*
HOW TO MERGE WITH FILE HANDLING + TRIE (idea):

1) At save time:
    unordered_map<char,string> codes;
    HuffmanNode* root = nullptr;
    string encoded = getCompressedDictionary(codes, root);

    // write 'encoded' to a file (binary or text)
    // also save 'codes' or a serialised version of freq table

2) At load time:
    // read encoded string back from file
    // rebuild Huffman tree using saved freq/codes
    string decoded = decodeDictionary(encoded, root);
    // now insert each word from 'decoded' into your Trie

You can now plug only this block into your student project.
*/
