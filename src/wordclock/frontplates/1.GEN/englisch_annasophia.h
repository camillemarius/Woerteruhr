#ifndef ENGLISCH_ANNASOPHIA_H
#define ENGLISCH_ANNASOPHIA_H

#define LANGUAGE_ENGLISH
/*
    Frontplatte: 11 Spalten x 10 Zeilen

    LED-Verlauf:
    
    links oben                                  rechts oben
       113  94  93  74  73  54  53  34  33  14  13
       112  95  92  75  72  55  52  35  32  15  12
       111  96  91  76  71  56  51  36  31  16  11
       110  97  90  77  70  57  50  37  30  17  10
       109  98  89  78  69  58  49  38  29  18   9
       108  99  88  79  68  59  48  39  28  19   8
       107 100  87  80  67  60  47  40  27  20   7
       106 101  86  81  66  61  46  41  26  21   6
       105 102  85  82  65  62  45  42  25  22   5
       104 103  84  83  64  63  44  43  24  23   4
       
    rechts unten = LED 4
    links oben   = LED 113
*/


/*
    ZEITBEREICHE

    00-04  IT IS
    05-09  IT IS FIVE PAST
    10-14  IT IS TEN PAST
    15-19  IT IS QUARTER PAST
    20-24  IT IS TWENTY PAST
    25-29  IT IS FIVE TO
    30-34  IT IS HALF
    35-39  IT IS FIVE TO
    40-44  IT IS TWENTY TO
    45-49  IT IS QUARTER TO
    50-54  IT IS TEN TO
    55-59  IT IS FIVE TO
*/

static const int HEADER1[] = {113,94,74,73,-1};                     // IT IS
static const int HEADER2[] = {63,44,43,24,23,4,-1};                 // O'CLOCK


static const int  INFIX_BEFORE[] = {17,10,-1};                           // TO
static const int  INFIX_AFTER[] = {109,98,89,78,-1};                   // PAST 
static const int  MINUTE_FIVE[] = {51,36,31,16,-1};                    // FIVE
static const int  MINUTE_TEN[] = {57,50,37,-1};                        // TEN
static const int  MINUTE_QUARTER[] = {92,75,72,55,52,35,32,-1};        // QUARTER
static const int  MINUTE_TWENTY[] = {111,96,91,76,71,56,-1};           // TWENTY
static const int  MINUTE_TWENTY_FIVE[] = {111,96,91,76,71,56,51,36,31,16,-1};// TWENTY FIVE
static const int  MINUTE_HALF[] = {110,97,90,77,-1};                   // HALF

static const int  HOUR_ONE[] = {108,99,88,-1};                         // ONE
static const int  HOUR_TWO[] = {27,20,7,-1};                           // TWO
static const int  HOUR_THREE[] = {48,39,28,19,8,-1};                   // THREE
static const int  HOUR_FOUR[] = {107,100,87,80,-1};                   // FOUR
static const int  HOUR_FIVE[] = {67,60,47,40,-1};                     // FIVE
static const int  HOUR_SIX[] = {79,68,59,-1};                         // SIX
static const int  HOUR_SEVEN[] = {105,102,85,82,65,-1};               // SEVEN
static const int  HOUR_EIGHT[] = {106,101,86,81,66,-1};               // EIGHT
static const int  HOUR_NINE[] = {38,29,18,9,-1};                      // NINE
static const int  HOUR_TEN[] = {104,103,84,-1};                       // TEN
static const int  HOUR_ELEVEN[] = {61,46,41,26,21,6,-1};              // ELEVEN
static const int  HOUR_TWELVE[] = {62,45,42,25,22,5,-1};              // TWELVE

static const int  preview_word[] = {34,33,14,13,-1};                  // TIME
static const int  NO_INTERNET[] = {4,-1};                                // nicht vorhanden



#endif