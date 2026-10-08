# Verifiering · 2026-10-08

## Teknisk verifiering – genomförd

Miljö: MacBook Air M5 (10 kärnor, 16 GB), macOS 26.5.2, Xcode 26.6,
CMake 3.29.2, lokal JUCE 8.0.13. Release, arm64, macOS deployment target 12.0.
macOS 12 har inte körtestats; det är endast byggmålet.

- AU byggd och ad hoc-signaturen kontrollerad med `codesign --verify --deep --strict`.
- Installerad AU godkänd av `auval -v aufx Snit BlRm`; se [hela loggen](auval.txt).
  Den innefattar parameterkontroller, renderingskontroller, state/class info,
  kanalformat och schemalagda/rampade parametrar. Den skapar inte ett Logic-projekt.
- CTest: 2/2 passerar. [CTest](ctest.txt), [DSP](dsp-test-results.txt),
  [processor/state](host-test-results.txt).
- 27 kombinationer av −100/0/+100 för tre makron × fem lägen × sex sample rates:
  22,05/44,1/48/88,2/96/192 kHz, deterministiskt brus ±0,2. Finita resultat och
  identisk stereoutgång. Varje kombination körs 150 ms; detta är ett avgränsat
  stresstest, inte en uttömmande stabilitetsbevisning.
- Impuls 0,5 följd av tystnad med snabba byten/makron/bypass: avklingning samt ingen
  läcka till den tysta kanalen. Logaritmiskt svep 20 Hz–20 kHz med topp 0,1:
  alla fem lägen ger ändlig utgång under 1.
- Processor jämför exakt samma signal i ett block om 8192 samples respektive
  varierande 1/17/64/127/511/1024/3, mono/stereo vid 44,1/48/88,2/96 kHz.
  Resultaten är identiska. Detta verifierar blockoberoende vid konstanta parametrar.
- State round-trip återställer sex parametrar och ger identisk audio i två nya
  instanser med samma uppstart. Okända state-/bankversioner avvisas i koden.
- Bypass ger 0,125 ut vid 0,125 in efter utjämning även med Output +12 dB.
- 10 sekunders snabb växling av klang, makron, Output och bypass med sinus 440 Hz,
  topp 0,1: största samplesprång cirka 0,00439. Regressionstestets gräns 0,05 gäller
  enbart denna testsignal. Ingen avlyssning eller universell klickfrihetsgaranti.
- Callbacken inspekterad: fasta arrayer, ingen egen allokering/låsning/loggning.
  Ingen instrumenterad malloc-spårning har körts.
- En instans, stereo 48 kHz/128: 10 sekunder ljud på cirka 0,025 sekunder väggtid,
  cirka 0,25 % av realtidsbudgeten för en sekventiell offlinekörning. Detta mäter
  DSP/processor, inte Logic-belastning, flera instanser eller värsta schemaläggning.
- GUI renderat direkt från JUCE-editorn och visuellt granskat: [bild](editor.png).
  Inte en skärmbild från Logic. Interaktiv mus/tangentbordstestning återstår.
- Sex 12-sekunders WAV-filer renderade från syntetisk harmonisk pad med svagt brus,
  i `build/listening-final`. Ingen normalisering, ingen musikalisk bedömning.

## Mätta neutrala roller

Verklig utgång demodulerad efter insvängning, sinus 0,1 vid 48 kHz; resultaten
inkluderar grenarnas komplexa summering. Fulla värden: [response.csv](response.csv).

| Klang | Exempel på uppmätt nivå relativt input |
|---|---|
| Filt | 400 Hz −2,4 dB; 80 Hz −39,7 dB; 8 kHz −90,6 dB |
| Frost | 8 kHz −1,1 dB; 400 Hz −89,0 dB |
| Botten + luft | 80 Hz −0,2 dB; 1,3 kHz −39,8 dB; 8 kHz −0,4 dB |
| Tom mitt | 80 Hz −0,05 dB; 1,3 kHz −28,6 dB; 8 kHz −0,09 dB |
| Resonanta öar | 300/1300/4500 Hz cirka +3,3..+3,6 dB; 650 Hz −15,4 dB |

## Logic-verifiering – användarens första prov genomfört

Användaren rapporterar att 0.1.0 fungerar bra och har visat Filt i användning.
Detaljerad projektverifiering är ännu inte dokumenterad.

Vid agentens tidigare försök svarade datorverktyget
”Computer Use was not approved to use Logic Pro”. Därför
har instansiering, verklig uppspelning, automation i projekt, spara/återöppna,
offlinebounce och flera instanser **inte** verifierats i Logic. Ingen alternativ
UI-styrning användes. Följ stegen i README för det lokala användartestet.

## Musikalisk utvärdering – återstår

Inga användar-pads fanns i projektet. Inga ljud har avlyssnats av agenten.
Syntetiska källor och mätningar stödjer de tekniska rollerna men avgör inte om
klangerna är användbara eller om övergångarna upplevs klickfria. Banken är
preliminär. Nästa steg är användarens provlyssning på egna pads, både solo och
lager, med anteckning om klang, makrovärden, roll och önskad ändring.


## Uppdatering 0.1.1

- Klangbeskrivning och makrovärden borttagna; Output-värdet kvar. GUI-renderingen
  ovan är uppdaterad och visuellt kontrollerad, 600 × 320 punkter.
- Installerad AU identifieras av `auval` som **Fronas**, version **0.1.1**;
  valideringen passerar. Komponentens ändringstid uppdateras vid installation
  eftersom macOS först återanvände cachad metadata för 0.1.0.
- CTest 2/2 passerar efter ombyggnad. Pluginens DSP och stateformat är oförändrade.
- Renderaren accepterar nu tre valfria makrovärden. Neutral och 100/−100/100
  rendering från syntetisk källa kontrollerade: 12 WAV-filer, stereo, 24 bit,
  48 kHz, 12 sekunder. Torra filer är identiska; samtliga fem bearbetade filer
  skiljer sig mellan inställningarna. NaN/Inf, out-of-range och ogiltig text
  avvisas före skrivning. Dessa är tekniska kontroller, ingen avlyssning.
- Den nya versionen är ännu inte provad i Logic av agenten eller bekräftad av
  användaren. Etapp 3 väntar på egna padklipp och musikalisk återkoppling;
  arbetsgången finns i [lyssningsloggen](listening.md).


## Egna pads – teknisk utvärdering

Fem egna AIFF-källor har nu analyserats och representativa 12-sekundersutdrag
renderats med neutrala makron, sex individuella ändlägen och tre kombinationer.
300 renderingar och 30 separata nivåanpassade kopior har kontrollerats genom
återläsning. Originalens hashvärden är oförändrade. Se [rapport](pad-analysis.md).
Detta ersätter inte avlyssning eller Logic-test; inga klangdefinitioner ändrades.
Musikalisk utvärdering i etapp 3 är fortfarande inte slutförd.


## 0.2.0 – alla 15 klanger

På användarens uttryckliga begäran är banken nu utökad till 15 för fortsatt
musikalisk utvärdering direkt i Logic. Etapp 3:s lyssningsarbete fortsätter parallellt.

- Release-bygge arm64 och lokal installation klara. Ad hoc-signaturen verifierad.
- AU-valideringen identifierar **Fronas, version 0.2.0** och visar **15** klangvärden
  (0–14). Samma parameter-ID för Klang, `3357091`, som i föregående AU-version.
  [auval.txt](auval.txt) innehåller den aktuella valideringen.
- CTest **3/3 passerar**: DSP, processor/state och separat kompatibilitetstest.
- Alla 15: 27 makrokombinationer vid sex samplingsfrekvenser, stereoidentitet,
  tyst kanal, avklingning, svep och snabba byten. Testade neutrala responsroller
  inkluderar de två röstfönstren och Sprickors tre centrumfrekvenser.
- Sprickor: mätt dämpning vid 350/1100/3200 Hz djupare än −26 dB i neutralläget;
  +100 öppnar minst 9 dB vid respektive centrum men bevarar utskärningen.
- [Kompatibilitetstestet](compatibility-v2.txt) jämför mot en fryst 0.1.1-motor:
  de första fem ger exakt samma float-samples vid 44,1/48/96 kHz med varierande
  klang, makron, Output och bypass. Nya motorer ändrar inte de gamla ljuden.
- State round-trip inklusive audio verifierat för samtliga 15 lägen. Gamla
  bankrevision-1-tillstånd återställer rätt klangindex och makron. Nytt state
  sparar bankrevision 2. Verklig projektåteröppning i Logic återstår.
- Offlinebenchmark: en stereoinstans vid 48 kHz/128, tio sekunder audio på cirka
  0,063 sekunder (0,63 % av en sekventiell realtidsbudget). Detta är inte en
  uppmätt Logic-belastning eller garanti för många samtidiga instanser.
- Fem egna padutdrag renderade genom samtliga 15 neutrala lägen: 75 bearbetade
  WAV plus fem torra. Alla 80 återlästa med rätt format/längd, finita värden och
  utan fullskaleklipp. Samma −12 dB testdämpning som i tidigare underlag; högsta
  sample-peak −19,37 dBFS. Se [CSV](pad-smoke-v2.csv).
- [DSP-resultat](dsp-v2.txt), [responsmätningar](response-v2.csv),
  [processor/state/CPU](host-v2.txt). GUI-rendering visuellt granskad.

Pluginens vy och tillverkarnamn följer de senaste önskemålen. Alla kurvor och
nya makroankare finns i [bankrevision 2](bank-v2.md). Inga ljud har avlyssnats av
agenten, och ingen ny interaktiv Logic-verifiering har gjorts. Slutlig musikalisk
kalibrering sker i användarens arbetsflöde; de nya klangerna är preliminära.
