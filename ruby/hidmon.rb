require 'hid'

class HIDmon

  def initialize(handle = HID::open(0x16c0,0x05df))
    @handle = handle
  end

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

  def create_buf(type)
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
    buf[0] = REPORT_ID1
    return buf
  end

  def ping(i = 11)
    buf = create_buf(REPORT_ID1)
    buf[BYTE_CMD] = CMD_PING
    (2..6).each do |p|
      buf[p] = i
    end
    @handle.set(buf)
    rbuf = create_buf(REPORT_ID2)
    @handle.get(rbuf)
    (2..6).each do |p|
      raise Exception.new("Ping error") if(rbuf[p] != i)
    end
    return true
  end

  def peek_byte(addr)
    if(RegisterMap.has_key?(addr))
      reg = RegisterMap[addr]
      if(reg.size == 1)
        return peek_byte(reg.addr)
      elsif(reg.size == 2)
        # Read lower byte before higher byte.
        return ((peek_byte(reg.addr    )) |
                (peek_byte(reg.addr + 1) << 8))
      else
        raise Exception.new("Unknown register size")
      end
    end
    raise Exception.new("Address error") if addr > 0xffff || addr < 0
    buf = create_buf(REPORT_ID1)
    buf[BYTE_CMD] = CMD_PEEK
    buf[BYTE_SIZE] = 1
    buf[BYTE_ADRL] = addr & 0xff
    buf[BYTE_ADRH] = (addr >> 8) & 0xff
    return nil if(!@handle.set(buf))

    rbuf = create_buf(REPORT_ID1)
    r = @handle.get(rbuf)
    if(r)
      return r[1]
    else
      return nil
    end
  end

  def peek_bit(addr, bit)
    max_bit = RegisterMap.has_key?(addr) ? RegisterMap[addr].size * 8 : 8
    raise Exception.new("bit error") unless 0 <= bit && bit < max_bit
    return peek_byte(addr) & (1<<bit)
  end

  def poke_byte(addr, val)
    if(RegisterMap.has_key?(addr))
      reg = RegisterMap[addr]
      if(reg.size == 1)
        return poke_byte(reg.addr, val)
      elsif(reg.size == 2)
        # Write high byte before lower byte.
        return (
          poke_byte(reg.addr + 1, (val >> 8) & 0xff) &&
          poke_byte(reg.addr    ,  val       & 0xff) &&
          true)
      else
        raise Exception.new("Unknown register size")
      end
    end
    raise Exception.new("Address error") if addr > 0xffff || addr < 0
    val = val & 0xff
    buf = create_buf(REPORT_ID1)
    buf[BYTE_CMD] = CMD_POKE
    buf[BYTE_SIZE] = 1
    buf[BYTE_ADRL] = addr & 0xff
    buf[BYTE_ADRH] = (addr >> 8) & 0xff
    buf[BYTE_DATA0] = val
    buf[BYTE_DATA1] = 0
    return @handle.set(buf)
  end

  def poke_bit(addr, bit, bool)
    if(RegisterMap.has_key?(addr))
      reg = RegisterMap[addr]
      if(reg.size == 1)
        return poke_bit(reg.addr, bit, val)
      elsif(reg.size == 2)
        raize NotImplementedError.new()
      else
        raise Exception.new("Unknown register size")
      end
    end
    raise Exception.new("Address error") if addr > 0xffff || addr < 0
    raise Exception.new("bit error") unless 0 <= bit && bit < 8
    buf = create_buf(REPORT_ID1)
    buf[BYTE_CMD] = CMD_POKE
    buf[BYTE_SIZE] = 1
    buf[BYTE_ADRL] = addr & 0xff
    buf[BYTE_ADRH] = (addr >> 8) & 0xff
    buf[BYTE_DATA0] = bool ? 1 << bit : 0
    buf[BYTE_DATA1] = 0xff ^ (1 << bit)
    return @handle.set(buf)
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

  def byte2bin(n,size=1)
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
    print " #{byte2bin(val,size)} #{" "*(16-size*8)}"
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

# vim: set foldmethod=syntax :
