#ifndef SEISLERDEUTSCH_H
#define SEISLERDEUTSCH_H

#define LANGUAGE_GERMAN
/*      -                   0- 5
        FÜÜF AB             5-10
        ZÄÄ AB             10-15
        VIERTU AB          15-20
        ZWENZG AB          20-25
        FÜÜF VOR HAUBI     25-30
        HAUBI              30-35
        FÜÜF AB HAUBI      35-40
        ZWENZG VOR         40-45
        VIERTU VOR         45-50
        ZÄÄ VOR            50-55
        FÜÜF VOR           55-60
*/
const int PREFIX_IT_IS[] = {113, 94, 74, 73, 54, 53, -1}; // ES IST
const int INFIX_BEFORE[] = {31, 16, 11, -1};              // VOR
const int INFIX_AFTER[] = {97, 110, -1};                  // NACH

const int MINUTE_FIVE[] = {112, 95, 92, 75, -1};            // FÜNF
const int MINUTE_TEN[] = {33, 14, 13, -1};                  // ZEHN
const int MINUTE_QUARTER[] = {111, 96, 91, 76, 71, 56, -1}; // VIERTEL
const int MINUTE_TWENTY[] = {55, 52, 35, 32, 15, 12, -1};   // ZWANZIG
const int MINUTE_HALF[] = {10, 17, 30, 37, 50, -1};         // HALB

const int HOUR_ONE[] = {6, 21, 26, -1};                    // EINS
const int HOUR_TWO[] = {109, 98, 89, 78, -1};              // ZWEI
const int HOUR_THREE[] = {4, 23, 24, 43, -1};              // DREI
const int HOUR_FOUR[] = {48, 39, 28, 19, 8, -1};           // VIER
const int HOUR_FIVE[] = {107, 100, 87, 80, 67, -1};        // FÜNF
const int HOUR_SIX[] = {60, 47, 40, 27, 20, 7, -1};        // SECHS
const int HOUR_SEVEN[] = {105, 102, 85, 82, 65, -1};       // SIEBEN
const int HOUR_EIGHT[] = {106, 101, 86, 81, 66, -1};       // ACHT
const int HOUR_NINE[] = {108, 99, 88, 79, 68, -1};         // NEUN
const int HOUR_TEN[] = {104, 103, 84, 83, 64, 63, 44, -1}; // ZEHN
const int HOUR_ELEVEN[] = {38, 29, 18, 9, -1};             // ELF
const int HOUR_TWELVE[] = {62, 45, 42, 25, 22, 5, -1};     // ZWÖLF

const int preview_word[] = {61, 46, 41, 26, -1}; // preview_word
const int NO_INTERNET[] =  {61, 46, 41, 26, -1}; // no internet

#endif