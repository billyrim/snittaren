#include "Engine.h"
#include <iostream>
#include <fstream>
#include <random>
#include <complex>
#include <cstdlib>
void check(bool v){if(!v){std::cerr<<"FAIL\n";std::exit(1);}}
int main(int argc,char** argv){
 std::mt19937 rng(42);std::uniform_real_distribution<float> noise(-.2f,.2f);double peak=0,delta=0;
 for(double rate:{44100.,48000.,88200.,96000.,22050.,192000.})for(int mode=0;mode<5;++mode)for(int combo=0;combo<27;++combo){snitt::Engine e;e.prepare(rate);std::array<double,3> m{double(combo%3-1),double(combo/3%3-1),double(combo/9-1)};float last=0;for(int n=0;n<int(rate*.15);++n){float x=noise(rng),v[2]{x,x};e.sample(v,2,mode,m,0,false);check(std::isfinite(v[0])&&v[0]==v[1]);peak=std::max(peak,double(std::abs(v[0])));delta=std::max(delta,double(std::abs(v[0]-last)));last=v[0];}}
 snitt::Engine e;e.prepare(48000);for(int n=0;n<96000;++n){float v[2]{n==0?.5f:0,0};e.sample(v,2,(n/137)%5,{std::sin(n*.001),std::cos(n*.003),std::sin(n*.002)},0,(n/977)%2);check(std::isfinite(v[0])&&v[1]==0);if(n>90000)check(std::abs(v[0])<1e-10);}
 std::cout<<"Stability / 27 macro combinations / stereo / isolation / decay PASS; noise input +/-0.2 peak="<<peak<<" sample delta="<<delta<<"\n";
 // Measured complex response of full parallel sum, after settling selection.
 std::ofstream csv(argc>1?argv[1]:"response.csv");csv<<"mode,hz,db\n";
 for(int p=0;p<5;++p){std::array<double,9> levels{};int j=0;for(double f:{80.,300.,400.,650.,1300.,2500.,4500.,8000.,12000.}){snitt::Engine filter;filter.prepare(48000);std::complex<double> acc{};for(int n=0;n<48000;++n){float v[2]{float(.1*std::sin(2*3.141592653589793*f*n/48000)),0};filter.sample(v,1,p,{0,0,0},0,false);if(n>=24000)acc+=double(v[0])*std::exp(std::complex<double>(0,-2*3.141592653589793*f*n/48000));}double db=20*std::log10(std::abs(acc)*2/24000/.1+1e-20);levels[j++]=db;csv<<snitt::presets[p].id<<","<<f<<","<<db<<"\n";}
 if(p==0)check(levels[2]>levels[0]+20&&levels[2]>levels[7]+35);
 if(p==1)check(levels[7]>levels[2]+50);
 if(p==2)check(levels[0]>levels[4]+20&&levels[7]>levels[4]+20);
 if(p==3)check(levels[0]>levels[4]+15&&levels[7]>levels[4]+15);
 if(p==4)check(levels[1]>levels[0]+20&&levels[4]>levels[3]+10);
 }
 std::cout<<"Neutral response roles PASS\n";
 double jump=0;snitt::Engine transition;transition.prepare(48000);float previous=0;
 for(int n=0;n<480000;++n){float v[2]{float(.1*std::sin(2*3.141592653589793*440*n/48000)),0};
 transition.sample(v,1,(n/401)%5,{(n/307)%2?1.:-1.,(n/701)%2?1.:-1.,(n/199)%2?1.:-1.},(n/1709)%2?12:-24,(n/1301)%2);
 jump=std::max(jump,double(std::abs(v[0]-previous)));previous=v[0];check(std::isfinite(v[0]));}
 // Conservative discontinuity regression bound for this 0.1-peak 440 Hz sine only.
 check(jump<.05);std::cout<<"Rapid mode/macro/output/bypass 440Hz sine stress: max sample delta="<<jump<<" (<0.05) PASS; not a listening test\n";
 for(int p=0;p<5;++p){snitt::Engine sweep;sweep.prepare(48000);double phase=0;for(int n=0;n<96000;++n){double frequency=20*std::pow(1000.,n/96000.);phase+=2*3.141592653589793*frequency/48000;float v[2]{float(.1*std::sin(phase)),0};sweep.sample(v,1,p,{0,0,0},0,false);check(std::isfinite(v[0])&&std::abs(v[0])<1);}}
 std::cout<<"20Hz..20kHz sweep, 0.1 peak input, finite output below 1 PASS\n";
}
