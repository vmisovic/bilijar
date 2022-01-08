# Simulacija bilijara
![screenshot](screenshots/bilijar-skrinsot2.png)
## Kako koristiti program
**Kada su kugle zaustavljene:**
 1. Pomeranjem misa namestate stap
 2. Tockicem misa birate jacinu udarca
 3. Desni klik fiksira stap
 4. Levi klik udara belu kuglu

**U koliko se kugla ubaci u rupu vracanje na sto se vrsi pomocu misa. Kada postavite misa na zeljeno mesto pritisnite desni klik.**

**Precice na tastaturi:**

 * `Q` ili `ESC` izlazi iz programa
 * `P` pauzira igru
 * `L` ukljucuje/iskljucuje pomocne linije
 * `R` rotira kugle ka igracu
 * `S` zaustavlja kugle u trenutku
 * `A` vraca kugle u pocetnu poziciju
 * `T` ukljucuje/iskljucuje jednostavno crtanje
 * `O` ukljucuje/iskljucuje osetljivost stapa za precizno bitanje jacine udarca

**Seme boja:**
 * `G` zelena
 * `B` plava

## Kako preuzeti program

Preporuceno je manuelno kompajlovati program na GNU/Linux-u, ali ne i na Windowsu.

Zbog toga, postoje vec kompajlovane verzije koje se mogu naci na sledecoj [stranici](https://github.com/vmisovic/bilijar/releases).

## Kompajlovanje

### Linux

##### SFML
Program koristi graficku biblioteku SFML v2.5.1+.
Za distrubucije bazirane na Arch Linux-u mozete je preuzeti komandom:
```
sudo pacman -S sfml
```
dok je za distrubucije bazirane na Debianu mozete preuzeti komandom
```
sudo apt install libsfml-dev
```
##### Kompajlovanje i pokretanje

Da bi ste kompajlovali program:
```
cd src
make run
```

### Windows
_Komajlujete na w*nd0wsu na sopstvenu odgovornost_

##### SFML
Na Windowsu mozete skinuti SFML biblioteku na sledecem linuku [v2.5.1](https://www.sfml-dev.org/files/SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit.zip).

##### Kompajlovanje
```
cd src
g++ -std=c++14 -O2 -o bilijar.exe *.cpp -I sfml\include -L sfml\lib -lsfml-graphics -lsfml-window -lsfml-system
```
