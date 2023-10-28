g++ -pg -Wall main.cpp json.hpp simulator.hpp -o main2profile
main2profile config_file.json
gprof main2profile.exe > profile_without_iter
pause