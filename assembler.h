#ifndef _ASSEMBLER_H_
#define _ASSEMBLER_H_
#include <string>
using namespace std;
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>


class Assembler{

public:
    Assembler(string inFileName,string outFileName);
    int firstPass();
    int secondPass();
    int printSymbolTable();
    int printRellocationTable();
    int printCode();

private:
    //FILES-RELATED DECLARATIONS
    string inputFileName, outputFileName;
    std::ifstream inputFile;

    int LC;
    string currentSection;

    class symbolTableEntry;
    class relocationTableEntry;

    map<string,symbolTableEntry> symbolTable;
    map<string,vector<relocationTableEntry>> relocationTable;
    
    
    class symbolTableEntry{
    public:
        string label;
        int offset;
        string section;
        bool isGlobal;
        bool isDefined;
        int size;
        // int id = ++idGen;
        // static int idGen;
    
        symbolTableEntry(){}
        symbolTableEntry(string label,int offset, string section, bool isGlobal,int size,bool isDefined=false)
                        :label(label),offset(offset),section(section),isGlobal(isGlobal),size(size),isDefined(isDefined)
        {

        }
    
    };
    class relocationTableEntry{
    public:
        string type; //ABS OR PC_REL
        int offset;
        string sectionName;
        string symbolName;

        relocationTableEntry(string type,int offset,string name):type(type),offset(offset),sectionName(name)
        {

        }
    };

    //FIRST PASS FUNCTION DECLARATIONS
    int equFirstPass(string,string);
    int globalFirstPass(string);
    int externFirstPass(string);
    int wordFirstPass(string);
    int skipFirstPass(string);
    int sectionFirstPass(string);
    int endFirstPass();
    int labelFirstPass(string);

    int instructionNoOperandFirstPass();
    int instructionTwoRegFirstPass();
    int instructionOneRegFirstPass(string);

    int instructionLdrStrApsoluteFirstPass();
    int instructionLdrStrMemDirFirstPass();
    int instructionLdrStrPcFirstPass();
    int instructionLdrStrRegDirFirstPass();
    int instructionLdrStrRegIndFirstPass();
    int instructionLdrStrRegIndDispFirstPass();
    
    int instructionJmpApsoluteAddrFirstPass();
    int instructionJmpMemDirAddrFirstPass();
    int instructionJmpPcRelAddrFirstPass();
    int instructionJmpRegDirAddrFirstPass();
    int instructionJmpRegIndAddrFirstPass();
    int instructionJmpRegIndDispAddrFirstPass();

    //SECOND PASS FUNCTION DECLARATIONS
    int equSecondPass(string,string);
    int globalSecondPass(string);
    int externSecondPass(string);
    int wordSecondPass(string);
    int skipSecondPass(string);
    int sectionSecondPass(string);
    int endSecondPass();
    int labelSecondPass(string);

    int instructionNoOperandSecondPass(string);
    int instructionTwoRegSecondPass(string,string,string);
    int instructionOneRegSecondPass(string,string);

    int instructionLdrStrApsoluteSecondPass(string,string,string);
    int instructionLdrStrMemDirSecondPass(string,string,string);
    int instructionLdrStrPcSecondPass(string , string , string );
    int instructionLdrStrRegDirSecondPass(string , string , string );
    int instructionLdrStrRegIndSecondPass(string , string , string );
    int instructionLdrStrRegIndDispSecondPass(string , string , string,string);
    
    int instructionJmpApsoluteAddrSecondPass(string , string);
    int instructionJmpMemDirAddrSecondPass(string , string);
    int instructionJmpPcRelAddrSecondPass(string , string);
    int instructionJmpRegDirAddrSecondPass(string , string);
    int instructionJmpRegIndAddrSecondPass(string , string);
    int instructionJmpRegIndDispAddrSecondPass(string , string, string);


    map<string,vector<int>> code;

    //helpers
    int stringToInt(string s);
    string removeSpaces(string s);
    int getBinaryCodeFromMnemonic(string s);
    int processAbsSymAddr(string);
    int processPcRelSymAddr(string);
};

#endif