#pragma once
#include <array>
#include <cmath>
#include <algorithm>
namespace legacy_v2 {
struct Anchor { double low, neutral, high; };
struct Branch { Anchor hp, lp, gain; int hpMacro, lpMacro, gainMacro; Anchor width {0.8,1.0,1.2}; Anchor bottomReach {1.0,1.0,1.0}; };
struct Cut { Anchor frequency, depth, q; int macro; };
struct Preset { const char* id; const char* name; const char* description; int count; std::array<Branch,3> branches; double q=0.7071067811865476; int cutCount=0; std::array<Cut,3> cuts{}; };
constexpr Anchor fixed(double x) { return {x,x,x}; }
// Bank revision 2: indices 0..4 and their curves are frozen from revision 1.
// Append future modes; never reorder released host indices.
inline constexpr std::array<Preset,15> presets {{
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
  {{4350,3900,3200},{4650,5200,6100},fixed(-2),2,2,2}}},1.05},
 // 5: Underström. Keep sub-bass; bottom controls subsonic edge, top the LP.
 {"01", "Underström", "Fundament under andra lager",1,{{
  {{35,20,12},{110,180,260},{-6,0,0},0,2,1,{0.85,1,1.15}}}}},
 // 6: Mörk kropp.
 {"02", "Mörk kropp", "Mörk ackordkropp med mindre djupbas",1,{{
  {{180,120,75},{300,450,650},{-6,0,1},0,2,1,{0.85,1,1.15}}}}},
 // 7: Trä.
 {"04", "Trä", "Brett, ihåligt mellanregister",1,{{
  {{600,450,320},{850,1200,1650},{-6,0,1},0,2,1,{0.85,1,1.15}}}}},
 // 8: Avlägsen radio.
 {"05", "Avlägsen radio", "Litet, avlägset lager",1,{{
  {{1200,900,700},{1600,2200,2900},{-8,0,1},0,2,1,{0.9,1,1.12}}}}},
 // 9: Strängrest. Modest edge resonance, no added harmonics.
 {"06", "Strängrest", "Strävhet och tydliga övertoner",1,{{
  {{1900,1500,1200},{2700,3500,4500},{-7,-1,0},0,2,1,{0.9,1,1.12}}}},0.85},
 // 10: Skimmer. Bottom gently opens the lower edge; top opens it further.
 {"07", "Skimmer", "Ljust lager med kvarvarande harmonik",1,{{
  {{3500,2500,1800},fixed(20000),{-6,0,1},2,2,1,{0.9,1,1.1},{1.12,1,0.9}}}}},
 // 11: Luftrest. Retain the extreme high-pass character at all settings.
 {"09", "Luftrest", "Lufttextur på övertonsrika källor",1,{{
  {{10500,8000,6500},fixed(22000),{-6,0,1},2,2,1,{0.95,1,1.05},{1.08,1,0.94}}}}},
 // 12: Kropp + skimmer. Middle opens width in both separated branches.
 {"11", "Kropp + skimmer", "Dov kropp med fristående skimmer",2,{{
  {{340,250,180},{450,600,800},{-6,0,0},0,0,1,{0.9,1,1.1}},
  {{5000,3500,2800},fixed(22000),{-6,-2,0},2,2,1,{0.9,1,1.1}}}}},
 // 13: Två röster. Bottom/top open their respective vowel-like windows.
 {"12", "Två röster", "Två färgade fönster",2,{{
  {{500,400,310},{650,800,1000},{-6,-1,0},0,0,1,{0.9,1,1.1}},
  {{1900,1600,1300},{2400,2800,3400},{-6,-1,0},2,2,1,{0.9,1,1.1}}}},0.8},
 // 14: Sprickor. Three finite-depth bell cuts; each macro narrows and shallows one cut.
 {"14", "Sprickor", "Fragmenterad övertonsstruktur",1,{{
  {fixed(20),fixed(22000),fixed(0),0,2,1,fixed(1)}}},0.7071067811865476,3,{{
   {fixed(350),{-42,-30,-18},{0.65,1.1,1.8},0},
   {fixed(1100),{-48,-36,-20},{0.6,0.9,1.5},1},
   {fixed(3200),{-42,-27,-15},{0.5,0.8,1.4},2}}}}
}};
inline constexpr int presetCount = static_cast<int>(presets.size());
inline double map(Anchor a,double x,bool logarithmic) {
 x=std::clamp(x,-1.0,1.0); double l=x<0?a.low:a.neutral, h=x<0?a.neutral:a.high, t=x<0?x+1:x;
 return logarithmic ? std::exp(std::log(l)+(std::log(h)-std::log(l))*t) : l+(h-l)*t;
}
}
