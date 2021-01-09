# TextComprimator
## This is school project of text comprimate.
Exam (CZ)
Napište program, který bude komprimovat a dekomprimovat textové soubory. Komprese by měla fungovat tak, že vezme posloupnost stejných znaků za sebou a nahradí ji dvojicí obsahující počet znaků a daný znak.

Program bude akceptovat tyto argumenty z příkazové řádky:

-i input - vstupní soubor pro (de)kompresi
-o output - výstupní soubor pro (de)kompresi
-b - použít binární mód (de)komprese
-d - provést dekompresi
Parametr -o vždy specifikuje výstupní soubor.

Pokud je zadán parametr -d, dojde k dekompresi, tj. -i značí vstupní komprimovaný soubor a -o výstupní dekomprimovaný soubor.
Pokud není parametr -d, dojde ke kompresi, tj. -i značí vstupní nekomprimovaný soubor a -o výstupní komprimovaný soubor.
Pokud je zadán parametr -b, (de)komprese bude probíhat v binárním módu, v opačném případě bude v textovém módu.

V textovém módu bude počet výskytů znaků uložen jako jedna číslice uložená jako ASCII znak, za ním bude ležet ASCII hodnota komprimovaného znaku. Pokud je počet výskytů větší než maximální možná hodnota pro jednu číslici (9), musíte počet výskytů rozdělit a uložit ho několikrát za sebou se stejným znakem.
V binárním módu bude počet výskytů znaků uložen jako bezznaménkové 1bytové číslo, za ním bude ležet 1 byte s ASCII hodnotou samotného znaku. Pokud je počet výskytů větší než maximální možná hodnota pro 1 byte (255), musíte počet výskytů rozdělit a uložit ho několikrát za sebou se stejným znakem.
Program po komprimování souboru vypíše statistiky, kolik bytů bylo ušetřeno nebo naopak vyplýtváno. Výstupem může být například HTML soubor, SVG graf atd. Na konkrétní podobě výstupu se domluvte se svým cvičícím.

Program bude schopen načítat soubory, které budou větší, než je operační paměť počítače (program tak musí umět zpracovávat soubor po částech, ne ho načíst celý do paměti).

Příklady vstupu a výstupu
Příklad 1:
./zipper -i input -o output
input: WWWWWWWWWWWWBWWWWWWWWWWWWBBBWWWWWWWWWWWWWWWWWWWWWWWWBWWWWWWWWWWWWWW
output: 9W3W1B9W3W3B9W9W6W1B9W5W
./zipper -o input -i output -d
output: 9W3W1B9W3W3B9W9W6W1B9W5W
input: WWWWWWWWWWWWBWWWWWWWWWWWWBBBWWWWWWWWWWWWWWWWWWWWWWWWBWWWWWWWWWWWWWW
Příklad 2:
./zipper -i a.txt -o b.zip -b
a.txt: AAAAAAAAAAAAAAAAABBC
b.zip: 17 65 2 66 1 67 (zobrazeny jsou zde jednotlivé byty oddělené mezerou)
./zipper -d -i b.zip -b -o a.txt
b.zip: 17 65 2 66 1 67 (zobrazeny jsou zde jednotlivé byty oddělené mezerou)
a.txt: AAAAAAAAAAAAAAAAABBC