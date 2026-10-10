# Bankrevision 3 · 23 presets

Åtta nya lägen, 16–23, kompletterar de 15 befintliga. De gamla täcker främst
sammanhängande bas-/mellan-/diskantfönster, två separerade områden, tre resonanta
öar och fasta urgröpningar. Den nya gruppen använder korsande filterkanter,
rörliga urgröpningar, täta resonanser och överlappande grenar. Åtta ger tydligt
olika experiment utan att fylla ut banken med små frekvensförskjutningar.

Numren följer väljarens ordning. Värdens index 0–14 och samtliga gamla
ljudparametrar är oförändrade. Tidigare namn → nytt nummer:

| Nummer | Tidigare namn |
|---|---|
| 01 | Filt |
| 02 | Frost |
| 03 | Botten + luft |
| 04 | Tom mitt |
| 05 | Resonanta öar |
| 06 | Underström |
| 07 | Mörk kropp |
| 08 | Trä |
| 09 | Avlägsen radio |
| 10 | Strängrest |
| 11 | Skimmer |
| 12 | Luftrest |
| 13 | Kropp + skimmer |
| 14 | Två röster |
| 15 | Sprickor |

## Nya försök

Dessa är tekniska anteckningar, inte namn i gränssnittet.

| Nummer | Experiment |
|---|---|
| 16 | Resonant fönster vars filterkanter kan korsa varandra och strypa ljudet. |
| 17 | Tre tätt liggande, oharmoniska resonanser som drar åt olika håll. |
| 18 | Bred kropp med tre djupa hål som vandrar genom spektrumet. |
| 19 | Mycket låg bas och en tunn högfrekvent kant med ett enormt tomrum emellan. |
| 20 | Ett ensamt resonant fönster som kan slukas av sin egen rörliga urgröpning. |
| 21 | Tre överlappande resonanta grenar med motstridiga rörelser. |
| 22 | Tre smala övre fragment; vissa rattförlopp vänder tillbaka mot lägre register. |
| 23 | Låg resonans under ett vandrande fragment som genomkorsas av urgröpningar. |

Prova blandade ytterlägen på rattarna, inte bara alla åt samma håll. Högre
rattvärde behöver inte öppna klangen. Nästan tysta inställningar är avsiktliga.
Motorn är fortfarande en linjär filterbank: den tillför ingen distorsion,
granulering, slump eller nya övertoner. Överraskningarna uppstår ur källjudet,
filtersummeringen och rattarnas rörelser. Ingen musikalisk lyssningsbedömning
på användarens material har gjorts i denna revision.

## Kompatibilitet

Bankrevision 3 sparas i state; revision 1 och 2 läses fortfarande. Regressionen
jämför alla 15 gamla lägen mot en fryst kopia av 0.2.0 med rörliga makron,
lägesbyten, Output och bypass. Äldre filer i docs är historiska protokoll och
kan använda den gamla bankens interna ID:n. Nya WAV-renderingar och
responsmätningar använder numret i väljaren.

Sparade heltalsval i plugin-state bevaras. Värdautomation som lagrats som
normaliserade värden kan påverkas när antalet alternativ ökar; befintliga
Logic-projekt med Klang-automation bör kontrolleras efter uppdateringen.

## Verifiering 2026-10-10

Release-bygge och lokal installation av 0.3.0 klara; installationsskriptet
säkerhetskopierade föregående AU. Apples `auval` godkände den installerade AU:n.
Alla 23 lägen omfattas av DSP-stabilitet över 27 rattkombinationer och sex
samplingsfrekvenser, svep, lägesbyten samt state-/ljudåterställning. De gamla
15 lägena ger sample-identiskt ljud mot den frysta 0.2.0-motorn.

De nya neutrala kurvorna jämfördes med samtliga övriga lägen vid testets nio
frekvenser, efter att konstant nivåskillnad räknats bort. Närmaste par bland de
nya är 16/20 med cirka 5,1 dB RMS-formskillnad; övriga nya lägens närmaste
jämförelser skiljer minst 7,5 dB. Detta är en grov teknisk kontroll mot dubletter,
inte ett mått på upplevd musikalisk skillnad.

Protokoll: [CTest](ctest-v3.txt), [state och prestanda](host-v3.txt),
[kompatibilitet](compatibility-v3.txt), [AU](auval-v3.txt),
[responsdata](response-v3.csv), [gränssnitt](editor-v3.png).
