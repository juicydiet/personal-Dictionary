#include<bits/stdc++.h>
using namespace std;

class TrieNode {
public:
    TrieNode* children[26];
    bool wordEnd;
    char data;
    string meaning;   
    TrieNode(char ch) {
        data=ch;
        wordEnd = false;
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
    }
};

class Trie {
public: 
  TrieNode* root;

  Trie() {
      root = new TrieNode('\0');
  }

  void insertHandler(TrieNode* node, string word, int i,string meaning) {      
    if (i == word.size()) {
      node->wordEnd = true;
      node->meaning = meaning; 
      return;
    }       
    int index = word[i] - 'a';
    TrieNode* child;
    if (node->children[index] != NULL) {
      child = node->children[index];
    }
    else {
      child = new TrieNode(word[i]);
      node->children[index] = child;
    }
    insertHandler(child, word, i + 1, meaning);
  }

  void insert(string word,string meaning) {
    insertHandler(root, word, 0, meaning);
  }

  bool searchHandler(TrieNode* node, string word, int i) {
    if (i == word.size()) {
        cout << "Meaning: " << node->meaning << endl;
        return node->wordEnd;
    }
    int index = word[i] - 'a';
    TrieNode* child;
    if (node->children[index] != NULL) {
        child = node->children[index];
    }
    else {
        return false;
    }
    return searchHandler(child, word, i + 1);
  }

  bool search(string word) {
    return searchHandler(root, word,0);
  }

  bool isEmpty(TrieNode* node) {
    for (int i = 0; i < 26; i++)
        if (node->children[i] != nullptr)
            return false;
    return true;
  }

  bool deleteHandler(TrieNode* node, string &word, int i) {
    if (i == word.size()) {
        if (!node->wordEnd) return false;
        node->wordEnd = false;           
        return true;
    }
    int index = word[i] - 'a';
    TrieNode* child = node->children[index];
    if (!child) return false; 
    bool deleted = deleteHandler(child, word, i + 1);
    if (deleted && !child->wordEnd && isEmpty(child)) {
        delete child;
        node->children[index] = nullptr;
    }
    return deleted;
  }

  void deleteWord(string word) {
    if (deleteHandler(root, word, 0))
        cout << "Deleted the entire word!" << endl;
    else
        cout << "Word not found!" << endl;
  }

  void printAllWords() {
    if (!root) return;
    cout << "All Words in Dictionary"<< endl;
    printAllWordsHandler(root, "");
  }

  void printAllWordsHandler(TrieNode* node, string current) {
    if (node->wordEnd) {
        cout << current << " : " << node->meaning << endl;
    }
    for (int i = 0; i < 26; i++) {
        if (node->children[i]) {
            char nextChar = 'a' + i;
            printAllWordsHandler(node->children[i], current + nextChar);
        }
    }
  }

  bool update(string word,string newMeaning) {
    return updateHandler(root, word,0,newMeaning);
  }

  bool updateHandler(TrieNode* node, string word, int i, string newMeaning) {
    if (i == word.size()) {
         node->meaning = newMeaning;
        return node->wordEnd;
    }
    int index = word[i] - 'a';
    TrieNode* child;
    if (node->children[index] != NULL) {
        child = node->children[index];
    }
    else {
        return false;
    }
    return updateHandler(child, word, i + 1,newMeaning);
  }
};

int main(){
  Trie* t = new Trie();
  int running = 1;
  int choice;
  cout <<"Welcome to the Dictionary Application"<<endl;
  cout<< "All words are to be inserted lowercase letters"<<endl;
  while (running){
    cout <<"Enter your choice"<<endl;
    cout <<"1. Add a word"<<endl;
    cout <<"2. Search a word"<<endl;
    cout <<"3. Update a word meaning"<<endl;
    cout <<"4. Show words starting from a letter"<<endl;
    cout <<"5. Show all words"<<endl;
    cout <<"6. Delete a word"<<endl;
    cout <<"7. Exit"<<endl;
    cin>> choice;
    string word,meaning;
    switch(choice){
      case 1:
        cout<<"enter word to insert: ";
        cin>>word;
        cout<<"enter meaning: ";
        cin>>meaning;
        t->insert(word,meaning);
        break;
      case 2:
        cout<<"enter word to search: ";
        cin>>word;
        if(!t->search(word)) cout <<"Word not found"<<endl;        
        break;
      case 3:
        cout<<"enter word to update: ";
        cin>>word;
        cout<<"enter updated meaning: ";
        cin>>meaning;
        t->update(word,meaning);
        break;
      case 4:
        cout <<"Showing words starting from a letter";
        break;
      case 5:
        t->printAllWords();
        break;
      case 6:
        cout<<"enter word to delete: ";
        cin>>word;
        t->deleteWord(word);
      case 7:
        running = 0;
        cout <<"Exited"<<endl;
        break;
      default:
        cout <<"Invalid choice"<<endl;
    }
  }
}
