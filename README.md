# SNITTAREN · 0.3.0

Lokal AUv2-effekt för ambientpads med 23 numrerade presets.
01–15 behåller tidigare ljud; 16–23 är åtta mer experimentella filterkombinationer.
Apple Silicon, macOS 12 eller senare. Se [den nya banken](docs/bank-v3.md).
Byggd och tekniskt testad på macOS 26.5.2, MacBook Air M5, Xcode 26.6.

## Provlyssna i Logic

Prototypen har installerats i `~/Library/Audio/Plug-Ins/Components/SNITTAREN.component`.

1. Spara arbetet och avsluta/öppna Logic om pluginen inte syns.
2. Lägg en inspelad pad på ett audio-spår och slå på cykeluppspelning.
3. Välj en Audio FX-plats → Audio Units → Fronas → SNITTAREN → Stereo
   (eller Mono på monospår).
4. Börja med 04 och alla rattar på 0. Byt klang med väljaren eller pilarna.
5. Vrid Botten, Mitt och Topp och prova även blandade ytterlägen.
   I 16–23 kan riktningen vända, fönster kollapsa och hål flytta sig. Makrona påverkar varandra; de är inte tre vanliga EQ-gainrattar.
6. Anpassa Output. CLIP betyder att utgången nådde ±1; sänk Output eller källnivån.
   Ingen limiter finns. Bypass jämför med originalnivån och ignorerar Output.
7. Dubbelklicka en ratt för 0, eller använd Återställ rattar för alla tre makron.
   Byte av klang behåller makron och Output.
8. Testa Logic-automation för Klang/Botten/Mitt/Topp/Output/Bypass. Spara ett
   testprojekt, öppna igen och jämför även offlinebounce samt flera instanser.

Om effekten saknas: Logic Pro → Inställningar → Plug-in-hanterare, leta efter
SNITTAREN och välj återställning/omskanning. Apples separata kontroll:

```sh
auval -v aufx Snit BlRm
```

Testa solo och ihop med ett befintligt lager. 02 kan bli mycket svagt på mörka
pads. Höj inte nivån obegränsat för att försöka skapa övertoner som saknas.

## Bygg och installera på nytt

CMake 3.22+, Xcode och en lokal JUCE-källkatalog behövs. Denna leverans använder
befintlig JUCE **8.0.13** i `/Users/fronas/Downloads/JUCE-master`, inte en hämtad
eller låst Git-tag. Ingen automatisk nätverkshämtning sker.

```sh
cd /Users/fronas/Documents/ChatGPT/Snittaren
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_OSX_ARCHITECTURES=arm64 \
  -DJUCE_DIR=/Users/fronas/Downloads/JUCE-master
cmake --build build -j 8
ctest --test-dir build --output-on-failure
./scripts/install-local.sh
auval -v aufx Snit BlRm
```

Artefakt: `build/Snittaren_artefacts/Release/AU/SNITTAREN.component`.
Installationsskriptet säkerhetskopierar en befintlig installation och ad hoc-signerar
lokalt. Ingen notarisation, Intel-binär, VST eller fristående produktapp ingår.
Bygg och installation kräver skrivåtkomst till respektive katalog.

JUCE är dubbelt licensierat under AGPLv3 och JUCE-licensen. Se den lokala
`JUCE-master/LICENSE.md` och https://juce.com/get-juce/. Ingen kommersiell
licens eller distributionsrätt har verifierats för användarens konto; ingen
publicering görs här. Bevara JUCE-versionen för reproducerbara framtida byggen.

## Utvecklingsverktyg

```sh
./build/dsp_tests docs/response.csv
./build/host_tests
./build/render_pad --synthetic build/new-listening
./build/render_pad /absolut/sökväg/pad.wav build/my-pad-listening
./build/render_pad /absolut/sökväg/pad.wav build/my-pad-open 100 100 100
```

Renderaren skriver 24-bitars WAV: torr källa samt 23 klanger med valda makron (standard 0/0/0). Ingen
nivånormalisering. Befintliga filer skrivs inte över. WAV/AIFF och andra format
som den lokala JUCE-läsaren stöder kan användas; högst två kanaler.
Syntetiska exempel finns redan i `build/listening-final/` (12 sekunder, 48 kHz stereo).
De är testmaterial och inget underlag för att fastställa musikaliska sweetspots.

## Status

- **Implementerat:** etapp 1–2 och utökningen i etapp 4, 23 klanger, tre makron, Output, mjuk bypass,
  kompakt GUI, värdautomation och versionsmärkt state. Noll rapporterad latens.
- **Tekniskt verifierat:** Release-bygge, CTest, faktisk AU-validering, mätt
  komplex filtersummering och GUI-rendering. Se [aktuellt testprotokoll](docs/bank-v3.md#verifiering-2026-10-10)
  och [historiska tester](docs/verification.md).
- **Logic:** användaren har provat 0.1.0 och rapporterat att den fungerar bra.
  Detaljtest av projektåteröppning, automation och bounce är ännu inte dokumenterat.
  Den uppdaterade 0.3.0 behöver kontrolleras i Logic.
- **Egna pads:** fem klipp har renderats och mätts tekniskt;
  [rapport](docs/pad-analysis.md) och [lyssningsplan](docs/listening.md).
  Musikalisk bedömning återstår. Alla frekvenser och makrogränser är preliminära.
  Alla 23 behöver fortsatt musikalisk utvärdering i Logic.

Implementation och makroankare: [tekniska val](docs/design.md).

## Etapp 3: provlyssning och kalibrering

Version 0.1.1 visar inga klangbeskrivningar eller siffervärden för de tre makrona.
Output behåller dB-värdet. Dubbelklick och Återställ rattar sätter makrona till 0.
Tillverkarnamnet är Fronas. Intern AU-kod `BlRm` och bundle-ID behålls så att
befintliga Logic-projekt kan hitta samma effekt; det synliga namnet styrs separat.
Om Logic visar det äldre tillverkarnamnet, starta om och använd omskanning i
Plug-in-hanteraren vid behov.

Frekvenserna är oförändrade i bankrevision 1. Fem egna pads är nu tekniskt analyserade;
se [lyssningsplan och logg](docs/listening.md). Etapp 3 väntar på musikalisk
återkoppling och ett ljusare klipp för Frost/luftdelen.


## Bankrevision 3

Alla presetnamn är nu nummer, 01–23, i befintlig väljarordning. De åtta nya
lägena ligger sist. [Bankdefinitioner och kompatibilitet](docs/bank-v3.md).
Starta om Logic efter installationen så att den nya versionen laddas.
