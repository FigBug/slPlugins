declare id "auto"; // selector crybaby / autowah
declare name "auto";

//-----------------------------------------------
//     Auto-Wah
//-----------------------------------------------

import("effect.lib"); //for crybaby definition
import("guitarix.lib");

l = crybaby_ctrl.level;
a = crybaby_ctrl.wah;
w = crybaby_ctrl.wet_dry;

d = 1-w;
Sum(n,x) = +(x - (x @ n)) ~_ ;

Average(n,x) = x * (1<<22) : int : abs : Sum(n) : float : /(1<<22)
                 : /(n);

Map(x) = x * a : max(0) : min(1) ;
process(x) = x : _<:*(d),(*(w):*(l) :a * crybaby(x : amp_follower_ud(0.01,0.1) : min(1)) + (1.0-a) * x):>_ ;
