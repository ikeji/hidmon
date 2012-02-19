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
    addr = RegisterMap[addr] if (RegisterMap.has_key?(addr))
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
    raise Exception.new("bit error") unless 0 <= bit && bit < 8
    return peek_byte(addr) & (1<<bit)
  end

  def poke_byte(addr, val)
    addr = RegisterMap[addr] if (RegisterMap.has_key?(addr))
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
    addr = RegisterMap[addr] if (RegisterMap.has_key?(addr))
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

  RegisterMap = {
    "DIDR" => 0x20 + 0x01,
    "UBRRH" => 0x20 + 0x02,
    "UCSRC" => 0x20 + 0x03,
    "ACSR" => 0x20 + 0x08,
    "UBRRL" => 0x20 + 0x09,
    "UCSRB" => 0x20 + 0x0A,
    "UCSRA" => 0x20 + 0x0B,
    "UDR" => 0x20 + 0x0C,
    "RXB" => 0x20 + 0x0C,
    "TXB" => 0x20 + 0x0C,
    "USICR" => 0x20 + 0x0D,
    "USISR" => 0x20 + 0x0E,
    "USIDR" => 0x20 + 0x0F,
    "PIND" => 0x20 + 0x10,
    "DDRD" => 0x20 + 0x11,
    "PORTD" => 0x20 + 0x12,
    "GPIOR0" => 0x20 + 0x13,
    "GPIOR1" => 0x20 + 0x14,
    "GPIOR2" => 0x20 + 0x15,
    "PINB" => 0x20 + 0x16,
    "DDRB" => 0x20 + 0x17,
    "PORTB" => 0x20 + 0x18,
    "PINA" => 0x20 + 0x19,
    "DDRA" => 0x20 + 0x1A,
    "PORTA" => 0x20 + 0x1B,
    "EECR" => 0x20 + 0x1C,
    "EEDR" => 0x20 + 0x1D,
    "EEAR" => 0x20 + 0x1E,
    "EEARL" => 0x20 + 0x1E,
    "PCMSK" => 0x20 + 0x20,
    "WDTCSR" => 0x20 + 0x21,
    "TCCR1C" => 0x20 + 0x22,
    "GTCCR" => 0x20 + 0x23,
    "ICR1L" => 0x20 + 0x24,
    "ICR1H" => 0x20 + 0x25,
    "CLKPR" => 0x20 + 0x26,
    "OCR1BL" => 0x20 + 0x28,
    "OCR1BH" => 0x20 + 0x29,
    "OCR1L" => 0x20 + 0x2A,
    "OCR1H" => 0x20 + 0x2B,
    "OCR1AL" => 0x20 + 0x2A,
    "OCR1AH" => 0x20 + 0x2B,
    "TCNT1L" => 0x20 + 0x2C,
    "TCNT1H" => 0x20 + 0x2D,
    "TCCR1B" => 0x20 + 0x2E,
    "TCCR1A" => 0x20 + 0x2F,
    "TCCR0A" => 0x20 + 0x30,
    "OSCCAL" => 0x20 + 0x31,
    "TCNT0" => 0x20 + 0x32,
    "TCCR0B" => 0x20 + 0x33,
    "MCUSR" => 0x20 + 0x34,
    "MCUCR" => 0x20 + 0x35,
    "OCR0A" => 0x20 + 0x36,
    "SPMCSR" => 0x20 + 0x37,
    "TIFR" => 0x20 + 0x38,
    "TIMSK" => 0x20 + 0x39,
    "EIFR" => 0x20 + 0x3A,
    "GIMSK" => 0x20 + 0x3B,
    "OCR0B" => 0x20 + 0x3C,
  }

  def byte2bin(n)
    r = "0b"
    (0..7).each do |i|
      r += ( (1 << (7-i)) & n == 0) ? "0" : "1"
    end
    return r
  end

  def dump_one_sfr(sfr, hasCr = true)
    val = peek_byte(sfr)
    print "#{sfr}#{" " * (6-sfr.size)} :"
    print " #{sprintf("0x%02X", val)} :"
    print " 0b#{byte2bin(val)}  "
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
