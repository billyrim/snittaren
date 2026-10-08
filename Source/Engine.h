#pragma once
#include "Presets.h"
namespace snitt {
// TPT state variable sections: positive g and damping, no coefficient interpolation.
struct SVF {
 double g=0,k=1.4142135623730951,a=1,s1=0,s2=0;
 void tune(double f,double sr,double q) {g=std::tan(3.141592653589793*std::clamp(f,10.0,sr*.45)/sr); k=1/q;a=1/(1+g*(g+k));}
 double tick(double x,bool high) {double v1=a*(s1+g*(x-s2)),v2=s2+g*v1;s1=2*v1-s1;s2=2*v2-s2;return high?x-k*v1-v2:v2;}
};
struct Engine {
 struct Lane { std::array<SVF,4> filters; double gain=1; };
 std::array<std::array<std::array<Lane,3>,2>,5> bank{};
 double sr=48000, alpha=0,fade=0,output=1,wet=1;
 std::array<double,3> macros{}; std::array<double,5> weights{0,0,0,1,0}; int tickCount=0;
 void prepare(double rate) { *this=Engine{};sr=rate;alpha=1-std::exp(-1/(sr*.03));fade=1-std::exp(-1/(sr*.025));tune(); }
 void tune() {
  for(int p=0;p<5;++p) for(int b=0;b<presets[p].count;++b) {
   auto d=presets[p].branches[b];double hp=map(d.hp,macros[d.hpMacro],true),lp=map(d.lp,macros[d.lpMacro],true);
   double width=map(d.width,macros[d.gainMacro],true);hp/=width;lp*=width;hp*=map(d.bottomReach,macros[0],true);
   for(int c=0;c<2;++c) {auto& l=bank[p][c][b];l.gain=std::pow(10.,map(d.gain,macros[d.gainMacro],false)/20.);
    for(int i=0;i<4;++i) l.filters[i].tune(i<2?hp:lp,sr,p==4?1.05:0.7071067811865476);
   }
  }
 }
 void sample(float* channels,int count,int preset,const std::array<double,3>& target,double db,bool bypass) {
  for(int i=0;i<3;++i) macros[i]+=alpha*(std::clamp(target[i],-1.,1.)-macros[i]);
  output+=alpha*(std::pow(10.,std::clamp(db,-24.,12.)/20)-output);wet+=alpha*((bypass?0.:1.)-wet);
  preset=std::clamp(preset,0,4);for(int p=0;p<5;++p) weights[p]+=fade*((preset==p?1.:0.)-weights[p]);
  if((tickCount++ & 31)==0)tune();
  for(int c=0;c<count;++c) {double x=std::isfinite(channels[c])?channels[c]:0,y=0;
   for(int p=0;p<5;++p) {double sum=0;for(int b=0;b<presets[p].count;++b) {auto& l=bank[p][c][b];double v=x;for(int i=0;i<4;++i)v=l.filters[i].tick(v,i<2);sum+=v*l.gain;}y+=weights[p]*sum;}
   double result=x*(1-wet)+y*output*wet;channels[c]=std::isfinite(result)?float(result):0;
  }
 }
};
}
