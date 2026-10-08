# Tekniska val och preliminär bankrevision 1

Historik för 0.1.x. Aktuell utökning till 15 lägen i 0.2.0 beskrivs i [bankrevision 2](bank-v2.md).

## Signalväg

`Source/Presets.h` innehåller läsbara presetdefinitioner och MacroMapper-funktionen
`map`. `Source/Engine.h` innehåller filtermotor och övergångshantering.
`Source/Plugin.cpp` innehåller JUCE-processor och den lilla editorn.

Varje gren har två högpass och två lågpass av andra ordningen i kaskad: nominellt
24 dB/oktav per kant. TPT state-variable-filter med positiv frekvens och dämpning;
Q = 1/√2, utom Resonanta öar med Q = 1,05. Öar har statisk grentrim −2 dB, övriga
0 dB vid neutrala makron. Det ger måttliga resonanstoppar, inte självsvängning.
Parallella grenar summeras som signaler med fas. CSV-mätningen demodulerar den
faktiska utgången; den summerar inte separata magnitudkurvor.

Samtliga fem motorer processar hela tiden. Därmed är deras tillstånd uppvärmda
även vid snabba avbrutna byten. Klangvikter följer en enpolig utjämning med
25 ms tidskonstant (cirka 75 ms till 95 %), summerar till 1 och ger en linjär
viktad signalblandning. Detta kostar mer CPU än två motorer men förenklar snabba
byten och eliminerar återställning av hörbara tillstånd. Slutläget är diskret.

Makron, Output och bypass har 30 ms tidskonstant. Filtermål beräknas var 32:a
sample från utjämnade makron. Frekvenser/bredder interpoleras logaritmiskt,
dämpning i dB. Inga godtyckliga IIR-koefficienter interpoleras. Frekvenser
begränsas till 10 Hz–0,45 × samplingsfrekvensen. Separata tillstånd per kanal.
Ingen allokering, låsning, filåtkomst eller loggning finns i den egna callbacken.
DSP använder fasta arrayer och dubbelprecision internt; värdens audio är float.
Ogiltiga insignalsvärden ersätts med noll. Denormaler stängs av i callbacken.
Ingen lookahead eller dold limiter; IIR-filter har fortfarande gruppfördröjning.

## Makroankare

Nedan betyder `a / b / c` −100 / 0 / +100. Alla siffror i Hz utom gain och bredd.
HP flyttar nedåt när ett område öppnas; LP flyttar uppåt. Dessutom finns ett
explicit breddankare 0,8 / 1 / 1,2 i varje gren: HP delas med bredd, LP multipliceras
med bredd. Bredden styrs av samma makro som grenens gain. Det gör att även Mitt
ändrar filterformen och inte enbart förstärker efter en utskärning.

| Klang | Gren | HP, styrning | LP, styrning | Gain dB + bredd, styrning |
|---|---|---|---|---|
| Filt | kropp | 340/250/170, Botten | 480/650/1000, Topp | −9/0/+2, Mitt |
| Frost | topp | 7000/5000/3500, Topp | 16000 fast | −8/0/+2, Mitt |
| Botten + luft | botten | 20 fast | 120/220/330, Botten | −8/0/0, Mitt |
| Botten + luft | luft | 6500/4000/3000, Topp | 22000 fast | −8/0/0, Mitt |
| Tom mitt | botten | 20 fast | 220/400/650, Botten | −6/0/0, Mitt |
| Tom mitt | topp | 4200/2800/1900, Topp | 22000 fast | −6/0/0, Mitt |
| Resonanta öar | låg | 285/250/200, Botten | 315/360/430, Botten | −2 fast, bredd Botten |
| Resonanta öar | mellan | 1250/1100/900, Mitt | 1350/1500/1800, Mitt | −2 fast, bredd Mitt |
| Resonanta öar | hög | 4350/3900/3200, Topp | 4650/5200/6100, Topp | −2 fast, bredd Topp |

Frosts Botten multiplicerar HP med 1,15 / 1 / 0,85 och öppnar därmed nederkanten
försiktigare än Topp. Detta uttrycks i grenens separata `bottomReach`-ankare
(1/1/1 i övriga lägen). Botten återför inte bas till Frost. Mitt öppnar båda
kanterna i de tvågrenade lägena.
Även de nominellt fasta kanterna påverkas av bredd och Nyquist-begränsningen.
Ingen adaptiv nivåkompensation används.

## Automation och kompatibilitet

Stabila parameter-ID: `mode`, `bottom`, `middle`, `top`, `output`, `bypass`.
Makron −100..100, Output −24..+12 dB. Alla värdparametrar är automatiserbara.
Återställning skickar tre separata begin/set/end-gester till värden.
Parametrar läses vid blockstart; AU-wrappern ansvarar för värdens schemaläggning.
Ingen garanti om sample-exakt automation för godtyckliga värdar.

Stabila mode-index är 0=03 Filt, 1=08 Frost, 2=10 Botten + luft,
3=13 Tom mitt och 4=15 Resonanta öar. Lägg senare till lägen sist; sortera inte om
släppta index. XML-state innehåller `stateVersion=1`, `bankRevision=1` och alla
parametrar. Okända revisioner avvisas. Bankrevision 1 ska frysas: vid framtida
kalibrering måste den gamla banken behållas och rätt revision väljas från state,
eller hela definitionen sparas. Ändra inte dessa tabeller under samma revision i
en ny leverans. Filterminne är inte state; statisk ljudjämförelse görs efter uppstart.

Prototypen är lokal, ingen push eller publicering gjord. GitHub-repot kontrollerades
med `git ls-remote` och annonserade inga refs (tomt repo). Projektmappen hade inga
befintliga filer eller projektinstruktioner att återanvända.

## UI- och namnrevision 0.1.1

Tillverkarnamnet i JUCE/AU-metadata är Fronas. `BlRm`, `Snit` och
`se.billyrim.snittaren` är kvar som tekniska identiteter för kompatibilitet med
sparade projekt. Parameter-ID:n, index, stateversion och bankrevision är oförändrade.
Klangbeskrivningen och textfälten under Botten/Mitt/Topp har tagits bort ur editorn.
Output visar dB som tidigare. Fönstret är 600 × 320 och layouten har komprimerats.
