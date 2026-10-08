# Egna pads – tekniskt kalibreringsunderlag, 2026-10-08

[Öppna lyssningspaketet](/Users/fronas/Documents/ChatGPT/Snittaren/build/pad-calibration-v1/README.md)

Bankrevision 1, plugin 0.1.1. Ingen DSP- eller presetändring gjord. Ingen faktisk avlyssning av agenten.
Fem AIFF-filer, 16-bitars stereo, 44,1 kHz. Originalen är orörda; SHA-256 kontrollerad efter körning.
Urval: ett 12-sekundersfönster per källa med RMS närmast medianen bland aktiva fönster,
provade var sjätte sekund. Aktiv = över 1 % av största fönsterenergin och över −120 dBFS RMS.
Urvalet är maskinellt och representerar inte nödvändigtvis hela arrangemanget eller bästa musikaliska frasen.
Upp till två sekunders föregående ljud värmer filtren; detta klipps bort efter rendering.

## Källor och valda utsnitt

| Källa | Längd | Utsnitt | Energi över 5 kHz i utsnitt |
|---|---:|---|---:|
| Absenteechords | 46.4 s | 0–12 s | <0,0005 % |
| droplets | 82.1 s | 36–48 s | <0,0005 % |
| eftershards-maskinen | 468.6 s | 162–174 s | <0,0005 % |
| rymdstrakar | 184.0 s | 120–132 s | 0.002 % |
| scalertest | 84.0 s | 6–18 s | <0,0005 % |

Energin är FFT-bandenergi med Hann-fönster, summerad över båda kanalerna utan monosummering.
Det är ingen perceptuell ljushetsbedömning. Avrundat 0 betyder inte att högfrekvensinnehåll saknas helt.

## Neutral klang relativt torrsignal

RMS-differens i dB; samma −12 dB ingångsdämpning för alla, Output 0 dB.

| Källa | Filt | Frost | Botten + luft | Tom mitt | Resonanta öar |
|---|---:|---:|---:|---:|---:|
| Absenteechords | -7.4 | -63.2 | -4.8 | -1.2 | -0.5 |
| droplets | -5.7 | -52.1 | -7.7 | -2.6 | +0.1 |
| eftershards-maskinen | -3.8 | -55.3 | -13.8 | -5.3 | -1.2 |
| rymdstrakar | -7.8 | -45.5 | -6.3 | -2.5 | -2.6 |
| scalertest | -5.0 | -58.3 | -12.8 | -4.2 | +0.3 |

Frosts svaga utgång stämmer med den låga energin över 5 kHz i dessa utsnitt.
Botten + luft kan här främst bedömas som bottenlager. Ett ljusare klipp behövs för att bedöma
Frosts roll och den övre grenen musikaliskt; ändra inte automatiskt dess HP-gräns för att få högre nivå.

## Makro- och headroomkontroll

Fem källor × tio makroinställningar × fem klanger = 250 bearbetade renderingar, samt 50 torra.
Inställningar: neutral; varje makro ensamt ±100; alla −100; alla +100; 100/−100/100.
Alla 300 filer lästa tillbaka: ändliga samples, rätt längd och format, inga fullskaleklipp.
De torra filerna är identiska mellan makroinställningar inom varje källa.
Därtill 30 neutrala nivåanpassade filer, verifierade med peak under −1 dBFS.
Testerna gäller dessa utsnitt och statiska makron. Verklig automation och musikaliska övergångar behöver lyssnas på.

Största uppmätta peak efter testdämpning: -18.59 dBFS (Absenteechords, Resonanta öar, all-plus).
Utan testdämpningen motsvarar det uppskattningsvis -6.59 dBFS vid Output 0 dB, eftersom filtervägen är linjär. Detta är inte en generell peakgaranti.

## Nivåanpassning och lyssning

Neutraljämförelsernas mål-RMS är torrfilens RMS efter −12 dB, dock högst −24 dBFS.
Gain begränsas till högst +12 dB, och sample-peak till −1 dBFS. Filer under −65 dBFS RMS lyfts inte.
Frost lyfts därför inte på dessa fem källor. Två Botten + luft-filer når +12 dB-gränsen och blir
fortfarande något svagare än referensen. Detta är begränsad RMS-anpassning, inte LUFS-matchning.
Normalisering finns endast i testfilerna. Lyssna vid rimlig nivå och höj inte volymen kraftigt för Frost.

Prioriterad återkoppling: källa, klang, eventuell makroändring, användbar roll, störande egenskap.
Bedöm solo och över ett eget lager. Agenten har inte bedömt klang, ringning eller klick genom avlyssning.

Exakta siffror: [CSV](pad-measurements.csv). Källhashar och tidspositioner: [manifest](pad-manifest.json).
