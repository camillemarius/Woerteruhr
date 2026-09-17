 #ifndef BERNDEUTSCH_JAYA_H
 #define BERNDEUTSCH_JAYA_H

 #define LANGUAGE_GERMAN
 #define ADVERTISING_ANIMATION
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
static const int PREFIX_IT_IS[] = {113, 94, 74, 73, 54, 53, -1};      // ES IST
static const int INFIX_BEFORE[] = {31, 16, 11, -1};                   // VOR
static const int INFIX_AFTER[] = {97, 110, -1};                       // NACH

static const int MINUTE_FIVE[] = {13, 14, 33, -1};                    // FÜNF
static const int MINUTE_TEN[] = {112, 95, 92, -1};                    // ZEHN
static const int MINUTE_QUARTER[] = {111, 96, 91, 76, 71, 56, -1};    // VIERTEL
static const int MINUTE_TWENTY[] = {55, 52, 35, 32, 15, 12, -1};      // ZWANZIG
static const int MINUTE_HALF[] = {10, 17, 30, 37, 50, -1};            // HALB 

static const int HOUR_ONE[] = {106, 101, 86, -1};                    // EINS 
static const int HOUR_TWO[] = {9, 18, 29, 38, -1};                    // ZWEI
static const int HOUR_THREE[] = {4, 23, 24, -1};                      // DREI
static const int HOUR_FOUR[] = {59, 48, 39, 28, 19, -1};              // VIER
static const int HOUR_FIVE[] = {107, 100, 87, 80, -1};                // FÜNF
static const int HOUR_SIX[] = {86, 81, 66, 61, 46, 41, -1};           // SECHS
static const int HOUR_SEVEN[] = {6, 21, 26, 41, 46, -1};              // SIEBEN
static const int HOUR_EIGHT[] = {105, 102, 85, 82, 65, -1};           // ACHT
static const int HOUR_NINE[] = {108, 99, 88, 79, -1};                 // NEUN
static const int HOUR_TEN[] = {104, 103, 84, 83, -1};                 // ZEHN
static const int HOUR_ELEVEN[] = {78, 89, 98, 109, -1};               // ELF
static const int HOUR_TWELVE[] = {62, 45, 42, 25, 22, 5, -1};         // ZWÖLF

static const int preview_word[] = {40, 47, 60, 67, -1};               // preview_word
static const int ZAHN[] = {57, 70, 90, 77, -1};                       // ZAHN
static const int NO_INTERNET[] = {4,-1};                                // nicht vorhanden

#endif