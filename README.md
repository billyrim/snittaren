# SNITTAREN · 0.1.0

Lokal AUv2-effekt för ambientpads. Fem preliminära klanglägen: Filt, Frost,
Botten + luft, Tom mitt och Resonanta öar. Apple Silicon, macOS 12 eller senare.
Byggd och tekniskt testad på macOS 26.5.2, MacBook Air M5, Xcode 26.6.

## Provlyssna i Logic

Prototypen har installerats i `~/Library/Audio/Plug-Ins/Components/SNITTAREN.component`.

1. Spara arbetet och avsluta/öppna Logic om pluginen inte syns.
2. Lägg en inspelad pad på ett audio-spår och slå på cykeluppspelning.
3. Välj en Audio FX-plats → Audio Units → Billy Rim → SNITTAREN → Stereo
   (eller Mono på monospår).
4. Börja med Tom mitt och alla rattar på 0. Byt klang med väljaren eller pilarna.
5. Vrid Botten, Mitt och Topp åt höger för att öppna området, åt vänster för att
   begränsa det. Makrona påverkar varandra; de är inte tre vanliga EQ-gainrattar.
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

Testa solo och ihop med ett befintligt lager. Frost kan bli mycket svagt på mörka
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
```

Renderaren skriver 24-bitars WAV: torr källa samt fem neutrala klanger. Ingen
nivånormalisering. Befintliga filer skrivs inte över. WAV/AIFF och andra format
som den lokala JUCE-läsaren stöder kan användas; högst två kanaler.
Syntetiska exempel finns redan i `build/listening-final/` (12 sekunder, 48 kHz stereo).
De är testmaterial och inget underlag för att fastställa musikaliska sweetspots.

## Status

- **Implementerat:** etapp 1–2, fem klanger, tre makron, Output, mjuk bypass,
  kompakt GUI, värdautomation och versionsmärkt state. Noll rapporterad latens.
- **Tekniskt verifierat:** Release-bygge, CTest, faktisk AU-validering, mätt
  komplex filtersummering och GUI-rendering. Se [testprotokoll](docs/verification.md).
- **Logic:** interaktivt test återstår. Datorverktyget gav inte åtkomst till Logic.
  AU-validering är inte samma sak som verifierad användning i Logic.
- **Musikaliskt:** ingen egen pad eller faktisk avlyssning har utvärderats.
  Alla frekvenser och makrogränser är preliminära. De övriga tio lägena ingår inte.

Implementation och makroankare: [tekniska val](docs/design.md).
