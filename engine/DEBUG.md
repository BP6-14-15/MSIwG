# Debugowanie 
## Konfiguracja gdb

1. w engine/example w pliku `CMakeLists.txt` w linii ~20 zaczynającej się od `SET(CMAKE_CXX_FLAGS` zamień końcówkę linii `... -nostdinc++"` na `... -nostdinc++ -g"`
2. zrób to samo ale w silniku (`engine/src`) około linii 44 
3. ponownie skompiluj silnik i swoją funkcję klienta
4. z konsoli zainstaluj narzędzie gdb: `sudo apt-get install gdb` 
5. ze skryptu uruchamiającego silnik `run.sh` skopiuj linię zaczynającą się od `export` i wpisz w aktywnym oknie konsoli 
6. ze skryptu uruchamiającego skopiuj swoje argumenty (całość tekstu po `./GameEngine`)
7. pozostając w folderze silnika, w konsoli odpal gdb: `gdb GameEngine`
8. następnie wpisz `run <twoje_argumenty>` (np. `run --fpLibPath=../example/libclientA.so --fpFcnName=updatePlayer --clientsPrint`
9. jeżeli wszystkie kroki zostały wykonane poprawnie to przy wystąpieniu błędu dostaniesz informację o dokładnym miejscu w kodzie, w którym ten błąd wystąpił


-----
## Zaawansowane 
### Breakpointy 
1. Przed uruchomieniem procesu (`run ...`) można wpisać np. `break client.cpp:95`
2. gdb za pierwszym razem może zadać pytanie typu `No source file named client.cpp. Make breakpoint pending on future shared library? (y or [n])`, należy na nie odpowiedź `y` 
3. dostaniesz informację zwrotną np. `Breakpoint 1 (client.cpp:95) pending`, gdzie `1` to jego numer
4. wtedy można już uruchomić proces (patrz wyżej pkt. 8)
5. w miejscu dojścia do breakpointu wykonywany proces się zatrzyma, wtedy można użyć innych komend do np sprawdzenia wartości różnych zmiennych itd (patrz niżej - `Więcej komend`)
6. aby wznowić uruchamianie programu można np. wpisać  `continue` 
7. jeżeli breakpoint nie jest już potrzebny to można go usunąć poprzez `delete <numer_breakpointa>` np `delete 1`
### Więcej komend 
[gdb cheat sheet](http://darkdust.net/files/GDB%20Cheat%20Sheet.pdf)
