#ifndef REGEXES_H
#define REGEXES_H
#include <regex>
using namespace std;

string sym = "[a-zA-Z_][a-zA-Z_0-9]*";
string litDecPos = "[0-9]+";
string litDecNeg = "-[0-9]+";
string litDec = litDecPos + "|" + litDecNeg;
string litHex = "0(x|X)[0-9a-fA-F]{1,4}";
string lit = litDec + "|" + litHex;

regex regexSymbol("("+sym+")");
regex regexLitDec("("+litDec+")");
regex regexLitHex("("+litHex+")");
regex regexLit("("+lit+")");

//COMMENT FROM BEGINNING
regex regexComment("^ {0,}#.*$");

//DIRECTIVES
regex regexGlobal(" {0,}\\.global (("+sym+")(, "+sym+")*)" + " {0,}(#.*)*");
regex regexExtern(" {0,}\\.extern (("+sym+")(, "+sym+")*)"+ " {0,}(#.*)*");
regex regexSection(" {0,}\\.section ("+sym+") {0,}(#.*)*");
regex regexWord(" {0,}\\.word (("+sym+"|"+lit+")(, ("+sym+"|"+lit+"))*) {0,}(#.*)*");
regex regexSkip(" {0,}\\.skip ("+lit+") {0,}(#.*)*");
regex regexEqu(" {0,}\\.equ ("+sym+"), ("+lit+") {0,}(#.*)*");
regex regexEnd(" {0,}\\.end {0,}(#.*)*");

//LABELS
regex regexLabel("^ {0,}("+sym+"): {0,}(#.*)*$"); //regex for label only
regex regexLabelWithDirOrInst("^ {0,}("+sym+"):"+"(.*)$"); //regex for label with directive or instruction

//INSTRUCTIONS
string regX = "r0|r1|r2|r3|r4|r5|r6|r7|psw";

regex regexInstructionNoOperand("^ {0,}(halt|iret|ret) {0,}(#.*)*$");
regex regexInstructionOneReg("^ {0,}(int|push|pop|not) ("+regX+") {0,}(#.*)*$");
regex regexInstructionTwoReg("^ {0,}(add|sub|mul|div|xchg|cmp|and|or|xor|test|shl|shr) ("+regX+"), ("+regX+") {0,}(#.*)*$");

//---------------------JMP INSTRUCTIONS--------------------------------------------
regex regexInstructionJmpApsoluteAddr("^ {0,}(call|jmp|jeq|jne|jgt) ("+lit+"|"+sym+") {0,}(#.*)*$");
regex regexInstructionJmpMemDirAddr("^ {0,}(call|jmp|jeq|jne|jgt) \\*("+lit+"|"+sym+") {0,}(#.*)*$");
regex regexInstructionJmpRegDirAddr("^ {0,}(call|jmp|jeq|jne|jgt) \\*("+regX+") {0,}(#.*)*$");
regex regexInstructionJmpRegIndAddr("^ {0,}(call|jmp|jeq|jne|jgt) \\*\\[("+regX+")\\] {0,}(#.*)*$");
regex regexInstructionJmpRegIndDispAddr("^ {0,}(call|jmp|jeq|jne|jgt) \\*\\[("+regX+") \\+ ("+sym+"|"+lit+")\\] {0,}(#.*)*$");
regex regexInstructionJmpPcRel("^ {0,}(call|jmp|jeq|jne|jgt) %("+sym+") {0,}(#.*)*$");

//--------------------LD/STR INSTRUCTIONS------------------------------------------
regex regexInstructionLdrStrApsolute("^ {0,}(ldr|str) ("+regX+"), \\$("+lit+"|"+sym+") {0,}(#.*)*$");
regex regexInstructionLdrStrMemDir("^ {0,}(ldr|str) ("+regX+"), ("+lit+"|"+sym+") {0,}(#.*)*$");
regex regexInstructionLdrStrRegDir("^ {0,}(ldr|str) ("+regX+"), ("+regX+") {0,}(#.*)*$");
regex regexInstructionLdrStrRegInd("^ {0,}(ldr|str) ("+regX+"), \\[("+regX+")\\] {0,}(#.*)*$");
regex regexInstructionLdrStrRegIndDisp("^ {0,}(ldr|str) ("+regX+"), \\[("+regX+") \\+ ("+sym+"|"+lit+")\\] {0,}(#.*)*$");
regex regexInstructionLdrStrPc("^ {0,}(ldr|str) ("+regX+"), %("+sym+") {0,}(#.*)*$");

#endif