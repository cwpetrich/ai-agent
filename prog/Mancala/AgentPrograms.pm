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
 {
   name => 'MinimaxRandom',
   key  => 'X',
   header => 'aiSampleMinimaxRandom.h',
   cpp    => ['aiSampleMinimaxRandom.cpp'],
   constructor => 'ai::Sample::MinimaxRandom(opts)',
 },
 {
   name => 'MinimaxEvaluation',
   key  => 'e',
   header => 'aiSampleMinimaxEvaluation.h',
   cpp    => ['aiSampleMinimaxEvaluation.cpp'],
   constructor => 'ai::Sample::MinimaxEvaluation(opts)',
 },
 {
   name => 'AlphaBeta',
   key  => 'a',
   header => 'aiSampleAlphaBeta.h',
   cpp    => ['aiSampleAlphaBeta.cpp'],
   constructor => 'ai::Sample::AlphaBeta(opts)',
 },
 {
   name => 'AlphaBetaRandom',
   key  => 'A',
   header => 'aiSampleAlphaBetaRandom.h',
   cpp    => ['aiSampleAlphaBetaRandom.cpp'],
   constructor => 'ai::Sample::AlphaBetaRandom(opts)',
 },
 {
   name => 'AlphaBetaRandomEvaluation',
   key  => 'E',
   header => 'aiSampleAlphaBetaRandomEvaluation.h',
   cpp    => ['aiSampleAlphaBetaRandomEvaluation.cpp'],
   constructor => 'ai::Sample::AlphaBetaRandomEvaluation(opts)',
 },
#{
#  name => '',
#  key  => '',
#  header => '',
#  constructor => '',
#},
 ];

1;
