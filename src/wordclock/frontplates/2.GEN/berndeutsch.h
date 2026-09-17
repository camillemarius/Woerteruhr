 #ifndef BERNDEUTSCH_H
 #define BERNDEUTSCH_H
 
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
const int PREFIX_IT_IS[]         = {103,104, 106, 107, 108, 109,-1}; // ES IST
const int INFIX_BEFORE[]         = {89,90,91,-1};                     // VOR
const int INFIX_AFTER[]          = {79,80,-1};                       // NACH

const int MINUTE_FIVE[]          = {111,112,113,-1};                  // FÜNF
const int MINUTE_TEN[]           = {100,101,102,-1};                  // ZEHN
const int MINUTE_QUARTER[]       = {81,82,83,84,85,86,-1};            // VIERTEL
const int MINUTE_TWENTY[]        = {92,93,94,95,96,97,-1};            // ZWANZIG
const int MINUTE_HALF[]          = {70,71,72,73,74,-1};               // HALB 

const int HOUR_ONE[]            = {34,35,36,-1};                    // EINS 
const int HOUR_TWO[]            = {63,64,65,66,-1};                    // ZWEI
const int HOUR_THREE[]          = {23,24,25,-1};                       // DREI
const int HOUR_FOUR[]           = {54,55,56,57,58,-1};                // VIER
const int HOUR_FIVE[]           = {37,38,39,40,-1};                 // FÜNF
const int HOUR_SIX[]            = {29,30,31,32,33,34,-1};             // SECHS
const int HOUR_SEVEN[]          = {26,27,28,29,30,-1};                 // SIEBEN
const int HOUR_EIGHT[]          = {10,11,12,13,14,-1};              // ACHT
const int HOUR_NINE[]           = {49,50,51,52,-1};                  // NEUN
const int HOUR_TEN[]            = {15,16,17,18,-1};                 // ZEHN
const int HOUR_ELEVEN[]         = {59,60,61,62,-1};                  // ELF
const int HOUR_TWELVE[]         = {4,5,6,7,8,9,-1};              // ZWÖLF

const int preview_word[]        = {19,20,21,22,-1};                   // preview_word
const int NO_INTERNET[]        =  {87, 88, 75, 76, 77, 78,-1};         // no internet
#endif