package Modular {
 private import ScalarValues::*;
 part def Square { in attribute x : Real; out attribute y : Real = x * x; }
 part def Monitor {
  in attribute mic : Real;
  in attribute threshold : Real;
  part processing : Square;
  bind processing.x = mic;
  out attribute level : Real = processing.y;
  out attribute alarm : Boolean = level > threshold;
 }
 part system : Monitor;
}