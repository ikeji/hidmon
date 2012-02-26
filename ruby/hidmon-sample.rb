require 'hidmon.rb'

#$hidmon_debug = true
$mon = HIDmon.new()
# $mock_hid_debug = true
# $mon = MockHIDmon.new()

def test_ping()
  10.times do |i|
    p $mon.ping i
  end
end

def test_portb0_input
  $mon.PORTB.dump
  $mon.PORTB[0].capture
  $mon.PORTB[0].input
  $mon.PORTB[0].pullup(true)
  loop do
    p $mon.PORTB[0].get
  end
end

def test_portb0_output
  $mon.PORTB.dump
  $mon.PORTB[0].capture
  $mon.PORTB[0].output
  loop do
    $mon.PORTB[0].set(true)
    sleep(1)
    $mon.PORTB[0].set(false)
    sleep(1)
  end
end

def test_timer0(mon)
  mon.poke_byte("TCCR0A", 0x83) # 1000 0011

  #mon.poke_byte("TCCR0B", 0x09) # 0000 1001
  #mon.poke_byte("TCCR0B", 0x0d) # 0000 1101

  mon.poke_byte("TCCR0B", 0x01) # 0000 0001
  # mon.poke_byte("TCCR0B", 0x05) # 0000 0101

  mon.poke_byte("OCR0A", 0xff)
  # mon.poke_byte("OCR0B", 0x30)

  loop do
    255.times do|i|
      mon.poke_byte("OCR0A", i)
      sleep 0.005
    end
    255.times do|i|
      mon.poke_byte("OCR0A", 255-i)
      sleep 0.005
    end
  end
end

def test_timer_pwm(mon)
  f_clk = 12 * 1000 * 1000 * 1.0
  clk_div = 8
  timer_clk = f_clk / clk_div
  servo_period = 1000 / 20
  servo_top = timer_clk / servo_period
  mid= timer_clk / (10000 / 15) #= 2252
  lo= timer_clk / (10000 / 7) #= 1050
  high= timer_clk / (10000 / 22) #= 3303
  p servo_top = servo_top.to_i
  p mid = mid.to_i
  p lo= lo.to_i
  p high= high.to_i
  mon.poke_byte("DDRB", 0b0011_1111)
  mon.poke_byte("TCCR1A", 0b0010_0011) # COM1B1 | WGM10 | WGM11
  mon.poke_byte("TCCR1B", 0b0001_1010) # WGM13 | WGM12 | CS11
  mon.poke_byte("OCR1A", servo_top)
  mon.poke_byte("OCR1B", mid)
  mon.dump_one_sfr("DDRB")
  mon.dump_one_sfr("TCCR1A")
  mon.dump_one_sfr("TCCR1B")
  mon.dump_one_sfr("OCR1A")
  loop do
    (high-lo).times do |i|
      mon.poke_byte("OCR1B", lo + i)
      p lo + i
    end
    (high-lo).times do |i|
      mon.poke_byte("OCR1B", lo + ((high-lo)-i))
      p lo + ((high-lo)-i)
    end
  end
end


# mon.poke_byte("DDRB",0x1f)
# mon.poke_byte("TCCR1A", 0xc3) # 0010 0011
# mon.poke_byte("TCCR1B", 0x1c) # 0001 1100
# # mon.poke_byte("ICR1H", 0xff)
# # mon.poke_byte("ICR1L", 0xff)
# 
# mon.poke_byte("OCR1AH", 0xff)
# mon.poke_byte("OCR1AL", 0xff)
#  
# mon.poke_byte("OCR1BH", 0x00)
# mon.poke_byte("OCR1BL", 70)
# 
# mon.dump_port_sfrs
# # puts
# # puts
# # puts
# # 
# # mon.dump_all_sfrs
# # 
# # loop do
# #   mon.dump_one_sfr("TCNT1L")
# #   mon.dump_one_sfr("TCNT1H")
# # end
#   loop do
#     71.times do|i|
#       mon.poke_byte("OCR0A", i + 32)
#       sleep 0.005
#     end
#     71.times do|i|
#       mon.poke_byte("OCR0A", 103-i)
#       sleep 0.005
#     end
#   end

# vim: set foldmethod=syntax :
