#!/usr/bin/env python3
"""Local calibration support; requires numpy and macOS afconvert, no plugin changes."""
import argparse
import csv
import hashlib
import json
import math
from pathlib import Path
import subprocess
import wave
import numpy as np

MODES = {'03':'Filt','08':'Frost','10':'Botten + luft','13':'Tom mitt','15':'Resonanta öar', '01':'Underström', '02':'Mörk kropp', '04':'Trä',
 '05':'Avlägsen radio', '06':'Strängrest', '07':'Skimmer', '09':'Luftrest',
 '11':'Kropp + skimmer', '12':'Två röster', '14':'Sprickor'}
SETTINGS = {'neutral':(0,0,0), 'bottom-minus':(-100,0,0), 'bottom-plus':(100,0,0),
 'middle-minus':(0,-100,0), 'middle-plus':(0,100,0), 'top-minus':(0,0,-100),
 'top-plus':(0,0,100), 'all-minus':(-100,-100,-100), 'all-plus':(100,100,100),
 'open-edges':(100,-100,100)}

def read_wav(path):
    with wave.open(str(path),'rb') as w:
        channels, width, rate, frames = w.getnchannels(), w.getsampwidth(), w.getframerate(), w.getnframes()
        raw = w.readframes(frames)
    if width == 2:
        x = np.frombuffer(raw,dtype='<i2').astype(np.float64)/32768
    elif width == 3:
        b = np.frombuffer(raw,dtype=np.uint8).reshape(-1,3).astype(np.int32)
        v = b[:,0] | b[:,1]<<8 | b[:,2]<<16
        v = np.where(v & 0x800000, v-0x1000000, v)
        x = v.astype(np.float64)/8388608
    else:
        raise ValueError(f'Unsupported PCM width: {width}')
    return rate, x.reshape(-1,channels)

def write_wav(path, rate, x):
    assert np.isfinite(x).all() and np.max(np.abs(x)) < 1, 'Output would clip'
    pcm = np.rint(x*8388607).astype(np.int32).reshape(-1)
    data = np.column_stack((pcm & 255,(pcm>>8)&255,(pcm>>16)&255)).astype(np.uint8)
    with wave.open(str(path),'wb') as w:
        w.setnchannels(x.shape[1]);w.setsampwidth(3);w.setframerate(rate);w.writeframes(data.tobytes())

def db(v): return 20*math.log10(max(float(v),1e-15))
def metrics(x):
    return {'rms_dbfs':db(np.sqrt(np.mean(x*x))), 'peak_dbfs':db(np.max(np.abs(x)))}

def bands(x,rate):
    # Average stereo power (no mono cancellation), Hann windows, no perceptual weighting.
    size=16384
    power=np.zeros(size//2+1)
    win=np.hanning(size)[:,None]
    for pos in range(0,len(x)-size+1,size):
        power += np.sum(np.abs(np.fft.rfft(x[pos:pos+size]*win,axis=0))**2,axis=1)
    freq=np.fft.rfftfreq(size,1/rate);total=max(float(power.sum()),1e-30)
    return {label:round(float(power[(freq>=lo)&(freq<hi)].sum()/total*100),3)
            for label,lo,hi in [('below250',0,250),('250to2500',250,2500),('2500to5000',2500,5000),('above5000',5000,rate/2+1)]}

def main():
    parser=argparse.ArgumentParser();parser.add_argument('sources',type=Path);parser.add_argument('output',type=Path)
    args=parser.parse_args();root=args.output.resolve()
    if root.exists(): raise SystemExit('Use a new output directory; existing results are preserved.')
    sources=sorted(p for p in args.sources.iterdir() if p.suffix.lower() in ('.aif','.aiff','.wav'))
    if not sources: raise SystemExit('No sources found')
    root.mkdir(parents=True);(root/'inputs').mkdir();(root/'converted').mkdir()
    renderer=Path(__file__).resolve().parents[1]/'build/render_pad'
    manifest=[];rows=[]
    for source in sources:
        print(f'Preparing {source.name}',flush=True)
        converted=root/'converted'/(source.stem+'.wav')
        subprocess.run(['afconvert','-f','WAVE','-d','LEI24',str(source),str(converted)],check=True)
        rate,x=read_wav(converted);assert x.shape[1] in (1,2) and np.isfinite(x).all()
        seconds=len(x)/rate;length=min(len(x),12*rate)
        positions=list(range(0,len(x)-length+1,6*rate)) or [0]
        energy=np.array([np.mean(x[p:p+length]**2) for p in positions])
        active=energy>max(float(energy.max())*0.01,1e-12)
        candidates=np.flatnonzero(active)
        if not len(candidates): raise ValueError(f'Silent source: {source.name}')
        median=np.median(energy[candidates]);choice=candidates[np.argmin(abs(energy[candidates]-median))]
        start=positions[choice]
        # Include two seconds of source context for filter warmup, exclude from comparisons.
        context=max(0,start-2*rate);crop=start-context
        segment=x[context:start+length]*10**(-12/20)
        path=root/'inputs'/(source.stem+'.wav');write_wav(path,rate,segment)
        selected=x[start:start+length]
        item={'source':str(source.resolve()),'sha256':hashlib.sha256(source.read_bytes()).hexdigest(),
          'duration_seconds':seconds,'rate':rate,'channels':x.shape[1], 'source_metrics':metrics(x),
          'excerpt_start_seconds':start/rate,'excerpt_seconds':length/rate,
          'excerpt_metrics':metrics(selected),'spectral_energy_percent':bands(selected,rate),
          'common_input_trim_db':-12,'warmup_seconds':crop/rate}
        manifest.append(item)
        for setting,macros in SETTINGS.items():
            folder=root/source.stem/setting
            subprocess.run([str(renderer),str(path),str(folder),*map(str,macros)],check=True,stdout=subprocess.DEVNULL)
            for mode in ['dry',*MODES]:
                wav=folder/(mode+'.wav');sr,y=read_wav(wav);assert sr==rate and len(y)==len(segment)
                y=y[crop:crop+length];assert len(y)==length and np.isfinite(y).all()
                # Originals are never modified; trim only generated output to the chosen interval.
                write_wav(wav,rate,y)
                m=metrics(y)
                assert m['peak_dbfs'] < -.01, f'Unexpected clipping: {wav}'
                row={'source':source.stem,'setting':setting,'mode':mode,'bottom':macros[0],'middle':macros[1],'top':macros[2],**m,
                  'predicted_peak_at_original_input_dbfs':m['peak_dbfs']+12,'matching_gain_db':'','matched_rms_dbfs':'','matched_peak_dbfs':''}
                if setting=='neutral':
                    target=min(-24,item['excerpt_metrics']['rms_dbfs']-12)
                    gain=0 if m['rms_dbfs'] < -65 else min(12,target-m['rms_dbfs'],-1-m['peak_dbfs'])
                    matched=y*10**(gain/20);dest=root/source.stem/'matched';dest.mkdir(exist_ok=True)
                    write_wav(dest/(mode+'.wav'),rate,matched)
                    row.update(matching_gain_db=gain,matched_rms_dbfs=metrics(matched)['rms_dbfs'],matched_peak_dbfs=metrics(matched)['peak_dbfs'])
                rows.append(row)
        # Converted full files are temporary products owned by this script.
        converted.unlink()
    (root/'manifest.json').write_text(json.dumps(manifest,indent=2,ensure_ascii=False)+'\n')
    with (root/'measurements.csv').open('w') as f:
        writer=csv.DictWriter(f,fieldnames=rows[0]);writer.writeheader();writer.writerows(rows)
    lines=['# SNITTAREN – lyssningspaket, bankrevision 2','',
      'Egna källor, 12-sekundersutdrag. Börja med neutrala lägen.',
      'Gemensam ingångsdämpning −12 dB i alla renderingar ger mätutrymme. Originalen är orörda.',
      'Nivåanpassningen är stereo-RMS, inte LUFS eller perceptuellt exakt: mål högst −24 dBFS RMS,',
      'högst +12 dB lyft, sample-peak högst −1 dBFS. Material under −65 dBFS RMS lyfts inte.',
      'Frost kan fortfarande vara svagare än övriga när förstärkningsgränsen nås.',
      'Lyssna först solo, sedan ihop med ett eget befintligt lager. En agent har inte avlyssnat filerna.','',
      '| Källa och start | Torr | '+' | '.join(MODES.values())+' |','|'+'---|'*(len(MODES)+2)]
    for item in manifest:
        stem=Path(item['source']).stem
        links=[f'[{"Torr" if m=="dry" else MODES[m]}]({root / stem / "matched" / (m+".wav")})' for m in ['dry',*MODES]]
        lines.append(f'| {stem}, {item["excerpt_start_seconds"]:.0f} s | '+' | '.join(links)+' |')
    lines+=['','Tabellen länkar till nivåanpassade neutrala versioner. I varje källmapp finns också',
      '`neutral` med faktisk relativ nivå samt `bottom-minus/plus`, `middle-minus/plus`,',
      '`top-minus/plus`, `all-minus`, `all-plus` och `open-edges` (100/−100/100).',
      'Återkoppla gärna källa + klang + inställning + användbar roll/problem.','',
      'Exakta tidspositioner, källhashar och mätningar finns i manifest.json och measurements.csv.']
    (root/'README.md').write_text('\n'.join(lines)+'\n')
    print(f'Done: {len(rows)} measured renders + {len(sources)*(len(MODES)+1)} matched copies in {root}',flush=True)

if __name__=='__main__': main()
