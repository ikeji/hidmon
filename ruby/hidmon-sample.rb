require 'hidmon.rb'

mon = HIDmon.new()

# 10.times do |i|
#   mon.ping i
# end

mon.dump_all_sfrs
# loop do
#   mon.poke_bit("PORTB", 2, false)
#   sleep(1)
#   mon.poke_bit("PORTB", 2, true)
#   sleep(1)
# end

# vim: set foldmethod=syntax :
