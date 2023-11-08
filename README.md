# blockchain

Ši programa simuliuoja blokų grandinės veikimą. Ji įgyvendina paprastą bloko, transakcijų ir naudotojų modelį, leidžiantį stebėti, kaip vyksta blokų kasyba ir transakcijų vykdymas.

## v0.1 (naujausia)

### Funkcionalumas

1. Blokų heneravimas: programa automatiškai generuoja blokus su atsitiktiniais naudotojais ir transakcijomis. Kiekvienas blokas turi savo unikalų identifikatorių, laikotarpį, versiją, Merkle šaknį ir kita informaciją.
2. Transakcijų vykdymas: sugeneruotos transakcijos yra vykdomos kaskart iškasant naują bloką. Kiekviena transakcija turi unikalų identifikatorių, siuntėją, gavėją ir sumą.
3. Naudotojų balansų atnaujinimas: po kiekvieno bloko iškėlimo naudotojų balansai yra atnaujinami pagal transakcijas.
4. Blokų informacijos išvedimas: programa leidžia peržiūrėti iškastų blokų informaciją, tokią kaip ankstesnio bloko hash, laikotarpį, versiją, Merkle hash'ą ir t.t.
5. Naudotojų informacijos išvedimas: galima peržiūrėti visus naudotojus ir jų dabartinius balansus.

---

1. **Blokų Stebėjimas**: Iškastų blokų informaciją galima išgauti su printBlocks() funkcija.
    ```
    container.printBlocks();
    ```
2. **Naudotojų Stebėjimas**: Naudotojų informacija gali būti gauta iškviečiant printUsers() funkciją.
    ```
    block.printUsers();
    ```

### Pastabos

- Kadangi [mano](https://github.com/N0rmalus/hash-generator) sukurta hash funkcija buvo pakankamai lėta su difficulty_target trimis nuliais ir daugiau, naudojau kitą, neapsakomai efektyvę ir atsitiktinę, hash funkciją ([Lauritos](https://github.com/LauritaR/Blockchain2023))
