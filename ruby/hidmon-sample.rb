require 'hidmon.rb'

$hidmon_debug = true
$mon = HIDmon.new()
#$mock_hid_debug = true
#$mon = MockHIDmon.new()

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

def test_pwm0
  pwm = FastPWM0.new($mon)
  $mon.dump_port_sfrs
  loop do
    255.times do|i|
      pwm.set_value(i)
      sleep 0.005
    end
    255.times do|i|
      pwm.set_value(255-i)
      sleep 0.005
    end
  end
end

def test_servo()
  servo = Servo.new($mon)
  loop do
    1000.times do |i|
      servo.set_value(i/1000.0)
      p i/1000.0
    end
    1000.times do |i|
      servo.set_value((1000-i)/1000.0)
      p (1000-i)/1000.0
    end
  end
end

test_servo()

# vim: set foldmethod=syntax :
