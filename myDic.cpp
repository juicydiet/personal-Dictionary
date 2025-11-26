#include<bits/stdc++.h>
using namespace std;

void main(){
  int choice;
  while (1){
    cout <<"Enter your choice";
    cout <<"1. Add a word";
    cout <<"2. Search a word";
    cout <<"3. delete a word";
    cout <<"4. Show words starting from a letter ";
    cout <<"5. Show all words";
    cin>> choice;
    switch(choice){
      case 1:
        cout <<"Adding a word";
        break;
      case 2:
        cout <<"Searching a word";
        break;
      case 3:
        cout <<"Deleting a word";
        break;
      case 4:
        cout <<"Showing words starting from a letter";
        break;
      case 5:
        cout <<"Showing all words";
        break;
      default:
        cout <<"Invalid choice";
    }
    
  }
}
