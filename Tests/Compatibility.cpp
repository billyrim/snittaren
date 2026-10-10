#include "Engine.h"
#include "LegacyV2/Engine.h"
#include <iostream>
#include <random>
int main() {
 for(double rate:{44100.,48000.,96000.}) {
  snitt::Engine current; legacy_v2::Engine old;
  current.prepare(rate);old.prepare(rate);
  std::mt19937 rng(73);std::uniform_real_distribution<float> noise(-.2f,.2f);
  for(int n=0;n<96000;++n) {
   int mode=(n/4001)%15;
   std::array<double,3> macros{std::sin(n*.0002),std::cos(n*.0007),std::sin(n*.0009)};
   float a[2]{noise(rng),noise(rng)},b[2]{a[0],a[1]};
   const double gain=(n/9001)%2?3:-6;const bool bypass=(n/7111)%2;
   current.sample(a,2,mode,macros,gain,bypass);old.sample(b,2,mode,macros,gain,bypass);
   if(a[0]!=b[0] || a[1]!=b[1]) {std::cerr<<"Legacy audio changed: "<<rate<<" / "<<n<<"\n";return 1;}
  }
 }
 std::cout<<"Original 15 modes: sample-identical with frozen 0.2.0 engine at 44.1/48/96k, automated macros/modes/output/bypass PASS\n";
}
