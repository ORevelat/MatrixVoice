#pragma once

#include <matrix_hal/microphone_core.h>

matrix_hal::FIRCoeff FIR_bandpass[] = {
    {8000,
     {-16,  -22,   -16,   6,    -1,   -3,   27,   25,    -7,    0,    7,
      -38,  -45,   9,     2,    -16,  56,   80,   -8,    -10,   30,   -77,
      -135, 1,     25,    -50,  98,   214,  16,   -50,   75,    -115, -322,
      -56,  86,    -105,  122,  468,  129,  -137, 137,   -113,  -662, -256,
      207,  -168,  80,    934,  476,  -304, 196,  -6,    -1360, -891, 456,
      -219, -158,  2212,  1884, -788, 233,  710,  -5630, -7664, 3984, 12855,
      3984, -7664, -5630, 710,  233,  -788, 1884, 2212,  -158,  -219, 456,
      -891, -1360, -6,    196,  -304, 476,  934,  80,    -168,  207,  -256,
      -662, -113,  137,   -137, 129,  468,  122,  -105,  86,    -56,  -322,
      -115, 75,    -50,   16,   214,  98,   -50,  25,    1,     -135, -77,
      30,   -10,   -8,    80,   56,   -16,  2,    9,     -45,   -38,  7,
      0,    -7,    25,    27,   -3,   -1,   6}},
    {12000,
     {-1,   -16,  -1,    6,     0,     -7,    3,     28,    37,   17,   -8,
      -9,   9,    12,    -26,   -72,   -69,   -14,   26,    6,    -31,  -5,
      93,   160,  106,   -10,   -52,   17,    62,    -47,   -237, -285, -116,
      73,   63,   -82,   -74,   206,   488,   423,   61,    -168, -21,  192,
      -3,   -570, -890,  -536,  104,   273,   -136,  -331,  351,  1412, 1618,
      589,  -513, -355,  651,   469,   -2056, -5101, -5104, -564, 5674, 8584,
      5674, -564, -5104, -5101, -2056, 469,   651,   -355,  -513, 589,  1618,
      1412, 351,  -331,  -136,  273,   104,   -536,  -890,  -570, -3,   192,
      -21,  -168, 61,    423,   488,   206,   -74,   -82,   63,   73,   -116,
      -285, -237, -47,   62,    17,    -52,   -10,   106,   160,  93,   -5,
      -31,  6,    26,    -14,   -69,   -72,   -26,   12,    9,    -9,   -8,
      17,   37,   28,    3,     -7,    0,     6}},
    {16000,
     {5,    -3,   5,     5,     0,     -7,    -4,    9,     30,   42,   35,
      12,   -9,   -15,   -1,    17,    17,    -16,   -71,   -111, -103, -47,
      16,   42,   13,    -38,   -49,   19,    148,   253,   253,  138,  -13,
      -95,  -49,  65,    115,   -1,    -261,  -504,  -550,  -346, -23,  185,
      137,  -92,  -243,  -68,   438,   994,   1203,  869,   182,  -371, -386,
      112,  590,  346,   -928,  -2780, -4112, -3816, -1528, 1994, 5170, 6444,
      5170, 1994, -1528, -3816, -4112, -2780, -928,  346,   590,  112,  -386,
      -371, 182,  869,   1203,  994,   438,   -68,   -243,  -92,  137,  185,
      -23,  -346, -550,  -504,  -261,  -1,    115,   65,    -49,  -95,  -13,
      138,  253,  253,   148,   19,    -49,   -38,   13,    42,   16,   -47,
      -103, -111, -71,   -16,   17,    17,    -1,    -15,   -9,   12,   35,
      42,   30,   9,     -4,    -7,    0,     5}},
    {22050,
     {-26,  -23,   -26,   -22,   -13,   -3,    5,     9,     6,     0,    -9,
      -13,  -5,    13,    42,    69,    83,    78,    52,    13,    -19,  -36,
      -26,  3,     37,    50,    22,    -46,   -143,  -233,  -280,  -259, -172,
      -49,  59,    109,   81,    -7,    -105,  -144,  -70,   122,   389,  638,
      769,  716,   483,   149,   -154,  -305,  -237,  10,    299,   431,  232,
      -361, -1261, -2218, -2894, -2975, -2286, -876,  970,   2811,  4164, 4660,
      4164, 2811,  970,   -876,  -2286, -2975, -2894, -2218, -1261, -361, 232,
      431,  299,   10,    -237,  -305,  -154,  149,   483,   716,   769,  638,
      389,  122,   -70,   -144,  -105,  -7,    81,    109,   59,    -49,  -172,
      -259, -280,  -233,  -143,  -46,   22,    50,    37,    3,     -26,  -36,
      -19,  13,    52,    78,    83,    69,    42,    13,    -5,    -13,  -9,
      0,    6,     9,     5,     -3,    -13,   -22}},
    {24000,
     {6,    4,     6,     4,     0,     -5,    -8,    -5,    3,     18,   35,
      49,   55,    49,    31,    7,     -13,   -24,   -21,   -3,    19,   35,
      31,   0,     -59,   -130,  -192,  -222,  -206,  -145,  -56,   30,   84,
      84,   33,    -44,   -110,  -116,  -35,   129,   345,   548,   668,  655,
      502,  248,   -24,   -223,  -274,  -159,  69,    300,   390,   220,  -256,
      -986, -1807, -2485, -2780, -2516, -1646, -280,  1325,  2835,  3908, 4296,
      3908, 2835,  1325,  -280,  -1646, -2516, -2780, -2485, -1807, -986, -256,
      220,  390,   300,   69,    -159,  -274,  -223,  -24,   248,   502,  655,
      668,  548,   345,   129,   -35,   -116,  -110,  -44,   33,    84,   84,
      30,   -56,   -145,  -206,  -222,  -192,  -130,  -59,   0,     31,   35,
      19,   -3,    -21,   -24,   -13,   7,     31,    49,    55,    49,   35,
      18,   3,     -5,    -8,    -5,    0,     4}},
    {32000,
     {-5,    -6,    -5,    -3,    0,    4,    7,     8,     6,     1,     -11,
      -26,   -43,   -58,   -69,   -72,  -66,  -51,   -29,   -3,    18,    34,
      37,    28,    6,     -20,   -46,  -60,  -52,   -18,   42,    126,   221,
      309,   373,   398,   375,   304,  195,  67,    -53,   -143,  -180,  -155,
      -72,   46,    170,   255,   261,  154,  -76,   -425,  -854,  -1304, -1700,
      -1966, -2031, -1852, -1416, -750, 81,   987,   1855,  2573,  3047,  3213,
      3047,  2573,  1855,  987,   81,   -750, -1416, -1852, -2031, -1966, -1700,
      -1304, -854,  -425,  -76,   154,  261,  255,   170,   46,    -72,   -155,
      -180,  -143,  -53,   67,    195,  304,  375,   398,   373,   309,   221,
      126,   42,    -18,   -52,   -60,  -46,  -20,   6,     28,    37,    34,
      18,    -3,    -29,   -51,   -66,  -72,  -69,   -58,   -43,   -26,   -11,
      1,     6,     8,     7,     4,    0,    -3}},
    {44100,
     {0,     -3,    0,     3,     6,    7,    7,     6,     3,     0,     -5,
      -10,   -14,   -16,   -14,   -8,   1,    17,    39,    65,    92,    119,
      142,   159,   167,   163,   147,  120,  83,    40,    -3,    -44,   -76,
      -94,   -95,   -78,   -43,   3,    58,   110,   150,   166,   150,   93,
      -6,    -151,  -336,  -548,  -774, -994, -1187, -1331, -1407, -1399, -1296,
      -1095, -801,  -426,  8,     477,  951,  1395,  1780,  2077,  2265,  2329,
      2265,  2077,  1780,  1395,  951,  477,  8,     -426,  -801,  -1095, -1296,
      -1399, -1407, -1331, -1187, -994, -774, -548,  -336,  -151,  -6,    93,
      150,   166,   150,   110,   58,   3,    -43,   -78,   -95,   -94,   -76,
      -44,   -3,    40,    83,    120,  147,  163,   167,   159,   142,   119,
      92,    65,    39,    17,    1,    -8,   -14,   -16,   -14,   -10,   -5,
      0,     3,     6,     7,     7,    6,    3}},
    {48000,
     {4,     5,     4,     1,     1,     -2,   -5,    -8,    -10,   -10,
      -7,    -3,    3,     14,    28,    45,   63,    81,    97,    111,
      119,   122,   116,   103,   82,    55,   25,    -5,    -35,   -59,
      -75,   -79,   -70,   -49,   -16,   22,   65,    104,   132,   143,
      129,   84,    7,     -102,  -245,  -412, -594,  -781,  -957,  -1107,
      -1216, -1271, -1260, -1176, -1016, -784, -485,  -135,  248,   648,
      1039,  1399,  1705,  1938,  2084,  2134, 2084,  1938,  1705,  1399,
      1039,  648,   248,   -135,  -485,  -784, -1016, -1176, -1260, -1271,
      -1216, -1107, -957,  -781,  -594,  -412, -245,  -102,  7,     84,
      129,   143,   132,   104,   65,    22,   -16,   -49,   -70,   -79,
      -75,   -59,   -35,   -5,    25,    55,   82,    103,   116,   122,
      119,   111,   97,    81,    63,    45,   28,    14,    3,     -3,
      -7,    -10,   -10,   -8,    -5,    -2,   1,     1}},
    {96000,
     {-2,   -3,   -2,   0,    -1,   1,    3,    5,    8,    10,   13,   15,
      17,   19,   19,   19,   17,   14,   8,    1,    -8,   -22,  -38,  -58,
      -82,  -109, -139, -172, -207, -245, -284, -323, -362, -400, -436, -468,
      -496, -518, -534, -541, -540, -529, -509, -477, -435, -382, -319, -245,
      -162, -70,  27,   132,  242,  354,  466,  578,  687,  790,  887,  975,
      1052, 1117, 1170, 1208, 1231, 1239, 1231, 1208, 1170, 1117, 1052, 975,
      887,  790,  687,  578,  466,  354,  242,  132,  27,   -70,  -162, -245,
      -319, -382, -435, -477, -509, -529, -540, -541, -534, -518, -496, -468,
      -436, -400, -362, -323, -284, -245, -207, -172, -139, -109, -82,  -58,
      -38,  -22,  -8,   1,    8,    14,   17,   19,   19,   19,   17,   15,
      13,   10,   8,    5,    3,    1,    -1,   0}},
    {0, {0}}};
