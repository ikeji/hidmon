require 'hid'

$hidmon_debug = false

class HIDmon

  attr :PORTB
  attr :TIMER0
  attr :TIMER1
  def initialize(handle = HID::open(0x16c0,0x05df))
    @handle = handle
    @PORTB = PortB.new(self)
    @TIMER0 = Timer0.new(self)
    @TIMER1 = Timer1.new(self)
  end

  BYTE_TYPE = 0
  BYTE_CMD = 1
  BYTE_SIZE = 2
  BYTE_ADRL = 3
  BYTE_ADRH = 4
  BYTE_DATA_BASE = 5
  BYTE_DATA0 = 5
  BYTE_DATA1 = 6

  CMD_PING = 0x01
  CMD_SETPAGE = 0x14
  CMD_POKE = 0x3e
  CMD_PEEK = 0x3f

  REPORT_ID1 = 1 # length = 7
  REPORT_ID2 = 2 # length = 31
  REPORT_ID3 = 3 # length = 39

  def self.create_buf(type)
    buf = nil
    case(type)
    when REPORT_ID1
      buf = "\0" * 7
    when REPORT_ID2
      buf = "\0" * 31
    when REPORT_ID3
      buf = "\0" * 39
    else
      raise Exception.new("Unknown report type")
    end
    buf[BYTE_TYPE] = type
    return buf
  end

  def ping(i = 11)
    buf = HIDmon.create_buf(REPORT_ID1)
    buf[BYTE_CMD] = CMD_PING
    (2..6).each do |p|
      buf[p] = i
    end
    @handle.set(buf)
    rbuf = HIDmon.create_buf(REPORT_ID2)
    @handle.get(rbuf)
    (2..6).each do |p|
      raise Exception.new("Ping error") if(rbuf[p] != i)
    end
    return true
  end

  def peek_byte(addr)
    sfr = nil
    if(RegisterMap.has_key?(addr))
      sfr = addr
      reg = RegisterMap[addr]
      if(reg.size == 1)
        # Process in current function
        addr = reg.addr
      elsif(reg.size == 2)
        # Read lower byte before higher byte.
        return ((peek_byte(reg.addr    )) |
                (peek_byte(reg.addr + 1) << 8))
      else
        raise Exception.new("Unknown register size")
      end
    end
    raise Exception.new("Address error") if addr > 0xffff || addr < 0
    buf = HIDmon.create_buf(REPORT_ID1)
    buf[BYTE_CMD] = CMD_PEEK
    buf[BYTE_SIZE] = 1
    buf[BYTE_ADRL] = addr & 0xff
    buf[BYTE_ADRH] = (addr >> 8) & 0xff
    return nil if(!@handle.set(buf))

    rbuf = HIDmon.create_buf(REPORT_ID1)
    r = @handle.get(rbuf)
    if(r)
      if $hidmon_debug
        puts "Read #{addr} #{sfr ? "which is #{sfr} ":""} as 0x#{r[1].to_s(16)}"
      end
      return r[1]
    else
      if $hidmon_debug
        puts "Read #{addr} #{sfr ? "which is #{sfr} ":""} as error"
      end
      return nil
    end
  end

  def peek_bit(addr, bit)
    max_bit = RegisterMap.has_key?(addr) ? RegisterMap[addr].size * 8 : 8
    raise Exception.new("bit error") unless 0 <= bit && bit < max_bit
    return peek_byte(addr) & (1<<bit)
  end

  def poke_byte(addr, val, mask = 0xffff)
    sfr = nil
    if(RegisterMap.has_key?(addr))
      sfr = addr
      reg = RegisterMap[addr]
      if(reg.size == 1)
        # Process in current function
        addr = reg.addr
      elsif(reg.size == 2)
        # Write high byte before lower byte.
        return (
          poke_byte(reg.addr + 1, (val >> 8) & 0xff, (mask >> 8) & 0xff) &&
          poke_byte(reg.addr    ,  val       & 0xff, mask        & 0xff) &&
          true)
      else
        raise Exception.new("Unknown register size")
      end
    end
    raise Exception.new("Address error") if addr > 0xffff || addr < 0
    buf = HIDmon.create_buf(REPORT_ID1)
    buf[BYTE_CMD] = CMD_POKE
    buf[BYTE_SIZE] = 1
    buf[BYTE_ADRL] = addr & 0xff
    buf[BYTE_ADRH] = (addr >> 8) & 0xff
    buf[BYTE_DATA0] = val & 0xff
    buf[BYTE_DATA1] = 0xff ^ (mask & 0xff)
    if $hidmon_debug
      puts "Write 0x#{val.to_s(16)} " +
        "#{(mask != 0xffff) ? " with mask 0x#{mask.to_s(16)} " : ""} " +
        "to #{addr} #{sfr ? "which is #{sfr} ":""} "
    end
    return @handle.set(buf)
  end

  def poke_bit(addr, bit, bool)
    # TODO: support double byte register.
    raise Exception.new("bit error") unless 0 <= bit && bit < 8
    return poke_byte(addr, bool ? 1 << bit : 0, 1 << bit)
  end

  class SFR
    attr_reader :addr, :size
    def initialize(addr, size=1)
      @addr = addr + 0x20
      @size = size
    end
  end
  RegisterMap = {
    "DIDR" => SFR.new(0x01),
    "UBRRH" => SFR.new(0x02),
    "UCSRC" => SFR.new(0x03),
    "ACSR" => SFR.new(0x08),
    "UBRRL" => SFR.new(0x09),
    "UCSRB" => SFR.new(0x0A),
    "UCSRA" => SFR.new(0x0B),
    "UDR" => SFR.new(0x0C),
    "RXB" => SFR.new(0x0C),
    "TXB" => SFR.new(0x0C),
    "USICR" => SFR.new(0x0D),
    "USISR" => SFR.new(0x0E),
    "USIDR" => SFR.new(0x0F),
    "PIND" => SFR.new(0x10),
    "DDRD" => SFR.new(0x11),
    "PORTD" => SFR.new(0x12),
    "GPIOR0" => SFR.new(0x13),
    "GPIOR1" => SFR.new(0x14),
    "GPIOR2" => SFR.new(0x15),
    "PINB" => SFR.new(0x16),
    "DDRB" => SFR.new(0x17),
    "PORTB" => SFR.new(0x18),
    "PINA" => SFR.new(0x19),
    "DDRA" => SFR.new(0x1A),
    "PORTA" => SFR.new(0x1B),
    "EECR" => SFR.new(0x1C),
    "EEDR" => SFR.new(0x1D),
    "EEAR" => SFR.new(0x1E),
    "EEARL" => SFR.new(0x1E),
    "PCMSK" => SFR.new(0x20),
    "WDTCSR" => SFR.new(0x21),
    "TCCR1C" => SFR.new(0x22),
    "GTCCR" => SFR.new(0x23),
    "ICR1" => SFR.new(0x24, 2),
    "ICR1L" => SFR.new(0x24),
    "ICR1H" => SFR.new(0x25),
    "CLKPR" => SFR.new(0x26),
    "OCR1B" => SFR.new(0x28, 2),
    "OCR1BL" => SFR.new(0x28),
    "OCR1BH" => SFR.new(0x29),
    "OCR1A" => SFR.new(0x2A, 2),
    "OCR1AL" => SFR.new(0x2A),
    "OCR1AH" => SFR.new(0x2B),
    "TCNT1" => SFR.new(0x2C, 2),
    "TCNT1L" => SFR.new(0x2C),
    "TCNT1H" => SFR.new(0x2D),
    "TCCR1B" => SFR.new(0x2E),
    "TCCR1A" => SFR.new(0x2F),
    "TCCR0A" => SFR.new(0x30),
    "OSCCAL" => SFR.new(0x31),
    "TCNT0" => SFR.new(0x32),
    "TCCR0B" => SFR.new(0x33),
    "MCUSR" => SFR.new(0x34),
    "MCUCR" => SFR.new(0x35),
    "OCR0A" => SFR.new(0x36),
    "SPMCSR" => SFR.new(0x37),
    "TIFR" => SFR.new(0x38),
    "TIMSK" => SFR.new(0x39),
    "EIFR" => SFR.new(0x3A),
    "GIMSK" => SFR.new(0x3B),
    "OCR0B" => SFR.new(0x3C),
  }

  def self.byte2bin(n,size=1)
    r = "0b"
    bits = size * 8
    bits.times do |i|
      r += ( (1 << (bits-1-i)) & n == 0) ? "0" : "1"
    end
    return r
  end

  def dump_one_sfr(sfr, hasCr = true)
    size = 1
    if(RegisterMap.has_key?(sfr))
      size = RegisterMap[sfr].size
    end
    val = peek_byte(sfr)
    print "#{sfr}#{" " * (6-sfr.size)} :"
    print " #{sprintf("0x%0#{size*2}X", val)} #{" "*(4-size*2)}:"
    print " #{HIDmon.byte2bin(val,size)} #{" "*(16-size*8)}"
    puts if hasCr
  end

  def dump_sfrs(sfrs)
    sfrs.each_with_index do|k,i|
      dump_one_sfr(k, i%2==1)
    end
    puts if sfrs.size%2 == 1
  end

  def dump_all_sfrs
    dump_sfrs(RegisterMap.keys.sort)
  end

  Ports = ["A", "B", "D"].map do |port|
    ["DDR", "PORT", "PIN"].map do |f|
      f + port
    end
  end.flatten

  def dump_port_sfrs
    dump_sfrs(Ports)
  end
end

$mock_hid_debug = false

class MockHIDmon < HIDmon
  class MockHID
    def initialize()
      @feature_buffer = []
      @memory = "\0" * (0x20 + 0x3C)
      @addr2sfr = nil
    end
    # TODO: Implement mock memory model such as SFR.
    def method_missing(name, *args)
      if args.size != 1 && args[0].class == String
        # FixMe
        raise NotImplementedError.new()
      end
      if $mock_hid_debug
        puts "#{name.to_s} called with #{args[0].unpack("C*").inspect }"
      end
      return args[0]
    end
    def set(buf)
      if buf[HIDmon::BYTE_TYPE] != HIDmon::REPORT_ID1
        raise NotImplementedError.new
      end
      case(buf[HIDmon::BYTE_CMD])
      when HIDmon::CMD_PING:
        # TODO: more readabile output
        puts "Ping: #{buf.unpack("C*")[2,4].inspect}" if $mock_hid_debug
        @feature_buffer = buf.dup()
      when HIDmon::CMD_PEEK:
        if buf[HIDmon::BYTE_SIZE] != 1
          raise NotImplementedError.new
        end
        addr = buf[HIDmon::BYTE_ADRH] << 8 |
          buf[HIDmon::BYTE_ADRL]
        @feature_buffer = HIDmon::create_buf(HIDmon::REPORT_ID1)
        @feature_buffer[1] = @memory[addr]
        if $mock_hid_debug
          # TODO: more readabile output
          puts("Read: addr-0x#{addr.to_s(16)}"+
           "val-#{HIDmon.byte2bin(@memory[addr])}")
        end
      when HIDmon::CMD_POKE:
        if buf[HIDmon::BYTE_SIZE] != 1
          raise NotImplementedError.new
        end
        addr = buf[HIDmon::BYTE_ADRH] << 8 |
          buf[HIDmon::BYTE_ADRL]
        data = buf[HIDmon::BYTE_DATA0]
        mask = buf[HIDmon::BYTE_DATA1]
        @memory[addr] = @memory[addr] & mask | data
        if $mock_hid_debug
          puts("Write: addr-0x#{addr.to_s(16)} "+
               "val-#{HIDmon.byte2bin(@memory[addr])}")
        end
      else
        puts "Unknown command" if $mock_hid_debug
      end
      return true
    end
    def get(buf)
      buf[0, buf.size] = @feature_buffer
      return buf
    end
  end
  def initialize()
    super(MockHID.new)
  end
end

class FeatureAlreadyUsedException < Exception
end

class Feature
  def initialize()
    @captured = false
  end
  def name
    return self.class.to_s
  end
  def capture(label=nil)
    if(is_captured?)
      raise FeatureAlreadyUsedException.new(
        "#{name} already used by #{@label_base}")
    end
    @captured = true
    @label_base = label
    @label = "#{name}:#{label}"
  end
  def release()
    @captured = false
    @label_base = "<Undef>"
    @label = "<Undef>"
  end
  def is_captured?()
    return @captured
  end
  def captured_or_die(label=nil)
    unless is_captured?
      raise Exception.new("Use feature #{name} without capture")
    end
    if label && @label_base != label
      raise Exception.new(
        "The feature #{name} are aready captured by #{@label_base}")
    end
  end
end

class PortB

  class Pin < Feature
    def initialize(port, pin)
      super()
      @port = port
      @pin = pin
      @mask = 1 << pin
    end
    def name
      return "PortB#{@pin}"
    end
    def input(label=nil)
      @port.set_input_output_byte(0, @mask, label)
    end
    def output(label=nil)
      @port.set_input_output_byte(@mask, @mask, label)
    end
    def pullup(bool, label=nil)
      bool = false if bool == 0
      @port.set_pullup_byte(bool ? @mask : 0, @mask, label)
    end
    def set(bool, label=nil)
      bool = false if bool == 0
      @port.set_byte(bool ? @mask : 0, @mask, label)
    end
    def get(label=nil)
      return((@port.get_byte(label) & @mask) != 0)
    end
  end

  attr_reader :pin
  def initialize(mon)
    @mon = mon
    @pin = (0..7).map{|i| Pin.new(self, i)}
    @pin.freeze
  end
  def [](bit)
    return pin[bit]
  end
  def each_bit(mask)
    if block_given?
      (0..7).each do |i|
        if(mask & (1 << i) != 0)
          yield(pin[i])
        end
      end
    else
      return (0..7).map do |i|
        if(mask & (1 << i) != 0)
          pin[i]
        else
          nil
        end
      end.reject {|i| i == nil}
    end
  end
  def captured_or_die(mask, label=nil)
    each_bit(mask) do |pin|
      pin.captured_or_die(label)
    end
  end
  def capture_byte(mask, label=nil)
    each_bit(mask) do |pin|
      pin.capture(label)
    end
  end
  def release_byte(mask, label=nil)
    each_bit(mask) do |pin|
      pin.release
    end
  end
  def capture(label=nil)
    capture_byte(0xff, label)
  end
  def release(label=nil)
    release_byte(0xff, label)
  end
  def set_input_output_byte(byte, mask, label=nil)
    captured_or_die(mask,label)
    @mon.poke_byte("DDRB", byte, mask)
  end
  def set_byte(byte, mask, label=nil)
    captured_or_die(mask,label)
    @mon.poke_byte("PORTB", byte, mask)
  end
  alias set_pullup_byte set_byte
  def get_byte(label=nil)
    @mon.peek_byte("PINB")
  end
  def dump
    @mon.dump_sfrs(["DDRB", "PORTB", "PINB"])
  end
end

class Timer0 < Feature
  def initialize(mon)
    super()
    @mon = mon
  end

  # for TCCR0A
  COM0A1 = 1 << 7
  COM0A0 = 1 << 6
  COM0B1 = 1 << 5
  COM0B0 = 1 << 4
  NA3    = 1 << 3
  NA2    = 1 << 2
  WGM01  = 1 << 1
  WGM00  = 1 << 0
  # for TCCR0B
  FOC0A = 1 << 7
  FOC0B = 1 << 6
  NA5   = 1 << 5
  NA4   = 1 << 4
  WGM02 = 1 << 3
  CS02  = 1 << 2
  CS01  = 1 << 1
  CS00  = 1 << 0

  #output mode
  OUTPUT_DISABLE    = 0
  OUTPUT_TOGGLE     = 1
  OUTPUT_MATCH_DOWN = 2
  OUTPUT_MATCH_UP   = 3
  def set_output_mode_a(mode, label=nil)
    captured_or_die(label)
    @mon.poke_byte("TCCR0A", mode << 6, 0b1100_0000)
  end
  # NOTE: Output Pin B is not supported on HIDmon
  # def set_output_mode_b(mode, label=nil)
  #   captured_or_die(label)
  #   @mon.poke_byte("TCCR0A", mode << 4, 0b0011_0000)
  # end

  PIN_OC0A = 2
  PIN_OC0B = 5 # PORTD
  def enable_output_a(label=nil)
    captured_or_die(label)
    @mon.PORTB[PIN_OC0A].capture(label)
    @mon.PORTB[PIN_OC0A].output(label)
    @mon.PORTB[PIN_OC0A].set(true, label)
  end
  # # NOTE: Output Pin B is not supported on HIDmon
  # def enable_output_b(label=nil)
  #   captured_or_die(label)
  #   @mon.PORTD[PIN_OC0B].capture(label)
  #   @mon.PORTD[PIN_OC0B].set(true, label)
  # end
  def disable_output_a()
    @mon.PORTB[PIN_OC0A].release
  end
  # NOTE: Output Pin B is not supported on HIDmon
  # def disable_output_b()
  #   @mon.PORTD[PIN_OC0B].release
  # end

  # div
  STOP     = 0
  NO_DIV   = 1
  DIV_8    = 2
  DIV_64   = 3
  DIV_256  = 4
  DIV_1024 = 5
  T1_DOWN  = 6
  T1_UP    = 7
  def set_div_mode(val, label=nil)
    captured_or_die(label)
    @mon.poke_byte("TCCR0B", val, 0b0000_0111)
  end

  # mode
  DEFAULT_MODE        = 0
  FAST_8BIT_PWM_MODE  = 3
  FAST_PWM_MODE       = 7
  def set_timer_mode(mode, label=nil)
    captured_or_die(label)
    wgm02   = mode >> 2 & 0b01
    wgm0100 = mode & 0b11
    @mon.poke_byte("TCCR0B", wgm02 << 3, 0b0000_1000)
    @mon.poke_byte("TCCR0A", wgm0100   , 0b0000_0011)
  end

  def set_a_counter(val, label=nil)
    captured_or_die(label)
    @mon.poke_byte("OCR0A", val)
  end
  def set_b_counter(val, label=nil)
    captured_or_die(label)
    @mon.poke_byte("OCR0B", val)
  end
end

# This class output PINB2
class FastPWM0 < Feature
  def initialize(mon, label="")
    super()
    @mon = mon
    @timer = mon.TIMER0
    capture(label)
  end
  def capture(label=nil)
    super(label)
    @timer.capture(@label)
    @timer.set_timer_mode(Timer0::FAST_8BIT_PWM_MODE, @label)
    @timer.enable_output_a(@label)
    @timer.set_output_mode_a(Timer0::OUTPUT_MATCH_DOWN, @label)
    @timer.set_div_mode(Timer0::NO_DIV)
  end
  def release()
    super(label)
    @timer.release()
  end
  def set_value(val)
    @timer.set_a_counter(val, @label)
  end
end

class Timer1 < Feature
  def initialize(mon)
    super()
    @mon = mon
  end

  # for TCCR1A
  COM1A1 = 1 << 7 
  COM1A0 = 1 << 6
  COM1B1 = 1 << 5
  COM1B0 = 1 << 4
  WGM11  = 1 << 1
  WGM10  = 1 << 0
  # for TCCR1B
  ICNC1  = 1 << 7
  ICNS1  = 1 << 6
  WGM13  = 1 << 4
  WGM12  = 1 << 3
  CS12   = 1 << 2
  CS11   = 1 << 1
  CS10   = 1 << 0

  # output mode
  OUTPUT_DISABLE    = 0
  OUTPUT_TOGGLE     = 1
  OUTPUT_MATCH_DOWN = 2
  OUTPUT_MATCH_UP   = 3
  def set_output_mode_a(mode, label)
    captured_or_die(label)
    @mon.poke_byte("TCCR1A", mode << 6, 0b1100_0000)
  end
  def set_output_mode_b(mode, label)
    captured_or_die(label)
    @mon.poke_byte("TCCR1A", mode << 4, 0b0011_0000)
  end

  PIN_OC1A = 3
  PIN_OC1B = 4
  def enable_output_a(label=nil)
    captured_or_die(label)
    @mon.PORTB[PIN_OC1A].capture(label)
    @mon.PORTB[PIN_OC1A].output(label)
    @mon.PORTB[PIN_OC1A].set(true, label)
  end
  def enable_output_b(label=nil)
    captured_or_die(label)
    @mon.PORTB[PIN_OC1B].capture(label)
    @mon.PORTB[PIN_OC1B].output(label)
    @mon.PORTB[PIN_OC1B].set(true, label)
  end
  def disable_output_a()
    @mon.PORTB[PIN_OC1A].release()
  end
  def disable_output_b()
    @mon.PORTB[PIN_OC1B].release()
  end

  # div
  STOP     = 0
  NO_DIV   = 1
  DIV_8    = 2
  DIV_64   = 3
  DIV_256  = 4
  DIV_1024 = 5
  T1_DOWN  = 6
  T1_UP    = 7
  def set_div_mode(val, label=nil)
    captured_or_die(label)
    @mon.poke_byte("TCCR1B", val, 0b0000_0111)
  end

  # timer mode
  DEFAULT_MODE        = 0
  FAST_8BIT_PWM_MODE  = 5
  FAST_9BIT_PWM_MODE  = 6
  FAST_10BIT_PWM_MODE = 7
  FAST_PWM_MODE_ICR   = 14
  FAST_PWM_MODE       = 15
  def set_timer_mode(mode, label=nil)
    captured_or_die(label)
    wgm0302 = mode >> 2 & 0b11
    wgm0100 = mode & 0b11
    @mon.poke_byte("TCCR1B", wgm0302 << 3, 0b0001_1000)
    @mon.poke_byte("TCCR1A", wgm0100     , 0b0000_0011)
  end
  def set_a_counter(val, label=nil)
    captured_or_die(label)
    @mon.poke_byte("OCR1A", val)
  end
  def set_b_counter(val, label=nil)
    captured_or_die(label)
    @mon.poke_byte("OCR1B", val)
  end
end

# This class outputs PORTB4
class Servo < Feature
  F_CLK = 12 * 1000 * 1000
  CLK_DIV = 8
  TIMER_CLK = F_CLK / CLK_DIV
  SERVO_PERIOD = 1000 / 20
  SERVO_TOP = TIMER_CLK / SERVO_PERIOD
  MID = TIMER_CLK / (10000 / 15) #= 2252
  LO = TIMER_CLK / (10000 / 7) #= 1050
  HIGH = TIMER_CLK / (10000 / 22) #= 3303
  RANGE = HIGH - LO
  def initialize(mon, label="Unknown")
    super()
    @mon = mon
    @timer = mon.TIMER1
    capture(label)
  end
  def capture(label=nil)
    super(label)
    @timer.capture(@label)
    @timer.set_timer_mode(Timer1::FAST_PWM_MODE, @label)
    @timer.enable_output_b(@label)
    @timer.set_output_mode_b(Timer1::OUTPUT_MATCH_DOWN, @label)
    @timer.set_div_mode(Timer1::DIV_8)
    set_value(MID)
    @timer.set_a_counter(SERVO_TOP)
  end
  def release()
    super(label)
    @timer.release()
  end
  def set_value(val)
    val = (RANGE * val).to_i + LO
    @timer.set_b_counter(val, @label)
  end
end

# vim: set foldmethod=syntax :
