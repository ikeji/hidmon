begin
  require 'nkf'
  module HID module Win32 HAS_NKF = true end end
rescue LoadError
  module HID module Win32 HAS_NKF = false end end
end
require 'dl/import'
require 'dl/struct'

if RUBY_VERSION < '1.9'
  module DL
    Importer = Importable
  end
end

module HID
  module Win32
    extend DL::Importer

    typealias 'USHORT', 'unsigned short'

    GUID = struct [
      'ULONG data1',
      'USHORT data2',
      'USHORT data3',
      'UCHAR data4[8]',
    ]

    module HID_DLL
      extend DL::Importer

      typealias 'USHORT', 'unsigned short'
      HIDD_ATTRIBUTES = struct [
        'ULONG Size',
        'USHORT VendorID',
        'USHORT ProductID',
        'USHORT VersionNumber'
      ]
      def HIDD_ATTRIBUTES.create()
        t = HIDD_ATTRIBUTES.malloc()
        t.Size = t.size;
        return t
      end
      typealias 'PHIDD_ATTRIBUTES', 'HIDD_ATTRIBUTES*'

      dlload 'hid.dll'
      extern 'void HidD_GetHidGuid(GUID*)'
      extern 'BOOL HidD_GetAttributes(HANDLE, PHIDD_ATTRIBUTES)'
      extern 'BOOL HidD_GetFeature (HANDLE, PVOID, ULONG)'
      extern 'BOOL HidD_SetFeature (HANDLE, PVOID, ULONG)'
    end

    module SETUPAPI_DLL
      extend DL::Importer

      dlload 'setupapi.dll'

      typealias 'ULONG_PTR', 'ULONG*'
      SP_DEVICE_INTERFACE_DATA = struct [
        'DWORD cbSize',
        'char InterfaceClassGuid[16]',
        'DWORD Flags',
        'ULONG_PTR Reserved',
      ]
      typealias 'PSP_DEVICE_INTERFACE_DATA', 'SP_DEVICE_INTERFACE_DATA*'
      def SP_DEVICE_INTERFACE_DATA.create()
        t = SP_DEVICE_INTERFACE_DATA.malloc()
        t.cbSize = t.size
        return t
      end

      SP_DEVINFO_DATA = struct [
        'DWORD cbSize',
        'char InterfaceClassGuid[16]',
        'DWORD DevInst',
        'ULONG_PTR Reserved',
      ]
      typealias 'PSP_DEVINFO_DATA', 'SP_DEVINFO_DATA*'

      SP_DEVICE_INTERFACE_DETAIL_DATA_A = struct [
        'DWORD cbSize',
        'char DevicePath[1]', # variable length
      ]
      def SP_DEVICE_INTERFACE_DETAIL_DATA_A.create(
          sz=SP_DEVICE_INTERFACE_DETAIL_DATA_A.size)
        t = SP_DEVICE_INTERFACE_DETAIL_DATA_A.malloc(sz)
        t.cbSize = SP_DEVICE_INTERFACE_DETAIL_DATA_A.size
        def t.devicePathSize=(s)
          @devicePathSize = s
        end
        def t.getDevicePath()
          return self.to_ptr[4, @devicePathSize-self.cbSize]
        end
        t.devicePathSize = sz
        return t
      end
      typealias 'PSP_DEVICE_INTERFACE_DETAIL_DATA_A',
          'SP_DEVICE_INTERFACE_DETAIL_DATA_A*'

      DWORD_ST = struct [
        'DWORD size'
      ]

      typealias 'HDEVINFO', 'void*'
      typealias 'PCSTR', 'void*'

      DIGCF_PRESENT        	= 0x00000002
      DIGCF_DEVICEINTERFACE =	0x00000010

      extern 'HDEVINFO SetupDiGetClassDevsA(GUID*, PCSTR, HWND, DWORD)'
      extern <<-EOL
        BOOL SetupDiEnumDeviceInterfaces(
            HDEVINFO, PSP_DEVINFO_DATA, CONST GUID *, DWORD,
            PSP_DEVICE_INTERFACE_DATA)
      EOL
      extern <<-EOL
        BOOL SetupDiGetDeviceInterfaceDetailA(
            HDEVINFO, PSP_DEVICE_INTERFACE_DATA,
            PSP_DEVICE_INTERFACE_DETAIL_DATA_A, DWORD, DWORD_ST*,
            PSP_DEVINFO_DATA)
      EOL
      extern 'BOOL SetupDiDestroyDeviceInfoList(HDEVINFO)'
    end

    module KERNEL32_DLL
      extend DL::Importer
      dlload 'kernel32.dll'

      GENERIC_READ	= 0x80000000
      GENERIC_WRITE	= 0x40000000
      FILE_SHARE_READ  = 0x00000001
      FILE_SHARE_WRITE = 0x00000002
      OPEN_EXISTING	= 3
      
      DWORD_ST = struct [
        'DWORD size'
      ]
      CHAR_BUF = struct [
        'char buf[1024]'
      ]

      typealias 'LPSECURITY_ATTRIBUTES', 'long'
      typealias 'LPOVERLAPPED', 'void*'
      typealias 'LPVOID', 'void*'
      typealias 'LPCVOID', 'const void*'
      typealias 'LPTSTR', 'void*'
      typealias 'LPDWORD', 'DWORD_ST*'
      extern <<-EOL
        HANDLE CreateFileA(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES,
            DWORD, DWORD, HANDLE)
      EOL
      extern 'BOOL ReadFile(HANDLE,LPVOID,DWORD,LPDWORD,LPOVERLAPPED)'
      extern 'BOOL WriteFile(HANDLE,LPCVOID,DWORD,LPDWORD,LPOVERLAPPED)'
      extern 'BOOL CloseHandle(HANDLE)'

      extern 'DWORD GetLastError()'
      extern <<-EOL
        DWORD FormatMessageA(
            DWORD,LPCVOID,DWORD,DWORD,LPTSTR,DWORD,va_list*)
      EOL

      def self.getLastErrorString()
        n = getLastError()
        msg = CHAR_BUF.malloc()
        len = formatMessageA(0x1200,nil,n,0,msg,msg.size,nil)
        if HAS_NKF
          return NKF.nkf('-w',msg.to_ptr[0,len])
        else
          return msg.to_ptr(0, len)
        end
      end
    end

    class Device < HidDevice
      def initialize(handle)
        @handle = handle
      end

      def recv(buf)
        rbuf = DL.malloc(buf.size)
        rbuf[0] = buf
        size_st = KERNEL32_DLL::DWORD_ST.malloc()
        if(!KERNEL32_DLL::readFile(@handle, rbuf, buf.size, size_st, nil))
          raise IOError.new(KERNEL32_DLL::getLastErrorString())
        end
        buf[0,buf.size] = rbuf.to_s(buf.size)
        return buf;
      end

      def send(data)
        size_st = KERNEL32_DLL::DWORD_ST.malloc()
        if(!KERNEL32_DLL::writeFile(@handle, data, data.size, size_st, nil))
          raise IOError.new(KERNEL32_DLL::getLastErrorString())
        end
        return data
      end

      def get(buf)
        tbuf = DL.malloc(buf.size)
        tbuf[0] = buf
        if(!HID_DLL::hidD_GetFeature(@handle, tbuf, buf.size))
          raise IOError.new(KERNEL32_DLL::getLastErrorString())
        end
        buf[0,buf.size] = tbuf.to_s(buf.size)
        return buf
      end

      def set(data)
        if(!HID_DLL::hidD_SetFeature(@handle, data, data.size))
          raise IOException.new(KERNEL32_DLL::getLastErrorString())
        end
        return true
      end

      def close()
        KERNEL32_DLL::closeHandle(@handle) if @handle != -1
        @handle = -1
      end
    end

    def self.open(vendor_id, product_id, opt={})
      guid = HID::Win32::GUID.malloc()
      HID_DLL.hidD_GetHidGuid(guid);
      devinfo = SETUPAPI_DLL.setupDiGetClassDevsA(
        guid,nil,0,
        SETUPAPI_DLL::DIGCF_PRESENT |
        SETUPAPI_DLL::DIGCF_DEVICEINTERFACE)

      index = 0
      loop do
        deviceInterface = SETUPAPI_DLL::SP_DEVICE_INTERFACE_DATA.create()
        if !SETUPAPI_DLL::setupDiEnumDeviceInterfaces(
            devinfo, nil, guid, index, deviceInterface)
          puts 'No device found'
          return nil
        end
        index+=1

        sizest = SETUPAPI_DLL::DWORD_ST.malloc()
        # Take the required size of DeviceInterfaceDetailData.
        # Note that this invocation will be *failed* and return the
        # required buffer size into the "sizest" variable.
        SETUPAPI_DLL.setupDiGetDeviceInterfaceDetailA(
            devinfo, deviceInterface, nil, 0, sizest, nil)

        deviceInterfaceDetail =
            SETUPAPI_DLL::SP_DEVICE_INTERFACE_DETAIL_DATA_A.create(sizest.size)
        
        if(!SETUPAPI_DLL.setupDiGetDeviceInterfaceDetailA(
            devinfo, deviceInterface, deviceInterfaceDetail,
            sizest.size, nil, nil))
          # unknown error
          next
        end

        handle = KERNEL32_DLL.createFileA(
            deviceInterfaceDetail.getDevicePath(),
            KERNEL32_DLL::GENERIC_READ |
            KERNEL32_DLL::GENERIC_WRITE,
            KERNEL32_DLL::FILE_SHARE_READ |
            KERNEL32_DLL::FILE_SHARE_WRITE,
            0,
            KERNEL32_DLL::OPEN_EXISTING,
            0,
            0)

        if handle == -1
          # Couldn't open
          next
        end

        hidd_attr = HID_DLL::HIDD_ATTRIBUTES.create()
        ret = HID_DLL.hidD_GetAttributes(handle, hidd_attr)

        puts "VendorID: 0x#{hidd_attr.VendorID.to_s(16)}"
        puts "ProductID: 0x#{hidd_attr.ProductID.to_s(16)}"

        if (hidd_attr.VendorID != vendor_id ||
            hidd_attr.ProductID != product_id)
          Device.new(handle).close()
          next
        end
        dev = Device.new(handle)
        if block_given?
          yield dev
          dev.close()
          return nil
        else
          return dev
        end
      end
    ensure
      SETUPAPI_DLL.setupDiDestroyDeviceInfoList(devinfo)
    end
  end
  def self.open(vendor_id, product_id, opt={})
    if block_given?
      Win32::open(vendor_id, product_id, opt) do |f|
        yield(f)
      end
    else
      return Win32::open(vendor_id, product_id, opt)
    end
  end
end

# vim: foldmethod=syntax
