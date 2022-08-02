#include "assembler.h"
#include "regexes.h"
#include <iomanip>


// int Assembler::symbolTableEntry::idGen=0;

Assembler::Assembler(string inFileName,string outFileName){
    this->inputFileName = inFileName;
    this->outputFileName = outFileName;
    this->LC = 0;
    this->currentSection = "";
    
    symbolTableEntry undef("UNDEF",0,"UNDEF",false,0);
    symbolTable.insert( std::pair<string,symbolTableEntry>("UNDEF",undef));

    symbolTableEntry abs("ABS",0,"ABS",false,0);
    symbolTable.insert( std::pair<string,symbolTableEntry>("ABS",abs));
    
}

//############################  FIRST PASS  ################################################

int Assembler::firstPass(){
    // std::ifstream input(inputFileName);
    inputFile.open(inputFileName);
    int ret = 0;
    if(inputFile.is_open()==false){
        //ERROR WITH OPENING FILE
        return -1;
    }
    for( std::string line; getline( inputFile, line ); )
    {
        smatch sm;
        cout<<"###"<<line<<"###"<<endl;
        //DIRECTIVES: 
        if (regex_match(line, sm, regexEqu)){
            ret = equFirstPass(sm.str(1),sm.str(2));
        }
        if(regex_match(line,sm,regexGlobal)){
            string globalList = sm.str(1);
            ret = globalFirstPass(globalList);
        }
        if(regex_match(line,sm,regexExtern)){
            string externList = sm.str(1);
            ret = externFirstPass(externList);
        }
        if(regex_match(line,sm,regexWord)){
            string wordList = sm.str(1);
            ret = wordFirstPass(wordList);
        }
        if(regex_match(line,sm,regexSkip)){
            ret = skipFirstPass(sm.str(1));
        }
        if(regex_match(line,sm,regexSection)){
            ret = sectionFirstPass(sm.str(1));
        }
        if(regex_match(line,sm,regexEnd)){
            ret = endFirstPass();
            break;
        }
        //INSTRUCTIONS
        if (regex_match(line, sm, regexInstructionNoOperand)){
            ret = instructionNoOperandFirstPass();
        }
        if (regex_match(line, sm, regexInstructionTwoReg)){
            ret = instructionTwoRegFirstPass();
        }
        if (regex_match(line, sm, regexInstructionOneReg)){
            ret = instructionOneRegFirstPass(sm.str(1));
        }

        if (regex_match(line,sm,regexInstructionLdrStrApsolute)){
            ret = instructionLdrStrApsoluteFirstPass();
        }else
        if (regex_match(line,sm,regexInstructionLdrStrPc)){
            ret = instructionLdrStrPcFirstPass();
        }else
        if (regex_match(line,sm,regexInstructionLdrStrRegDir)){
            ret = instructionLdrStrRegDirFirstPass();
        }else
        if (regex_match(line,sm,regexInstructionLdrStrMemDir)){
            ret = instructionLdrStrMemDirFirstPass();
        }else
        if (regex_match(line,sm,regexInstructionLdrStrRegInd)){
            ret = instructionLdrStrRegIndFirstPass();
        }else
        if (regex_match(line,sm,regexInstructionLdrStrRegIndDisp)){
            ret = instructionLdrStrRegIndDispFirstPass();
        }        

        if (regex_match(line,sm,regexInstructionJmpApsoluteAddr)){
            ret = instructionJmpApsoluteAddrFirstPass();
        }else        
        if (regex_match(line,sm,regexInstructionJmpMemDirAddr)){
            ret = instructionJmpMemDirAddrFirstPass();
        }else        
        if (regex_match(line,sm,regexInstructionJmpPcRel)){
            ret = instructionJmpPcRelAddrFirstPass();
        }else        
        if (regex_match(line,sm,regexInstructionJmpRegDirAddr)){
            ret = instructionJmpRegDirAddrFirstPass();
        }else        
        if (regex_match(line,sm,regexInstructionJmpRegIndAddr)){
            ret = instructionJmpRegIndAddrFirstPass();
        }else        
        if (regex_match(line,sm,regexInstructionJmpRegIndDispAddr)){
            ret = instructionJmpRegIndDispAddrFirstPass();
        }        
        


        //LABELS
        if(regex_match(line,sm,regexLabel)){
            cout<<sm.str(1)<<endl;
            ret = labelFirstPass(sm.str(1));            
        }
        else if(regex_match(line,sm,regexLabelWithDirOrInst)){
                //process label
                ret = labelFirstPass(sm.str(1));
                string dirOrInst = sm.str(2);
                smatch smch;
                //DIRECTIVES
                if (regex_match(dirOrInst, smch, regexEqu)){
                    ret = equFirstPass(smch.str(1),smch.str(2));
                }
                if(regex_match(dirOrInst,smch,regexGlobal)){
                    string globalList = smch.str(1);
                    ret = globalFirstPass(globalList);
                }
                if(regex_match(dirOrInst,smch,regexExtern)){
                    string externList = smch.str(1);
                    ret = externFirstPass(externList);
                }
                if(regex_match(dirOrInst,smch,regexWord)){
                    string wordList = smch.str(1);
                    ret = wordFirstPass(wordList);
                }
                if(regex_match(dirOrInst,smch,regexSkip)){
                    ret = skipFirstPass(smch.str(1));
                }
                if(regex_match(dirOrInst,smch,regexSection)){
                    ret = sectionFirstPass(smch.str(1));
                }
                if(regex_match(dirOrInst,smch,regexEnd)){
                    break;
                }
                //INSTRUCTIONS
                if (regex_match(line, sm, regexInstructionNoOperand)){
                    ret = instructionNoOperandFirstPass();
                }
                if (regex_match(line, sm, regexInstructionTwoReg)){
                    ret = instructionTwoRegFirstPass();
                }
                if (regex_match(line, sm, regexInstructionOneReg)){
                    ret = instructionOneRegFirstPass(sm.str(1));
                }
                
                if (regex_match(line,sm,regexInstructionLdrStrApsolute)){
                    ret = instructionLdrStrApsoluteFirstPass();
                }else
                if (regex_match(line,sm,regexInstructionLdrStrRegDir)){
                    ret = instructionLdrStrRegDirFirstPass();
                }else
                if (regex_match(line,sm,regexInstructionLdrStrMemDir)){
                    ret = instructionLdrStrMemDirFirstPass();
                }else
                if (regex_match(line,sm,regexInstructionLdrStrPc)){
                    ret = instructionLdrStrPcFirstPass();
                }else                
                if (regex_match(line,sm,regexInstructionLdrStrRegInd)){
                    ret = instructionLdrStrRegIndFirstPass();
                }else
                if (regex_match(line,sm,regexInstructionLdrStrRegIndDisp)){
                    ret = instructionLdrStrRegIndDispFirstPass();
                }

                if (regex_match(line,sm,regexInstructionJmpApsoluteAddr)){
                    ret = instructionJmpApsoluteAddrFirstPass();
                }else        
                if (regex_match(line,sm,regexInstructionJmpMemDirAddr)){
                    ret = instructionJmpMemDirAddrFirstPass();
                }else        
                if (regex_match(line,sm,regexInstructionJmpPcRel)){
                    ret = instructionJmpPcRelAddrFirstPass();
                }else        
                if (regex_match(line,sm,regexInstructionJmpRegDirAddr)){
                    ret = instructionJmpRegDirAddrFirstPass();
                }else        
                if (regex_match(line,sm,regexInstructionJmpRegIndAddr)){
                    ret = instructionJmpRegIndAddrFirstPass();
                }else        
                if (regex_match(line,sm,regexInstructionJmpRegIndDispAddr)){
                    ret = instructionJmpRegIndDispAddrFirstPass();
                }

        }

        //if there was some error then return 
        if(ret==-1) return -1;
    }
    inputFile.close();
    LC = 0;
    currentSection="";

    return ret;
}

//DIRECTIVES FIRST PASS
int Assembler::equFirstPass(string label, string value){
    
    int offset = stringToInt(value);
    //check if symbol already exists in symbol table
    if(symbolTable.count(label) > 0){
        if(symbolTable[label].offset>=0){
            cout<<"symbol cannot be redefined";
            return -1;
        }
        if(symbolTable[label].section=="UNDEF" && symbolTable[label].offset==0){
            cout<<"extern symbol cannot be defined";
            return -1;
        }
        symbolTable[label].section="ABS";
        symbolTable[label].offset = offset;

        // code["ABS"].push_back(offset);

        code["ABS"].push_back(offset & 0xff);
        code["ABS"].push_back((offset>>8) & 0xff);

    }else{
        symbolTableEntry newSymbol;
        newSymbol.label = label;
        newSymbol.offset = offset;
        newSymbol.section = "ABS";
        newSymbol.size=0;
        newSymbol.isGlobal=false;
        symbolTable.insert(std::pair<string,symbolTableEntry>(label,newSymbol)); 

        // code["ABS"].push_back(offset);
        code["ABS"].push_back(offset & 0xff);
        code["ABS"].push_back((offset>>8) & 0xff);
    }
    return 0;   
}

int Assembler::globalFirstPass(string s){
    std::istringstream ss(s);
    std::string sym;

    while(std::getline(ss, sym, ',')) {
        sym = removeSpaces(sym);
        if(symbolTable.count(sym)>0){
            symbolTable[sym].isGlobal=true;
        }else{
            symbolTableEntry newSymbol(sym,-1,"UNDEF",true,0); //offset=-1 -> UNDEF symbol
            symbolTable.insert(std::pair<string,symbolTableEntry>(sym,newSymbol));
        }
        cout<<"###"<<sym<<"###"<<endl;
    }
    return 0;
}

int Assembler::externFirstPass(string s){
    
    string sym;
    istringstream ss(s);
    while(getline(ss,sym,',')){
        sym = removeSpaces(sym);
        cout<<sym<<endl;
        if(symbolTable.count(sym)>0){
            cout<<"error: extern symbol already exists";
            return -1;
        }else{
            symbolTableEntry newSymbol(sym,0,"UNDEF",true,0);
            symbolTable.insert(std::pair<string,symbolTableEntry>(sym,newSymbol));
        }
    }
    return 0;
}
int Assembler::wordFirstPass(string s){
    if(currentSection=="") {
        return -1;
        cout<<"cannot generate code outside section";
    }
    std::string sym;
    std::istringstream ss(s);
    int cnt=0;
    while(getline(ss,sym,',')){
        cnt++;
    }
    LC += cnt*2;
    cout<<"###"<<"CNT:"<<"###"<<cnt*2<<"###"<<endl;
    return 0;
}

int Assembler::skipFirstPass(string s){
    if(currentSection==""){
        cout<<"error with skip in 1st pass";
        return -1;
    } 
    int lit = stringToInt(s);
    LC += lit;
    cout<<"SKIP:"<<lit<<endl;
    return 0;
}

int Assembler::sectionFirstPass(string s){
    if(currentSection!=""){
        symbolTable[currentSection].size = LC;
    }
    
    LC = 0;
    currentSection = s;

    //SECTION WITH THIS NAME ALREADY EXISTS, WRITE ERROR AND RETURN
    if(symbolTable.count(s)>0){
        cout<<"SECTION WITH THIS NAME ALREADY EXISTS"<<endl;
        return -1;
    }


    symbolTableEntry newSymbol(s,0,s,false,0);
    symbolTable.insert(std::pair<string,symbolTableEntry>(s,newSymbol));
    return 0;   
}

int Assembler::endFirstPass(){
    int sizeOfLastSection;
    if(currentSection!=""){
        sizeOfLastSection = symbolTable[currentSection].size = LC;
    }
    cout<< "SIZE OF LAST SECTION IS EQUAL ----> "<<LC;
    return 0;
}


//INSTRUCTIONS FIRST PASS
int Assembler::instructionNoOperandFirstPass(){
    LC++;
    return 0;
}

int Assembler::instructionTwoRegFirstPass(){
    LC += 2;
    return 0;
}

int Assembler::instructionOneRegFirstPass(string s){
    if(s=="pop" || s=="push") LC +=3;
    else LC += 2;  //not  /  int
    cout<<LC<<endl;
    return 0;
}

int Assembler::instructionLdrStrApsoluteFirstPass(){
    LC += 5;
    cout<<LC<<endl;
    return 0;
}
int Assembler::instructionLdrStrMemDirFirstPass(){
    LC += 5;
    cout<<LC<<endl;
    return 0;
}
int Assembler::instructionLdrStrPcFirstPass(){
    LC += 5;
    cout<<LC<<endl;
    return 0;
}
int Assembler::instructionLdrStrRegDirFirstPass(){
    LC += 3;
    return 0;
}
int Assembler::instructionLdrStrRegIndFirstPass(){
    LC += 3;
    return 0;
}
int Assembler::instructionLdrStrRegIndDispFirstPass(){
    LC += 5;
    return 0;
}


int Assembler::instructionJmpApsoluteAddrFirstPass(){
    LC += 5;
    return 0;
}
int Assembler::instructionJmpMemDirAddrFirstPass(){
    LC += 5;
    return 0;
}
int Assembler::instructionJmpPcRelAddrFirstPass(){
    LC += 5;
    return 0;
}
int Assembler::instructionJmpRegDirAddrFirstPass(){
    LC += 3;
    return 0;
}
int Assembler::instructionJmpRegIndAddrFirstPass(){
    LC += 3;
    return 0;
}
int Assembler::instructionJmpRegIndDispAddrFirstPass(){
    LC += 5;
    return 0;
}

string Assembler::removeSpaces(string s){
    s.erase(remove(s.begin(), s.end(), ' '), s.end());
    // cout<<"###"<<s<<"###"<<endl;
    return s;
}

int Assembler::labelFirstPass(string s){
    //SYMBOL HAS TO BE INSIDE A SECTION
    if(currentSection=="") return -1;

    if(symbolTable.count(s)>0){
            //SYMBOL EXISTS
            if(symbolTable[s].offset>=0){
                cout<<"Symbol was already defined";
                return -1;
            }
            if(symbolTable[s].section=="UNDEF" && symbolTable[s].offset==0){
                cout<<"cannot define extern symbol";
                return -1;
            }
        symbolTable[s].offset = LC;
        symbolTable[s].section = currentSection;
    }else{
        //SYMBOL DOES NOT EXIST
        symbolTableEntry sym(s,LC,currentSection,false,0);
        symbolTable.insert(std::pair<string,symbolTableEntry>(s,sym));    
    }    
    return 0;
}

//##########################################  SECOND PASS   ##########################################

int Assembler::secondPass(){
    // std::ifstream input(inputFileName);
    inputFile.open(inputFileName);
    int ret = 0;
    if(inputFile.is_open()==false){
        //ERROR WITH OPENING FILE
        return -1;
    }
    for( std::string line; getline( inputFile, line ); )
    {
        smatch sm;
        cout<<"###"<<line<<"###"<<endl;
        //DIRECTIVES: 
        if (regex_match(line, sm, regexEqu)){
            ret = equSecondPass(sm.str(1),sm.str(2));
        }
        if(regex_match(line,sm,regexGlobal)){
            string globalList = sm.str(1);
            ret = globalSecondPass(globalList);
        }
        if(regex_match(line,sm,regexExtern)){
            string externList = sm.str(1);
            ret = externSecondPass(externList);
        }
        if(regex_match(line,sm,regexWord)){
            string wordList = sm.str(1);
            ret = wordSecondPass(wordList);
        }
        if(regex_match(line,sm,regexSkip)){
            ret = skipSecondPass(sm.str(1));
        }
        if(regex_match(line,sm,regexSection)){
            ret = sectionSecondPass(sm.str(1));
        }
        if(regex_match(line,sm,regexEnd)){
            ret = endSecondPass();
            break;
        }

        //INSTRUCTIONS
        if (regex_match(line, sm, regexInstructionNoOperand)){
            ret = instructionNoOperandSecondPass(sm.str(1));
        }
        if (regex_match(line, sm, regexInstructionTwoReg)){
            string instName = sm.str(1), reg1 = sm.str(2), reg2 = sm.str(3);        
            ret = instructionTwoRegSecondPass(instName,reg1,reg2);
        }
        if (regex_match(line, sm, regexInstructionOneReg)){
            ret = instructionOneRegSecondPass(sm.str(1),sm.str(2));
        }

        if (regex_match(line,sm,regexInstructionLdrStrApsolute)){
            ret = instructionLdrStrApsoluteSecondPass(sm.str(1),sm.str(2),sm.str(3));
        }else
        if (regex_match(line,sm,regexInstructionLdrStrRegDir)){
            ret = instructionLdrStrRegDirSecondPass(sm.str(1),sm.str(2),sm.str(3));
        }else
        if (regex_match(line,sm,regexInstructionLdrStrMemDir)){
            ret = instructionLdrStrMemDirSecondPass(sm.str(1),sm.str(2),sm.str(3));
        }else
        if (regex_match(line,sm,regexInstructionLdrStrPc)){
            ret = instructionLdrStrPcSecondPass(sm.str(1),sm.str(2),sm.str(3));
        }else                
        if (regex_match(line,sm,regexInstructionLdrStrRegInd)){
            ret = instructionLdrStrRegIndSecondPass(sm.str(1),sm.str(2),sm.str(3));
        }else
        if (regex_match(line,sm,regexInstructionLdrStrRegIndDisp)){
            ret = instructionLdrStrRegIndDispSecondPass(sm.str(1),sm.str(2),sm.str(3),sm.str(4));
        }        

        if (regex_match(line,sm,regexInstructionJmpApsoluteAddr)){
            ret = instructionJmpApsoluteAddrSecondPass(sm.str(1),sm.str(2));
        }else        
        if (regex_match(line,sm,regexInstructionJmpRegDirAddr)){
            ret = instructionJmpRegDirAddrSecondPass(sm.str(1),sm.str(2));
        }else
        if (regex_match(line,sm,regexInstructionJmpMemDirAddr)){
            ret = instructionJmpMemDirAddrSecondPass(sm.str(1),sm.str(2));
        }else        
        if (regex_match(line,sm,regexInstructionJmpPcRel)){
            ret = instructionJmpPcRelAddrSecondPass(sm.str(1),sm.str(2));
        }else                    
        if (regex_match(line,sm,regexInstructionJmpRegIndDispAddr)){
            ret = instructionJmpRegIndDispAddrSecondPass(sm.str(1),sm.str(2),sm.str(3));
        }else
        if (regex_match(line,sm,regexInstructionJmpRegIndAddr)){
            ret = instructionJmpRegIndAddrSecondPass(sm.str(1),sm.str(2));
        }        
                

        //LABELS
        if(regex_match(line,sm,regexLabel)){
            cout<<sm.str(1)<<endl;
            ret = labelSecondPass(sm.str(1));            
        }
        else if(regex_match(line,sm,regexLabelWithDirOrInst)){
                //process label
                ret = labelSecondPass(sm.str(1));
                string dirOrInst = sm.str(2);
                smatch smch;
                //DIRECTIVES
                if (regex_match(dirOrInst, smch, regexEqu)){
                    ret = equSecondPass(smch.str(1),smch.str(2));
                }else
                if(regex_match(dirOrInst,smch,regexGlobal)){
                    string globalList = smch.str(1);
                    ret = globalSecondPass(globalList);
                }else
                if(regex_match(dirOrInst,smch,regexExtern)){
                    string externList = smch.str(1);
                    ret = externSecondPass(externList);
                }else
                if(regex_match(dirOrInst,smch,regexWord)){
                    string wordList = smch.str(1);
                    ret = wordSecondPass(wordList);
                }else
                if(regex_match(dirOrInst,smch,regexSkip)){
                    ret = skipSecondPass(smch.str(1));
                }else
                if(regex_match(dirOrInst,smch,regexSection)){
                    ret = sectionSecondPass(smch.str(1));
                }else
                if(regex_match(dirOrInst,smch,regexEnd)){
                    break;
                }
                //INSTRUCTIONS
                if (regex_match(line, smch, regexInstructionNoOperand)){
                    ret = instructionNoOperandSecondPass(smch.str(1));
                }
                if (regex_match(line, smch, regexInstructionTwoReg)){
                    ret = instructionTwoRegSecondPass(smch.str(1),smch.str(2),smch.str(3));
                }
                if (regex_match(line, smch, regexInstructionOneReg)){
                    ret = instructionOneRegSecondPass(smch.str(1),smch.str(2));
                }
                
                if (regex_match(line,smch,regexInstructionLdrStrApsolute)){
                    ret = instructionLdrStrApsoluteSecondPass(smch.str(1),smch.str(2),smch.str(3));
                }else
                if (regex_match(line,smch,regexInstructionLdrStrMemDir)){
                    ret = instructionLdrStrMemDirSecondPass(smch.str(1),smch.str(2),smch.str(3));
                }else
                if (regex_match(line,smch,regexInstructionLdrStrPc)){
                    ret = instructionLdrStrPcSecondPass(smch.str(1),smch.str(2),smch.str(3));
                }else
                if (regex_match(line,smch,regexInstructionLdrStrRegDir)){
                    ret = instructionLdrStrRegDirSecondPass(smch.str(1),smch.str(2),smch.str(3));
                }else
                if (regex_match(line,smch,regexInstructionLdrStrRegInd)){
                    ret = instructionLdrStrRegIndSecondPass(smch.str(1),smch.str(2),smch.str(3));
                }else
                if (regex_match(line,smch,regexInstructionLdrStrRegIndDisp)){
                    ret = instructionLdrStrRegIndDispSecondPass(smch.str(1),smch.str(2),smch.str(3),smch.str(4));
                }

                if (regex_match(line,smch,regexInstructionJmpApsoluteAddr)){
                    ret = instructionJmpApsoluteAddrSecondPass(smch.str(1),smch.str(2));
                }else        
                if (regex_match(line,smch,regexInstructionJmpRegDirAddr)){
                    ret = instructionJmpRegDirAddrSecondPass(smch.str(1),smch.str(2));
                }else
                if (regex_match(line,smch,regexInstructionJmpMemDirAddr)){
                    ret = instructionJmpMemDirAddrSecondPass(smch.str(1),smch.str(2));
                }else        
                if (regex_match(line,smch,regexInstructionJmpPcRel)){
                    ret = instructionJmpPcRelAddrSecondPass(smch.str(1),smch.str(2));
                }else                                
                if (regex_match(line,smch,regexInstructionJmpRegIndAddr)){
                    ret = instructionJmpRegIndAddrSecondPass(smch.str(1),smch.str(2));
                }else        
                if (regex_match(line,smch,regexInstructionJmpRegIndDispAddr)){
                    ret = instructionJmpRegIndDispAddrSecondPass(smch.str(1),smch.str(2),smch.str(3));
                }

        }

        //if there was some error then return 
        if(ret==-1) return -1;
    }
    inputFile.close();
    LC = 0;
    currentSection="";

    return ret;

}


int Assembler::equSecondPass(string s1,string s2){
    //NOTHING TO DO WITH EQU IN SECOND PASS
    return 0;
}
int Assembler::globalSecondPass(string s){
    //NOTHING TO DO WITH GLOBAL IN SECOND PASS
    return 0;
}
int Assembler::externSecondPass(string s){
    //NOTHING TO DO WITH EXTERN IN SECOND PASS
    return 0;
}
int Assembler::skipSecondPass(string s){
    int n = stringToInt(s);
    int x = n;
    while(n-- > 0)  
        (code[currentSection]).push_back(0);
    

    cout<<"LC BEFORE SKIP"<<LC;        
    LC += x;
    cout<<"LC AFTER SKIP"<<LC;
    return 0;
}
int Assembler::sectionSecondPass(string s){
    currentSection = s;
    LC = 0;
    return 0;
}
int Assembler::endSecondPass(){
    //END WITH SECOND PASS
    return 0;
}
int Assembler::wordSecondPass(string s){

    if(currentSection=="") {
        return -1;
        cout<<"cannot generate code outside section";
    }
    std::string sym;
    std::istringstream ss(s);
    while(getline(ss,sym,',')){
        sym = removeSpaces(sym);
        if(regex_match(sym,regexSymbol)){
            if(symbolTable.count(sym) == 0) return -1; //no symbol in symbol table      
            //local
            if( ! symbolTable[sym].isGlobal ){
                relocationTableEntry r = relocationTableEntry("ABS",LC,currentSection);
                // r.sectionName = currentSection;
                r.symbolName = symbolTable[sym].section;
                int val = symbolTable[sym].offset;
                int dl = val & 0xff;
                int dh = (val >> 8) & 0xff;
                code[currentSection].push_back(dl);
                code[currentSection].push_back(dh);
                relocationTable[currentSection].push_back(r);
                cout<<"LC BEGIN"<<LC<<endl;                
            }else if(symbolTable[sym].section == "ABS"){
                int val = symbolTable[s].offset;
                int dl = val & 0xff;
                int dh = (val >> 8) & 0xff;
                //PROVERITI REDOSLED STAVLJANJA U KOD
                code[currentSection].push_back(dl);
                code[currentSection].push_back(dh);  
            }else{
                //global
                relocationTableEntry r = relocationTableEntry("ABS",LC,currentSection);
                // r.sectionName = currentSection;
                r.symbolName = symbolTable[sym].label;                                            
                code[currentSection].push_back(0);//dh
                code[currentSection].push_back(0);//dl
                relocationTable[currentSection].push_back(r);
            }
        }

        if(regex_match(sym,regexLit)){
            int val = stringToInt(sym);
            int dl = val & 0xff;
            int dh = (val >> 8) & 0xff;
            //PROVERITI REDOSLED STAVLJANJA U KOD
            code[currentSection].push_back(dl);
            code[currentSection].push_back(dh); 
        }                
        
        LC += 2;        
        cout<<"LC END"<<LC<<endl;
    }
    return 0;
}

int Assembler::instructionNoOperandSecondPass(string s){
    if(currentSection!=""){
        int binCode = getBinaryCodeFromMnemonic(s);
        code[currentSection].push_back(binCode);
        // cout<<endl<< "CODE:" <<binCode<<endl;
        LC+=1;
        return 0;
    }else{
        return -1;
    }
}

int Assembler::instructionTwoRegSecondPass(string instName,string reg1,string reg2){
    int instDesc = getBinaryCodeFromMnemonic(instName);
    int v1,v2;
    if(reg1=="psw"){
        v1 = 8;
    }else{
        v1 = reg1.at(1)-'0';
    }
    if(reg2=="psw"){
        v2 = 8;
    }else{
        v2 = reg2.at(1)-'0';
    }
    cout<<reg1<<endl;
    cout<<reg2<<endl;

    if(currentSection!=""){
        code[currentSection].push_back(instDesc);
        int regDesc = (v1<<4) + (v2 & 0xf);
        code[currentSection].push_back(regDesc);
        
    }else{
        return -1;
    }
    LC+=2;
    cout<<LC<< "  <---------------------"<<endl;
    return 0;
}

int Assembler::instructionOneRegSecondPass(string s, string reg){
    int instDesc = getBinaryCodeFromMnemonic(s);
    code[currentSection].push_back(instDesc);

    int regDesc;
    int n;
    if(reg == "psw"){
        n = 8;         
    }else{
        n = (reg.at(1)-'0');                
    }
    if(s=="int" || s=="not"){
        regDesc = n<<4;
        regDesc += 15;
        code[currentSection].push_back(regDesc); 
        LC+=2;       
    }else if(s=="push"){
        regDesc = (6<<4)+n;                
        code[currentSection].push_back(regDesc);
        code[currentSection].push_back(0x12);
        LC+=3;

        cout<<endl<<"-------"<<endl;
        cout<<regDesc<<endl;
        for(int i=0;i<code[currentSection].size();i++){
            cout<<code[currentSection].at(i)<<" ";
        }
        cout<<endl<<"---------"<<endl;

    }else{ //if pop
        regDesc = n<<4;
        regDesc += 6;
        code[currentSection].push_back(regDesc);
        code[currentSection].push_back(0x42);
        LC+=3;
    }
    return 0;

}

int Assembler::instructionLdrStrApsoluteSecondPass(string inst, string v1, string v2){
    int instDesc = getBinaryCodeFromMnemonic(inst);    
    
    int regD = (v1 == "psw" ? 8 : v1.at(1)-'0');
    cout<<"REGD              "<<regD<<endl; 
    int regsDesc = (regD <<4 ) + 0xf;

    int DH;
    int DL;
    int mode = 0x00;

    if(regex_match(v2,regexSymbol)){
        // int data = symbolTable[v2].offset;        
        int data = processAbsSymAddr(v2);
        DH = (data>>8) & 0xff;
        DL = data & 0xff;
    }else{
        DH = (stringToInt(v2)>>8) & 0xff;
        DL = stringToInt(v2) & 0xff;
    }

    code[currentSection].push_back(instDesc);
    code[currentSection].push_back(regsDesc);
    code[currentSection].push_back(mode);
    code[currentSection].push_back(DH);
    code[currentSection].push_back(DL);

    LC += 5;
    return 0;
}
int Assembler::instructionLdrStrMemDirSecondPass(string inst, string v1, string v2){
    int instDesc = getBinaryCodeFromMnemonic(inst);  
    int regsDesc;

    int regD = (v1 == "psw" ? 8 : v1.at(1)-'0');
    if(inst == "ldr"){
        regsDesc = (regD <<4) + 0xf;
    }else{
        regsDesc = (0xf << 4) + regD;
    }
    int mode = 0x04;
    // int data  = symbolTable[v2].offset;
    int data = processAbsSymAddr(v2);
    int DH = data>>8;
    int DL = data & 0xff;

    code[currentSection].push_back(instDesc);
    code[currentSection].push_back(regsDesc);
    code[currentSection].push_back(mode);
    code[currentSection].push_back(DH);
    code[currentSection].push_back(DL);

    LC += 5;
    return 0;
}
int Assembler::instructionLdrStrPcSecondPass(string inst, string v1, string v2){
    int instDesc = getBinaryCodeFromMnemonic(inst);
    int regsDesc;

    int regD = (v1 == "psw" ? 8 : v1.at(1)-'0');
    if(inst == "ldr"){
        regsDesc = (regD <<4) + 0x7;
    }else{
        regsDesc = (0x7 << 4) + regD;
    }

    int mode = 0x03;
    int data = processPcRelSymAddr(v2);
    int DH = (data>>8) & 0xff;
    int DL = data & 0xff;

    code[currentSection].push_back(instDesc);
    code[currentSection].push_back(regsDesc);
    code[currentSection].push_back(mode);
    code[currentSection].push_back(DH);
    code[currentSection].push_back(DL);

    LC += 5;
    return 0;
}
int Assembler::instructionLdrStrRegDirSecondPass(string inst, string v1, string v2 ){
    int instDesc = getBinaryCodeFromMnemonic(inst);
    int regsDesc;

    int regD = (v1 == "psw" ? 8 : v1.at(1)-'0');
    int regS = (v2 == "psw" ? 8 : v2.at(1)-'0');
    cout << "regD  -------  " << regD<<endl;
    cout << "regS   -------  " << regS<<endl;
    int result;
    if(inst == "ldr"){
        result = regS & 0xf;
        result |= (regD & 0xf)<<4;

        cout <<  "regsDesc -------  "<<result<<endl; 
        regsDesc = result;
                                         
    }else{
        regsDesc = (0xf << 4) + regD;
    }
    int mode = 0x01;

    code[currentSection].push_back(instDesc);
    code[currentSection].push_back(regsDesc);
    code[currentSection].push_back(mode);
    LC += 3;
    return 0;
}
int Assembler::instructionLdrStrRegIndSecondPass(string inst, string v1, string v2){
    int instDesc = getBinaryCodeFromMnemonic(inst);
    int regsDesc;

    int regD = (v1 == "psw" ? 8 : v1.at(1)-'0');
    int regS = (v2 == "psw" ? 8 : v2.at(1)-'0');
    if(inst == "ldr"){
        regsDesc = (regD << 4) + regS;
    }else{
        regsDesc = (regS << 4) + regD;
    }
    
    int mode = 0x02;
    code[currentSection].push_back(instDesc);
    code[currentSection].push_back(regsDesc);
    code[currentSection].push_back(mode);
    
    LC += 3;
    return 0;
}
int Assembler::instructionLdrStrRegIndDispSecondPass(string inst, string v1, string v2,string v3){

    
    int instDesc = getBinaryCodeFromMnemonic(inst);
    int regsDesc;

    int regD = (v1 == "psw" ? 8 : v1.at(1)-'0');
    int regS = (v2 == "psw" ? 8 : v2.at(1)-'0');
    cout<<"&&&" <<regD<<endl;
    cout<<"&&&" <<regS<<endl;
    if(inst == "ldr"){
        regsDesc = (regD << 4) + regS;
    }else{
        regsDesc = (regS << 4) + regD;
    }

    int mode = 0x03;
    // int DH = stringToInt(v2)>>4;
    // int DL = stringToInt(v2) & 0xff;
    // int DH = processAbsSymAddr(v3) >> 8;
    // int DL = processAbsSymAddr(v3) & 0xff;
    int DH,DL;
    if(regex_match(v3,regexSymbol)){
        // int data = symbolTable[v2].offset;        
        int data = processAbsSymAddr(v3);
        DH = (data>>8) & 0xff;
        DL = data & 0xff;
    }else{
        DH = (stringToInt(v3)>>8) & 0xff;
        DL = stringToInt(v3) & 0xff;
    }




    
    code[currentSection].push_back(instDesc);
    code[currentSection].push_back(regsDesc);
    code[currentSection].push_back(mode);
    code[currentSection].push_back(DH);
    code[currentSection].push_back(DL);

    LC += 5;
    return 0;
}

int Assembler::instructionJmpApsoluteAddrSecondPass(string inst,string operand){
    int instDesc = getBinaryCodeFromMnemonic(inst);
    int regsDesc = 0xf0;
    int mode = 0x00;
    int opr;

    if(regex_match(operand,regexSymbol)){
        opr = processAbsSymAddr(operand);
        if(opr == -1) return -1;
    }else{
        opr = stringToInt(operand);
    }
    
    int DH = (opr >> 8) & 0xff;
    int DL = opr & 0xff;

    code[currentSection].push_back(instDesc);
    code[currentSection].push_back(regsDesc);
    code[currentSection].push_back(mode);
    code[currentSection].push_back(DH);
    code[currentSection].push_back(DL);

    LC+=5;
    return 0;
}

int Assembler::instructionJmpMemDirAddrSecondPass(string inst,string operand){
    int instDesc = getBinaryCodeFromMnemonic(inst);
    int regsDesc = 0xf0;
    int mode = 0x04;
    int opr;
    if(regex_match(operand,regexSymbol)){
        opr = processAbsSymAddr(operand);
        if(opr==-1) return -1;
    }else{
        opr = stringToInt(operand);
    }   
    int DH = (opr >> 8) & 0xff;
    int DL = opr & 0xff;

    code[currentSection].push_back(instDesc);
    code[currentSection].push_back(regsDesc);
    code[currentSection].push_back(mode);
    code[currentSection].push_back(DH);
    code[currentSection].push_back(DL);

    LC+=5;
    return 0;
}
int Assembler::instructionJmpPcRelAddrSecondPass(string inst,string operand){
    int instDesc = getBinaryCodeFromMnemonic(inst);
    int regsDesc = 0xf0;
    int mode = 0x03;

    int opr = processPcRelSymAddr(operand);
    
    int DH = (opr >> 8) & 0xff;
    int DL = opr & 0xff;
    
    code[currentSection].push_back(instDesc);
    code[currentSection].push_back(regsDesc);
    code[currentSection].push_back(mode);
    code[currentSection].push_back(DH);
    code[currentSection].push_back(DL);

    LC+=5;
    return 0;
}
int Assembler::instructionJmpRegDirAddrSecondPass(string inst,string operand){
    int instDesc = getBinaryCodeFromMnemonic(inst);
    int regsDesc = 0xf << 4;
    // smatch opr;
    // if(regex_match(operand,opr,regexInstructionJmpRegDirAddr)){
    int opr_int = (operand == "psw") ? 8 : operand.at(1)-'0';
    regsDesc += opr_int;
    // }else{
    //    return -1;
    //}
    int mode = 0x01;
    
    code[currentSection].push_back(instDesc);
    code[currentSection].push_back(regsDesc);
    code[currentSection].push_back(mode);

    LC+=3;
    return 0;
}
int Assembler::instructionJmpRegIndAddrSecondPass(string inst,string operand){
    int instDesc = getBinaryCodeFromMnemonic(inst);
    int regsDesc = 0xf << 4;
    // smatch opr;
    // if(regex_match(operand,opr,regexInstructionJmpRegIndAddr)){
    int opr_int = (operand == "psw") ? 8 : operand.at(1)-'0';
    regsDesc += opr_int;
    // }else{
        // return -1;
    // }
    int mode = 0x02;
    
    code[currentSection].push_back(instDesc);
    code[currentSection].push_back(regsDesc);
    code[currentSection].push_back(mode);

    LC+=3;
    return 0;
}
int Assembler::instructionJmpRegIndDispAddrSecondPass(string inst,string operand,string disp){    
    cout<<"IIIIIIIINNNNNNN"<<endl;
    int instDesc = getBinaryCodeFromMnemonic(inst);
    int regsDesc = 0xf << 4;
    int mode = 0x03;
    smatch opr;
    // if(regex_match(operand,opr,regexInstructionJmpRegIndDispAddr)){
        int opr_int = (operand == "psw") ? 8 : operand.at(1)-'0';
        regsDesc += opr_int;
    // }else{
        // return -1;
    // }
    int displacement = stringToInt(disp);
    int DH = (displacement >> 8) & 0xff;
    int DL = displacement & 0xff;

    code[currentSection].push_back(instDesc);
    code[currentSection].push_back(regsDesc);
    code[currentSection].push_back(mode);
    code[currentSection].push_back(DH);
    code[currentSection].push_back(DL);

    LC+=5;
    return 0;
}



int Assembler::processAbsSymAddr(string s){
    if( symbolTable.count(s) == 0 ){        
        // error
        return -1;
    }
    //no need to create relocation table for absolute symbol
    if(symbolTable[s].section=="ABS"){
        return symbolTable[s].offset;
    }
    //local symbol 
    if(!symbolTable[s].isGlobal){
        relocationTableEntry r = relocationTableEntry("ABS",LC+3,currentSection);
        r.symbolName = symbolTable[s].section;
        relocationTable[currentSection].push_back(r);
        return symbolTable[s].offset;

    }//global symbol
    if((symbolTable[s].section == "UNDEF") || symbolTable[s].isGlobal){
        relocationTableEntry r = relocationTableEntry("ABS",LC+3,currentSection);
        r.symbolName = symbolTable[s].label;
        relocationTable[currentSection].push_back(r);
        return 0;
    }
    return 0;
}

int Assembler::processPcRelSymAddr(string s){
    if(symbolTable.count(s)==0){
        return -1;
    }                
    relocationTableEntry r = relocationTableEntry("PC_REL",LC+3,currentSection);
    int retVal;
    if(symbolTable[s].section=="ABS"){ 
            r.symbolName = symbolTable[s].label;
            relocationTable[currentSection].push_back(r);
            return -2;
    }
    //global: -2
    if(symbolTable[s].isGlobal==true || symbolTable[s].section=="UNDEF"){
                r.symbolName = symbolTable[s].label;                
                relocationTable[currentSection].push_back(r);
                return -2;
    }
    //local: a - 2
    if(currentSection != symbolTable[s].section){
        r.symbolName = symbolTable[s].section;
        relocationTable[currentSection].push_back(r);
        return symbolTable[s].offset - 2; 
    } 
    //if both sections are same
    retVal = symbolTable[s].offset - 2 - (LC + 3);  
    return retVal;               
}


int Assembler::labelSecondPass(string s){    
    //nothing to do with label in second pass    
    return 0;
}


// ######################################   END OF SECOND PASS   #####################################
//helpers

int Assembler::stringToInt(string value){
    smatch sm;
    int offset=0;
    if(regex_match(value,sm,regexLitDec)){
        stringstream ss(value);        
        ss >> offset;
        cout<<"OFFSET_DEC"<<offset<<endl;
    }else if(regex_match(value,sm,regexLitHex)){
        stringstream ss;        
        ss << sm.str(1).substr(2);
        ss >> std::hex >> offset;
        cout<<"OFFSET_HEX"<<offset<<endl;
    }
    return offset;
} 


int Assembler::getBinaryCodeFromMnemonic(string s)
{
    int instrDesc;

    if(s=="halt") instrDesc = 0x00;
    else if(s=="int") instrDesc = 0x10;
    else if(s=="iret") instrDesc = 0x20;
    else if(s=="call") instrDesc = 0x30;
    else if(s=="ret") instrDesc = 0x40;
    else if(s=="jmp") instrDesc = 0x50;
    else if(s=="jeq") instrDesc = 0x51;
    else if(s=="jne") instrDesc = 0x52;
    else if(s=="jgt") instrDesc = 0x53;
    else if(s=="xchg") instrDesc = 0x60;
    else if(s=="add") instrDesc = 0x70;
    else if(s=="sub") instrDesc = 0x71;
    else if(s=="mul") instrDesc = 0x72;
    else if(s=="div") instrDesc = 0x73;
    else if(s=="cmp") instrDesc = 0x74;
    else if(s=="not") instrDesc = 0x80;
    else if(s=="and") instrDesc = 0x81;
    else if(s=="or") instrDesc = 0x82;
    else if(s=="xor") instrDesc = 0x83;
    else if(s=="test") instrDesc = 0x84;
    else if(s=="shl") instrDesc = 0x90;
    else if(s=="shr") instrDesc = 0x91;
    else if(s=="ldr") instrDesc = 0xA0;
    else if(s=="str") instrDesc = 0xB0;
    else if(s=="push") instrDesc = 0xB0;
    else if(s=="pop") instrDesc = 0xA0;

    return instrDesc;
}

//##############################   PRINT TABLES   ##############################

int Assembler::printSymbolTable(){
    ofstream myfile;
    myfile.open(outputFileName);
    if (myfile.is_open()==false){ 
        return -1; 
    }
    myfile << "PRINTING SYMBOL TABLE \n";
    myfile << "------------------------------------------------------------------------------"<<endl;
    myfile << "LABEL\t\t\t\tOFFSET\t\tSECTION\t\t\tGLOBAL?\t\tSIZE\n";
    for(const auto &sym : symbolTable){
        myfile << setw(12) << setfill(' ') << sym.second.label << "\t";
        myfile  << setw(12) << setfill('0') <<hex<< sym.second.offset << "\t";
        myfile << setw(12) << setfill(' ') << sym.second.section << "\t";
        if(sym.second.isGlobal==true)
            myfile << "TRUE" << "\t\t\t\t";
        else
            myfile << "FALSE" << "\t\t\t\t";
        myfile << sym.second.size << "\t";

        myfile << "\n";
    }
    myfile << "------------------------------------------------------------------------------"<<endl;    
    myfile.close();
    return 0;
}

int Assembler::printRellocationTable(){    
    ofstream myfile(outputFileName,ios::app);
    if (myfile.is_open()==false){ 
        return -1; 
    }
    myfile<<"\n\n";
    myfile << "PRINTING RELLOCATION TABLE \n";
    myfile << "----------------------------------------------------"<<endl;
    myfile << left;
    myfile <<setw(7)<<"TYPE"<<setw(7)<<"OFFSET\t"<<setw(7)<<"SECTION_NAME\t\t"<<setw(7)<<"SYMBOL_NAME\n";
    myfile << "----------------------------------------------------"<<endl;
    for(const auto &r : relocationTable){
        for(auto rte : r.second){
            myfile << setw(7) << rte.type<<"\t";
            myfile << setw(7) << rte.offset<<"\t";
            myfile << setw(7) << rte.sectionName<<"\t\t";
            myfile << setw(7) << rte.symbolName<<"\t\t\n";            
        }
        myfile<<"\n";
    }
    myfile << "----------------------------------------------------"<<endl;
    myfile.close();
    return 0;
}

int Assembler::printCode(){
    ofstream myfile(outputFileName,ios::app);
    if (myfile.is_open()==false){ 
        return -1; 
    }
    myfile<<endl<<endl;
    myfile << "PRINTING CODE\n";
    myfile << "----------------"<<endl;
    map<string, vector<int>>::iterator itr;
    for(itr = code.begin();itr!=code.end(); ++itr){
        myfile<< itr->first<<endl;
        vector<int> v = itr->second;
        myfile << "---------"<<endl;
        for(int i=0;i<v.size();i++){
            myfile<< setfill ('0') << setw (2) << std::hex << v[i] <<" ";//<<endl;
            if((i+1)%16 ==0) myfile<<endl;
        }
        myfile<<endl<<endl;
    }
    myfile << "---------"<<endl;
    myfile.close();
    return 0;
}
