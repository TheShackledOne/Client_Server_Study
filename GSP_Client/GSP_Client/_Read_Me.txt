"x64 Native Tools Command Prompt for VS 2022"

>cl Chess.cpp Board.cpp Pawn.cpp /I ".\Src\include" /link /LIBPATH:".\Src\lib" sfml-graphics.lib sfml-window.lib sfml-system.lib sfml-audio.lib sfml-network.lib sfml-main.lib /OUT:ChessClient.exe