require 'usb'

module HID
  module LibUSB
    USB_TYPE_CLASS = (0x01 << 5)
    USB_RECIP_INTERFACE = (0x01)
    USB_ENDPOINT_IN = 0x80
    USB_ENDPOINT_OUT = 0x00

    HID_REPORT_GET = 0x01
    HID_REPORT_SET = 0x09

    HID_REPORT_TYPE_INPUT = (0x01 << 8)
    HID_REPORT_TYPE_OUTPUT = (0x02 << 8)
    HID_REPORT_TYPE_FEATURE = (0x03 << 8)
    class Device
      def initialize(dev)
        @dev = dev
      end
      def get(buf)
        id = buf[0]
        ret = @dev.usb_control_msg(
          USB_TYPE_CLASS | USB_RECIP_INTERFACE | USB_ENDPOINT_IN,
          HID_REPORT_GET, HID_REPORT_TYPE_FEATURE | id, 0,
          buf, 5000)
        if 0 < ret
          return buf
        else
          raise IOException.new()
        end
      end
      def set(buf)
        id = buf[0]
        ret = @dev.usb_control_msg(
          USB_TYPE_CLASS | USB_RECIP_INTERFACE | USB_ENDPOINT_OUT,
          HID_REPORT_SET, HID_REPORT_TYPE_FEATURE | id, 0,
          buf, 5000)
        raise IOException.new() if ret < 0
        return buf
      end
      def close()
        @dev.usb_close
      end
    end

    def self.open(vendor_id, product_id, opt={})
      devs = USB.devices.reject do|d|
        d.idVendor != vendor_id ||
          d.idProduct != product_id
      end
      dev = devs[0]
      raise Exception.new("Device not found") if !dev
      if block_given?
        dev.open do|d|
          yield Device.new(d)
        end
      else
        return Device.new(dev.open)
      end
    end
  end

  def self.open(vendor_id, product_id, opt={})
    if block_given?
      LibUSB::open(vendor_id, product_id, opt) do |f|
        yield(f)
      end
    else
      return LibUSB::open(vendor_id, product_id, opt)
    end
  end
end 

# vim: foldmethod=syntax
