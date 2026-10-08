// Frozen 0.1.1 reference for audio compatibility tests. Do not edit.
#pragma once
#include <array>
#include <cmath>
#include <algorithm>
namespace legacy_v1 {
struct Anchor { double low, neutral, high; };
struct Branch { Anchor hp, lp, gain; int hpMacro, lpMacro, gainMacro; Anchor width {0.8,1.0,1.2}; Anchor bottomReach {1.0,1.0,1.0}; };
struct Preset { const char* id; const char* name; const char* description; int count; std::array<Branch,3> branches; };
constexpr Anchor fixed(double x) { return {x,x,x}; }
// Frozen bank revision 1. Append future modes; never reorder these host indices.
inline constexpr std::array<Preset,5> presets {{
 {"03", "Filt", "Dovt, instängt harmoniskt moln",1,{{{{340,250,170},{480,650,1000},{-9,0,2},0,2,1}}}},
 {"08", "Frost", "Tunn övertonsrest",1,{{{{7000,5000,3500},fixed(16000),{-8,0,2},2,0,1,{0.8,1.0,1.2},{1.15,1.0,0.85}}}}},
 {"10", "Botten + luft", "Två områden med avstånd emellan",2,{{
  {fixed(20),{120,220,330},{-8,0,0},0,0,1},
  {{6500,4000,3000},fixed(22000),{-8,0,0},2,2,1}}}},
 {"13", "Tom mitt", "Urholkad pad med sammanhang kvar",2,{{
  {fixed(20),{220,400,650},{-6,0,0},0,0,1},
  {{4200,2800,1900},fixed(22000),{-6,0,0},2,2,1}}}},
 {"15", "Resonanta öar", "Tre smala, ringande färgfält",3,{{
  {{285,250,200},{315,360,430},fixed(-2),0,0,0},
  {{1250,1100,900},{1350,1500,1800},fixed(-2),1,1,1},
  {{4350,3900,3200},{4650,5200,6100},fixed(-2),2,2,2}}}}
}};
inline double map(Anchor a,double x,bool logarithmic) {
 x=std::clamp(x,-1.0,1.0); double l=x<0?a.low:a.neutral, h=x<0?a.neutral:a.high, t=x<0?x+1:x;
 return logarithmic ? std::exp(std::log(l)+(std::log(h)-std::log(l))*t) : l+(h-l)*t;
}
}
