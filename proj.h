#ifndef PROJ_H_
#define PROJ_H_

#define BACKGROUND 0

static int rtc_hook=4, mousehook=3, hook=2;

static char * penguin[] = {
"64 64 9",
"  14",
". 15",
"+ 8",
"@ 14",
"# 4",
"$ 6",
"% 7",
"& 9",
"* 1",
"                                                                ",
"                                                                ",
"                           +#++#&+#+                            ",
"                        +..%......&.+++                         ",
"                     #...+..+.+.+.#.+.+++                       ",
"                    .*#%..+.%.&.#*+.#%.++++                     ",
"                  #.+.#*#%..+.#*#.%#*#+.++++                    ",
"                 .&.%......#*#.%.+.#&.+%#%++++                  ",
"                #*#..........+.+.+.+.+.++#+++++                 ",
"               &#.+..........%.+.&#*#&#*.+&+++++                ",
"              +.+.............#*#.+.......#++++++               ",
"             #+.+..............+.+..........+++++               ",
"            +&.+%..............%+............+++++              ",
"            ++.+#.............................+++++             ",
"           ++.+%.............#..#..#..........+++++             ",
"          +++.++%..........#    .    #........++++++            ",
"          ++++.+...........#   .     #.....%..+++++++           ",
"          ++%+#+&..........#   .     #........+++++++           ",
"         +++#*+#..%.........#  .    #........%+++++++           ",
"         ++++%+++..%..%......#.#..#...%..%#..+++++++++          ",
"        +++++#+&+#.......%..................++++++++++          ",
"        ++++*++.+++.%.#...@@@@@@@@@@@@...%.+++++++++++          ",
"        +++++#+++%#$#.&.+.%@@@@@@@@@@...#++++++++++++++         ",
"        +++++&+.++++*.#.....@@@@@@@@......+++++++++&+++         ",
"       ++++++++++%#%....%....@@@@@@........+++++++++++%         ",
"       +++++++++++#...........@@@@@.........+++++++++++         ",
"       ++++++++++..............@@@...........+++++++++++        ",
"      +++++++++++...............@.............+++++%+++++       ",
"    ++++++++++++.....................%.........+++++++++++      ",
"   ++++++++++++%.........................%..%..%++++%++++++     ",
"  +++++++++++++...............................#.++++++++++++    ",
" +++++++++++++%..%.........................#.....++++++++++++   ",
"++++++++++++++........................%..%..%.+.%++%++++++++++  ",
"++++++++++++++...................................#++%++++++++++ ",
"    +++++++++...%..%.....................%.+.%#.%.++++++        ",
"       ++++++%....................%...........$.#.#+#%+         ",
"       +++++#........................%..#..+.#.%$%+&+++         ",
"        ++++&.#.%..%....................%...*#.#...+#++         ",
"        ++++..%...........................%..%.+.+.+&++         ",
"        +++++...%.....%.........%...%..+..#.#..#.%.+#+          ",
"        ++++..#...%......%...............&.*.+.*#$#&++          ",
"         +++..*#...%.........%.....#..%...#.%.#%.+..++          ",
"         +++.#...#..............%...%..#..&.#*#.+.+++           ",
"          ++.%.&...%..%..%..........#..*#..+..+.#%.++           ",
"          ++..#..+..........%...%..$..%..+..&#*.+.+++           ",
"           +*.+.....%............#..$.#...+.#.+.+%.+            ",
"           +...%.+.#..%.%..#.%.#..%.#.*#%..&.+.+.+++            ",
"            +.+.......#..%.....%...%.#..&#$#%.+.+.+             ",
"            #..%#*#%....#..+..$..+..+.&#.+.+.+%+#&              ",
"             +.....#*#$.+...%.#...+....+.%#*#.+.+               ",
"              +.+.&..%..%.+.#.&.+..&#+.+.+.+.++++               ",
"               %..+.#.+.+.+.*#..%#.$.+.&#*#&#%.+                ",
"                +.%#*#%#$.%#.+.+.+.+%#$#+%#*+++                 ",
"                 @#.#$#%++.+.%.+.&.#*#%+#$##$#                  ",
"          .@.@@@@@@@@@@$#.++.+.#.+.+%#$......@@@@@@@            ",
"        @@@@@.@.@@@@@@@@@$.+$#&+.+.+@@..@....@.@@@$@@$          ",
"       @@@@@@.@@.@$@$@$$@$@+%#$#&#+@@.@..@@.@.@@$@@$$$$+        ",
"      @@@@@@@@@@@@@$@$@$@$$$+%+.+%$$@@$@@.@.@$@$@$$@$$$$        ",
"       @@@@$@$@$@$$@$$$$$$$#++++++#$@$@$$$$@$$@$$$$$$$$+        ",
"        @$@@$@$@$@$$$$$$$$          $$$$@$$$$$$$$$$$$$+         ",
"          $$@$$$$$$$$$$$               $$$$$$$$$$$$+            ",
"                                                                ",
"                                                                ",
"                                                                "};


static char *spaceship[] = {
"40 100 2",
"  43",
". 0",
"........................................",
"........................................",
"........................................",
"........................................",
"........................................",
"........................................",
"........................................",
"........................................",
"........................................",
"........................................",
"........................................",
"........................................",
"........................................",
"........................................",
".................. .....................",
".................. .....................",
".................. .....................",
".................. .....................",
".................. .....................",
".................. .....................",
".................. .....................",
".................. .....................",
".................. .....................",
".................   ....................",
".................    ...................",
"................     ...................",
"................     ...................",
"................      ..................",
"...............       ..................",
"...............       ..................",
"...............       ..................",
"...............       ..................",
"...............        .................",
"..............         .................",
"..............         .................",
"..............         .................",
"..............         .................",
"..............          ................",
".............           ................",
".............           ................",
".............           ................",
".............           ................",
".............           ................",
".............           ................",
".............           ................",
"............             ...............",
"............             ...............",
"............             ...............",
"............             ...............",
"............             ...............",
"............             ...............",
"............             ...............",
"............             ...............",
"............             ...............",
"............             ...............",
"............             ...............",
"............             ...............",
"............             ...............",
"............             ...............",
"............             ...............",
"............             ...............",
"............             ...............",
"...........               ..............",
"...........               ..............",
"...........               ..............",
"...........               ..............",
"...........               ..............",
"...........               ..............",
"...........               ..............",
"...........               ..............",
"...........               ..............",
"...........               ..............",
"...........               ..............",
"...........               ..............",
".....   ...               ....   .......",
".....   ...               ...    .......",
".....   ...               ...    .......",
"....     ..               ...     ......",
"....     ..                ..     ......",
"....     .                 ..     ......",
"....     .                 ..     ......",
"....                              ......",
"....                              ......",
"....                              ......",
"...                                .....",
"...                                .....",
"...                                .....",
"...                                .....",
"...                                .....",
"....                              ......",
"....                              ......",
"....     ....            ...      ......",
"....     ....           .....     ......",
"..        ....         ....        .....",
"..         .....     ......        .....",
"..         ................        .....",
"..         ................        .....",
"..         ................        .....",
"........................................",
"........................................"};

static char * pilha[] = {
"108 55 3",
", 17",
"a 25",
"_ 0",
"____________________________________________________________________________________________________________",
"____________________________________________________________________________________________________________",
"____________________________________________________________________________________________________________",
"____________________________________________________________________________________________________________",
"______________,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,__",
"______________,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,__",
"______________,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,__",
"__________,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"__________,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"__________,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"__________,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"__________,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"__________,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"__________,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"__________,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"__________,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"__________,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"__________,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"______,,,,,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"______,,,,,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"______,,,,,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"______,,,,,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"______,,,,,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"______,,,,,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"______,,,,,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"______,,,,,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"______,,,,,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"______,,,,,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"______,,,,,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"______,,,,,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"______,,,,,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"______,,,,,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"______,,,,,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"______,,,,,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"______,,,,,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"__________,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"__________,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"__________,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"__________,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"__________,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"__________,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"__________,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"__________,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"__________,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"__________,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"__________,,,,,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"______________,,,aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,,,__",
"______________,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,__",
"______________,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,__",
"______________,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,__",
"____________________________________________________________________________________________________________",
"____________________________________________________________________________________________________________",
"____________________________________________________________________________________________________________",
"____________________________________________________________________________________________________________",
"____________________________________________________________________________________________________________"};

static char * pilhadentro[] = {
"15 30 1",
"  27",
"               ",
"               ",
"               ",
"               ",
"               ",
"               ",
"               ",
"               ",
"               ",
"               ",
"               ",
"               ",
"               ",
"               ",
"               ",
"               ",
"               ",
"               ",
"               ",
"               ",
"               ",
"               ",
"               ",
"               ",
"               ",
"               ",
"               ",
"               ",
"               ",
"               "};

#endif
