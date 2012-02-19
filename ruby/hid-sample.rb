require 'hid'

HID::open(0x16c0, 0x05dc) do |dev|
  loop do
    p dev.recv([0,0,0,0,0,0,0,0,0].pack("C*"))
    # print dev.recv([0,0,0,0,0,0,0,0,0].pack("C*"))[1].chr
  end
end

