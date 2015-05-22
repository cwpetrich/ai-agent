#
#
#
our $aps =
[
 {
   name => 'RandomPick',
   key  => 'r',
   header => 'RandomPick.h',
   cpp    => ['RandomPick.cpp'],
   constructor => 'ai::Sample::RandomPick()',
 },
 {
   name => 'Dumbo1',
   key  => 'd',
   header => 'Dumbo1.h',
   cpp    => ['Dumbo1.cpp'],
   constructor => 'ai::Sample::Dumbo1()',
 },
 {
   name => 'Dumbo2',
   key  => 'D',
   header => 'Dumbo2.h',
   cpp    => ['Dumbo2.cpp'],
   constructor => 'ai::Sample::Dumbo2()',
 },
# {
#   name => 'Dumbo3',
#   key  => '3',
#   header => 'Dumbo3.h',
#   cpp    => ['Dumbo3.cpp'],
#   constructor => 'ai::Sample::Dumbo3()',
# },
 {
   name => 'Smarty',
   key  => 's',
   header => 'Smarty.h',
   cpp    => ['Smarty.cpp'],
   constructor => 'ai::Sample::Smarty(opts)',
 },
 {
   name => 'Pikachu',
   key  => 'p',
   header => 'Pikachu.h',
   cpp    => ['Pikachu.cpp'],
   constructor => 'cgl::Pikachu()',
 },
#{
#  name => '',
#  key  => '',
#  header => '',
#  constructor => '',
#},
 ];

1;
