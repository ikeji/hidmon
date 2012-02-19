
module HID
  class DeviceNotFoundException < IOError
  end

  class HidDevice
    # Receive input packet from the device.
    # buf: The buffer to receive. The length of buffer should be a packet
    #      length and first byte of the buffer should be a report id.
    # This method returns the reed buffer when receive is complete without
    # error otherwise raise IOException.
    def recv(buf)
      raise NotImplementedError.new()
    end
    # Send output packet to the device.
    # buf: The buffer to send. The length of buffer should be a packet
    #      length and first byte of the buffer should be a report id.
    # This method returns the buffer when send is complete without
    # error otherwise raise IOException.
    def send(buf)
      raise NotImplementedError.new()
    end

    # Get feature packet from the device.
    # buf: The buffer to receive. The length of buffer should be a packet
    #      length and first byte of the buffer should be a report id.
    # This method returns the reed buffer when receive is complete without
    # error otherwise raise IOException.
    def get(buf)
      raise NotImplementedError.new()
    end
    # Set feature packet to the device.
    # buf: The buffer to send. The length of buffer should be a packet
    #      length and first byte of the buffer should be a report id.
    # This method returns the buffer when send is complete without
    # error otherwise raise IOException.
    def set(buf)
      raise NotImplementedError.new()
    end
   
    # Close this device.
    def close()
    end
  end
end

begin
  # Try to require ruby-usb.
  require 'usb'
  # If we can load ruby-usb, use libusb version.
  require 'hid-libusb'
rescue LoadError
  # If we can't load ruby-usb module, use win32 version.
  require 'hid-win32'
end

# vim: foldmethod=syntax
