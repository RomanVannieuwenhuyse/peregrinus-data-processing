# Peregrinus draaiboek
## Voor de vlucht
### Rockblock account voor het versturen van berichten
Berichten versturen via het iridium sattelietennetwerk is helaas niet gratis en we moeten betalen voor elke zending. We doen dit op volgende wijze. Een creditcard is nodig voor de betaling.

1. Surf naar de [rockblock website](https://rockblock.rock7.com/). Als je nog geen account hebt, druk dan op **create account**, en volg de stappen van de website om een account aan te maken. 
2. Klik in het linkermenu op **My ROCKBLOCKS** en voeg de rockblock toe aan je account, door de zes of zeven letters in te voeren in het vak met de naam **registration code** en klik op **add** (Als je bij deze stap problemen ondervindt, bijvoorbeeld indien de rockblock niet verschijnt bij het toevoegen, is het altijd een goed idee customer service te contacteren via [support@rock7.com])
3. Navigeer nu in het menu links naar **Credits and Line Rental** en schaf voldoende credits aan. (Zie volgende hoofdstuk.) Credits vervallen niet, dus het kan geen kwaad deze op voorhand te kopen.
4. Naast credits moet je ook een line rental kopen voor de maand waarin je berichten wil versturen. Een line rental vervalt na een maand maar moet enkel actief zijn indien je op dat moment berichten wil versturen met iridium.

### Benodigde credits
In de eerste fase (5244180 ms) gaan we elke 10000ms een bericht versturen dat 3 credits gebruikt. Voor de eerste fase hebben we dus 1574 credits nodig. Voor de tweede fase is het minder duidelijk hoeveel berichten we gaan kunnen versturen. Die duurt 1441400ms en we gebruiken elke 10000ms 7 credits, wat zou neerkomen op 1008 credits. Omdat we waarschijnlijk wel niet altijd van de eerste keer erin gaan slagen om het bericht te versturen denk ik dat voor de hele missie 2000 credits een goede schatting is.
---
## De vlucht
Nu de credits aangekocht zijn en de line rental actief is, is het tijd voor de lancering. Bidden dat alles goed gaat!

Normaal kunnen we tijdens de vlucht al berichten zien binnnenkomen op de [rockblock website](https://rockblock.rock7.com/), klik hiervoor links op **Messages**.
---
## Na de vlucht
### Rauwe data omzetten in een .csv bestand
We hebben nu een heleboel berichten ontvangen met voor mensen compleet onleesbare data. Download alle berichten van tijdens de vlucht via de [rockblock website](https://rockblock.rock7.com/), klik links op **Messages**, rechts bovenaan klik je op de blauwe knop **Export** en je kiest voor **CSV**. Nu is het tijd om op [deze](https://github.com/RomanVannieuwenhuyse/peregrinus-data-processing) website de software te downloaden die de data verwerkt. Hoe dit proces exact verloopt hangt af van je besturingssysteem

- **Windows**
    1. Download het bestand [cstb.exe](https://github.com/RomanVannieuwenhuyse/peregrinus-data-processing/releases/download/v0.0-alpha/cstb.exe).
    2. Zet het in een folder, samen met de gedownloade messages van hierboven.
    3. Klik met de rechtermuisknop in de folder en klik op **Open in terminal**
    4. Typ in het terminalvenster dat nu verschijnt volgend commando en voer het uit (vervang *messages-1234567890123.csv* door de naam van  het messages bestand dat je daarnet hebt gedownload van de rockblock website)
        ```
        .\cstb.exe .\messages-1234567890123.csv
        ```
    5. Nu zouden er in de folder twee nieuwe bestanden moeten verschijnen: *phase_one_data.csv* en *phase_two_data.csv*
- **Mac of Linux**
    Download de code, compileer het bestand en volg met dat bestand stappen 2-5 uit het bovenstaande. Let op! het commando dat je zal moeten gebruiken zal er waarschijnlijk eerder als volgt uitzien, maar dit verschilt per besturingssysteem
        ```
        ./cstb messages-1234567890123.csv
        ```

### CSV bestand importeren in Google Sheets
Open een leeg Google spreadsheets. Klik op **Bestand > Importeren > Uploaden** en importeer een voor een de *phase_one_data.csv* en *phase_two_data.csv* bestanden. Ik zou beiden naar een ander blad importeren. Zorg ervoor dat "Tekst naar getallen, datums en formules converten" afgevinkt staat. Doe dan ctrl+f, vervang alle punten door komma's en zet de cellen daarna om naar getallen, door ze te selecteren, en dan op **meer indelingen** te klikken en een indeling te maken met vijf of zes getallen na de komma. Het kan ook visueel handig zijn om conditionele opmaak toe te voegen met volgende regels voor respectievelijk fase één en fase twee.
    ```
    =LT(REST(RIJ()-2;20);10)
    =LT(REST(RIJ()-2;46);23)
    ```
Zo wordt de data uit verschillende berichten gemakkelijk onderscheiden

### Data verder verwerken
Nu is de data beschikbaar voor verdere analyse.