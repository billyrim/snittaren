# Bankrevision 2 · SNITTAREN 0.2.0

Alla 15 klanglägen är nu implementerade för provlyssning i Logic.
Frekvenserna är startpunkter, inte färdigkalibrerade sweetspots. De tio nya lägena
har lagts sist i den befintliga väljaren. De första fem är oförändrade.

## Väljarordning och grundformer

| Index | ID | Klang | Neutral grundform |
|---:|---|---|---|
| 0 | 03 | Filt | 250–650 Hz |
| 1 | 08 | Frost | 5–16 kHz |
| 2 | 10 | Botten + luft | under 220 Hz och över 4 kHz |
| 3 | 13 | Tom mitt | under 400 Hz och över 2,8 kHz |
| 4 | 15 | Resonanta öar | 250–360, 1100–1500, 3900–5200 Hz; Q 1,05 |
| 5 | 01 | Underström | 20–180 Hz |
| 6 | 02 | Mörk kropp | 120–450 Hz |
| 7 | 04 | Trä | 450–1200 Hz |
| 8 | 05 | Avlägsen radio | 900–2200 Hz |
| 9 | 06 | Strängrest | 1500–3500 Hz; Q 0,85, trim −1 dB |
| 10 | 07 | Skimmer | över 2500 Hz |
| 11 | 09 | Luftrest | över 8000 Hz |
| 12 | 11 | Kropp + skimmer | 250–600 Hz samt över 3500 Hz |
| 13 | 12 | Två röster | 400–800 samt 1600–2800 Hz; Q 0,8 |
| 14 | 14 | Sprickor | utskärningar vid 350, 1100, 3200 Hz |

Gränserna är nominella, med ändliga filterlutningar. HP/LP har två andraordnings-
sektioner per kant (24 dB/oktav), Q 1/√2 om inget annat anges. Toppgränser
begränsas till 0,45 × samplingsfrekvensen. Ingen adaptiv gain eller limiter.

## Nya makroankare

`a / b / c` betyder −100 / 0 / +100. Frekvenser i Hz. För ett passfönster styr
Botten HP-gränsen, Topp LP-gränsen och Mitt bredd + gain, utom där tabellen
anger grenvis mappning. Bredd delar HP och multiplicerar LP, så att Mitt också
ändrar formen. Tabellen för de ursprungliga fem finns i design.md.

| Klang/gren | HP-ankare | LP-ankare | Gain dB | Bredd |
|---|---|---|---|---|
| Underström | 35/20/12 | 110/180/260 | −6/0/0 | 0,85/1/1,15 |
| Mörk kropp | 180/120/75 | 300/450/650 | −6/0/+1 | 0,85/1/1,15 |
| Trä | 600/450/320 | 850/1200/1650 | −6/0/+1 | 0,85/1/1,15 |
| Avlägsen radio | 1200/900/700 | 1600/2200/2900 | −8/0/+1 | 0,9/1/1,12 |
| Strängrest | 1900/1500/1200 | 2700/3500/4500 | −7/−1/0 | 0,9/1/1,12 |
| Skimmer | 3500/2500/1800 (Topp) | 20000 fast | −6/0/+1 | 0,9/1/1,1 |
| Luftrest | 10500/8000/6500 (Topp) | 22000 fast | −6/0/+1 | 0,95/1/1,05 |
| Kropp + skimmer: kropp | 340/250/180 (Botten) | 450/600/800 (Botten) | −6/0/0 | 0,9/1/1,1 |
| Kropp + skimmer: skimmer | 5000/3500/2800 (Topp) | 22000 fast | −6/−2/0 | 0,9/1/1,1 |
| Två röster: låg | 500/400/310 (Botten) | 650/800/1000 (Botten) | −6/−1/0 | 0,9/1/1,1 |
| Två röster: hög | 1900/1600/1300 (Topp) | 2400/2800/3400 (Topp) | −6/−1/0 | 0,9/1/1,1 |

Botten i Skimmer multiplicerar HP med 1,12/1/0,9, i Luftrest med 1,08/1/0,94.
Det öppnar nederkanten försiktigare än Topp. Mitt styr gain och bredd i båda
grenarna i Kropp + skimmer och Två röster. Fasta kanter påverkas fortfarande av
bredd och Nyquist-begränsning.

### Sprickor

En bred 20–22000 Hz-gren följs av tre TPT-bellutskärningar i serie. Utgången från
varje sektion är `x + (gain − 1) × k × bandpass`. Det ger ändligt djup vid centrum,
utan skarpa koefficientbyten. Varje makro öppnar sitt område genom grundare och
smalare utskärning; centrumfrekvensen ligger fast.

| Makro | Centrum | Djup dB −100/0/+100 | Q −100/0/+100 |
|---|---:|---|---|
| Botten | 350 Hz | −42/−30/−18 | 0,65/1,1/1,8 |
| Mitt | 1100 Hz | −48/−36/−20 | 0,6/0,9/1,5 |
| Topp | 3200 Hz | −42/−27/−15 | 0,5/0,8/1,4 |

## Kompatibilitet och övergångar

- AU-identitet, tillverkarnamn Fronas och parameter-ID:n är oförändrade.
- De ursprungliga fem indexen bevaras. JUCE:s AUv2-wrapper exponerar diskreta
  klangvärden som heltal: det nya intervallet är 0–14, de gamla 0–4 finns kvar.
- Stateversion är fortsatt 1. Nytt state sparar bankrevision 2; både revision 1
  och 2 läses. Revision 1:s fem definitioner finns oförändrade i revision 2.
- Tests/LegacyV1 är en fryst kopia av femlägesmotorn. Sample-för-sample-test
  jämför den mot nya motorn under varierande klang, makron, Output och bypass.
- Samtliga 15 motorer hålls uppvärmda. Samma mjuka vikter och parametertider som
  tidigare. Högre CPU-kostnad kontrolleras med offlinebenchmark.
- Ingen ny UI-funktion: inga band, kurvor, beskrivningar eller makrosiffror.

Framtida ändringar i klangernas kurvor kräver en ny bankrevision som fortfarande
kan återge äldre kurvor. Den här revisionen utökar banken, ändrar inte gamla ljud.
