# Simulacija bilijara
![screenshot](screenshots/bilijar-skrinsot2.png)
## Kako koristiti program
Kada su kugle zaustavljene:
Pomeranjem misa namestate stap
Tockicem misa birate jacinu udarca
Levi klik udara belu kuglu
Desni klik fiksira stap

U koliko se kugla ubaci u rupu vracanje na sto se vrsi pomocu misa. Kada postavite misa na zeljeno mesto pritisnite desni klik.

Precice na tastaturi:
 `Q` ili `ESC` izlazi iz programa
 `P` pauzira igru
 `R` rotira kugle ka igracu
 `S` zaustavlja kugle u trenutku
 `A` vraca kugle u pocetnu poziciju
 `T` ukljucuje/iskljucuje jednostavno crtanje
 `O` ukljucuje/iskljucuje osetljivost stapa za precizno bitanje jacine udarca

## Linux

```
pacman -S sfml
cd bilijar/
make run
```

## Windows

```
g++ -std=c++14 -O2 -o bilijar.exe bilijar\*.cpp -I sfml\include -L sfml\lib -lsfml-graphics -lsfml-window -lsfml-system
```
