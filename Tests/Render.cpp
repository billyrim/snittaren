#include "../Source/Plugin.h"
#include <iostream>
int main(int argc,char** argv) {
 juce::ScopedJuceInitialiser_GUI init;
 if(argc<3){std::cout<<"Usage: render_pad input.wav|--synthetic output-directory\n";return 1;}
 juce::AudioFormatManager formats;formats.registerBasicFormats();std::unique_ptr<juce::AudioFormatReader> reader;
 bool synthetic=juce::String(argv[1])=="--synthetic";
 if(!synthetic){reader.reset(formats.createReaderFor(juce::File::getCurrentWorkingDirectory().getChildFile(argv[1])));if(!reader){std::cerr<<"Cannot read input\n";return 2;}}
 const double rate=synthetic?48000:reader->sampleRate;const int channels=synthetic?2:int(reader->numChannels);if(channels>2)return 3;
 const juce::int64 length=synthetic?juce::int64(rate*12):reader->lengthInSamples;
 juce::File dir=juce::File::getCurrentWorkingDirectory().getChildFile(argv[2]);dir.createDirectory();juce::Random random(42);
 for(int mode=-1;mode<5;++mode){snitt::Engine engine;engine.prepare(rate);juce::WavAudioFormat wav;
  auto destination=dir.getChildFile(mode<0?"dry.wav":juce::String(snitt::presets[mode].id)+".wav");
  if(destination.exists()){std::cerr<<"Refusing to overwrite "<<destination.getFullPathName()<<"\n";return 4;}
  std::unique_ptr<juce::AudioFormatWriter> writer(wav.createWriterFor(new juce::FileOutputStream(destination),rate,unsigned(channels),24,{},0));if(!writer)return 5;
  random.setSeed(42);
  // Settle the preset blend in silence; no fade from default Tom mitt in these comparisons.
  for(int n=0;n<int(rate);++n){float v[2]{};engine.sample(v,channels,std::max(0,mode),{0,0,0},0,false);}
  juce::AudioBuffer<float> buffer(channels,512);
  for(juce::int64 pos=0;pos<length;pos+=512){int count=int(std::min(juce::int64(512),length-pos));buffer.clear();
   if(reader)reader->read(&buffer,0,count,pos,true,channels==2);
   else for(int i=0;i<count;++i){double t=double(pos+i)/rate;double v=0;for(double f:{110.,138.591,164.814,220.})for(int harmonic=1;harmonic<=32;++harmonic)v+=.017/harmonic*std::sin(juce::MathConstants<double>::twoPi*f*harmonic*t+.1*std::sin(t*.7));v+=.008*(random.nextFloat()*2-1);v*=std::min(1.,t/1.5)*std::min(1.,(12-t)/2.);for(int c=0;c<channels;++c)buffer.setSample(c,i,float(v));}
   if(mode>=0)for(int i=0;i<count;++i){float v[2]{};for(int c=0;c<channels;++c)v[c]=buffer.getSample(c,i);engine.sample(v,channels,mode,{0,0,0},0,false);for(int c=0;c<channels;++c)buffer.setSample(c,i,v[c]);}
   if(!writer->writeFromAudioSampleBuffer(buffer,0,count))return 6;
  }
 }
 std::cout<<"Rendered dry + five modes; no loudness normalization.\n";
}
