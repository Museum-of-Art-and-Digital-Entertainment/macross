
# line 10 "macross_6502.y"

#include "macrossTypes.h"
#include "macrossGlobals.h"

# define A 257
# define ALIGN 258
# define ASSERT 259
# define BLOCK 260
# define BYTE 261
# define CONSTRAIN 262
# define DBYTE 263
# define DEFINE 264
# define DO 265
# define ELSE 266
# define ELSEIF 267
# define ENDFILE 268
# define EOL 269
# define EXTERN 270
# define FRETURN 271
# define FUNCTION 272
# define HERE 273
# define IF 274
# define INCLUDE 275
# define LONG 276
# define MACRO 277
# define MCASE 278
# define MDEFAULT 279
# define MDEFINE 280
# define MDO 281
# define MELSE 282
# define MELSEIF 283
# define MFOR 284
# define MIF 285
# define MSWITCH 286
# define MUNTIL 287
# define MVARIABLE 288
# define MWHILE 289
# define ORG 290
# define REL 291
# define START 292
# define STRING 293
# define STRUCT 294
# define TARGET 295
# define UNDEFINE 296
# define UNTIL 297
# define VARIABLE 298
# define WHILE 299
# define WORD 300
# define X 301
# define Y 302
# define ConditionCode 303
# define Identifier 304
# define MacroName 305
# define Number 306
# define Opcode 307
# define TextString 308
# define ASSIGN 309
# define LOGICAL_OR 310
# define LOGICAL_XOR 311
# define LOGICAL_AND 312
# define BITWISE_OR 313
# define BITWISE_XOR 314
# define BITWISE_AND 315
# define EQUAL_TO 316
# define NOT_EQUAL_TO 317
# define LESS_THAN 318
# define LESS_THAN_OR_EQUAL_TO 319
# define GREATER_THAN 320
# define GREATER_THAN_OR_EQUAL_TO 321
# define LEFT_SHIFT 322
# define RIGHT_SHIFT 323
# define ADD 324
# define SUB 325
# define MUL 326
# define DIV 327
# define MOD 328
# define UNARY_MINUS 329
# define LOGICAL_NOT 330
# define BITWISE_NOT 331
# define HI_BYTE 332
# define LO_BYTE 333
# define SELECT 334
# define INCREMENT 335
# define DECREMENT 336
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#ifndef YYSTYPE
#define YYSTYPE int
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256
short yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
-1, 48,
	40, 242,
	-2, 200,
-1, 158,
	40, 242,
	-2, 200,
-1, 323,
	318, 0,
	319, 0,
	320, 0,
	321, 0,
	-2, 222,
-1, 324,
	318, 0,
	319, 0,
	320, 0,
	321, 0,
	-2, 223,
-1, 325,
	318, 0,
	319, 0,
	320, 0,
	321, 0,
	-2, 224,
-1, 326,
	318, 0,
	319, 0,
	320, 0,
	321, 0,
	-2, 225,
-1, 327,
	316, 0,
	317, 0,
	-2, 226,
-1, 328,
	316, 0,
	317, 0,
	-2, 227,
	};
# define YYNPROD 266
# define YYLAST 1367
short yyact[]={

 108, 207, 418, 526, 417, 357, 262, 415, 381, 293,
 292, 261, 356,  46,  41,  21, 201,  33,  12, 413,
  20, 183, 303,  19,  18, 343, 142, 245, 202,  17,
 398, 247,  16,  15, 143, 244,   9, 138,   5, 123,
 122, 119, 120, 121, 351, 556, 384, 184, 185, 138,
 555, 111, 112, 113, 114, 115, 116, 117, 118, 119,
 120, 121, 467, 160, 161, 162, 163, 138, 464, 347,
 168, 169, 170, 171, 172, 173, 174, 175, 176, 177,
 178, 346, 179, 180, 181, 182, 349, 352, 353, 304,
 216, 419, 186, 187, 188, 189, 190, 191, 192, 193,
 124, 125, 123, 122, 119, 120, 121, 513, 476, 457,
 402, 215, 138, 354, 335, 286, 209, 210, 211, 212,
 217, 218, 219, 220, 221, 222, 223, 224, 225, 226,
 227, 228, 229, 230, 231, 232, 233, 234, 235, 236,
 205, 110, 199, 126, 128, 127, 129, 124, 125, 123,
 122, 119, 120, 121, 242, 243, 260, 256, 254, 138,
  11, 252, 250,  99,  99, 257, 258, 237, 240, 433,
 106, 431, 213, 358, 359, 239, 419, 264, 388, 432,
 387, 430, 110, 276, 278, 248, 412, 391, 372, 559,
 139, 204, 525, 409, 103, 104, 289, 290, 291, 300,
 294, 295, 259, 296, 297,   3, 298, 299,  99,  85,
 437, 554, 553, 542, 538, 301, 132, 130, 131, 126,
 128, 127, 129, 124, 125, 123, 122, 119, 120, 121,
 314, 311, 313, 241, 536, 138, 502, 312, 501, 336,
 310, 309, 337, 492, 491, 342, 308, 344, 345, 307,
 306, 159,   4, 305, 471, 196, 195, 107, 200, 164,
 300, 470, 350, 360, 361, 362, 363, 364, 365, 366,
 355, 215, 573, 246, 246, 367, 368, 552, 246, 544,
 370, 130, 131, 126, 128, 127, 129, 124, 125, 123,
 122, 119, 120, 121, 532, 378, 379, 531, 530, 138,
 522, 389, 517, 496, 348, 493, 441, 144, 562, 382,
  61, 263,  99, 203, 475,  49, 275,  61, 274, 314,
 311, 313, 268, 266, 197,  70, 312, 165, 151, 310,
 309, 392, 371, 150, 149, 308, 383, 148, 307, 306,
 147, 558, 528, 529, 401, 528, 529, 405, 406, 407,
 408, 410, 411, 414, 421, 422, 423, 146, 208, 424,
 425,  58, 110, 394, 396, 145,  10, 399,  58, 110,
 419, 395, 397, 420, 442, 443, 105, 444, 270, 376,
 102,  86, 448, 101, 100, 450, 194, 141, 451, 452,
 140, 434, 449,  91, 166, 393, 447, 446, 429, 455,
 527, 456, 285, 287, 458, 459, 314, 311, 313, 462,
 199,  61, 465, 312, 468, 454, 310, 309, 152, 152,
 206, 472, 308, 524, 110, 307, 306, 167, 480, 305,
 436, 481, 482, 483, 484, 485, 267, 479, 487, 488,
 489, 490, 478, 255, 253, 416, 474, 288,  91, 251,
 249, 302, 214, 386,  47,  45,  44, 495, 494, 497,
  43,  42,  58,  40, 504, 505, 106, 506, 507,  39,
  38, 509, 510,  37, 390, 512,  36,  35,  34, 265,
 498, 499, 377, 514,  32, 516,  31, 523, 518, 519,
 520, 521, 515,  30,  29,  28,  27,  26, 533, 152,
  25, 534,  61,  24, 535,  23,  22, 537,  14, 539,
  13,   7, 541,   6, 540, 154, 109,   2,   1,   0,
  91, 375,   0, 545,   0, 547,   0,   0, 548, 549,
   0,   0, 550, 269,   0, 271, 272, 273,   0,   0,
   0, 277,   0,   0, 153, 279, 280, 281, 282, 283,
 284, 560,   0,  58,   0,   0, 564, 565,   0, 566,
   0,   0,   0, 567,   0,   0, 568, 569,   0, 571,
   0,   0, 572,   0,   0,   0, 316, 317, 318, 319,
 320, 321, 322, 323, 324, 325, 326, 327, 328, 329,
 330, 331, 332, 333, 334,   0,   0,   0,   0,   0,
   0, 338, 341,  99, 380,   0, 385, 136, 137, 135,
 133, 134, 132, 130, 131, 126, 128, 127, 129, 124,
 125, 123, 122, 119, 120, 121,  89,  78,   0,  55,
  54, 138,  53,   0,   0,   0,   0, 369,   0,   0,
   0,   0,   0,   0,   0,  83,   0,   0, 373, 374,
 152,   0,   0,  91, 339, 340,  88, 158,   0,  87,
   0,  90,  56,  57,   0,   0,   0,   0, 426,  52,
 428,   0,   0, 315,   0,   0,   0,   0,  92,  61,
  95,  89,   0,  93,  94,  96, 445,   0,  97,  98,
   0,   0,   0,   0,   0, 152, 152,   0,   0,   0,
   0, 400,   0,   0, 403,   0,   0,   0,   0,   0,
   0,  88, 158,   0,  87,   0,  90,   0,   0,   0,
 427,   0, 469, 435,   0, 438, 439, 440,   0, 473,
  58,   0, 477,  92,  91,  95,  99, 155,  93,  94,
  96,   0,   0,  97,  98,   0,   0,   0, 453,   0,
   0,   0,   0,  89,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0, 460, 461,   0, 463,   0,   0,
 466,   0,   0,   0,   0,   0, 508,   0,   0,   0,
 511, 156, 157,  88, 158, 110,  87,   0,  90, 134,
 132, 130, 131, 126, 128, 127, 129, 124, 125, 123,
 122, 119, 120, 121,   0,  92,   0,  95,   0, 138,
  93,  94,  96,  91, 500,  97,  98,  99, 503,   0,
 543,   0,   0,   0,   0,   0, 136, 137, 135, 133,
 134, 132, 130, 131, 126, 128, 127, 129, 124, 125,
 123, 122, 119, 120, 121, 486,   0,   0,   0,   0,
 138,   0,   0, 557,   0,   0, 561,   0, 563,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   8,
   0,  78,  82,  55,  54,  81,  53,  62,  51,   0,
   0, 198, 546,  76,  68,  67,  89,  49,  75,  83,
  66, 551,   0,  63,  72,   0,  99,  73,  70,  74,
   0,  65,  71,  79,  80,  77,  56,  57,  84,  69,
   0,  64,  50,  52,   0,   0,  88,  48,  60,  87,
  59,  90, 154,   0,   0, 570,   0,  91,   0,   0,
   0, 238,   0,   0, 154,   0,   0,   0,  92,  91,
  95,   0,   0,  93,  94,  96,   0,   0,  97,  98,
   8, 153,  78,  82,  55,  54,  81,  53,  62,  51,
   0,   0,   0, 153,  76,  68,  67,  89,  49,  75,
  83,  66,   0,   0,  63,  72,   0,   0,  73,  70,
  74,   0,  65,  71,  79,  80,  77,  56,  57,  84,
  69,   0,  64,  50,  52,   0,   0,  88,  48,  60,
  87,  59,  90,   0,   0,   0,   0,   0,   0,   0,
  99,   0,   0,   0,   0,   0,   0,   0,   0,  92,
   0,  95,  99,   0,  93,  94,  96,   0,   0,  97,
  98,  78,  82,  55,  54,  81,  53,  62,  51,   0,
   0,   0,   0,  76,  68,  67,  89,  49,  75,  83,
  66,   0,   0,  63,  72,   0,   0,  73,  70,  74,
   0,  65,  71,  79,  80,  77,  56,  57,  84,  69,
  91,  64,  50,  52,   0,   0,  88,  48,  60,  87,
  59,  90,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0, 404,   0,   0,   0,  92,   0,
  95,   0,   0,  93,  94,  96,   0,   0,  97,  98,
   0, 136, 137, 135, 133, 134, 132, 130, 131, 126,
 128, 127, 129, 124, 125, 123, 122, 119, 120, 121,
   0,   0,   0,   0,   0, 138,   0,   0,   0,   0,
   0,   0,   0,   0, 155,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0, 155,   0,   0,   0,
  89,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,  89,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0, 156, 157,
  88, 158,   0,  87,   0,  90,   0,   0,   0,   0,
 156, 157,  88, 158,   0,  87,   0,  90,   0,   0,
   0,   0,  92,   0,  95,   0,   0,  93,  94,  96,
   0,   0,  97,  98,  92,   0,  95,   0,   0,  93,
  94,  96,   0,   0,  97,  98, 136, 137, 135, 133,
 134, 132, 130, 131, 126, 128, 127, 129, 124, 125,
 123, 122, 119, 120, 121,   0,   0,   0,   0,   0,
 138, 137, 135, 133, 134, 132, 130, 131, 126, 128,
 127, 129, 124, 125, 123, 122, 119, 120, 121,   0,
   0,   0,   0,   0, 138, 135, 133, 134, 132, 130,
 131, 126, 128, 127, 129, 124, 125, 123, 122, 119,
 120, 121,   0,  89,   0,   0,   0, 138, 133, 134,
 132, 130, 131, 126, 128, 127, 129, 124, 125, 123,
 122, 119, 120, 121,   0,   0,   0,   0,   0, 138,
   0,   0,   0,  88, 158,   0,  87,   0,  90,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,  92,   0,  95,   0,   0,
  93,  94,  96,   0,   0,  97,  98 };
short yypact[]={

 -63,-1000, 694,-1000, -75,-1000,-1000,-1000, -73, 773,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000, 199, -87,
 -87, -87, -87, -87, -87, -87, -87, -87, 926, 899,
 899,-1000, -87, -87, -87, -87,-1000,-1000, 408, -87,
 -87, -87, -87, -87, -87, -87, -87, -87, -87, -87,
-1000, -87, -87, -87, -87,-288,-1000,-1000,-1000,-1000,
-1000, -87, -87, -87, -87, -87, -87, -87, -87,-1000,
 165, 164, 284, 613,-1000,-1000,-1000, 200, 273, -78,
-1000, 273,  85, 408, 408, 408, 408, 408, -33, -87,
 -87, -87, -87, -87, -87, -87, -87, -87, -87, -87,
 -87, -87, -87, -87, -87, -87, -87, -87, -87,-1000,
 887, 189,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000, 926, -87, -87,-1000, -56, -60,-1000,-1000,
-142,-143,-146,-147, -87, -87,-1000, 926,-148, 271,
 271,  85, 283, 282, 408,-148, 408, 408, 408, 278,
 276, 408, 408, -87,-1000,-1000, 408, 408, 408, 408,
 408, 408,-189,-189, 694, -87, -87, -87,-1000,-1000,
-1000, -66, -87, -87,-1000, -87, -87, 216, 926, 216,
 216, 216, 216,-1000, -87,-1000, 369, 408, 408, 408,
 408, 408, 408, 408, 408, 408, 408, 408, 408, 408,
 408, 408, 408, 408, 408, 408,-190,-1000, -87,-1000,
-1000, -87, 353, 408, -87,-309, -87, -87,-309,-1000,
-228,-1000,-240,-1000,  -5,-1000, -47,-217,-191, 226,
-1000,-109, -87, -87, -87, -87, -87, -87, -87, 926,
 226, 926, 926, 926, -87, -87, 216, 926, 408, 516,
-297,-297,-297,-297,-297,-1000,-1000,-1000,  63, 408,
 408, 480,-1000,-1000, -87, -87,  85,   6,  85,-119,
 -87,-193,  62,-1000,-1000, 369,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000, 199,-297,-297,-297,-267,
-267,-285,-285,-222,-222,-222,-222,-175,-175, -35,
 475, -99, 995, 950, 973,-1000, 899, 899, 926, -61,
-1000, 926, 408, -87,-194,1030, -87, -87, -87, 100,
 -87,  93,-128,-128, -87, -87,-1000,-1000, -87, -87,
  85, 408,  85,-118, 408,  87, 408, 408, 408, 926,
 265, 694,-1000, 516, 516,-1000, -87,-1000,  41, 273,
-1000, -87,-1000,   6, -87,-1000,-1000, -87, -87, 408,
-1000,-1000, 369,-1000,-1000,-1000,-1000,-1000, -87,-309,
 516,-195,-1000, 516, -87, 408, 408, 408, 408,-241,
 408, 408,-247, -87,  85,-1000, 217,-1000,-1000, 163,
 -87,  85, 274,-196,  40, 271,-1000, 516,-1000,-1000,
 -87, -87, -87, -87, -87, 801,-1000, -87, 516, 516,
 516,-1000, 151, 150, 264,-1000,-1000, -66, 262,-1000,
   6, 273, 273, 926,-1000, 408, 145,-1000, 143, 408,
 926, 926, 216, 516, -87, 216, 516, -87,  85,-1000,
 -87, -87,  85,-1000, -87,  66,-1000,-1000,-1000,-109,
 261, 271, 271, 271, 271, 259, -87,  67, 257, 256,
 253,-1000,-1000,-1000,-1000,-1000,-1000, -87,-1000,-1000,
 516,-1000,-1000, 516, 141, 408, 121, 408,-1000,-213,
 120,-1000,  85,-1000, 238,-1000,-1000,-1000,-1000,-1000,
-1000,-1000, -87, 408, -87,-1000,-1000, -87, -87,-1000,
-1000, -87, 408, 236, 119, 118,-259, 216,-264, 216,
-1000,-1000,-1000,-1000,-1000,  85, 297,  64,  85, 268,
  85, 926,-1000,-1000,-1000, -87, -87,-1000, -87,-1000,
-1000,-1000, -87,-1000, 408, 408, 408, 408, 216, 216,
 926, 155, 231,-1000 };
short yypgo[]={

   0, 518, 517, 252,   0, 516,  38, 513, 511,  36,
 366, 160,  18, 510, 508,  33,  32,  29,  24,  23,
  20,  15, 506, 505, 503, 500, 497, 496, 495, 494,
 493, 486, 484,  17, 478, 477, 476, 473, 470, 469,
 463,  14, 461, 460, 456, 455,  13, 454,  16,  10,
   9,  28, 307,   8, 453,   1, 358, 172, 452, 451,
  22,  89, 190, 450, 449, 259,  19,   7, 446, 202,
 445,   4,   2, 444, 443,  11,  12,   5,   6, 436,
 430, 423,   3, 400, 398, 394, 391, 390, 387,  26,
  34, 386,  27, 384, 383, 209, 381, 380, 379, 365,
 357, 340, 337, 334, 333, 328 };
short yyr1[]={

   0,   1,   1,   2,   4,   4,   5,   5,   3,   3,
   6,   6,   6,   6,   7,   8,   9,   9,  11,  11,
  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,
  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,
  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,
  10,  10,  10,  10,  10,  10,  12,  12,  12,  48,
  51,  53,  53,  53,  49,  50,  50,  50,  50,  13,
  14,  54,  54,  15,  46,  55,  55,  44,  45,  45,
  16,  17,  18,  19,  21,  21,  57,  58,  59,  59,
  60,  60,  61,  61,  61,  61,  61,  61,  61,  61,
  61,  20,  23,  23,  23,  23,  24,  25,  63,  63,
  64,  64,  29,  29,  29,  29,  65,  66,  30,  32,
  69,  69,  68,  68,  67,  67,  67,  70,  70,  71,
  72,  27,  73,  73,  73,  73,  73,  28,  74,  74,
  74,  74,  74,  26,  33,  33,  33,  75,  78,  76,
  77,  77,  77,  77,  37,  79,  80,  80,  81,  81,
  82,  83,  83,  34,  35,  84,  84,  84,  84,  31,
  31,  85,  36,  86,  38,  39,  40,  41,  42,  47,
  43,  22,  62,  62,  87,  87,  87,  87,  87,  88,
  88,  88,  88,  89,  52,  91,  92,  92,  93,  93,
  94,  95,  95,  56,  56,  56,  56,  56,  56,  56,
  56,  56,  56,  56,  56,  56,  56,  56,  56,  56,
  56,  56,  56,  56,  56,  56,  56,  56,  56,  56,
  56,  56,  56,  56,  56,  56,  56,  56,  56,  56,
  96,  96,  97,  98,  90,  90,  90,  90,  90,  90,
  90,  99, 100, 101, 102, 102, 102, 103, 103, 103,
 103, 103, 103, 104, 104, 105 };
short yyr2[]={

   0,   4,   1,   0,   1,   0,   1,   2,   1,   3,
   1,   1,   2,   1,   2,   1,   1,   2,   2,   3,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   3,   4,   4,   3,
   5,   1,   2,   5,   3,   3,   3,   4,   4,   5,
   5,   3,   3,   3,   3,   1,   4,   9,   8,   7,
   3,   3,   3,   3,   3,   5,   1,   3,   1,   3,
   1,   2,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   0,   1,   2,   1,   2,   1,   3,   1,   4,
   1,   4,   7,   6,   7,   6,   0,   1,   8,   3,
   1,   4,   2,   3,   1,   4,   1,   1,   4,   4,
   1,   3,   1,   4,   6,   9,   6,   3,   1,   4,
   6,   9,   6,   3,   3,   4,   4,   3,   5,   3,
   3,   3,   4,   4,   5,   5,   5,   3,   1,   3,
   3,   7,   1,   5,   5,   3,   3,   3,   3,   1,
   2,   1,   9,   7,   3,   3,   3,   3,   3,   3,
   1,   1,   1,   1,   1,   2,   4,   2,   4,   1,
   4,   2,   4,   1,   4,   0,   3,   4,   6,   6,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   5,
   3,   3,   3,   3,   3,   4,   4,   4,   4,   4,
   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
   4,   4,   4,   4,   4,   4,   2,   2,   3,   3,
   4,   6,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   3,   3,   1,   1,   1,   6,   6,   2,
   2,   3,   3,   8,   4,   8 };
short yychk[]={

-1000,  -1,  -2, 268,  -3,  -6,  -7,  -8, 256,  -9,
 -10, -11, -12, -13, -14, -15, -16, -17, -18, -19,
 -20, -21, -22, -23, -24, -25, -26, -27, -28, -29,
 -30, -31, -32, -33, -34, -35, -36, -37, -38, -39,
 -40, -41, -42, -43, -44, -45, -46, -47, 304, 274,
 299, 265, 300, 263, 261, 260, 293, 294, -56, 307,
 305, -52, 264, 280, 298, 288, 277, 272, 271, 296,
 285, 289, 281, 284, 286, 275, 270, 292, 258, 290,
 291, 262, 259, 276, 295, -95, -96, 306, 303, 273,
 308,  40, 325, 330, 331, 327, 332, 335, 336, 123,
 -93, -94, -97, 269, 268, -10, -11,  58,  -4,  -5,
 269,  -4,  -4,  -4,  -4,  -4,  -4,  -4,  -4, 326,
 327, 328, 325, 324, 322, 323, 318, 320, 319, 321,
 316, 317, 315, 313, 314, 312, 310, 311, 334, -62,
 -87, -88, -89, -90, -52, -99,-100,-101,-102,-103,
-104,-105, -56,  64,  35, 257, 301, 302, 304, -62,
  -4,  -4,  -4,  -4, -65, -65, -85, -56,  -4,  -4,
  -4,  -4,  -4,  -4,  -4,  -4,  -4,  -4,  -4,  -4,
  -4,  -4,  -4, 309, 335, 336,  -4,  -4,  -4,  -4,
  -4,  -4,  -4,  -4, -91,  91,  91,  40, 268,  -6,
  58, -48, -51,  40, 269, -51, -52, -55, -56, -55,
 -55, -55, -55, -57, -58, 304, 123,  -4,  -4,  -4,
  -4,  -4,  -4,  -4,  -4,  -4,  -4,  -4,  -4,  -4,
  -4,  -4,  -4,  -4,  -4,  -4,  -4, -89,  44, -90,
 -89,  44,  -4,  -4,  91, -92, 334,  91, -92, -63,
 304, -64, 304, -73, 304, -74, 304,  -4,  -4, -69,
 304, -75, -78,  40, -78, -52,  40, -79,  40, -56,
 -69, -56, -56, -56,  40,  40, -55, -56,  -4, -56,
 -56, -56, -56, -56, -56, -95, 304, -95,  -3,  -4,
  -4,  -4, -49, -50, 266, 267,  -4,  -4,  -4,  -4,
  44,  -4, -59, -60, -61,  -9, -15, -16, -17, -18,
 -19, -41, -20, -21, -46, 304, -56, -56, -56, -56,
 -56, -56, -56, -56, -56, -56, -56, -56, -56, -56,
 -56, -56, -56, -56, -56, 304,  -4,  -4, -56, 301,
 302, -56,  -4, 334,  -4,  -4, 309, 309, 309,  91,
 309,  91, 304, 305, 304,  44, -76, -77, 282, 283,
  -4,  -4,  -4,  -4,  -4,  -4,  -4,  -4,  -4, -56,
  -4, 269, 125, -56, -56,  41, -98, -62,  -4,  -4,
 -52, -53, 303, 330,  40, -52, -54, 299, 297,  -4,
 -57, 125, 269, -61, -89, -90, -89, -90,  91, -92,
 -56,  -4, 304, -56,  64,  -4,  -4,  -4,  -4,  93,
  -4,  -4,  93, -66,  -4, -67, -70, -71, -72, 304,
 -66,  -4,  -4,  -4,  -4,  -4, -52, -56, -52, -84,
 299, 289, 297, 287, -86, -56, -80, 123, -56, -56,
 -56,  41,  -4,  -4,  -4, -52, -12, -48,  -4, -53,
  -4,  -4,  -4, -56, -60,  -4,  -4, 304,  -4,  -4,
 -56, -56, -55, -56, 309, -55, -56, 309,  -4, -52,
  44,  91,  -4, -52, -68,  40, 304, -52, -33, -75,
  -4,  -4,  -4,  -4,  -4,  -4,  44,  -4,  -4,  -4,
  -4,  93,  93,  41, -49, -50,  41, -53, -51, -51,
 -56,  93,  93, -56,  -4,  -4,  -4,  -4, -52,  -4,
  -4, -52,  -4,  41, -67, -76, -77,  41, -78, -78,
 -78, -78,  41,  -4, -81, 125, -82, -83, 278, 279,
  41,  41,  41,  -4,  -4,  -4,  93, -55,  93, -55,
 -71, -72,  93, -52,  41,  -4, -56,  -4,  -4,  -4,
  -4, -56,  41,  93,  93, 309, 309, -52,  44, 125,
 -82, -52,  40, -52,  -4,  -4,  -4,  -4, -55, -55,
 -56, -55,  -4,  41 };
short yydef[]={

   3,  -2, 101,   2,   0,   8,  10,  11,  13, 101,
  15,  16,  20,  21,  22,  23,  24,  25,  26,  27,
  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,
  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
  48,  49,  50,  51,  52,  53,  54,  55,  -2,   5,
   5,   5,   5,   5,   5,   5,   5,   5, 181, 102,
 104, 106,   5,   5,   5,   5, 116, 116, 169,   5,
   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,
 180,   5,   5,   5,   5, 203, 204, 205, 206, 207,
 208,   5,   5,   5,   5,   5,   5,   5,   5, 195,
 201, 202,   0, 101,  12,  14,  17,  18,   0,   4,
   6,   0,   0,   0,   0,   0,   0,   0,   0,   5,
   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,
   5,   5,   5,   5,   5,   5,   5,   5,   5, 103,
 182, 183, 184, 189, 193, 244, 245, 246, 247, 248,
 249, 250, 251,   5,   5, 254, 255, 256,  -2, 105,
   0,   0,   0,   0,   5,   5, 170, 171,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   5, 236, 237,   0,   0,   0,   0,
   0,   0,   0,   0, 101,   5,   5,   5,   1,   9,
  19,  56,   5,   5,   7,   5,   5,  73,  75,  80,
  81,  82,  83,  84,   5,  86, 101,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0, 185,   5, 191,
 187,   5,   0,   0,   5, 259,   5,   5, 260, 107,
 108, 143, 110, 131, 132, 137, 138,   0,   0, 119,
 120, 144,   5,   5,   5,   5,   5,   5,   5, 174,
 175, 176, 177, 178,   5,   5,  74, 179,   0,   5,
 210, 211, 212, 213, 214, 238, 200, 239,   0,   0,
   0,   0,  57,  58,   5,   5,   0,   0,   0,   0,
   5,   0,   0,  88,  90, 101,  92,  93,  94,  95,
  96,  97,  98,  99, 100,   0, 215, 216, 217, 218,
 219, 220, 221,  -2,  -2,  -2,  -2,  -2,  -2, 228,
 229, 230, 231, 232, 233, 234,   0,   0, 252, 261,
 262, 253,   0,   5,   0,   0,   5,   5,   5,   5,
   5,   5,   5,   5,   5,   5, 145, 146,   5,   5,
   0,   0,   0,   0,   0,   0,   0,   0,   0, 235,
   0, 101, 194,   5,   5, 240,   5, 243,   0,   0,
  59,   5,  61,   0,   5,  69,  70,   5,   5,   0,
  85,  87, 101,  91, 186, 192, 188, 190,   5, 264,
   5,   0, 196,   5,   5,   0,   0,   0,   0,   0,
   0,   0,   0,   5,   0, 117, 124, 126, 127, 130,
   5,   0,   0,   0,   0,   0, 147,   5, 163, 164,
   5,   5,   5,   5,   5,   0, 154,   5,   5,   5,
   5, 209,   0,   0,   0,  64,  65,  66,   0,  62,
   0,   0,   0,  76,  89,   0,   0, 197,   0,   0,
 109, 111, 133,   5,   5, 139,   5,   5,   0, 113,
   5,   5,   0, 115,   5,   0, 121, 149, 150, 151,
   0,   0,   0,   0,   0,   0,   5,   0,   0,   0,
   0, 199, 198, 241,  67,  68,  60,   5,  71,  72,
   5, 257, 258,   5,   0,   0,   0,   0, 112,   0,
   0, 114,   0, 122,   0, 152, 153, 148, 165, 166,
 167, 168,   5,   0,   5, 157, 158,   5,   5, 162,
 155,   5,  79,   0,   0,   0, 134, 136, 140, 142,
 125, 128, 129, 118, 123,   0,   0,   0,   0,   0,
   0,  78,  63, 263, 265,   5,   5, 172,   5, 156,
 159, 160,   5,  77,   0,   0,   0,   0, 135, 141,
 173,   5,   0, 161 };
#ifndef lint
static	char yaccpar_sccsid[] = "@(#)yaccpar 1.1 83/07/20 SMI"; /* from UCB 4.1 83/02/11 */
#endif

#
# define YYFLAG -1000
# define YYERROR goto yyerrlab
# define YYACCEPT return(0)
# define YYABORT return(1)

/*	parser for yacc output	*/

#ifdef YYDEBUG
int yydebug = 0; /* 1 for debugging */
#endif
YYSTYPE yyv[YYMAXDEPTH]; /* where the values are stored */
int yychar = -1; /* current input token number */
int yynerrs = 0;  /* number of errors */
short yyerrflag = 0;  /* error recovery flag */

yyparse() {

	short yys[YYMAXDEPTH];
	short yyj, yym;
	register YYSTYPE *yypvt;
	register short yystate, *yyps, yyn;
	register YYSTYPE *yypv;
	register short *yyxi;

	yystate = 0;
	yychar = -1;
	yynerrs = 0;
	yyerrflag = 0;
	yyps= &yys[-1];
	yypv= &yyv[-1];

 yystack:    /* put a state and value onto the stack */

#ifdef YYDEBUG
	if( yydebug  ) printf( "state %d, char 0%o\n", yystate, yychar );
#endif
		if( ++yyps> &yys[YYMAXDEPTH] ) { yyerror( "yacc stack overflow" ); return(1); }
		*yyps = yystate;
		++yypv;
		*yypv = yyval;

 yynewstate:

	yyn = yypact[yystate];

	if( yyn<= YYFLAG ) goto yydefault; /* simple state */

	if( yychar<0 ) if( (yychar=yylex())<0 ) yychar=0;
	if( (yyn += yychar)<0 || yyn >= YYLAST ) goto yydefault;

	if( yychk[ yyn=yyact[ yyn ] ] == yychar ){ /* valid shift */
		yychar = -1;
		yyval = yylval;
		yystate = yyn;
		if( yyerrflag > 0 ) --yyerrflag;
		goto yystack;
		}

 yydefault:
	/* default state action */

	if( (yyn=yydef[yystate]) == -2 ) {
		if( yychar<0 ) if( (yychar=yylex())<0 ) yychar = 0;
		/* look through exception table */

		for( yyxi=yyexca; (*yyxi!= (-1)) || (yyxi[1]!=yystate) ; yyxi += 2 ) ; /* VOID */

		while( *(yyxi+=2) >= 0 ){
			if( *yyxi == yychar ) break;
			}
		if( (yyn = yyxi[1]) < 0 ) return(0);   /* accept */
		}

	if( yyn == 0 ){ /* error */
		/* error ... attempt to resume parsing */

		switch( yyerrflag ){

		case 0:   /* brand new error */

			yyerror( "syntax error" );
		yyerrlab:
			++yynerrs;

		case 1:
		case 2: /* incompletely recovered error ... try again */

			yyerrflag = 3;

			/* find a state where "error" is a legal shift action */

			while ( yyps >= yys ) {
			   yyn = yypact[*yyps] + YYERRCODE;
			   if( yyn>= 0 && yyn < YYLAST && yychk[yyact[yyn]] == YYERRCODE ){
			      yystate = yyact[yyn];  /* simulate a shift of "error" */
			      goto yystack;
			      }
			   yyn = yypact[*yyps];

			   /* the current yyps has no shift onn "error", pop stack */

#ifdef YYDEBUG
			   if( yydebug ) printf( "error recovery pops state %d, uncovers %d\n", *yyps, yyps[-1] );
#endif
			   --yyps;
			   --yypv;
			   }

			/* there is no state on the stack with an error shift ... abort */

	yyabort:
			return(1);


		case 3:  /* no shift yet; clobber input char */

#ifdef YYDEBUG
			if( yydebug ) printf( "error recovery discards char %d\n", yychar );
#endif

			if( yychar == 0 ) goto yyabort; /* don't discard EOF, quit */
			yychar = -1;
			goto yynewstate;   /* try again in the same state */

			}

		}

	/* reduction by production yyn */

#ifdef YYDEBUG
		if( yydebug ) printf("reduce %d\n",yyn);
#endif
		yyps -= yyr2[yyn];
		yypvt = yypv;
		yypv -= yyr2[yyn];
		yyval = yypv[1];
		yym=yyn;
			/* consult goto table to find next state */
		yyn = yyr1[yyn];
		yyj = yypgo[yyn] + *yyps + 1;
		if( yyj>=YYLAST || yychk[ yystate = yyact[yyj] ] != -yyn ) yystate = yyact[yypgo[yyn]];
		switch(yym){
			
case 1:
# line 64 "macross_6502.y"
{
	yyval = yypvt[-2];
} break;
case 2:
# line 68 "macross_6502.y"
{
	yyval = 0;
} break;
case 3:
# line 74 "macross_6502.y"
{
	statementListNestingDepth = 0;
	errorFlag = FALSE;
} break;
case 6:
# line 87 "macross_6502.y"
{
	if (amListing())
		saveEOLForListing();
} break;
case 7:
# line 92 "macross_6502.y"
{
	if (amListing())
		saveEOLForListing();
} break;
case 8:
# line 99 "macross_6502.y"
{
	if (statementListNestingDepth == 0) {
		yyval = eatStatement(yypvt[-0]);
	} else {
		yyval = buildStatementList(yypvt[-0], NULL);
	}
} break;
case 9:
# line 107 "macross_6502.y"
{
	if (statementListNestingDepth == 0) {
		yyval = eatStatement(yypvt[-0]);
	} else {
		yyval = buildStatementList(yypvt[-0], yypvt[-2]);
	}
} break;
case 12:
# line 119 "macross_6502.y"
{
	yyval = NULL;
	YYACCEPT;
} break;
case 13:
# line 124 "macross_6502.y"
{
	yyerrok;
	yyclearin;
	resynchronizeInput();
	yyval = NULL;
} break;
case 14:
# line 134 "macross_6502.y"
{
	yyval = addLabelToStatement(yypvt[-1], yypvt[-0]);
} break;
case 16:
# line 144 "macross_6502.y"
{
	yyval = buildLabelList(yypvt[-0], NULL);
} break;
case 17:
# line 148 "macross_6502.y"
{
	yyval = buildLabelList(yypvt[-0], yypvt[-1]);
} break;
case 18:
# line 154 "macross_6502.y"
{
	yyval = lookupOrEnterSymbol(yypvt[-1], LABEL_SYMBOL);
} break;
case 19:
# line 158 "macross_6502.y"
{
	yyval = lookupOrEnterSymbol(yypvt[-2], LABEL_SYMBOL);
	addAttributeToSymbol(yyval, TENTATIVE_GLOBAL_ATT);
} break;
case 56:
# line 204 "macross_6502.y"
{
	yyval = buildIfStatement(yypvt[-0], NULL, NO_CONTINUATION);
} break;
case 57:
# line 208 "macross_6502.y"
{
	yyval = buildIfStatement(yypvt[-1], yypvt[-0], ELSE_CONTINUATION);
} break;
case 58:
# line 212 "macross_6502.y"
{
	yyval = buildIfStatement(yypvt[-1], yypvt[-0], ELSEIF_CONTINUATION);
} break;
case 59:
# line 218 "macross_6502.y"
{
	yyval = buildIfHead(yypvt[-2], yypvt[-0]);
} break;
case 60:
# line 224 "macross_6502.y"
{
	yyval = yypvt[-2];
} break;
case 62:
# line 232 "macross_6502.y"
{
	yyval = invertConditionCode(yypvt[-0]);
} break;
case 63:
# line 236 "macross_6502.y"
{
	yyval = yypvt[-2];
} break;
case 64:
# line 242 "macross_6502.y"
{
	yyval = yypvt[-0];
} break;
case 65:
# line 248 "macross_6502.y"
{
	yyval = extractIfBody(yypvt[-0]);
} break;
case 66:
# line 252 "macross_6502.y"
{
	yyval = extractIfBody(buildIfStatement(yypvt[-0], NULL, NO_CONTINUATION));
} break;
case 67:
# line 256 "macross_6502.y"
{
	yyval = extractIfBody(buildIfStatement(yypvt[-1], yypvt[-0], ELSE_CONTINUATION));
} break;
case 68:
# line 260 "macross_6502.y"
{
	yyval = extractIfBody(buildIfStatement(yypvt[-1], yypvt[-0], ELSEIF_CONTINUATION));
} break;
case 69:
# line 266 "macross_6502.y"
{
	yyval = buildWhileStatement(yypvt[-2], yypvt[-0]);
} break;
case 70:
# line 272 "macross_6502.y"
{
	yyval = buildDoStatement(yypvt[-2], yypvt[-0]);
} break;
case 71:
# line 278 "macross_6502.y"
{
	yyval = buildDoEnd(yypvt[-0], WHILE_END);
} break;
case 72:
# line 282 "macross_6502.y"
{
	yyval = buildDoEnd(yypvt[-0], UNTIL_END);
} break;
case 73:
# line 288 "macross_6502.y"
{
	yyval = buildWordStatement(yypvt[-0]);
} break;
case 74:
# line 294 "macross_6502.y"
{
	yyval = buildLongStatement(yypvt[-0]);
} break;
case 75:
# line 300 "macross_6502.y"
{
	yyval = buildExpressionList(yypvt[-0], NULL);
} break;
case 76:
# line 304 "macross_6502.y"
{
	yyval = buildExpressionList(yypvt[-0], yypvt[-3]);
} break;
case 77:
# line 311 "macross_6502.y"
{
	yyval = buildConstrainStatement(yypvt[-4], yypvt[-0]);
} break;
case 78:
# line 318 "macross_6502.y"
{
	yyval = buildAssertStatement(yypvt[-3], yypvt[-0]);
} break;
case 79:
# line 322 "macross_6502.y"
{
	yyval = buildAssertStatement(yypvt[-2], NULL);
} break;
case 80:
# line 328 "macross_6502.y"
{
	yyval = buildDbyteStatement(yypvt[-0]);
} break;
case 81:
# line 334 "macross_6502.y"
{
	yyval = buildByteStatement(yypvt[-0]);
} break;
case 82:
# line 340 "macross_6502.y"
{
	yyval = buildBlockStatement(yypvt[-0]);
} break;
case 83:
# line 347 "macross_6502.y"
{
	yyval = buildStringStatement(yypvt[-0]);
} break;
case 84:
# line 354 "macross_6502.y"
{
	yyval = buildStructStatement(yypvt[-0], NULL);
} break;
case 85:
# line 358 "macross_6502.y"
{
	yyval = buildStructStatement(yypvt[-0], yypvt[-2]);
} break;
case 86:
# line 364 "macross_6502.y"
{
	yyval = lookupOrEnterSymbol(yypvt[-0], STRUCT_NAME_SYMBOL);
} break;
case 87:
# line 370 "macross_6502.y"
{
	yyval = yypvt[-1];
} break;
case 88:
# line 377 "macross_6502.y"
{
	yyval = buildStatementList(yypvt[-0], NULL);
} break;
case 89:
# line 381 "macross_6502.y"
{
	yyval = buildStatementList(yypvt[-0], yypvt[-2]);
} break;
case 91:
# line 389 "macross_6502.y"
{
	yyval = addLabelToStatement(yypvt[-1], yypvt[-0]);
} break;
case 101:
# line 407 "macross_6502.y"
{
	yyval = buildNullStatement();
} break;
case 102:
# line 414 "macross_6502.y"
{
	yyval = buildInstructionStatement(yypvt[-0], NULL);
} break;
case 103:
# line 418 "macross_6502.y"
{
	yyval = buildInstructionStatement(yypvt[-1], yypvt[-0]);
} break;
case 104:
# line 422 "macross_6502.y"
{
	yyval = buildMacroInstructionStatement(yypvt[-0], NULL);
} break;
case 105:
# line 426 "macross_6502.y"
{
	yyval = buildMacroInstructionStatement(yypvt[-1], yypvt[-0]);
} break;
case 106:
# line 432 "macross_6502.y"
{
	yyval = buildGroupStatement(yypvt[-0]);
} break;
case 107:
# line 438 "macross_6502.y"
{
	yyval = yypvt[-0];
} break;
case 108:
# line 445 "macross_6502.y"
{
	yyval = buildDefineStatement(yypvt[-0], UNASSIGNED);
} break;
case 109:
# line 449 "macross_6502.y"
{
	checkDefineAssignmentOperator(yypvt[-2]);
	yyval = buildDefineStatement(yypvt[-3], yypvt[-0]);
} break;
case 110:
# line 457 "macross_6502.y"
{
	yyval = buildMdefineStatement(yypvt[-0], UNASSIGNED);
} break;
case 111:
# line 461 "macross_6502.y"
{
	checkDefineAssignmentOperator(yypvt[-2]);
	yyval = buildMdefineStatement(yypvt[-3], yypvt[-0]);
} break;
case 112:
# line 469 "macross_6502.y"
{
	yyval = buildMacroStatement(createMacro(yypvt[-3]), yypvt[-2], yypvt[-0]);
	popMacroOrFunctionNestingDepth();
} break;
case 113:
# line 474 "macross_6502.y"
{
	yyval = buildMacroStatement(createMacro(yypvt[-2]), NULL, yypvt[-0]);
	popMacroOrFunctionNestingDepth();
} break;
case 114:
# line 479 "macross_6502.y"
{
	yyval = buildMacroStatement(yypvt[-3], yypvt[-2], yypvt[-0]);
	popMacroOrFunctionNestingDepth();
} break;
case 115:
# line 484 "macross_6502.y"
{
	yyval = buildMacroStatement(yypvt[-2], NULL, yypvt[-0]);
	popMacroOrFunctionNestingDepth();
} break;
case 116:
# line 491 "macross_6502.y"
{
	pushMacroOrFunctionNestingDepth();
} break;
case 118:
# line 499 "macross_6502.y"
{
	yyval = buildFunctionStatement(yypvt[-4], yypvt[-2], yypvt[-0]);
	popMacroOrFunctionNestingDepth();
} break;
case 119:
# line 507 "macross_6502.y"
{
	yyval = buildUndefineStatement(yypvt[-0]);
} break;
case 120:
# line 513 "macross_6502.y"
{
	yyval = buildIdentifierList(yypvt[-0], NULL, unknownSymbolTag);
} break;
case 121:
# line 517 "macross_6502.y"
{
	yyval = buildIdentifierList(yypvt[-0], yypvt[-3], unknownSymbolTag);
} break;
case 122:
# line 523 "macross_6502.y"
{
	yyval = NULL;
} break;
case 123:
# line 527 "macross_6502.y"
{
	yyval = yypvt[-1];
} break;
case 124:
# line 533 "macross_6502.y"
{
	yyval = yypvt[-0];
} break;
case 125:
# line 537 "macross_6502.y"
{
	yyval = buildArgumentList(yypvt[-0], yypvt[-3], TRUE);
} break;
case 126:
# line 541 "macross_6502.y"
{
	yyval = buildArgumentList(yypvt[-0], NULL, TRUE);
} break;
case 127:
# line 548 "macross_6502.y"
{
	yyval = buildArgumentList(yypvt[-0], NULL, FALSE);
} break;
case 128:
# line 552 "macross_6502.y"
{
	yyval = buildArgumentList(yypvt[-0], yypvt[-3], FALSE);
} break;
case 131:
# line 565 "macross_6502.y"
{
	yyval = yypvt[-0];
} break;
case 132:
# line 572 "macross_6502.y"
{
	yyval = buildVariableStatement(yypvt[-0], UNASSIGNED, NULL);
} break;
case 133:
# line 576 "macross_6502.y"
{
	checkDefineAssignmentOperator(yypvt[-2]);
	yyval = buildVariableStatement(yypvt[-3], yypvt[-0], NULL);
} break;
case 134:
# line 581 "macross_6502.y"
{
	yyval = buildVariableStatement(yypvt[-5], UNASSIGNED, yypvt[-2]);
} break;
case 135:
# line 585 "macross_6502.y"
{
	checkDefineAssignmentOperator(yypvt[-7]);
	yyval = buildVariableStatement(yypvt[-8], yypvt[-0], yypvt[-5]);
} break;
case 136:
# line 590 "macross_6502.y"
{
	checkDefineAssignmentOperator(yypvt[-4]);
	yyval = buildVariableStatement(yypvt[-5], yypvt[-0], -1);
} break;
case 137:
# line 598 "macross_6502.y"
{
	yyval = yypvt[-0];
} break;
case 138:
# line 605 "macross_6502.y"
{
	yyval = buildMvariableStatement(yypvt[-0], UNASSIGNED, NULL);
} break;
case 139:
# line 609 "macross_6502.y"
{
	checkDefineAssignmentOperator(yypvt[-2]);
	yyval = buildMvariableStatement(yypvt[-3], yypvt[-0], NULL);
} break;
case 140:
# line 614 "macross_6502.y"
{
	yyval = buildMvariableStatement(yypvt[-5], UNASSIGNED, yypvt[-2]);
} break;
case 141:
# line 618 "macross_6502.y"
{
	checkDefineAssignmentOperator(yypvt[-7]);
	yyval = buildMvariableStatement(yypvt[-8], yypvt[-0], yypvt[-5]);
} break;
case 142:
# line 623 "macross_6502.y"
{
	checkDefineAssignmentOperator(yypvt[-4]);
	yyval = buildMvariableStatement(yypvt[-5], yypvt[-0], -1);
} break;
case 143:
# line 631 "macross_6502.y"
{
	yyval = yypvt[-0];
} break;
case 144:
# line 638 "macross_6502.y"
{
	yyval = buildMifStatement(yypvt[-0], NULL, NO_CONTINUATION);
} break;
case 145:
# line 642 "macross_6502.y"
{
	yyval = buildMifStatement(yypvt[-1], yypvt[-0], ELSE_CONTINUATION);
} break;
case 146:
# line 646 "macross_6502.y"
{
	yyval = buildMifStatement(yypvt[-1], yypvt[-0], ELSEIF_CONTINUATION);
} break;
case 147:
# line 652 "macross_6502.y"
{
	yyval = buildMifHead(yypvt[-2], yypvt[-0]);
} break;
case 148:
# line 658 "macross_6502.y"
{
	yyval = yypvt[-2];
} break;
case 149:
# line 664 "macross_6502.y"
{
	yyval = yypvt[-0];
} break;
case 150:
# line 670 "macross_6502.y"
{
	yyval = extractMifBody(yypvt[-0]);
} break;
case 151:
# line 674 "macross_6502.y"
{
	yyval = extractMifBody(buildMifStatement(yypvt[-0], NULL, NO_CONTINUATION));
} break;
case 152:
# line 678 "macross_6502.y"
{
	yyval = extractMifBody(buildMifStatement(yypvt[-1], yypvt[-0], ELSE_CONTINUATION));
} break;
case 153:
# line 682 "macross_6502.y"
{
	yyval = extractMifBody(buildMifStatement(yypvt[-1], yypvt[-0], ELSEIF_CONTINUATION));
} break;
case 154:
# line 689 "macross_6502.y"
{
	yyval = buildMswitchStatement(yypvt[-2], yypvt[-0]);
} break;
case 155:
# line 695 "macross_6502.y"
{
	yyval = yypvt[-2];
} break;
case 156:
# line 701 "macross_6502.y"
{
	yyval = yypvt[-2];
} break;
case 157:
# line 706 "macross_6502.y"
{
	yyval = NULL;
} break;
case 158:
# line 712 "macross_6502.y"
{
	yyval = buildCaseList(yypvt[-0], NULL);
} break;
case 159:
# line 716 "macross_6502.y"
{
	yyval = buildCaseList(yypvt[-0], yypvt[-2]);
} break;
case 160:
# line 722 "macross_6502.y"
{
	yyval = buildCase(yypvt[-2], yypvt[-0]);
} break;
case 161:
# line 728 "macross_6502.y"
{
	yyval = yypvt[-2];
} break;
case 162:
# line 732 "macross_6502.y"
{
	yyval = NULL;
} break;
case 163:
# line 739 "macross_6502.y"
{
	yyval = buildMwhileStatement(yypvt[-2], yypvt[-0]);
} break;
case 164:
# line 746 "macross_6502.y"
{
	yyval = buildMdoStatement(yypvt[-2], yypvt[-0]);
} break;
case 165:
# line 752 "macross_6502.y"
{
	yyval = buildMdoEnd(yypvt[-0], WHILE_END);
} break;
case 166:
# line 756 "macross_6502.y"
{
	yyval = buildMdoEnd(yypvt[-0], WHILE_END);
} break;
case 167:
# line 760 "macross_6502.y"
{
	yyval = buildMdoEnd(yypvt[-0], UNTIL_END);
} break;
case 168:
# line 764 "macross_6502.y"
{
	yyval = buildMdoEnd(yypvt[-0], UNTIL_END);
} break;
case 169:
# line 771 "macross_6502.y"
{
	yyval = buildFreturnStatement(NULL);
} break;
case 170:
# line 775 "macross_6502.y"
{
	yyval = buildFreturnStatement(yypvt[-0]);
} break;
case 172:
# line 785 "macross_6502.y"
{
	yyval = buildMforStatement(yypvt[-4], yypvt[-0]);
} break;
case 173:
# line 792 "macross_6502.y"
{
	yyval = buildForExpressions(yypvt[-6], yypvt[-3], yypvt[-0]);
} break;
case 174:
# line 799 "macross_6502.y"
{
	yyval = buildIncludeStatement(yypvt[-0]);
} break;
case 175:
# line 806 "macross_6502.y"
{
	yyval = buildExternStatement(yypvt[-0]);
} break;
case 176:
# line 812 "macross_6502.y"
{
	yyval = buildStartStatement(yypvt[-0]);
} break;
case 177:
# line 818 "macross_6502.y"
{
	yyval = buildAlignStatement(yypvt[-0]);
} break;
case 178:
# line 824 "macross_6502.y"
{
	yyval = buildOrgStatement(yypvt[-0]);
} break;
case 179:
# line 831 "macross_6502.y"
{
	yyval = buildTargetStatement(yypvt[-0]);
} break;
case 180:
# line 837 "macross_6502.y"
{
	yyval = buildRelStatement();
} break;
case 181:
# line 844 "macross_6502.y"
{
	yyval = buildPerformStatement(yypvt[-0]);
} break;
case 184:
# line 855 "macross_6502.y"
{
	yyval = buildOperandList(yypvt[-0], NULL);
} break;
case 185:
# line 859 "macross_6502.y"
{
	yyval = buildOperandList(yypvt[-0], yypvt[-1]);
} break;
case 186:
# line 863 "macross_6502.y"
{
	yyval = buildOperandList(yypvt[-0], yypvt[-3]);
} break;
case 187:
# line 867 "macross_6502.y"
{
	yyval = buildOperandList(yypvt[-0], yypvt[-1]);
} break;
case 188:
# line 871 "macross_6502.y"
{
	yyval = buildOperandList(yypvt[-0], yypvt[-3]);
} break;
case 189:
# line 878 "macross_6502.y"
{
	yyval = buildOperandList(yypvt[-0], NULL);
} break;
case 190:
# line 882 "macross_6502.y"
{
	yyval = buildOperandList(yypvt[-0], yypvt[-3]);
} break;
case 191:
# line 886 "macross_6502.y"
{
	yyval = buildOperandList(yypvt[-0], yypvt[-1]);
} break;
case 192:
# line 890 "macross_6502.y"
{
	yyval = buildOperandList(yypvt[-0], yypvt[-3]);
} break;
case 193:
# line 896 "macross_6502.y"
{
	yyval = buildOperand(BLOCK_OPND, yypvt[-0]);
} break;
case 194:
# line 902 "macross_6502.y"
{
	yyval = yypvt[-1];
	statementListNestingDepth--;
} break;
case 195:
# line 909 "macross_6502.y"
{
	statementListNestingDepth++;
} break;
case 196:
# line 914 "macross_6502.y"
{
	yyval = buildSelectionList(yypvt[-0], NULL);
} break;
case 197:
# line 918 "macross_6502.y"
{
	yyval = buildSelectionList(yypvt[-0], yypvt[-3]);
} break;
case 198:
# line 924 "macross_6502.y"
{
	yyval = buildExpressionTerm(ARRAY_EXPR, yypvt[-5], yypvt[-2]);
} break;
case 199:
# line 928 "macross_6502.y"
{
	yyval = buildExpressionTerm(ARRAY_EXPR, yypvt[-5], yypvt[-2]);
} break;
case 200:
# line 934 "macross_6502.y"
{
	yyval = buildExpressionTerm(IDENTIFIER_EXPR, lookupOrEnterSymbol(yypvt[-0],
			unknownSymbolTag));
} break;
case 203:
# line 945 "macross_6502.y"
{
	yyval = yypvt[-0];
} break;
case 204:
# line 949 "macross_6502.y"
{
	yyval = buildExpressionTerm(FUNCTION_CALL_EXPR, yypvt[-0]);
} break;
case 205:
# line 953 "macross_6502.y"
{
	yyval = buildExpressionTerm(NUMBER_EXPR, yypvt[-0]);
} break;
case 206:
# line 957 "macross_6502.y"
{
	yyval = buildExpressionTerm(CONDITION_CODE_EXPR, yypvt[-0]);
} break;
case 207:
# line 961 "macross_6502.y"
{
	yyval = buildExpressionTerm(HERE_EXPR);
} break;
case 208:
# line 965 "macross_6502.y"
{
	yyval = buildExpressionTerm(STRING_EXPR, yypvt[-0]);
} break;
case 209:
# line 969 "macross_6502.y"
{
	yyval = buildExpressionTerm(SUBEXPRESSION_EXPR, yypvt[-2]);
} break;
case 210:
# line 973 "macross_6502.y"
{
	yyval = buildExpressionTerm(UNOP_EXPR, UNARY_MINUS, yypvt[-0]);
} break;
case 211:
# line 977 "macross_6502.y"
{
	yyval = buildExpressionTerm(UNOP_EXPR, LOGICAL_NOT, yypvt[-0]);
} break;
case 212:
# line 981 "macross_6502.y"
{
	yyval = buildExpressionTerm(UNOP_EXPR, BITWISE_NOT, yypvt[-0]);
} break;
case 213:
# line 985 "macross_6502.y"
{
	yyval = buildExpressionTerm(UNOP_EXPR, LO_BYTE, yypvt[-0]);
} break;
case 214:
# line 989 "macross_6502.y"
{
	yyval = buildExpressionTerm(UNOP_EXPR, HI_BYTE, yypvt[-0]);
} break;
case 215:
# line 993 "macross_6502.y"
{
	yyval = buildExpressionTerm(BINOP_EXPR, MUL, yypvt[-3], yypvt[-0]);
} break;
case 216:
# line 997 "macross_6502.y"
{
	yyval = buildExpressionTerm(BINOP_EXPR, DIV, yypvt[-3], yypvt[-0]);
} break;
case 217:
# line 1001 "macross_6502.y"
{
	yyval = buildExpressionTerm(BINOP_EXPR, MOD, yypvt[-3], yypvt[-0]);
} break;
case 218:
# line 1005 "macross_6502.y"
{
	yyval = buildExpressionTerm(BINOP_EXPR, SUB, yypvt[-3], yypvt[-0]);
} break;
case 219:
# line 1009 "macross_6502.y"
{
	yyval = buildExpressionTerm(BINOP_EXPR, ADD, yypvt[-3], yypvt[-0]);
} break;
case 220:
# line 1013 "macross_6502.y"
{
	yyval = buildExpressionTerm(BINOP_EXPR, LEFT_SHIFT, yypvt[-3], yypvt[-0]);
} break;
case 221:
# line 1017 "macross_6502.y"
{
	yyval = buildExpressionTerm(BINOP_EXPR, RIGHT_SHIFT, yypvt[-3], yypvt[-0]);
} break;
case 222:
# line 1021 "macross_6502.y"
{
	yyval = buildExpressionTerm(BINOP_EXPR, LESS_THAN, yypvt[-3], yypvt[-0]);
} break;
case 223:
# line 1025 "macross_6502.y"
{
	yyval = buildExpressionTerm(BINOP_EXPR, GREATER_THAN, yypvt[-3], yypvt[-0]);
} break;
case 224:
# line 1029 "macross_6502.y"
{
	yyval = buildExpressionTerm(BINOP_EXPR, LESS_THAN_OR_EQUAL_TO, yypvt[-3], yypvt[-0]);
} break;
case 225:
# line 1033 "macross_6502.y"
{
	yyval = buildExpressionTerm(BINOP_EXPR, GREATER_THAN_OR_EQUAL_TO, yypvt[-3],yypvt[-0]);
} break;
case 226:
# line 1037 "macross_6502.y"
{
	yyval = buildExpressionTerm(BINOP_EXPR, EQUAL_TO, yypvt[-3], yypvt[-0]);
} break;
case 227:
# line 1041 "macross_6502.y"
{
	yyval = buildExpressionTerm(BINOP_EXPR, NOT_EQUAL_TO, yypvt[-3], yypvt[-0]);
} break;
case 228:
# line 1045 "macross_6502.y"
{
	yyval = buildExpressionTerm(BINOP_EXPR, BITWISE_AND, yypvt[-3], yypvt[-0]);
} break;
case 229:
# line 1049 "macross_6502.y"
{
	yyval = buildExpressionTerm(BINOP_EXPR, BITWISE_OR, yypvt[-3], yypvt[-0]);
} break;
case 230:
# line 1053 "macross_6502.y"
{
	yyval = buildExpressionTerm(BINOP_EXPR, BITWISE_XOR, yypvt[-3], yypvt[-0]);
} break;
case 231:
# line 1057 "macross_6502.y"
{
	yyval = buildExpressionTerm(BINOP_EXPR, LOGICAL_AND, yypvt[-3], yypvt[-0]);
} break;
case 232:
# line 1061 "macross_6502.y"
{
	yyval = buildExpressionTerm(BINOP_EXPR, LOGICAL_OR, yypvt[-3], yypvt[-0]);
} break;
case 233:
# line 1065 "macross_6502.y"
{
	yyval = buildExpressionTerm(BINOP_EXPR, LOGICAL_XOR, yypvt[-3], yypvt[-0]);
} break;
case 234:
# line 1069 "macross_6502.y"
{
	yyval = buildExpressionTerm(BINOP_EXPR, SELECT, yypvt[-3],
				lookupOrEnterSymbol(yypvt[-0], STRUCT_FIELD_SYMBOL));
} break;
case 235:
# line 1074 "macross_6502.y"
{
	yyval = buildExpressionTerm(ASSIGN_EXPR, yypvt[-2], yypvt[-3], yypvt[-0]);
} break;
case 236:
# line 1078 "macross_6502.y"
{
	yyval = buildExpressionTerm(POSTOP_EXPR, INCREMENT, yypvt[-1]);
} break;
case 237:
# line 1082 "macross_6502.y"
{
	yyval = buildExpressionTerm(POSTOP_EXPR, DECREMENT, yypvt[-1]);
} break;
case 238:
# line 1086 "macross_6502.y"
{
	yyval = buildExpressionTerm(PREOP_EXPR, INCREMENT, yypvt[-0]);
} break;
case 239:
# line 1090 "macross_6502.y"
{
	yyval = buildExpressionTerm(PREOP_EXPR, DECREMENT, yypvt[-0]);
} break;
case 240:
# line 1097 "macross_6502.y"
{
	yyval = buildFunctionCall(yypvt[-3], NULL);
} break;
case 241:
# line 1101 "macross_6502.y"
{
	yyval = buildFunctionCall(yypvt[-5], yypvt[-2]);
} break;
case 251:
# line 1127 "macross_6502.y"
{
	yyval = buildOperand(EXPRESSION_OPND, yypvt[-0]);
} break;
case 252:
# line 1133 "macross_6502.y"
{
	yyval = buildOperand(INDIRECT_OPND, yypvt[-0]);
} break;
case 253:
# line 1139 "macross_6502.y"
{
	yyval = buildOperand(IMMEDIATE_OPND, yypvt[-0]);
} break;
case 254:
# line 1146 "macross_6502.y"
{
	yyval = buildOperand(A_REGISTER_OPND, NULL);
} break;
case 255:
# line 1150 "macross_6502.y"
{
	yyval = buildOperand(X_REGISTER_OPND, NULL);
} break;
case 256:
# line 1154 "macross_6502.y"
{
	yyval = buildOperand(Y_REGISTER_OPND, NULL);
} break;
case 257:
# line 1161 "macross_6502.y"
{
	yyval = buildOperand(X_INDEXED_OPND, yypvt[-2]);
} break;
case 258:
# line 1165 "macross_6502.y"
{
	yyval = buildOperand(Y_INDEXED_OPND, yypvt[-2]);
} break;
case 259:
# line 1169 "macross_6502.y"
{
	yyval = buildOperand(X_SELECTED_OPND, yypvt[-0]);
} break;
case 260:
# line 1173 "macross_6502.y"
{
	yyval = buildOperand(Y_SELECTED_OPND, yypvt[-0]);
} break;
case 261:
# line 1177 "macross_6502.y"
{
	yyval = buildOperand(X_INDEXED_OPND, NULL);
} break;
case 262:
# line 1181 "macross_6502.y"
{
	yyval = buildOperand(Y_INDEXED_OPND, NULL);
} break;
case 263:
# line 1188 "macross_6502.y"
{
	yyval = buildOperand(PRE_INDEXED_X_OPND, yypvt[-2]);
} break;
case 264:
# line 1192 "macross_6502.y"
{
	yyval = buildOperand(PRE_SELECTED_X_OPND, yypvt[-0]);
} break;
case 265:
# line 1199 "macross_6502.y"
{
	yyval = buildOperand(POST_INDEXED_Y_OPND, yypvt[-2]);
} break;
		}
		goto yystack;  /* stack new state and value */

	}
