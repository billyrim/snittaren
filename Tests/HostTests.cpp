#include "../Source/Plugin.h"
#include <chrono>
#include <iostream>
#include <cstdlib>
static void require(bool ok,const char* label) { if(!ok){std::cerr<<label<<" FAILED\n";std::exit(1);} }
static void set(Snittaren& p,const char* id,float value) {auto* v=p.state.getParameter(id);v->setValueNotifyingHost(v->convertTo0to1(value));}
int main(int argc,char** argv) {
 juce::ScopedJuceInitialiser_GUI init;
 Snittaren a,b;
 for(auto id:{"mode","bottom","middle","top","output","bypass"})a.state.getParameter(id)->setValueNotifyingHost(.73f);
 juce::MemoryBlock data;a.getStateInformation(data);b.setStateInformation(data.getData(),int(data.getSize()));
 for(int i=0;i<6;++i)require(a.values[i]->load()==b.values[i]->load(),"State parameters");
 a.prepareToPlay(48000,127);b.prepareToPlay(48000,127);juce::MidiBuffer midi;
 juce::AudioBuffer<float>x(2,127),y(2,127);
 for(int n=0;n<1000;++n){for(int c=0;c<2;++c)for(int i=0;i<127;++i)x.setSample(c,i,.2f*std::sin(float(n*127+i)*.07f));y.makeCopyOf(x);a.processBlock(x,midi);b.processBlock(y,midi);for(int c=0;c<2;++c)for(int i=0;i<127;++i)require(x.getSample(c,i)==y.getSample(c,i),"State audio");}
 std::cout<<"State and audio round-trip PASS\n";
 for(double rate:{44100.,48000.,88200.,96000.})for(int channels:{1,2}) {
  Snittaren whole,split;auto layout=whole.getBusesLayout();layout.inputBuses.set(0,juce::AudioChannelSet::canonicalChannelSet(channels));layout.outputBuses=layout.inputBuses;
  require(whole.setBusesLayout(layout)&&split.setBusesLayout(layout),"Mono/stereo layout");
  whole.prepareToPlay(rate,8192);split.prepareToPlay(rate,17);
  juce::AudioBuffer<float> full(channels,8192);
  for(int c=0;c<channels;++c)for(int i=0;i<8192;++i)full.setSample(c,i,.1f*std::sin(float(i)*.13f));
  juce::AudioBuffer<float> original;original.makeCopyOf(full);whole.processBlock(full,midi);
  int pos=0,k=0;const int sizes[]{1,17,64,127,511,1024,3};
  while(pos<8192){int size=std::min(sizes[k++%7],8192-pos);juce::AudioBuffer<float> part(channels,size);for(int c=0;c<channels;++c)part.copyFrom(c,0,original,c,pos,size);split.processBlock(part,midi);for(int c=0;c<channels;++c)for(int i=0;i<size;++i)require(part.getSample(c,i)==full.getSample(c,pos+i),"Block independence");pos+=size;}
 }
 std::cout<<"Mono/stereo, 44.1/48/88.2/96 kHz, irregular blocks 1..1024 and oversize block PASS\n";
 Snittaren bypass;bypass.prepareToPlay(48000,512);set(bypass,"bypass",1);set(bypass,"output",12);juce::AudioBuffer<float> dry(2,512);
 for(int block=0;block<200;++block){for(int c=0;c<2;++c)for(int i=0;i<512;++i)dry.setSample(c,i,.125f);bypass.processBlock(dry,midi);}
 require(std::abs(dry.getSample(0,511)-.125f)<1e-6f,"Bypass unity");std::cout<<"Bypass original level despite Output +12 dB PASS\n";
 Snittaren benchmark;benchmark.prepareToPlay(48000,128);juce::AudioBuffer<float> buf(2,128);auto start=std::chrono::steady_clock::now();
 for(int n=0;n<3750;++n){for(int c=0;c<2;++c)for(int i=0;i<128;++i)buf.setSample(c,i,.1f);benchmark.processBlock(buf,midi);}
 double seconds=std::chrono::duration<double>(std::chrono::steady_clock::now()-start).count();std::cout<<"One instance, stereo 48k/128, 10s audio: "<<seconds<<"s wall, "<<seconds/10*100<<"% of one realtime budget (offline measurement)\n";
 if(argc>1){Snittaren preview;std::unique_ptr<juce::AudioProcessorEditor> editor(preview.createEditor());auto img=editor->createComponentSnapshot(editor->getLocalBounds(),true,2);juce::File file=juce::File::getCurrentWorkingDirectory().getChildFile(argv[1]);juce::FileOutputStream stream(file);juce::PNGImageFormat png;require(png.writeImageToStream(img,stream),"Editor render");}
}
