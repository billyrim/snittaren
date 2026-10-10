#include "Plugin.h"
static juce::AudioProcessorValueTreeState::ParameterLayout parameters() {
 juce::AudioProcessorValueTreeState::ParameterLayout p;
 juce::StringArray names;for(auto& d:snitt::presets)names.add(juce::String::fromUTF8(d.name));
 p.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"mode",1},"Klang",names,3));
 for(auto pair: {std::pair{"bottom","Botten"},{"middle","Mitt"},{"top","Topp"}})
  p.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{pair.first,1},pair.second,juce::NormalisableRange<float>(-100,100,0.1f),0));
 p.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"output",1},"Output",juce::NormalisableRange<float>(-24,12,.1f),0));
 p.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"bypass",1},"Bypass",false));return p;
}
Snittaren::Snittaren(): AudioProcessor(BusesProperties().withInput("Input",juce::AudioChannelSet::stereo(),true).withOutput("Output",juce::AudioChannelSet::stereo(),true)),state(*this,nullptr,"SNITTAREN",parameters()) {
 int n=0;for(auto id:{"mode","bottom","middle","top","output","bypass"}) values[n++]=state.getRawParameterValue(id);
}
void Snittaren::process(juce::AudioBuffer<float>& b,bool forced) {
 juce::ScopedNoDenormals guard;
 const int mode=int(values[0]->load());const std::array<double,3> m{values[1]->load()/100.,values[2]->load()/100.,values[3]->load()/100.};
 const double gain=values[4]->load();const bool bypass=forced||values[5]->load()>.5f;
 for(int i=0;i<b.getNumSamples();++i){float v[2]{};for(int c=0;c<b.getNumChannels();++c)v[c]=b.getSample(c,i);engine.sample(v,b.getNumChannels(),mode,m,gain,bypass);for(int c=0;c<b.getNumChannels();++c){b.setSample(c,i,v[c]);if(std::abs(v[c])>=1)clipped.store(true,std::memory_order_relaxed);}}
}
void Snittaren::processBlock(juce::AudioBuffer<float>& b,juce::MidiBuffer&){process(b,false);}
void Snittaren::processBlockBypassed(juce::AudioBuffer<float>& b,juce::MidiBuffer&){process(b,true);}
void Snittaren::getStateInformation(juce::MemoryBlock& b){auto s=state.copyState();s.setProperty("stateVersion",1,nullptr);s.setProperty("bankRevision",3,nullptr);copyXmlToBinary(*s.createXml(),b);}
void Snittaren::setStateInformation(const void* d,int n){auto xml=getXmlFromBinary(d,n);if(!xml||!xml->hasTagName("SNITTAREN"))return;auto s=juce::ValueTree::fromXml(*xml);if(int(s.getProperty("stateVersion",0))!=1||(int(s.getProperty("bankRevision",0))!=1 && int(s.getProperty("bankRevision",0))!=2 && int(s.getProperty("bankRevision",0))!=3))return;
 for(auto child:s){auto* p=state.getParameter(child.getProperty("id").toString());double v=child.getProperty("value");if(!p||!std::isfinite(v))return;}
 state.replaceState(s);
}
class Editor final:public juce::AudioProcessorEditor,private juce::Timer {
 Snittaren& p;juce::ComboBox mode;juce::TextButton prev{"<"},next{">"},reset{juce::String::fromUTF8("Återställ rattar")};juce::ToggleButton bypass{"Bypass"};
 std::array<juce::Slider,4> knobs;std::array<juce::Label,4> labels;juce::Label title,clip;
 using SA=juce::AudioProcessorValueTreeState::SliderAttachment;std::array<std::unique_ptr<SA>,4> attachments;
 std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ma;std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> ba;
 int hold=0;
 void timerCallback() override {if(p.clipped.exchange(false))hold=30;clip.setText(hold-->0?"CLIP":"",juce::dontSendNotification);}
public:
 Editor(Snittaren& proc):AudioProcessorEditor(proc),p(proc){
 setSize(600,320);title.setText("SNITTAREN",juce::dontSendNotification);title.setFont(juce::FontOptions(26));addAndMakeVisible(title);
 int i=1;for(auto& d:snitt::presets)mode.addItem(juce::String::fromUTF8(d.name),i++);
 for(juce::Component* c:std::initializer_list<juce::Component*>{&mode,&prev,&next,&reset,&bypass,&clip})addAndMakeVisible(c);
 ma=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(p.state,"mode",mode);ba=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(p.state,"bypass",bypass);
 prev.onClick=[this]{mode.setSelectedItemIndex((mode.getSelectedItemIndex()+snitt::presetCount-1)%snitt::presetCount);};next.onClick=[this]{mode.setSelectedItemIndex((mode.getSelectedItemIndex()+1)%snitt::presetCount);};
 reset.onClick=[this]{for(auto id:{"bottom","middle","top"}){auto* param=p.state.getParameter(id);param->beginChangeGesture();param->setValueNotifyingHost(param->convertTo0to1(0));param->endChangeGesture();}};
 std::array<const char*,4> ids{"bottom","middle","top","output"},names{"Botten","Mitt","Topp","Output"};
 for(int k=0;k<4;++k){auto& s=knobs[k];s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);s.setTextBoxStyle(k==3?juce::Slider::TextBoxBelow:juce::Slider::NoTextBox,false,80,24);s.setDoubleClickReturnValue(true,0);s.setColour(juce::Slider::rotarySliderFillColourId,juce::Colour(0xffb5d6c0));s.setTextValueSuffix(k==3?" dB":"");s.setTooltip(k==3?"Utgångsnivå":"Förändrar klangen. Dubbelklick: 0.");addAndMakeVisible(s);labels[k].setText(names[k],juce::dontSendNotification);labels[k].setJustificationType(juce::Justification::centred);addAndMakeVisible(labels[k]);attachments[k]=std::make_unique<SA>(p.state,ids[k],s);}
 clip.setColour(juce::Label::textColourId,juce::Colours::salmon);startTimerHz(20);timerCallback();
 }
 void paint(juce::Graphics& g) override {g.fillAll(juce::Colour(0xff18221f));}
 void resized() override {title.setBounds(24,15,400,40);prev.setBounds(24,68,40,36);mode.setBounds(72,68,456,36);next.setBounds(536,68,40,36);
 for(int k=0;k<3;++k){labels[k].setBounds(25+k*185,126,180,24);knobs[k].setBounds(25+k*185,150,180,110);}labels[3].setBounds(325,270,65,24);knobs[3].setBounds(395,251,85,66);reset.setBounds(24,273,145,28);bypass.setBounds(187,273,105,28);clip.setBounds(502,273,70,28);}
};
juce::AudioProcessorEditor* Snittaren::createEditor(){return new Editor(*this);}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter(){return new Snittaren;}
