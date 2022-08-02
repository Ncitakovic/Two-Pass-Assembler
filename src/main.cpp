
#include <iostream>
#include <string>
#include "assembler.h"

int main (int argc, char *argv[]) {
    //regex checking
    // string equ = ".equ a, 5";      
    // smatch ss;
    // if (regex_match(equ, ss, regexEqu)){
    //     cout<<"RADI equ"<<endl;
    // }else{
    //     cout<<"Ne radi equ"<<endl;
    // } 

    //MAIN
    if(argc!=4){
        cout<<"not valid number of arguments"<<endl;
        return -1;
    } 
    
    std::string o(argv[1]);
    if("-o" != o){
        cout<<"arguments are not correct"<<endl;
        return -1;
    }
    Assembler assembler(argv[3],argv[2]);
    if(assembler.firstPass()==-1){
        cout<<"ERROR IN FIRST PASS"<<endl;
        return -1;        
    }
    if(assembler.secondPass()==-1){
        cout<<"ERROR IN SECOND PASS"<<endl;
        return -1;        
    }

    // assembler.printAllTables();
    assembler.printSymbolTable();
    assembler.printRellocationTable();
    assembler.printCode();
    
    return 0;
}	
