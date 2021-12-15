# Simulacija interagovanja kugli
kugle imaju masu koja im se moze razlikovati ali su poluprecnici kugli isti, sudari su neelasticni
![image](https://user-images.githubusercontent.com/70685786/137217847-efeca658-4b1d-44a3-a662-5d940cca51aa.png)
![sketch-1631383043564](https://user-images.githubusercontent.com/70685786/133665943-e394c126-a5ca-4fea-a067-c93ed6304a16.png)
![kugla_teme_ivice](https://user-images.githubusercontent.com/70685786/134636621-f552fb91-865a-4570-a868-4cd222e08af2.png)

## LINUX
install sfml and g++

git clone repository:
```
git clone https://github.com/vmisovic/bilijar/
```
change directory:
```
cd bilijar/bilijar/
```
compile:
```
g++ -std=c++14 -g -O2 -Wall -o program.out *.cpp -I SFML-2.5.1\include -L SFML-2.5.1\lib -lsfml-graphics -lsfml-window -lsfml-system
```
run:
```
./program.out
```
