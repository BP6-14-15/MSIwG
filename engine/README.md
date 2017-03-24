# Kompilacja

Zakładam, że będziemy pracować na Linuxie (Ubuntu ?), jednak różne wersje mają różne kompilatory, a projekt wymaga  standardu C++17.  
Ja korzystam z Clang, więc przygotowałem krótką instrukcję i skrypty do instalacji najnowszej wersji obsługującej ten standard, przygotowane i przetestowane były pod Ubuntu.  

Oczywiście możecie korzystać z innych narzędzi, nie wiem jednak jak wygląda wsparcie dla C++17 w GCC.   
Jeśli chcecie natomiast pracować na Windowsie to VS2017 chyba wspiera już C++17. 

## Krok po kroku

**System**: Ubuntu  
**Wersja**: 16.10 

* Na początek musimy dodać repozytoria, na końcu pliku `/etc/apt/sources.list` dodajemy: 

       deb http://apt.llvm.org/yakkety/ llvm-toolchain-yakkety-4.0 main  
       deb-src http://apt.llvm.org/yakkety/ llvm-toolchain-yakkety-4.0 main
       
W przypadku innych wersji, dystrybucji [tutaj](http://apt.llvm.org) można znaleźć odpowiednie pakiety. 

* W konsoli wpisujemy: 
`wget -O - http://apt.llvm.org/llvm-snapshot.gpg.key|sudo apt-key add -`

* Następnie  
`sudo apt-get update`

* Bibliotekę standardową należy skompilować oddzielnie gdyż obecnie nie ma pakietu dla wersji 4.0.  
Uruchamiamy więc skrypt `clangSetup.sh`, domyślnie źródło zostanie pobrane do `~/Downloads`, można jednak jako pierwszy argument skryptu podać inną ściężkę. 
Po wykonaniu skryptu, możemy usunąć folder `~/Downloads/LLVM/llvm`, trochę miejsca zajmuje, a potrzebujemy jedynie folderu `build`. 

* Następnie pozostaje nam już tylko kompilacja.  
Będąc w folderze `engine/example` w konsoli wpisujemy `CXX=clang++-4.0 cmake ./`.  
Jeśli jednak kompilując bibliotekę standardową podaliśmy własną ścieżkę to tutaj także należy to zrobić - wtedy kompilacji możemy dokonać wpisując `CXX=clang++-4.0 cmake ./ -DLIBCXXPATH=/moja/sciezka`. Taka uwaga, tutaj podając ścieżkę nie możemy użyć znaku `~`, zamiast tego możemy jednak wpisać `$HOME` np. `-DLIBCXXPATH=$HOME/Dev`

* Jeśli wszystko poprawnie się wykonało, to teraz wystarczy uruchomić z konsoli polecenie `make` i nasz program zostanie skompilowany do biblioteki dynamicznej. 
