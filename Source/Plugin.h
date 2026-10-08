#pragma once
#include <juce_audio_utils/juce_audio_utils.h>
#include "Engine.h"
class Snittaren final : public juce::AudioProcessor {
public:
 Snittaren();
 juce::AudioProcessorValueTreeState state;
 snitt::Engine engine;
 std::array<std::atomic<float>*,6> values;
 std::atomic<bool> clipped{false};
 const juce::String getName() const override {return "SNITTAREN";}
 void prepareToPlay(double sr,int) override {engine.prepare(sr);setLatencySamples(0);}
 void releaseResources() override {}
 bool isBusesLayoutSupported(const BusesLayout& l) const override {return l.getMainInputChannelSet()==l.getMainOutputChannelSet() && (l.getMainOutputChannelSet()==juce::AudioChannelSet::mono() || l.getMainOutputChannelSet()==juce::AudioChannelSet::stereo());}
 void processBlock(juce::AudioBuffer<float>&,juce::MidiBuffer&) override;
 void processBlockBypassed(juce::AudioBuffer<float>&,juce::MidiBuffer&) override;
 void process(juce::AudioBuffer<float>&,bool);
 bool acceptsMidi() const override {return false;} bool producesMidi() const override {return false;}
 double getTailLengthSeconds() const override {return .5;}
 bool hasEditor() const override {return true;}
 juce::AudioProcessorEditor* createEditor() override;
 int getNumPrograms() override {return 1;} int getCurrentProgram() override {return 0;} void setCurrentProgram(int) override {}
 const juce::String getProgramName(int) override {return {}; } void changeProgramName(int,const juce::String&) override {}
 void getStateInformation(juce::MemoryBlock&) override;
 void setStateInformation(const void*,int) override;
 juce::AudioProcessorParameter* getBypassParameter() const override {return state.getParameter("bypass");}
};
