#include<iostream>

using namespace std;

void byteOrder(){
    union{
        short val;
        char union_bytes[sizeof(val)];
    } test;
    test.val = 0x0102;

    if (test.union_bytes[0] == 1 && test.union_bytes[1] == 2){
        cout << "big endian" << endl;
    }
    else if (test.union_bytes[0] == 2 && test.union_bytes[1] == 1){
        cout << "little endian" << endl;
    } 
    else{
        cout << "unknow..." << endl;
    }
}

int main(){
    byteOrder();
    return 0;
}