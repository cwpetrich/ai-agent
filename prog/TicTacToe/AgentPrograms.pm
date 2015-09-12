#
#
#
our $aps =
[
 {
   name => 'Manual',
   key  => 'M',
   header => 'aiSampleManual.h',
   cpp    => ['aiSampleManual.cpp'],
   constructor => 'ai::Sample::Manual(opts)',
 },
 {
   name => 'Random',
   key  => 'r',
   header => 'aiSampleRandom.h',
   cpp    => ['aiSampleRandom.cpp'],
   constructor => 'ai::Sample::Random(opts)',
 },
 {
   name => 'Maximum',
   key  => 'm',
   header => 'aiSampleMaximum.h',
   cpp    => ['aiSampleMaximum.cpp'],
   constructor => 'ai::Sample::Maximum(opts)',
 },
 {
   name => 'Minimax',
   key  => 'x',
   header => 'aiSampleMinimax.h',
   cpp    => ['aiSampleMinimax.cpp'],
   constructor => 'ai::Sample::Minimax(opts)',
 },
#{
#  name => '',
#  key  => '',
#  header => '',
#  constructor => '',
#},
 ];

1;
