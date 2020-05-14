# Write-Up gruppo 1

La challenge inizia con uno zip con all'interno un file di testo e un eseguibile.

## Start

Osserviamo lo start fare cose (LOL). Alla fine ci indirizza ad un [sito web](https://joshuachp.github.io/CTF/CCIT_challenge/).

## Sito

Accediamo al sito e troviamo che il campo username è vulnerabile a SQLI ber il
bypass dell'autenticazione.

```sql
admin' or '1'='1 --
```

Questo ci permette di scaricare un file `disk`.

### Hint 1

Nel source del sito possiamo trovare un commento che indica a SQL Injection.

## Disk

Utilizzando il comando `file` su disk possiamo vedere:

```bash
disk: Linux rev 1.0 ext2 filesystem data,...
```

Essendo un file-system procediamo quindi a creare una cartella dove eseguire il mount

```bash
mkdir mnt
sudo mount disk mnt
```

Nella cartella `mnt` troviamo i seguenti file:

- **next_stage.txt** : un testo che come all'inizio ci indica la strada da
  proseguire, contiene un indizio.
- **helpful.png** : Una immagine che ci servirà in seguito ma visto che siamo in
  gruppi di 4 qualcuno può lavorarci in quanto non servono tutte le persone su
  un singolo aspetto della challenge.

### Hint 2

Analizzando il file `next_stage.txt` notiamo una stringa in base64 e proseguiamo
ad analizzarla notando che è stato applicato un ROT13 otteniamo quindi il
messaggio originale.

```bash
echo "RnIgZnJ2IG95YnBwbmdiIHZhIGdoZ2d2IGRocmZndiBPVkEgY2ViaW4gbiBzbmVndiBoYW4gSk5ZWCA6KQo=" | base64 -d | tr ‘n-za-mN-ZA-M’ ‘a-zA-Z’
# Oppure se si ha bsd_games
echo "RnIgZnJ2IG95YnBwbmdiIHZhIGdoZ2d2IGRocmZndiBPVkEgY2ViaW4gbiBzbmVndiBoYW4gSk5ZWCA6KQo=" | base64 -d | rot13
```

Il suggerimento ci dice che un file è stato eliminato e ci consiglia di
utilizzare `binwalk`.

### Forensics

Il file system mantiene in scritti su disco i dati di un file anche dopo
l'eliminazione. Per estrarre i dati ci sono vari modi: testdisk, ext-undelete,
scalpel... Noi mostreremo un metodo utilizzando binwalk.

Come suggerito proviamo a utilizzare binwalk su disk per trovare tutti i dati
contenuti, nelle nuove versioni è necessario utilizzare il tag `-b`. Una volta
visto l'esistenza di un file non presente alla montatura procediamo con
l'estrazione e l'analisi dei files.

```bash
binwalk -e -b disk
file \*
```

Notiamo la presenza di un archivio tar e proseguiamo ad estrarlo

```bash
tar -xvf 7400
```

Il tar contiene i seguenti files:

- **final_stage.txt** : testo
- **crackme** : eseguibile

### Crackme

#### Hint 3

Nell'eseguibile viene eseguito un controllo tra la stringa "stuck" e una
variabile di nome `hint`. Se andiamo a controllare il valore della variabile,
ci consiglia di trovare dinamicamente la funzione chiamata dal main o di
cambiare il tipo della variabile `table` in un array di funzioni per capire
quale di queste chiami.

#### Reverse Engineering

Analizziamo crack me troviamo che dal main viene chiamato una funzione da un
array di funzioni nominato `table`. La funzione chiamata è la funzione `j` che
al suo interno chiama una funzione di controllo (`fourth_check`) sul valore che
abbaiamo inserito come argomento del binary.

Il `fourth_check` divide il valore che abbiamo inserito in 4 parti che utilizza
in un sistema lineare a quattro incognite. La funzione `j` controllerà poi il
risultati di questo sistema. Conoscendo tutti questi valori possiamo quindi
impostare un sistema a impostando come incognite le quattro parti in cui il
nostro valore di ingresso è stato diviso, possiamo poi risolvere questo sistema
in python.

```python
import numpy as np
import math

"""
  uint32_t a = n & 0xFF;
  uint32_t b = (n >> 8) & 0xFF;
  uint32_t c = (n >> 16) & 0xFF;
  uint32_t d = (n >> 24) & 0xFF;

  uint32_t v1 = 'J' * a + 'o' * b + 'j' * c + 'o' * d;
  uint32_t v2 = 'D' * a + 'I' * b + 'O' * c + '!' * d;
  uint32_t v3 = 'Z' * a + 'A' * b + 'W' * c + 'A' * d;
  uint32_t v4 = 'R' * a + 'U' * b + 'D' * c + 'O' * d;

  uint32_t vals[4] = {61319, 38111, 48787, 48939};
"""


def char_to_str(chrs):
    return [ord(c) for c in chrs]


incognite = np.array(
    [char_to_str("Jojo"), char_to_str("DIO!"), char_to_str("ZAWA"), char_to_str("RUDO")]
)
valori = np.array([61319, 38111, 48787, 48939])
risultato = np.linalg.solve(incognite, valori)

print("Values are: {}".format(risultato))


values = [198, 96, 146, 185]
values.reverse()

binary = ["{0:08b}".format(b) for b in values]
print("Value: {}".format(int("".join(binary), 2)))
```

Passando questo valore al crackme otteniamo un Hash.

```bash
./crackme 3113377990
# 0051377D1EE813EBFB59991D9165D88EC8C28C51AB37268A01DE386D00AE873AE34051D74CEEF2E457AB2D06BF283BEE87D0A209242B004DC27892DA6DB1AE1C
```

### Stenography

Torniamo a considerare un attimo l'immagine `helpful.png`

```bash
strings helpful.png
```

Dato il suggerimento proviamo a vedere se abbiamo qualcosa nei [bit meno
significativi](https://www.boiteaklou.fr/Steganography-Least-Significant-Bit.html).

```python
import base64
from PIL import Image

image = Image.open("./helpful.png")

extracted = ''

pixels = image.load()
# Iterate over pixels of the first row
for x in range(0,image.width):
    r,g,b = pixels[x,0]
    # Store LSB of each color channel of each pixel
    extracted += bin(r)[-1]
    extracted += bin(g)[-1]
    extracted += bin(b)[-1]

chars = []
for i in range(len(extracted)//8):
    byte = extracted[i*8:(i+1)*8]
    chars.append(chr(int(''.join([str(bit) for bit in byte]), 2)))

# Don't forget that the message was base64-encoded
flag = ''.join(chars)
print(flag)
```

### Cracking

Dal sito con un attimo di fantasia capiamo di utilizzare #cat -> hashcat e
tramite la canzone di supportarci con rockyou.txt

```bash
hashcat -a 0 -m 1700 hash rockyou.txt
# Mr.Pickles
```

### Conclusione

Eseguiamo il file python per ottenere la flag

`FLAG{i_7urn3d_mys31f_i1n70_4_pick13}`

