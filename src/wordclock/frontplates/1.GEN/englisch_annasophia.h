#ifndef ENGLISCH_ANNASOPHIA_H
#define ENGLISCH_ANNASOPHIA_H

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

const int PREFIX_IT_IS[] = {113,94,74,73,-1};                    // IT IS
const int INFIX_BEFORE[] = {109,98,89,78,-1};                   // PAST
const int INFIX_AFTER[] = {17,10,-1};                           // TO
const int MINUTE_FIVE[] = {51,36,31,16,-1};                    // FIVE
const int MINUTE_TEN[] = {57,50,37,-1};                        // TEN
const int MINUTE_QUARTER[] = {92,75,72,55,52,35,32,-1};        // QUARTER
const int MINUTE_TWENTY[] = {111,96,91,76,71,56,-1};           // TWENTY
const int MINUTE_HALF[] = {110,97,90,77,-1};                   // HALF

const int HOUR_ONE[] = {108,99,88,-1};                         // ONE
const int HOUR_TWO[] = {27,20,7,-1};                           // TWO
const int HOUR_THREE[] = {48,39,28,19,8,-1};                   // THREE
const int HOUR_FOUR[] = {107,100,87,80,-1};                   // FOUR
const int HOUR_FIVE[] = {67,60,47,40,-1};                     // FIVE
const int HOUR_SIX[] = {79,68,59,-1};                         // SIX
const int HOUR_SEVEN[] = {105,102,85,82,65,-1};               // SEVEN
const int HOUR_EIGHT[] = {106,101,86,81,66,-1};               // EIGHT
const int HOUR_NINE[] = {38,29,18,9,-1};                      // NINE
const int HOUR_TEN[] = {104,103,84,-1};                       // TEN
const int HOUR_ELEVEN[] = {61,46,41,26,21,6,-1};              // ELEVEN
const int HOUR_TWELVE[] = {62,45,42,25,22,5,-1};              // TWELVE

const int preview_word[] = {34,33,14,13,-1};                  // TIME
const int NO_INTERNET[] = {4,-1};                                // nicht vorhanden



#endif