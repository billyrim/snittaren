#pragma once
#include <array>
#include <cmath>
#include <algorithm>
namespace snitt {
struct Anchor { double low, neutral, high; };
struct Branch { Anchor hp, lp, gain; int hpMacro, lpMacro, gainMacro; Anchor width {0.8,1.0,1.2}; Anchor bottomReach {1.0,1.0,1.0}; };
struct Cut { Anchor frequency, depth, q; int macro; };
struct Preset { const char* id; const char* name; const char* description; int count; std::array<Branch,3> branches; double q=0.7071067811865476; int cutCount=0; std::array<Cut,3> cuts{}; };
constexpr Anchor fixed(double x) { return {x,x,x}; }
// Bank revision 3: indices 0..14 and their curves are frozen from revision 2.
// Append future modes; never reorder released host indices.
inline constexpr std::array<Preset,23> presets {{
 {"03", "01", "Dovt, instängt harmoniskt moln",1,{{{{340,250,170},{480,650,1000},{-9,0,2},0,2,1}}}},
 {"08", "02", "Tunn övertonsrest",1,{{{{7000,5000,3500},fixed(16000),{-8,0,2},2,0,1,{0.8,1.0,1.2},{1.15,1.0,0.85}}}}},
 {"10", "03", "Två områden med avstånd emellan",2,{{
  {fixed(20),{120,220,330},{-8,0,0},0,0,1},
  {{6500,4000,3000},fixed(22000),{-8,0,0},2,2,1}}}},
 {"13", "04", "Urholkad pad med sammanhang kvar",2,{{
  {fixed(20),{220,400,650},{-6,0,0},0,0,1},
  {{4200,2800,1900},fixed(22000),{-6,0,0},2,2,1}}}},
 {"15", "05", "Tre smala, ringande färgfält",3,{{
  {{285,250,200},{315,360,430},fixed(-2),0,0,0},
  {{1250,1100,900},{1350,1500,1800},fixed(-2),1,1,1},
  {{4350,3900,3200},{4650,5200,6100},fixed(-2),2,2,2}}},1.05},
 // 5: Underström. Keep sub-bass; bottom controls subsonic edge, top the LP.
 {"01", "06", "Fundament under andra lager",1,{{
  {{35,20,12},{110,180,260},{-6,0,0},0,2,1,{0.85,1,1.15}}}}},
 // 6: Mörk kropp.
 {"02", "07", "Mörk ackordkropp med mindre djupbas",1,{{
  {{180,120,75},{300,450,650},{-6,0,1},0,2,1,{0.85,1,1.15}}}}},
 // 7: Trä.
 {"04", "08", "Brett, ihåligt mellanregister",1,{{
  {{600,450,320},{850,1200,1650},{-6,0,1},0,2,1,{0.85,1,1.15}}}}},
 // 8: Avlägsen radio.
 {"05", "09", "Litet, avlägset lager",1,{{
  {{1200,900,700},{1600,2200,2900},{-8,0,1},0,2,1,{0.9,1,1.12}}}}},
 // 9: Strängrest. Modest edge resonance, no added harmonics.
 {"06", "10", "Strävhet och tydliga övertoner",1,{{
  {{1900,1500,1200},{2700,3500,4500},{-7,-1,0},0,2,1,{0.9,1,1.12}}}},0.85},
 // 10: Skimmer. Bottom gently opens the lower edge; top opens it further.
 {"07", "11", "Ljust lager med kvarvarande harmonik",1,{{
  {{3500,2500,1800},fixed(20000),{-6,0,1},2,2,1,{0.9,1,1.1},{1.12,1,0.9}}}}},
 // 11: Luftrest. Retain the extreme high-pass character at all settings.
 {"09", "12", "Lufttextur på övertonsrika källor",1,{{
  {{10500,8000,6500},fixed(22000),{-6,0,1},2,2,1,{0.95,1,1.05},{1.08,1,0.94}}}}},
 // 12: Kropp + skimmer. Middle opens width in both separated branches.
 {"11", "13", "Dov kropp med fristående skimmer",2,{{
  {{340,250,180},{450,600,800},{-6,0,0},0,0,1,{0.9,1,1.1}},
  {{5000,3500,2800},fixed(22000),{-6,-2,0},2,2,1,{0.9,1,1.1}}}}},
 // 13: Två röster. Bottom/top open their respective vowel-like windows.
 {"12", "14", "Två färgade fönster",2,{{
  {{500,400,310},{650,800,1000},{-6,-1,0},0,0,1,{0.9,1,1.1}},
  {{1900,1600,1300},{2400,2800,3400},{-6,-1,0},2,2,1,{0.9,1,1.1}}}},0.8},
 // 14: Sprickor. Three finite-depth bell cuts; each macro narrows and shallows one cut.
 {"14", "15", "Fragmenterad övertonsstruktur",1,{{
  {fixed(20),fixed(22000),fixed(0),0,2,1,fixed(1)}}},0.7071067811865476,3,{{
   {fixed(350),{-42,-30,-18},{0.65,1.1,1.8},0},
   {fixed(1100),{-48,-36,-20},{0.6,0.9,1.5},1},
   {fixed(3200),{-42,-27,-15},{0.5,0.8,1.4},2}}}},
 // 16: Crossed edges choke a resonant window; opening can close it instead.
 {"16", "16", "",1,{{
  {{90,1400,6200},{9500,2100,180},{-10,-6,-12},0,2,1,{1.7,1,0.55}}}},1.5},
 // 17: Three crowded, inharmonic low-mid resonances move in opposite directions.
 {"17", "17", "",3,{{
  {{110,370,900},{160,440,1100},fixed(-10),0,0,0,fixed(1)},
  {{1800,530,180},{2100,620,230},fixed(-10),1,1,1,fixed(1)},
  {{240,790,2600},{300,880,2900},fixed(-10),2,2,2,fixed(1)}}},1.65},
 // 18: A broad body loses moving chunks; neutral cuts coincide with its edges.
 {"18", "18", "",1,{{
  {fixed(95),fixed(7600),fixed(-3),0,2,1,fixed(1)}}},1.1,3,{{
   {{120,480,4600},{-12,-42,-24},{1.8,0.55,1.3},0},
   {{6200,1700,210},{-45,-24,-48},{0.6,1.5,0.5},1},
   {{700,5200,9500},{-18,-48,-30},{1.6,0.65,1.1},2}}}},
 // 19: Almost nothing between a subsonic ledge and a thin ultrasonic rim.
 {"19", "19", "",2,{{
  {{12,18,45},{28,48,75},{-12,-3,-18},2,0,1,{1.4,1,0.65}},
  {{17500,12500,8000},{19000,15500,11000},{-16,-7,-3},0,0,2,{0.8,1,1.3}}}},1.3},
 // 20: A moving cut hollows out the only narrow resonant passage.
 {"20", "20", "",1,{{
  {{300,1600,5500},{500,2300,7200},fixed(-6),0,0,1,{1.8,1,0.6}}}},1.6,1,{{
   {{7000,1900,170},{-12,-48,-18},{1.8,0.5,1.6},2}}}},
 // 21: Broad overlapping lanes accumulate unequal resonant shoulders.
 {"21", "21", "",3,{{
  {{35,120,700},{2500,950,380},{-14,-9,-16},0,2,1,{0.6,1,1.5}},
  {{2800,650,180},{700,3100,9000},{-16,-10,-14},1,0,2,{1.4,1,0.7}},
  {{600,2200,8500},{12000,5400,1600},{-12,-10,-18},2,1,0,{0.7,1,1.6}}}},1.35},
 // 22: Three pinched upper-mid slivers; outer macro positions revisit lower bands.
 {"22", "22", "",3,{{
  {{800,2700,420},{950,2920,570},fixed(-9),0,0,0,fixed(1)},
  {{6800,4100,1500},{7150,4350,1750},fixed(-9),1,1,1,fixed(1)},
  {{2100,6300,3200},{2350,6650,3500},fixed(-9),2,2,2,fixed(1)}}},1.65},
 // 23: An anchored low drone under a travelling, self-erasing upper fragment.
 {"23", "23", "",2,{{
  {fixed(55),fixed(105),{-15,-6,-18},0,0,1,{0.8,1,1.2}},
  {{420,1800,7800},{850,2600,9800},{-14,-8,-5},2,2,0,{1.5,1,0.65}}}},1.45,2,{{
   {{280,2100,8500},{-36,-48,-12},{0.5,0.8,1.8},0},
   {{11000,4600,650},{-12,-30,-48},{1.8,1,0.55},2}}}}
}};
inline constexpr int presetCount = static_cast<int>(presets.size());
inline double map(Anchor a,double x,bool logarithmic) {
 x=std::clamp(x,-1.0,1.0); double l=x<0?a.low:a.neutral, h=x<0?a.neutral:a.high, t=x<0?x+1:x;
 return logarithmic ? std::exp(std::log(l)+(std::log(h)-std::log(l))*t) : l+(h-l)*t;
}
}
