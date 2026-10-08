# Kreativ filterbank för ambient

Produktbeskrivning och genomförandeplan för Codex · 2026-10-08

Status: genomarbetad byggspecifikation för en första prototyp. Presetfrekvenser och makrogränser är hypoteser att kalibrera genom lyssning. Produktnamn är inte bestämt.

## 1. Produktidé

En ljudeffekt för Logic som förvandlar en inspelad pad till ett nytt, tydligt avgränsat lager genom kraftig spektral skulptering. Användaren väljer mellan 15 avsiktligt utformade klanglägen och anpassar resultatet med tre rattar: **Botten, Mitt och Topp**.

Pluginen ska göra det lätt att upptäcka ljud genom att lyssna. Den ska erbjuda spännande startpunkter och begränsad, användbar kontroll. Enskilda filterband och EQ-kurvor visas inte.

Målgruppen i första versionen är produktägaren själv: en ambientproducent som arbetar med stora padackord och många lager, med Tim Hecker och Loscil som estetiska referenser. Referenserna beskriver en riktning, inte ett löfte om att återskapa specifika produktioner.

### Problemet

Användaren börjar ofta med ett stort ackord och söker sedan en klang genom hård EQ. Stora frekvensområden skärs bort tills en intressant rest finns kvar. Nästa pad får en annan plats i ljudbilden. Vanlig parametrisk EQ kräver många aktiva beslut och lockar till detaljarbete. Fasta oktavfilter ger kreativt motstånd men begränsar placeringen av de områden som behålls.

### Produktlöfte

”Hitta en plats för din pad: välj en klang, öppna eller stäng tre områden och fortsätt skapa.”

## 2. Användarflöde

1. Spela in en pad och låt den gå i loop i Logic.
2. Lägg pluginen på spåret.
3. Bläddra mellan namngivna klanglägen medan ljudet spelar.
4. Stanna vid något användbart och justera Botten, Mitt och Topp.
5. Anpassa utgångsnivån vid behov och gå vidare till nästa lager.
6. Använd en separat EQ efter pluginen för eventuell detaljkorrigering.

Resultatet behöver inte vara imponerande i solo. Ett tunt, mörkt eller märkligt ljud kan vara mycket användbart tillsammans med andra lager. Lyssningsutvärderingen ska omfatta båda situationerna.

## 3. Omfattning och avgränsningar

### Ingår i första fullständiga versionen

- 15 fasta, kuraterade klanglägen.
- Tre makrorattar med neutralt mittläge.
- Utgångsvolym och intern bypass för jämförelse.
- Klickfria byten och mjuk parameterändring under uppspelning.
- Mono och stereo; samma filterinställningar i vänster och höger kanal.
- Sparande och återställning i Logic-projekt samt automation av användarparametrarna.
- Ett kompakt gränssnitt utan spektrumanalysator eller avancerad bandsida.

### Ingår inte

Reverb, delay, distorsion, granulär syntes, modulation, slumpknapp, MIDI-styrning, tonartsanalys, automatisk mixanpassning, stereobreddning, mid/side eller redigering av enskilda band. Ingen kontinuerlig morph mellan alla 15 lägen i första versionen; väljaren är diskret.

En filtereffekt kan framhäva befintliga övertoner men skapar inte en fräsande topp i en källa som saknar högfrekvent innehåll. Ett extremt toppreset får därför bli mycket svagt på mörka källor. Detta ska inte kompenseras med obegränsad förstärkning.

## 4. Gränssnitt och beteende

Prioritetsordning: stor klangväljare med föregående/nästa, tydligt namn och kort beskrivning; tre lika stora rattar; mindre utgångsvolym och bypass. En enkel signal-/clipindikator får finnas. Visuellt ska detta kännas som ett litet klanginstrument.

| Kontroll | Föreslaget beteende |
|---|---|
| Klang | Diskret val av 15 lägen; inga bandvärden visas |
| Botten | −100 till +100; 0 är presetets avsedda botten |
| Mitt | −100 till +100; 0 är presetets avsedda mellanregister |
| Topp | −100 till +100; 0 är presetets avsedda topp |
| Output | Startförslag −24 till +12 dB, standard 0 dB |
| Återställ rattar | Återställer de tre makrona till 0 |
| Bypass | Mjukt byte till obehandlad signal vid originalnivå |

Föreslaget deterministiskt beteende: byte av klang behåller makrovärden och Output. Klangväljaren får inte i hemlighet skriva om automationsparametrar. Användaren kan återställa makrona med en separat knapp; denna knapp ändrar alla tre via värdens parameterhantering. Första instansen startar i Tom mitt med makrona på 0. Dessa interaktionsval kan omprövas efter användartest.

## 5. Makronas innebörd

Makrona ska påverka filterformen, inte enbart förstärka signalen efter en extrem utskärning.

- Åt höger: öppna motsvarande område genom mindre dämpning, bredare passområde eller flyttad gränsfrekvens.
- Åt vänster: minska områdets närvaro genom mer dämpning eller snävare passområde.
- 0: exakt presetets grundform.

Varje preset behöver egen explicit mappning. Upplevelsen ska vara konsekvent även när olika filterparametrar ändras. Exempel: Topp åt höger i Filt höjer lågpassgränsen; i Frost breddar den det bevarade toppområdet nedåt. Hjälptexten kan säga ”Öppnar toppområdet” för att inte lova en vanlig diskantförstärkning.

Makrona är inte garanterat perceptuellt oberoende. Begränsa deras överlappning och prova kombinationer, särskilt alla tre på max. Ändlägena ska vara användbara och bevara något av presetets karaktär. En neutralisering till nästan torr signal är inget mål.

Utgångspunkt för implementation: interpolera frekvensvärden logaritmiskt och dämpning i dB mellan explicit definierade ankare −100, 0 och +100. Begränsa Q, frekvens och gain till stabila områden. Undvik att interpolera godtyckliga IIR-koefficienter utan stabilitetskontroll. Exakta intervall väljs och dokumenteras per preset efter prototyptest.

## 6. Första presetbanken

Tabellen definierar målkurvor och roller, inte färdiga filterkoefficienter. Angivna frekvenser är ungefärliga utgångspunkter. Övergångar har ändlig lutning; ”behåller” betyder huvudsakligt fokus.

| ID | Namn | Grundform | Avsedd roll |
|---|---|---|---|
| 01 | Underström | Huvudsakligen under 180 Hz | Fundament under andra lager |
| 02 | Mörk kropp | Tyngd 120–450 Hz, dämpad topp | Mörk ackordkropp med mindre djupbas |
| 03 | Filt | Fokus 250–650 Hz, centrum omkring 400 Hz | Dovt, instängt harmoniskt moln |
| 04 | Trä | Brett fönster 450–1 200 Hz | Ihålig eller träig klang |
| 05 | Avlägsen radio | Snävare fönster 900–2 200 Hz | Litet, avlägset lager |
| 06 | Strängrest | Fokus 1,5–3,5 kHz, försiktig resonans | Strävhet och tydliga övertoner |
| 07 | Skimmer | Främst över 2,5 kHz | Ljust lager med kvarvarande harmonik |
| 08 | Frost | Främst över 5 kHz | Tunn, fräsande övertonsrest |
| 09 | Luftrest | Främst över 8 kHz | Extrem lufttextur på rika källor |
| 10 | Botten + luft | Under 220 Hz och över 4 kHz; djup utskärning mellan | Två kraftigt separerade områden |
| 11 | Kropp + skimmer | 250–600 Hz plus högre övertoner, initialt över 3,5 kHz | Dov kropp med fristående skimmer |
| 12 | Två röster | Två fönster; start 400–800 Hz och 1,6–2,8 kHz | Färgad, nästan vokal karaktär |
| 13 | Tom mitt | Bred djup sänkning cirka 500 Hz–2,5 kHz, mjukare kanter | Urholkad pad som behåller sammanhang |
| 14 | Sprickor | Flera ojämnt placerade djupa utskärningar | Fragmenterad övertonsstruktur |
| 15 | Resonanta öar | Ett fåtal smalare passområden med måttlig resonans | Mest färgande, ringande karaktär |

För Sprickor kan första försöket använda utskärningar runt 350 Hz, 1,1 kHz och 3,2 kHz med olika bredder. För Resonanta öar kan första försöket använda fönster runt 300 Hz, 1,3 kHz och 4,5 kHz. Dessa värden har lägre säkerhet än rollerna och ska särskilt prövas på olika ackord och register. Ingen påstådd harmonisk universalitet.

Startområden för tuning: djupa utskärningar omkring −24 till −48 dB och filterkanter omkring 24–48 dB/oktav där hård isolering behövs. Mjukare former får använda mildare lutning. Högre ordning eller resonans är inga kvalitetsmål i sig; undvik oavsiktliga tonhöjder, långa ringningar och smärtsamma toppar.

Banken ska täcka olika funktioner och grader av ingrepp. Två lägen som regelbundet låter likadant ska ändras eller ersättas. Alla 15 behöver inte fungera på varje pad.

## 7. Tekniskt genomförande

### Plattform

Föreslagen standard för ett nytt projekt: C++ med JUCE och CMake, med AU-effekt för Logic som leveransmål. Återanvänd befintlig projektstruktur om ett repo finns. Kontrollera lokala verktyg, JUCE-version, licensvillkor och målmaskin innan byggkonfigurationen låses. Versionsnummer och minsta macOS-version fastställs vid implementation.

Prioritera användarens Mac-arkitektur. Intel/universalbygge och andra pluginformat är separata leveransval. DSP-utveckling och offlinekontroller kan göras utan Logic; AU-bygge, auval och faktisk Logic-verifiering kräver en lämplig Mac-miljö. Rapportera tydligt vad som verkligen har testats.

### DSP-arkitektur

Lås inte produkten till exakt 8–10 band. Använd en begränsad filtermotor som kan uttrycka målformerna: högpass, lågpass, shelves, breda/djupa bell-utskärningar och vid behov parallella passgrenar.

Börja med kaskader av stabila IIR-sektioner. De kan ofta åstadkomma både isolerade fönster och hål utan en fullständig crossoverbank. Om parallella grenar behövs ska deras komplexa summering mätas och lyssnas på: fasförskjutningar kan ge oväntade toppar och utsläckningar. Summerade magnitudkurvor är inte ett tillräckligt test.

Förslag på moduler:

- `PresetDefinition`: stabilt ID, namn, beskrivning, grundform, makroankare, begränsad nivåtrim och revisionsnummer.
- `MacroMapper`: översätter klang och tre makrovärden till filtermål.
- `FilterEngine`: kör filter och eventuella grenar med separat tillstånd per kanal.
- `TransitionManager`: mjuka ändringar, klangbyte och bypass.
- `PluginProcessor`: värdparametrar, ljudbuffertar och state.
- `PluginEditor`: den begränsade produktvyn.

Definitionerna ska ligga som läsbara data separerade från DSP-kod, exempelvis C++-tabeller eller validerad data som laddas utanför ljudtråden. Ingen användarredigerbar presetmotor behövs i första versionen.

### Nivå, stereobild och övergångar

- Inga allokeringar, lås, filoperationer eller loggutskrifter i ljudcallbacken.
- Förbered filter och buffertar utanför ljudtråden; hantera varierande blockstorlek och samplingsfrekvens.
- Använd samma koefficienter men separata filtertillstånd för vänster/höger. Identisk input i båda kanalerna ska ge identisk output.
- Sikta på inga extra lookaheadbuffertar och rapportera korrekt latens till värden. IIR-filtrering har fortfarande fas-/gruppfördröjning.
- Prova parameterutjämning runt 20–50 ms och klangbyten runt 50–100 ms som startvärden, inte fasta kvalitetskrav.
- Vid byte av topologi: använd två förallokerade motorer och mjuk korsfädning. Hantera filteruppvärmning och upprepade snabba byten utan att återställa en hörbar motor abrupt.
- Börja med linjär korsfädning och kontrollera nivån under övergången. Equal-power är inte automatiskt korrekt för korrelerade signaler.
- Använd begränsad statisk presettrim, initialt högst +6 dB, och manuell Output. Ingen adaptiv normalisering som pumpar eller lyfter nästan tyst material.
- Undvik dold limiter eller saturation som förändrar klangen. Behåll intern headroom, visa överstyrning och skydda mot ogiltiga numeriska värden.

### Sparande och automation

Exponera stabila parameter-ID:n för klang, Botten, Mitt, Topp, Output och bypass. Spara stateversion, presetbankens revision och parametrar. Ändra inte ID:n eller betydelsen av släppta klangindex mellan versioner. Om fabrikskurvor ändras efter release måste gamla projekt behålla sin klang genom versionshantering eller sparad definition.

## 8. Genomförandeplan

### Etapp 1 — Projekt och spelbar grund

Undersök repo, instruktioner och byggmiljö. Skapa effektprojektet, stabila parametrar, state och ett enkelt gränssnitt. Implementera pass-through, Output och mjuk bypass. Dokumentera bygg- och installationssteg för målmaskinen.

Klart när: bygget fungerar i tillgänglig miljö, state kan sparas/återställas och signalvägen fungerar. Verifiera AU i Logic när Mac finns; kalla annars detta steg lokalt verifierat med kvarstående värdtest.

### Etapp 2 — Fem kontrasterande klanglägen

Implementera Filt, Frost, Botten + luft, Tom mitt och Resonanta öar med de tre makrona. Använd slutliga stabila ID:n men exponera bara de implementerade lägena i prototypen. Lägg till offlineverktyg för frekvensrespons och ljudrendering som utvecklingsstöd; de ska inte synas i produktgränssnittet.

Klart när: alla fem uttrycker olika klangliga idéer, deras makron går att använda under uppspelning och byten inte ger störande klick eller instabilitet.

### Etapp 3 — Lyssning och kalibrering

Be om fyra till sex representativa padklipp om de inte redan finns: mörk, övertonsrik, luftig, rörlig och gärna olika ackordregister. Torr betyder här före den nya skulpteringen; synthens normala inbyggda effekter får finnas kvar.

Testa grundläget och makronas ytterlägen samt några kombinationer. Lyssna både solo och över ett befintligt lager. Jämför även nivåanpassade renderingar offline så att starkare ljud inte systematiskt vinner. Bygg inte in denna testnormalisering i produkten.

Logga kort: källa, preset, makron, användbar roll, problem och föreslagen ändring. Produktägaren bedömer musikalisk användbarhet; Codex rapporterar tekniska mätningar och får inte påstå sig ha hört filer som inte har kunnat avlyssnas.

Om klipp saknas: fortsätt teknisk implementation med syntetiska testkällor och märk banken som preliminär. Lås inte sweetspots på basis av brus och svep ensamt.

Klart när: de fem rollerna är tydliga på flera källor och makrona förbättrar nästan bra resultat utan att kräva ett avancerat läge. Återkommande svaga lägen omarbetas.

### Etapp 4 — Utöka till 15

Fyll ut familjerna med övriga tio lägen. Dokumentera grundfilter och makroankare per preset. Jämför närliggande lägen; byt ut redundanta former. Behåll samma begränsade gränssnitt.

Klart när: samtliga 15 har en avsikt och en dokumenterad plats i banken. Varje läge har minst ett representativt ljudexempel där produktägaren finner det användbart; vissa får vara nischade.

### Etapp 5 — Logic och leverans

Verifiera AU-validering, instansiering, uppspelning, automation, projektåteröppning, bypass och offlinebounce på målmaskinen. Kontrollera flera samtidiga instanser eftersom arbetsflödet bygger på layering. Dokumentera uppmätt prestanda med maskin, samplingsfrekvens, buffertstorlek och antal instanser.

Leverera källkod, bygginstruktioner, AU-byggartefakt där miljön tillåter, kort användarguide och lista över kvarstående begränsningar. Signering, notarisation och publikt distributionspaket blir aktuella först om pluginen ska distribueras utanför den egna arbetsmiljön.

## 9. Verifiering och acceptanskriterier

Automatisera tester för de konkreta DSP-riskerna:

- Responsen överensstämmer med varje presets dokumenterade mål vid neutrala makron.
- Tystnad, impulser, brus och svep ger ändlig output utan NaN/Inf eller självförstärkande förlopp.
- Makrogränser, kombinationer, snabba klangbyten och automation förblir stabila.
- Prova 44,1/48/88,2/96 kHz och flera blockstorlekar, inklusive små och oregelbundna. Klampa frekvenser säkert under Nyquist vid andra stödda frekvenser.
- State round-trip återställer samma parametrar och statiska ljudresultat efter korrekt filteruppstart.
- Stereoidentitet och mono fungerar; ingen kanal läcker till den andra.
- Övergångar kontrolleras med både signalmätning och lyssning; inga absoluta peakgränser uppfinns utan angiven testsignal och ingångsnivå.

Produktens viktigaste acceptanskriterium är praktiskt: användaren kan hitta ett intressant lager, justera det med tre rattar och gå vidare utan att sakna individuell bandkontroll. Teknisk stabilitet bevisar inte detta; lyssningssteget behövs.

## 10. Instruktion till implementerande Codex

Använd detta dokument som produktmål och arbetsplan. Börja med etapp 1 och 2, håll lösningen liten och arbeta fram till en spelbar femlägesprototyp. Gör rimliga tekniska val inom ramen utan att fråga om varje detalj. Dokumentera valen och deras konsekvenser.

Frekvenser, lutningar, Q och makroankare är startförslag. Produktprinciperna — lyssna, välj, tre rattar, ingen bandredigering — är starkare krav. Lägg inte till nya funktioner för att lösa tuningproblem. Om målen kräver annan DSP-topologi, ändra implementationen och bevara användarflödet.

Markera tydligt vad som är implementerat, tekniskt verifierat, testat i Logic respektive musikaliskt utvärderat. Påstå inte att de 15 sweetspotsen är färdiga förrän lyssningsunderlag finns. Vid slutet av varje etapp: redovisa resultat, relevanta tester och nästa konkreta steg.

## Referens

[AudioThing Octaves](https://www.audiothing.net/effects/octaves/) är produktägarens referens för kreativ bandpassfiltrering. Den nya pluginens presetroller, fria frekvensplacering och begränsade makron är egna designförslag; ingen emulering av Octaves kretsmodell ingår.
