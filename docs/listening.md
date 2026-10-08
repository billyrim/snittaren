# Etapp 3 — kalibrering av de fem klangerna

## Nuvarande underlag

Användaren har laddat prototypen 0.1.0 och rapporterat ”Det funkar bra!”.
Skärmbilden visar Filt i användning. Detta är positiv användaråterkoppling men
specificerar inte resultat per klang eller på flera källor. Fem egna AIFF-klipp har
nu tillhandahållits i `/Users/fronas/Documents/snittaren-pads`. Tekniska renderingar
och mätningar är klara; musikalisk återkoppling återstår. Se
[analys av egna pads](pad-analysis.md) och
[lyssningspaketet](/Users/fronas/Documents/ChatGPT/Snittaren/build/pad-calibration-v1/README.md).

## Lyssningspass

Använd 4–6 representativa padklipp: mörk, övertonsrik, luftig, rörlig och olika
ackordregister. Exportera före SNITTAREN, men behåll gärna synthens egna effekter.

För varje klipp och klang:

1. Återställ de tre rattarna. Lyssna solo och tillsammans med ett befintligt lager.
2. Prova en ratt i taget i båda ändlägena. Dubbelklicka tillbaka till mitten.
3. Prova alla på minimum, alla på maximum samt Botten/Topp öppna med Mitt stängd.
4. Leta efter en användbar roll. Notera särskilt tunna källor i Frost, bottenbevarande
   i Tom mitt och oönskade toner/ringningar i Resonanta öar.
5. Kontrollera klangbyten och långsamma makrorörelser på ihållande ljud.
6. Jämför också ungefär nivåanpassat. Separat offline-nivåanpassning är teststöd,
   inte en funktion i pluginen. Mycket svagt material ska inte förstärkas obegränsat.

Använd Logic-automationens parametervärden om exakta makrovärden behövs i loggen;
pluginens tre rattar visar avsiktligt inga siffror. Output visar fortfarande dB.

## Lyssningslogg

| Källa/register | Klang | Botten/Mitt/Topp | Solo/lager | Användbar roll | Problem | Föreslagen ändring |
|---|---|---|---|---|---|---|
| Absenteechords | Ej bedömt | | | | | |
| droplets | Ej bedömt | | | | | |
| eftershards-maskinen | Ej bedömt | | | | | |
| rymdstrakar | Ej bedömt | | | | | |
| scalertest | Ej bedömt | | | | | |

Etappen är klar när de fem rollerna fungerar tydligt på flera källor och makrona
hjälper användaren att hitta ett lager. Användaren bedömer musikalisk användbarhet.
Innan vi levererar ändrade kurvor behöver banken versionshanteras så att tidigare
projekt behåller bankrevision 1. Utökning till 15 lägen kommer efter denna kalibrering.

## Reproducerbara renderingar

`render_pad` tar valfria Botten/Mitt/Topp-värden −100..100 efter utmatningsmappen.
Använd en ny mapp per kombination; varje körning ger torr källa och fem klanger.
Exempel för öppna ytterområden med stängd mitt:

```sh
./build/render_pad /absolut/sökväg/pad.wav build/pad-open-edges 100 -100 100
```

Dessa filer behåller faktisk nivå. Nivåanpassad jämförelse görs separat efter
analys av klippen, så att tysta källor eller extrema utskärningar inte lyfts blint.


## Återskapa paketet

`scripts/prepare-listening.py` kräver Python med NumPy samt macOS `afconvert`.
Den befintliga bundlade Python-miljön användes (ingen installation gjord):

```sh
/Users/fronas/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/bin/python3 \
  scripts/prepare-listening.py /Users/fronas/Documents/snittaren-pads build/pad-calibration-v2
```

Välj en ny utmatningsmapp. Skriptet lämnar originalen orörda, renderar tio
makroinställningar och skriver metadata, CSV och ett lyssningsindex. Räkna med
cirka 1 GB för fem klipp. V1-resultaten ligger i build/pad-calibration-v1;
manifest och mätningar finns även under docs för Git. Ljudfilerna ligger i den
ignorerade byggmappen och följer inte med en commit.


## Fortsättning i Logic med hela banken

Användaren har godkänt att gå vidare med alla 15 presets och vill göra den
huvudsakliga musikaliska utvärderingen i Logic. 0.2.0 är därför levererad med
[bankrevision 2](bank-v2.md). Paketet pad-calibration-v1 ovan är ett historiskt
femlägesunderlag. Det aktuella renderverktyget och skriptet stöder alla 15.
Ett nytt komplett offlinepaket behövs inte för att fortsätta arbetet i Logic.
