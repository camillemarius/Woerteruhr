 #ifndef BERNDEUTSCH_DOEMU_H
 #define BERNDEUTSCH_DOEMU_H
 
 #define LANGUAGE_GERMAN
 /*      -                  0- 5
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
const  int PREFIX_IT_IS[7]         = {6,113, 94, 74, 73, 54, 53};      // ES IST
const  int INFIX_BEFORE[4]         = {3,31,16,11};                     // VOR
const  int INFIX_AFTER[3]          = {2,97,110};                       // NACH

const  int MINUTE_FIVE[4]          = {3,13,14,33};                     // FÜNF
const  int MINUTE_TEN[4]           = {3,112,95,92};                    // ZEHN
const  int MINUTE_QUARTER[7]       = {6,111,96,91,76,71,56};           // VIERTEL
const  int MINUTE_TWENTY[7]        = {6,55,52,35,32,15,12};            // ZWANZIG
const  int MINUTE_HALF[6]          = {5,10,17,30,37,50};               // HALB 

const  int HOUR_ONE[4]            = {3,106,101,86};                    // EINS 
const  int HOUR_TWO[5]            = {4,9,18,29,38};                    // ZWEI
const  int HOUR_THREE[4]          = {3,4,23,24};                       // DREI
const  int HOUR_FOUR[6]           = {5,59,48,39,28,19};                // VIER
const  int HOUR_FIVE[5]           = {4,107,100,87,80};                 // FÜNF
const  int HOUR_SIX[7]            = {6,86,81,66,61,46,41};             // SECHS
const  int HOUR_SEVEN[6]          = {5,6,21,26,41,46};                 // SIEBEN
const  int HOUR_EIGHT[6]          = {5,105,102,85,82,65};              // ACHT
const  int HOUR_NINE[5]           = {4,108,99,88,79};                  // NEUN
const  int HOUR_TEN[5]            = {4,104,103,84,83};                 // ZEHN
const  int HOUR_ELEVEN[5]         = {4,78,89,98,109};                  // ELF
const  int HOUR_TWELVE[7]         = {6,62,45,42,25,22,5};              // ZWÖLF

const  int preview_word[8]             = {7,7,20,27,40,47,60,67};      // preview_word
#endif