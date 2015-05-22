#
#
#
our $aps =
[
 {
   name => 'Snorlax',
   key  => 's',
   header => 'Snorlax.h',
   cpp    => ['Snorlax.cpp'],
   constructor => 'ai::Sample::Snorlax(opts)',
 },
 {
   name => 'Manual',
   key  => 'm',
   header => 'Manual.h',
   cpp    => ['Manual.cpp'],
   constructor => 'ai::Sample::Manual()',
 },
#{
#  name => '',
#  key  => '',
#  header => '',
#  constructor => '',
#},
 ];

1;
